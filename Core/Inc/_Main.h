
#include "_Set_Timers.h"		////////////////////////////////////////////////////////
#include "_PeriphInit.h"		////////////////////////////////////////////////////////

prfm("\033[H");
prfm("\033[2J");

set_(CS_M95P32);
SPI4_SetStatusConfig(); // unlock eeprom
SPI4_WriteVolatRegDisableBuff();
//write_Dat_to_EEp_fn(); // write default variables to eep. Can be used when adding new item to Eep data array.
SPI4_EEP_ReadDataSettingsRegion(3145728, NUM_SET_ENUM);
//print_Eep_data_f();

actions_after_charge_mode_change(5); // set charge mode values
get_max_min_lims_from_DEV_NOM_VOUT();
update_VDC_high_low_lim_fc();
delay_1ms(100);

GLCD_Init();
delay_1ms(10);
GLCD_ClearScreen(0x00);
GLCD_RefreshGRAM();

swap_scr_lines(&SCR_R, &SCR_T); // faz sıralamasına göre değşim gerekiyorsa bu fonksiyon kullanılacak.

// RTC init
if (Read_RTC_Osc_Status() == 0) {
    Write_To_Register(0, 0b10000000); // osc en
    Write_To_Register(2, 0b00000000); // 12/24_n set
    Write_To_Register(RTC_OSCTRIM_REG, 0b00000000 | 5); // write cal val
    Write_To_Register(RTC_RTCC_CONTROL_REG, 0b01000010); // enable square wave output
    sprintf(DUB,"RTC first time started"); prfm(DUB);
} else {
	sprintf(DUB,"RTC already started"); prfm(DUB);
}

DROPP_BATT_CTRL(EpD[SET_DROPPER_K1][0].V1);
DROPP_LOAD_CTRL(EpD[SET_DROPPER_K2][0].V1);
apply_state_changes_f(DROPPER1_BYP_FC, EpD[SET_DROPPER_K1][0].V1);
apply_state_changes_f(DROPPER2_BYP_FC, EpD[SET_DROPPER_K2][0].V1);


delay_1ms(1000);
SW_LINE_OFF=!isInSet_(SW_LINE_P);
SW_BATT_OFF=!isInSet_(SW_BATT_P);
SW_LOAD_OFF=!isInSet_(SW_LOAD_P);
if (is_state_active(LINE_FUSE_OFF_FC) != SW_LINE_OFF) {
	apply_state_changes_f(LINE_FUSE_OFF_FC, SW_LINE_OFF);
}
if (is_state_active(BATT_FUSE_OFF_FC) != SW_BATT_OFF) {
	apply_state_changes_f(BATT_FUSE_OFF_FC, SW_BATT_OFF);
}
if (is_state_active(LOAD_FUSE_OFF_FC) != SW_LOAD_OFF) {
	apply_state_changes_f(LOAD_FUSE_OFF_FC, SW_LOAD_OFF);
}

delay_1ms(100);

SPI4_ReadDataFaultRegion(FAULT_RECORD_START_ADDRESS, NUM_FAULT_RECORD);
// find address to write faults
for (int i = 0; i < NUM_FAULT_RECORD; i++) {	// find first record which is ff
	if (array_fault_data[i][0] == 0xffffffff) {
		flt_array_index_next = i;
		flt_array_index_last=(flt_array_index_next-1+NUM_FAULT_RECORD)%NUM_FAULT_RECORD;
		flt_disp_index=(flt_array_index_last-5+NUM_FAULT_RECORD)%NUM_FAULT_RECORD;
		flt_disp_index=flt_array_index_last;
		flt_array_index_found=1;
		sprintf(DUB,"st ff found eep location %lu", flt_array_index_next); prfm(DUB);
		break;
	}
}
if (flt_array_index_found == 0) { // if not found, find first record which is grater than next record
	flt_array_index_next=0;
	int j=0;
	for (int i = 0; i < NUM_FAULT_RECORD; i++) {
		j=(i+1)%NUM_FAULT_RECORD;
		if (array_fault_data[i][0] > array_fault_data[j][0]) {
			flt_array_index_next=j;
			flt_array_index_found=1;
			flt_array_index_last=(flt_array_index_next-1+NUM_FAULT_RECORD)%NUM_FAULT_RECORD;
			flt_disp_index=(flt_array_index_last-5+NUM_FAULT_RECORD)%NUM_FAULT_RECORD;
			sprintf(DUB,"st n_ff found eep location %lu", flt_array_index_next); prfm(DUB);
			break;
		}
	}
}
if (flt_array_index_found == 0) { // couldn't find last fault record location. Consider memory is corrupted. Clear all fault records.
	for (int i = 0; i < 48; i++) { // 3/4 of memory is for fault codes. 48 of 64 blocks.
		SPI4_BlockErase(FAULT_RECORD_START_ADDRESS+i*64); // clear fault records
	}
	flt_array_index_next=0; // next fault to write index 0
	flt_array_index_found=1;
}
//printFaultCodes();



en_uart_msg_group(blm_u);  // Enable uart messages for batt line inspection
//disb_uart_msg_group(pr_btln);


