
#include "_Set_Timers.h"		////////////////////////////////////////////////////////
#include "_PeriphInit.h"		////////////////////////////////////////////////////////

prfm("\033[H");
prfm("\033[2J");

set_(CS_M95P32);
SPI4_SetStatusConfig(); // unlock eeprom
SPI4_WriteVolatRegDisableBuff();
//write_Dat_to_EEp_fn(); // write default variables to eep. Can be used when adding new item to Eep data array.
SPI4_EEP_ReadDataSettingsRegion(3145728, NUM_SET_ENUM);
print_Eep_data_f();

actions_after_charge_mode_change(5); // set charge mode values
get_max_min_lims_from_DEV_NOM_VOUT();
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

set_(DROPP_LOAD);
set_(DROPP_BATT);

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



//en_uart_msg_group(pr_btln);  // Enable uart messages for batt line inspection
disb_uart_msg_group(pr_btln);
bat_inspection_req_timer_h=ms_50_cnt-bat_inspection_req_timer_per; // fast restart inspection


//LED_7_Data |= FLOAT_CHARGE_LED;

// saat ayarlarlandı
// 1740176028 (2.2+6pf)+(2.2+6pf)
// 1740873329 -10 cal yapıldı.
// 1741189852 316523 saniyede bu unix time a geldi. 1 saniye geri kaldı. kalibrasyon -10 du -5 yapıldı
// 1741385888 196036 saniyede bu unix time a geldi. yaklaşık 0.1 saniye geri kaldı. kalibrasyon -5 ti -4 yapıldı
// 1741536423 150535 saniyede bu unix time a geldi. yaklaşık 0.5 saniye ileri gitti. kalibrasyon -4 ti -6 yapıldı
// 9pf yapılırsa iyi sonuç alınacaktır
// akü hattı kopuk için eklemeler gerekiyor. mcb yardımcı kontağını kullan. akım sınırlaması durumunda akü hattı kopuk belirleme sistemi voltajı düşüremiyor ve akü hattı var diyor. bunu akü bağlı değilken ve mcb yardımcı kontak okuma yapmadığım zamanda gördüm.
// akü arızası. akü voltajı
// ayarlanan akım ve gerilim değerleri arıza kodu olarak kaydedilecek.
// button release disables buttons for some ms
// data kayıt için adrese ihtiyaç yok
// adc offset sıfırlama seçeneği
// şebeke off yazması lazım bir faz bile gitse
// arız kodu listesi son arıza numarasını 1 yap öyle sırala
// açma kapama sistemi nasıl olsun
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



