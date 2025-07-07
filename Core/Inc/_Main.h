
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
delay_1ms(100);

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
    Write_To_Register(RTC_OSCTRIM_REG, 0b00000000 | 0); // write cal val
    Write_To_Register(RTC_RTCC_CONTROL_REG, 0b01000010); // enable square wave output
    PRF_GEN("RTC first time started");
} else {
	PRF_GEN("RTC already started");
}

delay_1ms(100);

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
		PRF_GEN(" found flt_array_index_next %lu", flt_array_index_next);
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
			PRF_GEN(" found flt_array_index_next %lu", flt_array_index_next);
			break;
		}
	}
}
if (flt_array_index_found == 0) { // couldn't find last fault record location. Consider memory is corrupted. Clear all fault records.
	for (int i = 0; i < 48; i++) { // 3/4 of memory is for fault codes. 48 of 64 blocks.
		SPI4_BlockErase(FAULT_RECORD_START_ADDRESS+i*64); // clear fault records
	}
	PRF_GEN(" couldn't find last fault record location.");
	PRF_GEN(" eeprom clear fault records region.");
	flt_array_index_next=0; // next fault to write index 0
	flt_array_index_found=1;
}
//printFaultCodes();
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

PRF_GEN("USE EEPROM TABLE DATA"); // eeprom okunduktan sonra kayıtlı dataya göre değişkenleri belirle
if (EpD[SET_CHARGE_MODE][0].V1 == FLOAT) {
	state_set(ST_FLOAT_CHARGE, 1); PRF_GEN("Startup eeprom CHARGE MODE FLOAT");
	temp_targ_DC_voltage=EpD[VBAT_FLOAT][0].V1;
	targ_DC_current=EpD[SET_IBAT_FLOAT][0].V1;
} else if (EpD[SET_CHARGE_MODE][0].V1 == BOOST) {
	state_set(BOOST_CHARGE_FC, 1); PRF_GEN("Startup eeprom CHARGE MODE BOOST");
}
if (EpD[SET_CH_CONT_MODE][0].V1 == MANUAL) {
	state_set(MANUAL_CHARGE_FC, 1); PRF_GEN("Startup eeprom CHARGE CONTROL MODE MANUAL");
} else if (EpD[SET_CH_CONT_MODE][0].V1 == AUTO) {
	state_set(AUTO_CHARGE_ST, 1); PRF_GEN("Startup eeprom CHARGE CONTROL MODE AUTO");
} else if (EpD[SET_CH_CONT_MODE][0].V1 == TIMED) {
	state_set(TIMED_CHARGE_FC, 1); PRF_GEN("Startup eeprom CHARGE CONTROL MODE TIMED");
}
Vbat_flt = EpD[DEV_NOM_VOUT][0].V1 * 0.1; // Vbat too low control. Batt line monitor checks this to determine if battery is connected or not. If voltge is too low

Irect_set_limit_max = EpD[IRECT_LIM_RT_][0].V1 * 1.0; // rectifier çıkış akımı ayar max
Irect_set_limit_min = EpD[IRECT_LIM_RT_][0].V1 * 0.01; // rectifier çıkış akımı ayar min
Ibat_charge_set_limit_max  = EpD[IRECT_LIM_RT_][0].V1 * 1.0; // batarya şarj akımı ayar max
Ibat_charge_set_limit_min  = EpD[IRECT_LIM_RT_][0].V1 * 0.1; // batarya şarj akımı ayar min


blm_I_step_025perc  = EpD[IRECT_LIM_RT_][0].V1 * 0.0025; // ibat_stable
blm_I_step_075perc = EpD[IRECT_LIM_RT_][0].V1 * 0.0075; // stability_irect_fc ve batt curr detect threshold
blm_V_step_05perc  = EpD[DEV_NOM_VOUT][0].V1 * 0.005; // bring_vtarg_back_to_chrgV
blm_V_step_15perc  = EpD[DEV_NOM_VOUT][0].V1 * 0.015; // stability_vrect_fc
blm_V_move_up_set  = EpD[DEV_NOM_VOUT][0].V1 * 0.02; // blm_move_up_down_vtarg_limits
blm_V_move_dn_set  = EpD[DEV_NOM_VOUT][0].V1 * 0.02; // blm_move_up_down_vtarg_limits

set_dropper_l_hg_perc=EpD[SET_DROPP_L_HG_PERC][0].V1 / 100; // yüzdeleri al eepromdan.
set_dropper_l_lw_perc=EpD[SET_DROPP_L_LW_PERC][0].V1 / 100;
set_dropper_l_hg_V = EpD[DEV_NOM_VOUT][0].V1 * (1 + (EpD[SET_DROPP_L_HG_PERC][0].V1 / 100)); // voltaja çevir. daha önce voltaj kaydedildiğinde zaten yüzde de voltaja göre kaydedilmişti. yani şimdi yüzdeyi voltaja dönüştürmenin bir sakıncası yok. daha önce yüzdeye dönüştürülmüş olan voltajı geri bulmuş oluyoruz.
set_dropper_l_lw_V  = EpD[DEV_NOM_VOUT][0].V1 * (1 - (EpD[SET_DROPP_L_LW_PERC][0].V1 / 100));

VAC_Hg_Lim = VAC_Nom * (1 + 0.1); // Giriş voltajı monitör
VAC_Lo_Lim = VAC_Nom * (1 - 0.12); // Giriş voltajı monitör

ovtmp_open_per=(uint32_t) (EpD[SET_OVT_OPEN_DELAY][0].V1*1000/50); // calculate alarm to open duration in 50ms

DROPP_BATT_CTRL(EpD[SET_DROPPER_K1][0].V1);  // set dropper control pin according to EpD[SET_DROPPER_K1][0].V1 value
DROPP_LOAD_CTRL(EpD[SET_DROPPER_K2][0].V1);
state_set(ST_DROPPER_K1, 1);
state_set(ST_DROPPER_K2, 1);

frq_cal_k=275e6*EpD[SET_FRQ_CAL][0].V1;
// n015
gen_SIRALI_TABLO_RELOUT_from_eep(); // eepromdan sıkışmış datayı al ve decompress et
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

state_set(ST_LINE_MCCB_OFF, !isInSet_(SW_LINE_P));
state_set(ST_BATT_MCCB_OFF, !isInSet_(SW_BATT_P));
state_set(ST_LOAD_MCCB_OFF, !isInSet_(SW_LOAD_P));

///////////////////////////////////////////////////////////
// Temp sensor init
tmp144_init_and_assign();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cihaz ilk açılışta doğrultucuyu direk devreye alacak mı.
// eğer çıkış voltaj değeri ayarlanmamışsa açılışta recrifieri direk devreye almak tehlikeli olabilir.
if (EpD[RECT_ACTV_AT_STARTUP][0].V1==1) {
	thy_drv_en_req=1; // cihaz açılışında trsitör sürme başlatma isteği
} else {
	thy_drv_en_req=0;
	after_a_state_changes_f(USER_STOP_FC, 1);
	PRF_GEN("Cihaz açılışta otomatik devreye girmeyecek.");
	PRF_GEN("Yönetim ayarları -> Açılışta aktif özelliği.");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//en_uart_msg_group(blm_u);  // Enable uart messages for batt line inspection
//disb_uart_msg_group(pr_btln);


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