//REL_OUT_order_chng_f(17, BATTERY_FAULT_FC_REL);
//print_REL_OUT_Table();

///////////////////////////////////////////////////////////
// Temp sensor init
USART10_SendByte(0x55);
delayA_1us(10);
USART10_SendByte(0xB4);
delay_1ms(100);

temp_sens_count = tmp144_init_and_assign();
if (temp_sens_count == 0) {
	sprintf(DUB,"Temp sensor init failed"); prfm(DUB);
	sogut_sensor_exists = 0;
	trafo_sensor_exists = 0;
	batt_sensor_exists = 0;
} else {
	sprintf(DUB,"Temp sensor init success %d Sensors", temp_sens_count); prfm(DUB);
	if (temp_sens_count == 1) {
		sogut_sensor_exists = 1;
		trafo_sensor_exists = 0;
		batt_sensor_exists = 0;
	} else if (temp_sens_count == 2) {
		sogut_sensor_exists = 1;
		trafo_sensor_exists = 1;
		batt_sensor_exists = 0;
	} else if (temp_sens_count == 3) {
		sogut_sensor_exists = 1;
		trafo_sensor_exists = 1;
		batt_sensor_exists = 1;
	}
}
// Temp sensor init
///////////////////////////////////////////////////////////

generate_REL_OUT_order_vect_from_eeprom_parts_fc(); // eepromdan sıkışmış datayı al ve decompress et
generate_rel_ord_tb_from_REL_OUT_order_vector_fc(); // tabloya aktar. buraya kadar henüz röleler aktif edilmiyor. Hepsi 0. Program işleyişi rölelerin durumunu belirleyecek.


if (EpD[RECT_ACTV_AT_STARTUP][0].V1==1;) {
	thy_drv_en_req=1;
} else {
	thy_drv_en_req=0;
}

//LED_7_Data |= FLOAT_CHARGE_LED;

// butona basar basmaz tepki olmuyor. bunu hızlandırmak lazım. butonu bırakınca kısa süreliğine butonu disable etmek lazım.
// data kayıt için adrese ihtiyaç yok
// arız kodu listesi son arıza numarasını 1 yap öyle sırala
// activate device seçeneği koy ki açılışta çalışmasın. voltaj ayarları yapıldıktan sonra cihaz aktif edilsin.
// arıza tekrarladıkça geri dönme süresini uzatma algoritması
// arıza durumunda start led kapanmıyor.
// EpD değerleri 0 olmalı. cihaz kurulumunda belirlenmeli.
// eeprom okuma başarısız ise arıza durumu gösterilmeli. değerler eepromdan alındığı için.
// bir anda batarya akımı kesilirse battery inspection hemen başlat.
//_Fnc_Gen.h uses sprintf(DUB, ...); prfm(DUB); multiple times in functions like batt_line_broken_fn().
//This could slow down execution due to frequent memory operations.
//Possible Fix: Use predefined format strings or snprintf() with fixed buffer sizes.
//akü hattı kopuk araştırmasını nominal voltajın 1 volt üzeri ve altına yavaş yavaş dolaşarak yapsın.
// araştırma başlangıç voltajı sabit değilse. ona göre algoritma oluşturmak gerekiyor. mesela artış sürecinde ise.
// voltajdaki değişime rağmen batarya akımında değişim olmaması bir gösterge.
// voltajı düşürrmeye çalışırken düşürememe de bir gösterge, çünkü batarya bağlıdır ve voltajı belli bir seviyede tutmaktadır.
// batarya devreye alındığında DC short hatası oluyor. DC short hatası olmadan batarya devreye alınmalı.
// bunu yapmak için kısa devre akımındaki değişime bakılabilir. aşağı yönlü bir değişim varsa bunu batarya devreye alma olarak algıla.
// eeprom kaydetemden önce oku. aynı değer varsa yazma.
// çıkış voltajı yokken. yani ne doğrultucu ne de batarya dc vermiyorsa çıkışa, dc kaçak yüzdesi yüksek olabiliyor.
// böyle bir durumda yanlış dck oluşmaması için çıkış voltajı nominal voltajın %20 altında ve çıkış, doğrultucu ve
// batarya akımı yoksa dck yüzdesini önemseme
// rectifier kapalı iken, akü şalter devre dışı iken kaçak voltaj arızası oluşması engellenmeli.
// NOTE_FOR_CODE_PART notu olan satırlaın olduğu bölümlerde inceleme ve geliştirme gerekiyor olaiblir.


//release_chg yazan satırları bulup değişiklikleri eski haline getirmek gerekiyor.

//short circuit detection için. short circuit akımındaki değişime bakılabilir.
//		if (thy_drv_en==1 && IRECT_smp_sc > EpD[RECT_SHORT][0].V1 && IRECT_smp_sc_num <= 10) {
//			IRECT_Short_Vector[IRECT_smp_sc_num]=adc_ch4;
//			if (IRECT_smp_sc_num==1 && IRECT_Short_Vector[1]*1.05 < IRECT_Short_Vector[0]) {
//
//			}
//			IRECT_smp_sc_num++;
//
//		}
//		else {
//			IRECT_smp_sc_num=0;
//		}



