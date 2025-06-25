
#include "_Set_Timers.h"		////////////////////////////////////////////////////////
#include "_PeriphInit.h"		////////////////////////////////////////////////////////

prfm("\033[H");
prfm("\033[2J");

PRF_GEN("EEPROM INIT");
set_(CS_M95P32);
SPI4_SetStatusConfig(); // unlock eeprom
SPI4_WriteVolatRegDisableBuff();

PRF_GEN("EEPROM READ TABLE DATA");
track_table_change=SPI4_ReadDataSetting(3145728+TRACK_TABLE_CHANGE*8); // programlarken eep table da değişiklik yapılmış ise değişikliklere göre işlemleri yap
if ((uint32_t)EpD[TRACK_TABLE_CHANGE][0].V1 != (uint32_t)track_table_change) { // tablo sonundaki değer sadece okunuyor. kayma varsa programdaki değerden farklı olacaktır.
	PRF_GEN(" - - - - Default değerler eeprom a yazılıyor.");
	write_Dat_to_EEp_fn(); // write default variables to eep.
	SPI4_EEP_ReadDataSettingsRegion(3145728, NUM_SET_ENUM);
	track_table_change=SPI4_ReadDataSetting(3145728+TRACK_TABLE_CHANGE*8);
	PRF_GEN("track_table_change %f", track_table_change);
} else {
	PRF_GEN("EEP Table size OK");
	SPI4_EEP_ReadDataSettingsRegion(3145728, NUM_SET_ENUM);
}

PRF_GEN("GLCD INIT");
GLCD_Init();
delay_1ms(10);
GLCD_ClearScreen(0x00);
GLCD_PrintString(16, 25, "POWER ELEKTRONIK");
GLCD_RefreshGRAM();

// FAZ SIRASI
swap_scr_lines(&SCR_R, &SCR_T); // faz sıralamasına göre değşim gerekiyorsa bu fonksiyon kullanılacak.

PRF_GEN("RTC INIT");
if (Read_RTC_Osc_Status() == 0) {
    Write_To_Register(0, 0b10000000); // osc en
    Write_To_Register(2, 0b00000000); // 12/24_n set
    Write_To_Register(RTC_OSCTRIM_REG, 0b00000000 | 5); // write cal val
    Write_To_Register(RTC_RTCC_CONTROL_REG, 0b01000010); // enable square wave output
    PRF_GEN("RTC first time started");
} else {
	PRF_GEN("RTC already started");
}

PRF_GEN("EEP READ FAULT RECORDS");
SPI4_ReadDataFaultRegion(FAULT_RECORD_START_ADDRESS, NUM_FAULT_RECORD);
// find address to write faults
for (int i = 0; i < NUM_FAULT_RECORD; i++) {	// find first record which is ff
	if (array_fault_data[i][0] == 0xffffffff) {
		flt_array_index_next = i;
		flt_array_index_last=(flt_array_index_next-1+NUM_FAULT_RECORD)%NUM_FAULT_RECORD;
		flt_disp_index=(flt_array_index_last-5+NUM_FAULT_RECORD)%NUM_FAULT_RECORD;
		flt_disp_index=flt_array_index_last;
		flt_array_index_found=1;
		PRF_GEN("st ff found eep location %lu", flt_array_index_next);
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
			PRF_GEN("st n_ff found eep location %lu", flt_array_index_next);
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
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


PRF_GEN("USE EEPROM TABLE DATA"); // eeprom okunduktan sonra kayıtlı dataya göre değişkenleri belirle
PRF_GEN("Startup eeprom charge mode %f", EpD[SET_CHARGE_MODE][0]);
Current_charge_voltage=EpD[VBAT_FLOAT][0].V1;
I_batt_targ_con_sy=EpD[SET_IBAT_FLOAT][0].V1;
LED_7_Data |= FLOAT_CHARGE_LED;
LED_7_Data &= ~BOOST_CHARGE_LED;
Vbat_flt = EpD[DEV_NOM_VOUT][0].V1 * 0.1;

Irect_max = EpD[IRECT_LIM_RT_][0].V1 * 1.0;
Irect_min = EpD[IRECT_LIM_RT_][0].V1 * 0.01;
Ibat_max  = EpD[IRECT_LIM_RT_][0].V1 * 1.0;
Ibat_min  = EpD[IRECT_LIM_RT_][0].V1 * 0.1;

blm_I_step_05perc  = EpD[IRECT_LIM_RT_][0].V1 * 0.005;
blm_I_step_075perc = EpD[IRECT_LIM_RT_][0].V1 * 0.0075;
blm_I_step_10perc  = EpD[IRECT_LIM_RT_][0].V1 * 0.010;
blm_V_step_05perc  = EpD[DEV_NOM_VOUT][0].V1 * 0.005;
blm_V_step_10perc  = EpD[DEV_NOM_VOUT][0].V1 * 0.010;
blm_V_step_15perc  = EpD[DEV_NOM_VOUT][0].V1 * 0.015;
blm_V_move_up_set  = EpD[DEV_NOM_VOUT][0].V1 * 0.02;
blm_V_move_dn_set  = EpD[DEV_NOM_VOUT][0].V1 * 0.02;

//  voltaja çevir dev nom vout a göre. zaten yüzdeler kaydedildiğinde
set_dropper_l_hg_perc=EpD[SET_DROPP_L_HG_PERC][0].V1 / 100; // yüzdeleri al eepromdan.
set_dropper_l_lw_perc=EpD[SET_DROPP_L_LW_PERC][0].V1 / 100;
set_dropper_l_hg_V = EpD[DEV_NOM_VOUT][0].V1 * (1 + (EpD[SET_DROPP_L_HG_PERC][0].V1 / 100)); // voltaja çevir. daha önce voltaj aydedildiğinde zaten yüzde de voltaja göre kaydedilmişti. yani şimdi yüzdeyi voltaja dönüştürmenin bir sakıncası yok. daha önce yüzdeye dönüştürülmüş olan voltajı geri bulmuş oluyoruz.
set_dropper_l_lw_V  = EpD[DEV_NOM_VOUT][0].V1 * (1 - (EpD[SET_DROPP_L_LW_PERC][0].V1 / 100));

VAC_Hg_Lim = VAC_Nom * (1 + 0.1); // Giriş voltajı monitör
VAC_Lo_Lim = VAC_Nom * (1 - 0.12); // Giriş voltajı monitör

ovtmp_open_per=(uint32_t) (EpD[SET_OVT_OPEN_DELAY][0].V1*1000/50); // calculate alarm to open duration in 50ms
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

DROPP_BATT_CTRL(EpD[SET_DROPPER_K1][0].V1);
DROPP_LOAD_CTRL(EpD[SET_DROPPER_K2][0].V1);
apply_state_changes_f(DROPPER1_BYP_FC, EpD[SET_DROPPER_K1][0].V1);
apply_state_changes_f(DROPPER2_BYP_FC, EpD[SET_DROPPER_K2][0].V1);
frq_cal_k=275e6*EpD[SET_FRQ_CAL][0].V1;


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

//en_uart_msg_group(blm_u);  // Enable uart messages for batt line inspection
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
	PRF_GEN("Temp sensor init failed");
	sogut_sensor_exists = 0;
	trafo_sensor_exists = 0;
	batt_sensor_exists = 0;
} else {
	PRF_GEN("Temp sensor init success %d Sensors", temp_sens_count);
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cihaz ilk açılışta doğrultucuyu direk devreye alacak mı.
// eğer çıkış voltaj değeri ayarlanmamışsa açılışta recrifieri direk devreye almak tehlikeli olabilir.
if (EpD[RECT_ACTV_AT_STARTUP][0].V1==1) {
	thy_drv_en_req=1;
	user_wants_allows_thy_drv=1;
} else {
	thy_drv_en_req=0;
	user_wants_allows_thy_drv=0;
	PRF_GEN("Cihaz açılışta otomatik devreye girmeyecek.");
	PRF_GEN("Yönetim ayarları -> Açılışta aktif özelliği.");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 4 - eeprom okuma başarısız ise arıza durumu gösterilmeli.
// 3 - bir anda batarya akımı kesilirse battery inspection hemen başlat.
// 5 - blm bat bağlı, rect akım sınırı 3.4 ve bat akımı 0 iken blm sistemi kararsız çalışıyor. bat inceleme yöntemi kullanıldı sonuç alınamadığında başka bir yöntem kullanılmalı. mesela hızlı yöntem olan korelasyon yerine yavaş yavaş düşürerek takip edip etmediğine bakma.
// batarya devreye alma kısa devre hatası veriyor mu. denemek lazım.
// eeprom kaydetemden önce oku. aynı değer varsa yazma.
// 6 - arıza tekrarladıkça geri dönme süresini uzatma algoritması
// çıkış voltajı yokken. yani ne doğrultucu ne de batarya dc vermiyorsa çıkışa, dc kaçak yüzdesi yüksek olabiliyor.
// böyle bir durumda yanlış dck oluşmaması için çıkış voltajı nominal voltajın %20 altında ve çıkış, doğrultucu ve
// batarya akımı yoksa dck yüzdesini önemseme
// rectifier kapalı iken, akü şalter devre dışı iken kaçak voltaj arızası oluşması engellenmeli.
//short circuit detection için. short circuit akımındaki değişime bakılabilir.
// 1 - data kayıt için adrese ihtiyaç yok


