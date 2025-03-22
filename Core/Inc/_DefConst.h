
#define BUZZ_P GPIOE, LL_GPIO_PIN_1
#define E12 GPIOE, LL_GPIO_PIN_12

// VARIABLES
///////////////////////////////////////////////////////////////////////////
#define NUM_CHANNELS 9
volatile uint32_t adc_buffer[NUM_CHANNELS];
//uint32_t adc_ch10=0;
//uint32_t adc_ch11=0;
//uint32_t adc_ch16=0;
//uint32_t adc_ch17=0;
uint32_t adc_ch18=0;
uint32_t adc_ch15=0;
uint32_t adc_ch7=0;
uint32_t adc_ch8=0;
uint32_t adc_ch9=0;
uint32_t adc_ch19=0;
uint32_t adc_ch5=0;
uint32_t adc_ch4=0;
uint32_t adc_ch3=0;

#define MAX_PID_OUTPUT 10000.0f
#define MIN_PID_OUTPUT 0.0f
uint32_t sf_sta_req=0;
uint32_t sf_sta_req_cnt=0;
uint32_t sf_sta_req_ok=0;
uint32_t sfst_1_t_hold=0;
uint32_t sfst_1_step=5;
double sfst_1_step_perc=0.005;
double sfst_1_step_perc_inc=0.005;
double vtarg_float_s_down=0.01;
uint32_t soft_start_cannot_increase_voltage_cnt=0;
uint32_t there_is_ongoing_cr1t_fault=0;
uint32_t there_is_ongoing_ncr1t_fault=0;
uint32_t there_is_ongoing_fault=0;
uint32_t return_from_ongoing_cr1t_fault_state_cnt=0;
uint32_t return_from_ongoing_ncr1t_fault_cnt=0;
uint32_t return_from_ongoing_fault_state_cnt=0;
uint32_t return_from_ongoing_cr1t_fault_state_per=20;
uint32_t return_from_ongoing_ncr1t_fault_per=20;
uint32_t return_from_ongoing_fault_state_per=20;
uint32_t there_is_past_unseen_fault=0;
//double vnow_sum = 0;

uint8_t req_reset_db = 0;
// CONTROL SYSTEM
#define ZCRENDELY 475
double VOUT_smp_sc = 0.0f;
double VLOAD_per_avg_sc = 0.0f;
double VDCKP = 0.0f;
double VDCKN = 0.0f;
double VOUT_max_float = 55.2f;
double VOUT_max_boost = 62.4f;
float VOUT_smp = 0.0f;
float V_Charg_Hg_10_perc = 0.0f;
float V_Charg_Hg_10_perc_ret = 0.0f;
uint32_t V_Charg_Hg_10_perc_Acc_cnt = 0;
uint32_t V_Charg_Hg_10_perc_Acc_per = 200;
uint32_t V_Charg_Hg_10_perc_ret_Acc_cnt = 0;
uint32_t V_Charg_Hg_10_perc_ret_Acc_per = 20;
float V_Charg_Lo_10_perc = 0.0f;
float V_Charg_Lo_10_perc_ret = 0.0f;
uint32_t V_Charg_Lo_10_perc_Acc_cnt = 0;
uint32_t V_Charg_Lo_10_perc_Acc_per = 200;
uint32_t V_Charg_Lo_10_perc_ret_Acc_cnt = 0;
uint32_t V_Charg_Lo_10_perc_ret_Acc_per = 20;
float IRECT_smp = 0.0f;
float IRECT_smp_sc = 0.0f;
float IRECT_per_sc = 0.0f;
float ILOAD_per_sc = 0.0f;
//float IRECT_f = 0.0f;
float VBAT_smp = 0.0f;
float VBAT_smp_sc = 0.0f;
float IBAT_smp = 0.0f;
float IBAT_smp_sc = 0.0f;
float IBAT_per_sc = 0.0f;
//float IBAT_f = 0.0f;
float VBAT_per_avg_sc = 0.0f;
float VBAT_f = 0.0f;
float vtarg_float = 52.8f;
float vtarg_set = 1.0f;
float cs_vtarg_vi = 0;
float itarg_rect = 100;
float itarg_batt = 100;
float Kp_v = 200.0f;
float Ki_v = 0.2f;
float Kp_i_rect = 0.2f;
float Ki_i_rect = 0.001f;
float Kp_i_batt = 0.001f;
float Ki_i_batt = 0.0001f;
uint32_t charge_mode_data_update=1;
double integral_v = 0.0f;
double integral_i_rect = 0.0f;
double integral_i_batt = 0.0f;
double Ki_integral_v = 3500.0f;
double Ki_integral_i_rect= 0.0f;
double Ki_integral_i_batt = 0.0f;
double Ki_x_int = 0.0f;
double integralp = 0.0f;
double integral_max_v = 40000.0f;
double integral_min_i_rect = -500000.0f;
double integral_min_i_batt = -500000.0f;
uint8_t integral_v_allowed = 1;
double Ki_x_int_lim = 2000000.0f;
double error_v = 0.0f;
double error_i_rect = 0.0f;
double error_i_batt = 0.0f;
double pid_output_v = 3200.0f;
double pid_output_i_rect = 3200.0f;
double pid_output_i_batt = 3200.0f;
uint8_t Cont_mode = 1;
uint8_t Mode_CV = 0;
uint8_t Mode_CC = 1;
uint8_t thy_drv_en=0;
uint16_t thy_drv_en_cnt=0;
uint8_t thy_drv_en_req = 1;
uint8_t user_wants_thy_drv = 1;
uint8_t batt_switch_on = 1; // debug
uint8_t batt_switch_status = 1;
uint8_t load_switch_status = 0;
uint8_t batt_line_broken = 0;
//uint8_t targetV_go_start = 0;
// CONTROL SYSTEM
///////////////////////////////////////////////////////////////////////////

float IRECT_sum = 0;
float IRECT_avg = 0;
float IRECT_lim = 15;
uint32_t IRECT_smp_count = 0;
uint32_t IRECT_samp_end = 0;
float IRECT_zero = 32784;
//uint32_t IRECT_zero_req = 0;
//uint32_t IRECT_zero_req_start = 0;
//uint32_t IRECT_zero_req_start_cnt = 0;
//uint32_t IRECT_zero_req_ok = 1;
//uint32_t IRECT_zero_ok_cnt = 0;

float IBAT_sum = 0;
float IBAT_avg = 0;
float IBAT_lim = 15;
uint32_t IBAT_smp_count = 0;
uint32_t IBAT_samp_end = 0;
float IBAT_zero = 32768;
//uint32_t IBAT_zero_req = 0;
//uint32_t IBAT_zero_req_start = 1;
//uint32_t IBAT_zero_req_start_cnt = 0;
//uint32_t IBAT_zero_req_ok = 1;
//uint32_t IBAT_zero_ok_cnt = 0;

float VBAT_sum = 0;
float VBAT_per_avg = 0;
float VBAT_lim = 15;
uint32_t VBAT_smp_count = 0;
uint32_t VBAT_samp_end = 0;
float VBAT_zero = 32768;
uint32_t VBAT_zero_req = 0;
uint32_t VBAT_zero_req_start = 1;
uint32_t VBAT_zero_req_start_cnt = 0;
uint32_t VBAT_zero_req_ok = 1;
uint32_t VBAT_zero_ok_cnt = 0;

float VAC_R_zero = 32760;
float VAC_S_zero = 32700;
float VAC_T_zero = 32770;
float VDCKP_per_avg = 0;
float VDCKN_per_avg = 0;
double VAC_R_per_avg = 0;
double VAC_S_per_avg = 0;
double VAC_T_per_avg = 0;
float VOUT_per_avg_sc = 0;
float VOUT_per_avg = 0;
float VDCKP_per_avg_roll = 0;
float VDCKN_per_avg_roll = 0;
float VAC_R_roll_per_avg = 0;
float VAC_S_roll_per_avg = 0;
float VAC_T_roll_per_avg = 0;
float VOUT_per_avg_roll_sc = 0;
float VOUT_per_avg_roll = 0;
float VOUT_per_avg_roll_half = 0;
float VDCKP_avg2_s = 0;
float VDCKN_avg2_s = 0;
uint32_t VDCKP_smp_count = 0;
uint32_t VDCKN_smp_count = 0;
uint32_t VAC_R_smp_count = 0;
uint32_t VAC_S_smp_count = 0;
uint32_t VAC_T_smp_count = 0;
uint32_t VOUT_smp_count = 0;
uint32_t VDCKP_samp_end = 0;
uint32_t VDCKN_samp_end = 0;
uint32_t VAC_R_samp_end = 0;
uint32_t VAC_S_samp_end = 0;
uint32_t VAC_T_samp_end = 0;
uint32_t VOUT_samp_end = 0;
float VDCKP_sum = 0;
float VDCKN_sum = 0;
float VOUT_sum_sc = 0;
uint32_t VOUT_sum = 0;
float VINR = 0;
float VAC_R_off = 0;
float VINS = 0;
float VINT = 0;
float VAC_R_sum = 0;
float VAC_S_sum = 0;
float VAC_T_sum = 0;
float VAC_R_sq_sum = 0;
float VAC_S_sq_sum = 0;
float VAC_T_sq_sum = 0;
double VAC_R_per_meansqr = 0;
double VAC_S_per_meansqr = 0;
double VAC_T_per_meansqr = 0;
double VAC_R_per_avgsqr = 0;
double VAC_S_per_avgsqr = 0;
double VAC_T_per_avgsqr = 0;
double VAC_R = 0;
double VAC_S = 0;
double VAC_T = 0;
float VDCKPN_div = 0;
//float VDCKPN_div2 = 0;
uint32_t VDCKP_per_avg_roll_perc = 0;
uint32_t VDCKN_per_avg_roll_perc = 0;
float VDCKP_per_avg_roll_scaled = 0;
float VDCKN_per_avg_roll_scaled = 0;
float VDCK_sc = 0;
float VDCK_raw = 0;
float VDCK_perc = 0;
float VDCKP_perc = 0;
float VDCKN_perc = 0;
int VDCK_side = 0;

// LCD MENU SYSTEM
uint8_t adjustmentMode = 0;
uint8_t out_en_h=0;
uint32_t UART_Debg_t_h=0;
uint32_t while_LCD_delay_h=0;
uint32_t while_LCD_delay_per=500;
uint32_t while_LCD_delay_normal_per=500;
uint32_t while_LCD_delay_clock_page_per=40;
uint32_t while_RTC_delay_h=0;
uint32_t while_RTC_delay_per=100;
uint32_t while_RTC_delay_normal_per=500;
uint32_t while_RTC_delay_clock_page_per=100;
uint32_t while_LCD_reinit_h=2000;
uint32_t while_LCD_reinit_per=500;
#define SHOW_MENU_NOW while_LCD_delay_h=ms_tick_cnt-while_LCD_delay_per;

typedef enum {
    HOME_PAGE_pg,
    MAIN_MENU_pg,
	CHARGE_SETT_pg,
	DEVICE_SETT_pg,
	MANAGEMENT_pg,
	FAULT_CODES_REPORT_pg,
	FAULT_CODES_RESET_pg,
	DEVICE_RESET_pg,
	DATE_TIME_pg,
	BATT_REVERSE_DETECT_pg,
	CONT_SYS_pg,
    NUM_PAGES
} MenuPage;

MenuPage currentPage = HOME_PAGE_pg;
uint8_t HOME_PAGE_pg_sel = 1;

uint8_t fault_codes_reset_req = 0;
uint8_t device_reset_req = 0;
uint8_t fault_codes_reset_req_right = 0;
uint8_t device_reset_req_right = 0;
uint8_t fault_codes_reset_qst_accept = 0;
uint8_t fault_codes_reset_completed = 0;
uint8_t batt_reverse_req_right = 0;

const char* MAIN_MENU_Items[] = {
    "Şarj Ayarları",
    "Cihaz Ayarları",
    "Arıza Kod Raporu",
    "Yönetim"
};
#define NUM_MAIN_MENU_ITEMS (sizeof(MAIN_MENU_Items) / sizeof(MAIN_MENU_Items[0]))
uint8_t selected_MAIN_MENU = 0;
uint32_t switch_to_auto_mode_completed=0;
uint32_t timed_mode_actions_do_once=0;
uint32_t timed_mode_time_ended=0;
uint32_t boost_of_auto_mode_active=0;
uint32_t float_of_auto_mode_active=0;
uint32_t charge_mode_timed_time_cnt=0;
uint32_t charge_mode_timed_time_sec=0;

typedef enum {
	FLOAT,
    BOOST,
	AUTO,
	TIMED
} Charge_Mode_Type;

const char* CH_MOD_SEL_Items[] = {
    "  Normal",
    "   Tam  ",
    "   Oto  ",
    " Zamanlı"
};
#define NUM_CH_MOD_SEL_ITEMS (sizeof(CH_MOD_SEL_Items) / sizeof(CH_MOD_SEL_Items[0]))
uint8_t selected_CH_MOD_SEL=0;

typedef struct {
    uint32_t setting_id;  // Unique ID for the setting
    float V1;          // Value stored as a float
} EEPROM_Data_Type;

typedef enum {
	SETTING_RECORD_START_ADDRESS,
    SET_CHARGE_MODE,    // Charge mode selection
    SET_BOOST_TIME,		// Boost charge duration
    VBAT_FLOAT,          // Float Voltage
    VBAT_BOOST,          // Boost Voltage
    SET_IBAT_FLOAT,          // Float Current
    SET_IBAT_BOOST,          // Boost Current
	IRECT_LIM_RT_,
	I_LIM_TO_FLOAT,
	I_LIM_TO_BOOST,
	SET_IRECT_CAL,          // Cal val
	SET_VBAT_CAL,
	SET_UNSEEN_FLT,
	SET_BATT_REV_DET,
	DEV_NOM_VOUT, 		// Cihaz Nom VDC
	DEV_NOM_IOUT, 		// Cihaz Nom IDC rect out
	BATT_NOM_IOUT,
	DC_KAC_POS, 			// DC kaçak pozitif
	DC_KAC_NEG, 			// DC kaçak negatif
	RECT_SHORT,
	BATT_SHORT,
    NUM_SET_ENUM            // Keeps track of total settings
} EEPROM_Setting_ID;

//uint32_t SETTING_RECORD_START_ADDRESS=0x300000; // 3145728
EEPROM_Data_Type EpD[NUM_SET_ENUM][2] = {
    { {SETTING_RECORD_START_ADDRESS, 3145728.0}, {SETTING_RECORD_START_ADDRESS, 3145728.0} },
    { {SET_CHARGE_MODE, 0.0}, {SET_CHARGE_MODE, 0.0} },
    { {SET_BOOST_TIME, 4.0}, {SET_BOOST_TIME, 4.0} },
    { {VBAT_FLOAT, 52.8}, {VBAT_FLOAT, 52.8} },
    { {VBAT_BOOST, 55.2}, {VBAT_BOOST, 55.2} },
    { {SET_IBAT_FLOAT, 5.0}, {SET_IBAT_FLOAT, 5.0} },
    { {SET_IBAT_BOOST, 10.0}, {SET_IBAT_BOOST, 10.0} },
    { {IRECT_LIM_RT_, 2.0}, {IRECT_LIM_RT_, 2.0} },
    { {I_LIM_TO_FLOAT, 3.0}, {I_LIM_TO_FLOAT, 3.0} },
    { {I_LIM_TO_BOOST, 4.0}, {I_LIM_TO_BOOST, 4.0} },
    { {SET_IRECT_CAL, 0.0011680884}, {SET_IRECT_CAL, 0.0011680884} },
    { {SET_VBAT_CAL, 0.0055583874}, {SET_VBAT_CAL, 0.0055583874} },
    { {SET_UNSEEN_FLT, 0.0}, {SET_UNSEEN_FLT, 0.0} },
    { {SET_BATT_REV_DET, 1.0}, {SET_BATT_REV_DET, 1.0} },
    { {DEV_NOM_VOUT, 48.0}, {DEV_NOM_VOUT, 48.0} }, // Cihaz Nom VDC
    { {DEV_NOM_IOUT, 40.0}, {DEV_NOM_IOUT, 40.0} },
    { {BATT_NOM_IOUT, 40.0}, {BATT_NOM_IOUT, 40.0} },
    { {DC_KAC_POS, 5.0}, {DC_KAC_POS, 5.0} }, // DC kaçak pozitif
    { {DC_KAC_NEG, 5.0}, {DC_KAC_NEG, 5.0} }, // DC kaçak negatif
    { {RECT_SHORT, 20.0}, {RECT_SHORT, 20.0} }, // RECT short Amp
    { {BATT_SHORT, 20.0}, {BATT_SHORT, 20.0} } // BATT short Amp
};

const char* Eep_data_Names[] = { // for printing in uart
    "SETTING_RECORD_START_ADDRESS",
    "SET_CHARGE_MODE",
    "SET_BOOST_TIME",
    "VBAT_FLOAT",
    "VBAT_BOOST",
    "SET_IBAT_FLOAT",
    "SET_IBAT_BOOST",
    "IRECT_LIM_RT_",
    "I_LIM_TO_FLOAT",
    "I_LIM_TO_BOOST",
    "SET_IRECT_CAL",
    "SET_VBAT_CAL",
    "SET_UNSEEN_FLT",
    "SET_BATT_REV_DET",
    "DEV_NOM_VOUT", // Cihaz Nom VDC
    "DEV_NOM_IOUT",
    "BATT_NOM_IOUT",
    "DC_KAC_POS",	 	// DC kaçak pozitif
    "DC_KAC_NEG", 		// DC kaçak negatif
    "RECT_SHORT",
    "BATT_SHORT"
};

typedef struct {
    const char* setting_id;
    uint8_t V1;
    uint8_t type;
} SETT_type;

SETT_type CHARGE_SETT_Items[] = {
{"Sarj Modu", SET_CHARGE_MODE, 99},
{"Süre Tam", SET_BOOST_TIME, 99},
{"V Şarj Normal", VBAT_FLOAT, 0}, // type grouping floats to use one else if in lcd display
{"V Şarj Tam", VBAT_BOOST, 0},
{"I Şarj Normal", SET_IBAT_FLOAT, 0},
{"I Şarj Tam", SET_IBAT_BOOST, 0},
{"Norma Geçiş", I_LIM_TO_FLOAT, 0},
{"Tama Geçiş", I_LIM_TO_BOOST, 0}
};


#define NUM_CHARGE_SETT_ITEMS (sizeof(CHARGE_SETT_Items) / sizeof(CHARGE_SETT_Items[0]))
uint8_t selected_CHARGE_SETT = 1;
uint8_t chg_set_disp_index =0;
uint8_t chg_set_arrow_loc = 1;
uint8_t chg_setting_edit_mode = 0;

const char* AKTFPAS_SEL_Items[] = {
    "  Pasif ",
    "  Aktif "
};

float IRECT_Short_Lim = 8;
float IBAT_Short_Lim = 8;
float IRECT_Short_Vector[10]={0};
uint32_t IRECT_smp_sc_num=0;

SETT_type DEVICE_SETT_Items[] = {
{"Tarh Saat Ayr", 255, 1},
{"Akü Ters Alg", SET_BATT_REV_DET, 2},
{"Cihaz Nom VDC", DEV_NOM_VOUT, 3},
{"I Doğrlt Max", IRECT_LIM_RT_, 3},
{"DC Kaçak +%", DC_KAC_POS, 3},
{"DC Kaçak -%", DC_KAC_NEG, 3},
{"KDevr Doğr A", RECT_SHORT, 3},
{"KDevr Bat A", BATT_SHORT, 3}
};
#define NUM_DEVICE_SETT_ITEMS (sizeof(DEVICE_SETT_Items) / sizeof(DEVICE_SETT_Items[0]))
uint8_t selected_DEVICE_SETT = 0;
uint8_t dev_set_disp_index =7;
uint8_t dev_set_arrow_loc = 1;
uint8_t dev_setting_edit_mode = 0;

const char* MANAGEMENT_Items[] = {
"Arıza Kaytları Sil",
"Cihaz Yeniden Başlat"
};
#define NUM_MANAGEMENT_ITEMS (sizeof(MANAGEMENT_Items) / sizeof(MANAGEMENT_Items[0]))
uint8_t selected_MANAGEMENT = 0;

float array_settings_data[NUM_SET_ENUM][2]={{0.0f, 0.0f}};
uint32_t settings_rec_addr=3145728;

#define BOOST_CHARGE_TIME_MAX 90
//#define Vdc_float_max 52.8
#define VDC_NOM_MAX 300.0
#define VDC_NOM_MIN 6.0
#define DC_KAC_POS_MAX 50.0
#define DC_KAC_POS_MIN 5.0
#define DC_KAC_NEG_MAX 50.0
#define DC_KAC_NEG_MIN 5.0
//#define Vdc_float_max 52.8
//#define Vdc_boost_max 55.2
//#define IDC_FLOAT_MAX 6
//#define IDC_BOOST_MAX 12
float Vdc_float_max=0.0f; // updated at startup and when DEV_NOM_VOUT changed
float Vdc_float_min=0.0f; // updated at startup and when DEV_NOM_VOUT changed
float Vdc_boost_max=0.0f; // updated at startup and when DEV_NOM_VOUT changed
float Vdc_boost_min=0.0f; // updated at startup and when DEV_NOM_VOUT changed
float Irect_max=0.0f; // updated at startup and when DEV_NOM_VOUT changed
float Irect_min=0.0f; // updated at startup and when DEV_NOM_VOUT changed
float Ibat_max=0; // updated at startup and when DEV_NOM_VOUT changed
float Ibat_min=0; // updated at startup and when DEV_NOM_VOUT changed
float Vdc_drop_in_min=0.0f; // updated at startup and when DEV_NOM_VOUT changed
float Vdc_drop_in_max=0.0f; // updated at startup and when DEV_NOM_VOUT changed
float Vdc_drop_out_min=0.0f; // updated at startup and when DEV_NOM_VOUT changed
float Vdc_drop_out_max=0.0f; // updated at startup and when DEV_NOM_VOUT changed
float Batt_inspect_min=0.0f; // updated at startup and when DEV_NOM_VOUT changed
float Batt_inspect_max=0.0f; // updated at startup and when DEV_NOM_VOUT changed
#define BOOST_CHARGE_TIME_MIN 1
//#define VDC_FLOAT_MIN 10
//#define VDC_BOOST_MIN 11
//#define IDC_FLOAT_MIN 0
//#define IDC_BOOST_MIN 0
#define VAC_HG_RET_LIM VAC_Hg_Lim-2
#define VAC_LW_RET_LIM VAC_Lo_Lim+2
#define VAC_0_LIM VAC_Nom * 0.5f
#define VAC_0_RET_LIM VAC_0_LIM+2

float VAC_Nom=36.0f;
float VAC_Hg_Lim=0;
float VAC_Lo_Lim=0;


// FAULT CODES
///////////////////////////////////////////////////////////////////////////

#define MAX_STRING_LENGTH 128
char last_sent_string[MAX_STRING_LENGTH] = ""; // Store the last sent message
uint8_t prfm_r_counter = 0;  // Counter for uart_debug_cnt()

uint32_t unexpected_program_state=0;

float I_rect_lim_con_sy = 0;
float I_batt_targ_con_sy = 0;
float V_targ_con_sy = 0; // v target cont sys
//float Current_charge_voltage = 0; // v target soft start
float Current_charge_voltage = 0; // v target charge mode
uint32_t sfst_1_unexpected_state = 0;
uint32_t no_result_from_last_reduction = 0;
uint32_t take_vout_sample_for_batt_insp = 1;
uint32_t repeating_string_sent = 0;
uint32_t while_cnt_string_sent = 0;
uint32_t device_start_up_delay_completed = 0;
uint32_t batt_current_detected = 0;
uint32_t bat_inspection_allowed = 1;
uint32_t vout_sample_ready = 0;
uint32_t batt_inspection_not_needed_disp = 0;
uint32_t batt_current_detected_disp = 0;
uint32_t cannot_go_to_target_v = 0;
uint32_t vbatt_vout_diff = 0;
uint32_t min_v_reached_no_result = 0;
uint32_t min_v_reached_no_result_cnt = 0;
uint32_t VOUT_roll_per_avg_take_smp_cnt = 0;
uint32_t VOUT_roll_per_avg_take_smp_per = 20;
uint32_t VOUT_roll_per_avg_stable_Acc_cnt = 0;
uint32_t VOUT_roll_per_avg_stable_Acc_per = 5;
uint32_t gercek_voltaj_stable_ms = 0;
uint32_t gercek_voltaj_stable_ms_Acc_cnt = 0;
uint32_t check_gercek_voltaj_stable_ms = 1;
uint32_t bat_inspection_req_timer_h = 0;
uint32_t bat_inspection_req_timer_cnt = 0;
uint32_t bat_inspection_req_timer_per = 500;
uint32_t distance_from_inspect_start_Acc_h = 0;
uint32_t distance_from_inspect_start_Acc_per = 30;
uint32_t gercek_voltaj_smp_ms_timer_h = 0;
uint32_t gercek_voltaj_smp_ms_timer_per = 20;
uint32_t distance_from_inspect_start_accepted = 0;
float gercek_voltaj_diff_from_sec_ms = 0;
float gercek_voltaj_smp_ms = 0;
float hedef_baslangic_fark = 0;
float hedef_gercek_fark = 0;
float gercek_baslangic_fark = 0;
float cont_sys_vbatt_diff = 0;
uint32_t ms_50_cnt = 0;
uint32_t baslangic_gercek_fark_cok = 0;
uint32_t hedef_gercek_fark_az = 0;
uint32_t start_bat_inspection_req = 0;
uint32_t bat_inspection_canceled = 1;
uint32_t insp_loop_delay_h = 0;
uint32_t insp_loop_delay_per = 20;
uint32_t delay_take_sample_h = 0;
uint32_t delay_take_sample_per = 100;
uint32_t DCK_mon_start_cnt = 0;
uint32_t DCK_mon_start_per = 100;
float baslangic_voltaj_ms = 0;
uint32_t change_ispect_dir_cnt = 0;
uint32_t change_ispect_dir_per = 20;
uint32_t batt_inspection_req = 0;
uint32_t batt_inspection_step = 1;
int batt_inspection_direction = -1;
uint32_t restart_batt_inspection = 1;
uint32_t batt_inspection_start = 0;
uint32_t batt_insp_dir_neg_V = 0;
uint32_t vdif_status = 2;
uint32_t dir_change_v_up = 2;
uint32_t dir_change_v_dn = 2;
uint32_t can_change_inspect_dir = 1;
uint32_t batt_inspection_vchange = 1;
uint32_t batt_inspection_vchange_cnt = 0;
uint32_t can_reduce_voltage_0_2 = 40;
uint32_t decrease_vout_0_2_disp = 40;
uint32_t bat_inspection_unknow_state_cnt = 0;
#define BATT_CURRENT_DETECT_THRESHOLD 0.2f
uint32_t device_start_up_delay_completed_cnt = 0;
uint32_t batt_reverse_fc = 0;
uint32_t batt_not_connected_fc = 0;
uint32_t VDCK_pos_fc = 0;
uint32_t VDCK_neg_fc = 0;
uint32_t Eeprom_fc = 0;
uint32_t batt_reverse_Acc_cnt = 0;
uint32_t batt_current_detected_Acc_cnt = 0;
uint32_t batt_not_connected_Acc_cnt = 0;
uint32_t batt_current_zero_Acc_cnt = 0;
uint32_t batt_reverse_Acc_per = 20;
uint32_t batt_current_detected_Acc_per = 20;
uint32_t batt_not_connected_Acc_per = 20;
uint32_t batt_current_zero_Acc_per = 20;
uint32_t vout_sample_req = 0;
uint32_t batt_reverse_return_Acc_cnt = 0;
uint32_t batt_not_connected_return_Acc_cnt = 0;
uint32_t batt_reverse_return_Acc_per = 20;
uint32_t batt_not_connected_return_Acc_per = 20;

uint32_t VAC_R_Off_fc = 0;
uint32_t VAC_S_Off_fc = 0;
uint32_t VAC_T_Off_fc = 0;
uint32_t VAC_R_Hg_fc = 0;
uint32_t VAC_S_Hg_fc = 0;
uint32_t VAC_T_Hg_fc = 0;
uint32_t VAC_R_Lo_fc = 0;
uint32_t VAC_S_Lo_fc = 0;
uint32_t VAC_T_Lo_fc = 0;
uint32_t VAC_R_0_Ret_Acc_cnt = 0;
uint32_t VAC_S_0_Ret_Acc_cnt = 0;
uint32_t VAC_T_0_Ret_Acc_cnt = 0;
uint32_t VIN_Lo_Acc_cnt = 0;
uint32_t VAC_R_Hg_Acc_cnt = 0;
uint32_t VAC_S_Hg_Acc_cnt = 0;
uint32_t VAC_T_Hg_Acc_cnt = 0;
uint32_t VAC_R_Lo_Acc_cnt = 0;
uint32_t VAC_S_Lo_Acc_cnt = 0;
uint32_t VAC_T_Lo_Acc_cnt = 0;
uint32_t VIN_0_Acc_cnt = 0;
uint32_t VAC_R_0_Acc_cnt = 0;
uint32_t VAC_S_0_Acc_cnt = 0;
uint32_t VAC_T_0_Acc_cnt = 0;
uint32_t VAC_R_Hg_Ret_Acc_cnt = 0;
uint32_t VAC_S_Hg_Ret_Acc_cnt = 0;
uint32_t VAC_T_Hg_Ret_Acc_cnt = 0;
uint32_t VAC_R_Lo_Ret_Acc_cnt = 0;
uint32_t VAC_S_Lo_Ret_Acc_cnt = 0;
uint32_t VAC_T_Lo_Ret_Acc_cnt = 0;

uint32_t VAC_R_0_Acc_per = 2;
uint32_t VAC_S_0_Acc_per = 2;
uint32_t VAC_T_0_Acc_per = 2;
uint32_t VAC_R_0_Ret_Acc_per = 40;
uint32_t VAC_S_0_Ret_Acc_per = 40;
uint32_t VAC_T_0_Ret_Acc_per = 40;

uint32_t VAC_R_Hg_Acc_per = 2;
uint32_t VAC_S_Hg_Acc_per = 2;
uint32_t VAC_T_Hg_Acc_per = 2;
uint32_t VAC_R_Hg_Ret_Acc_per = 20;
uint32_t VAC_S_Hg_Ret_Acc_per = 20;
uint32_t VAC_T_Hg_Ret_Acc_per = 20;

uint32_t VAC_R_Lo_Acc_per = 20;
uint32_t VAC_S_Lo_Acc_per = 20;
uint32_t VAC_T_Lo_Acc_per = 20;
uint32_t VAC_R_Lo_Ret_Acc_per = 20;
uint32_t VAC_S_Lo_Ret_Acc_per = 20;
uint32_t VAC_T_Lo_Ret_Acc_per = 20;

uint32_t VLOAD_Lo_Acc_cnt = 0;
uint32_t VLOAD_Lo_Acc_per = 20;
uint32_t VLOAD_Lo_return_Acc_cnt = 0;
uint32_t VLOAD_Lo_return_Acc_per = 20;
uint32_t VLOAD_Hg_Acc_cnt = 0;
uint32_t VLOAD_Hg_Acc_per = 20;
uint32_t VLOAD_Hg_Ret_Acc_cnt = 0;
uint32_t VLOAD_Hg_Ret_Acc_per = 20;
uint32_t VLOAD_Lo_fc = 0;
uint32_t VLOAD_Hg_fc = 0;
uint32_t VLOAD_LoHg_Ret_Acc_cnt = 0;
uint32_t VLOAD_LoHg_Ret_Acc_per = 20;

uint32_t IBAT_Short_Acc_cnt = 0;
uint32_t IRECT_Short_Acc_cnt = 0;
uint32_t IBAT_Short_Ret_Acc_cnt = 0;
uint32_t IRECT_Short_Ret_Acc_cnt = 0;
uint32_t IBAT_Short_Acc_per = 40;
uint32_t IRECT_Short_Acc_per = 40;
uint32_t IBAT_Short_Ret_Acc_per = 20;
uint32_t IRECT_Short_Ret_Acc_per = 20;


uint32_t rectifier_current_limit_Acc_cnt = 0;
uint32_t battery_current_limit_Acc_cnt = 0;
uint32_t rectifier_current_limit_Acc_per = 20;
uint32_t battery_current_limit_Acc_per = 20;
uint32_t battery_current_limit_return_Acc_cnt = 0;
uint32_t rectifier_current_limit_return_Acc_cnt = 0;
uint32_t battery_current_limit_return_Acc_per = 20;
uint32_t rectifier_current_limit_return_Acc_per = 20;
uint32_t battery_current_limit_accepted = 0;
uint32_t rectifier_current_limit_accepted = 0;
uint32_t DC_leak_above_pos_lim = 0;
uint32_t DC_leak_below_neg_lim = 0;

// Each fault gets a single bit in a 32-bit integer.
//#define GENERAL_FAULT_LED     	 	(1U << 0)   // bit 0
//#define BATTERY_FAULT_LED		    (1U << 1)   // bit 1
//#define OVERTEMP_ALARM_LED  	    (1U << 2)   // bit 2
//#define OVERTEMP_OPEN_LED		    (1U << 3)   // bit 3
//#define BATTERY_CURRENT_LIMIT_LED   (1U << 4)   // bit 4
//#define RECTIFIER_CURRENT_LIMIT_LED (1U << 5)   // bit 5
//#define DC_LEAK_NEGATIVE_LED	    (1U << 6)   // bit 6
//#define DC_LEAK_POSITIVE_LED    	(1U << 7)   // bit 7
//#define DC_LW_LED				    (1U << 8)   // bit 8
//#define DC_HG_LED				    (1U << 9)   // bit 9
//#define VAC_LW_FAULT_LED	    (1U << 10)  // bit 10
//#define VAC_HG_FAULT_LED  	(1U << 11)  // bit 11
//#define STOP_LED				    (1U << 12)  // bit 12
//#define START_LED				    (1U << 13)  // bit 13
//#define VAC_OFF_LED			    (1U << 14)  // bit 14
//#define VAC_ON_LED			    (1U << 15)  // bit 15

#define LOAD_MCB_OFF_LED     	 	(1U << 0)   // bit 0
#define DROPPER_2_BYPASS_LED	    (1U << 1)   // bit 1
#define DROPPER_1_BYPASS_LED  	    (1U << 2)   // bit 2
#define BATT_MCB_OFF_LED		    (1U << 3)   // bit 3
#define BOOST_CHARGE_LED  			(1U << 4)   // bit 4
#define FLOAT_CHARGE_LED			(1U << 5)   // bit 5
#define INPUT_MCB_OFF_LED	    	(1U << 6)   // bit 6


typedef enum {
	GENERAL_FAULT_FC,
	BATTERY_FAULT_FC,
	OVERTEMP_ALARM_FC,
	OVERTEMP_OPEN_FC,
	BATTERY_CURRENT_LIMIT_FC,
	RECTIFIER_CURRENT_LIMIT_FC,
	DC_LEAK_NEGATIVE_FC,
	DC_LEAK_POSITIVE_FC,
	DC_LW_FC,
	DC_HG_FC,
	VAC_LO_FC,
	VAC_HG_FC,
	STOP_FC,
	START_FC,
	VAC_OFF_FC,
	VAC_ON_FC,
	RECT_SHORT_FC,
	BATT_SHORT_FC,
	BATT_REVERSE_FC,
	VAC_R_RMS_HG_FAULT_FC,
	VAC_S_RMS_HG_FAULT_FC,
	VAC_T_RMS_HG_FAULT_FC,
	VAC_R_RMS_LW_FAULT_FC,
	VAC_S_RMS_LW_FAULT_FC,
	VAC_T_RMS_LW_FAULT_FC,
	VAC_R_RMS_0_FAULT_FC,
	VAC_S_RMS_0_FAULT_FC,
	VAC_T_RMS_0_FAULT_FC,
	EEPROM_FAULT_FC,	// Kayit Sistemi Arz
    NUM_FAULTS
} FaultCode;


typedef enum {
	SET_GEN_F_LED,
	SAVE,
	THYSTOP,
	ACTIVE
} Fault_Code_Action_Bits;

typedef struct {
    FaultCode code;
    Fault_Code_Action_Bits action;
    const char *name;
} FaultInfo;
//  status   action   action  action
//  ACTIVE   THYSTOP  SAVE    SET_GEN_F_LED
FaultInfo faultList[] = {
    { GENERAL_FAULT_FC,           0b0010,	"Genel Arıza" },
	{ BATTERY_FAULT_FC,           0b0010,	"Akü Arızası" },
	{ OVERTEMP_ALARM_FC,          0b0110,	"Aşrı Sıckl Uyar" },
	{ OVERTEMP_OPEN_FC,           0b0110,	"Aşrı Sıckl Açık" },
	{ BATTERY_CURRENT_LIMIT_FC,   0b0000,	"Akü Akım Sınırı" },
	{ RECTIFIER_CURRENT_LIMIT_FC, 0b0000,	"Doğrltc Akm Sınr" },
	{ DC_LEAK_NEGATIVE_FC,        0b0010,	"DC Kaçak Negatif" },
	{ DC_LEAK_POSITIVE_FC,        0b0010,	"DC Kaçak Pozitif" },
	{ DC_LW_FC,                   0b0011,	"DC Düşük" },
	{ DC_HG_FC,                   0b0111,	"DC Yüksek" },
	{ VAC_LO_FC,                  0b0110,	"VIN RMS Düşük" },
	{ VAC_HG_FC,                  0b0110,	"VIN RMS Yüksek" },
	{ STOP_FC,                    0b0010,	"Manuel Durdur" },
	{ START_FC,                   0b0000,	"Başlat" },
	{ VAC_OFF_FC,                 0b0110,	"VAC OFF" },
	{ VAC_ON_FC,                  0b0000,	"VAC ON" },
	{ RECT_SHORT_FC,              0b0110,	"DC Kısa Devre" },
	{ BATT_SHORT_FC,              0b0110,	"Akü Kısa Devre" },
	{ BATT_REVERSE_FC,            0b0010,	"Akü Ters" },
	{ VAC_R_RMS_HG_FAULT_FC,      0b0000,	"VINR RMS Yüksek" },
	{ VAC_S_RMS_HG_FAULT_FC,      0b0000,	"VINS RMS Yüksek" },
	{ VAC_T_RMS_HG_FAULT_FC,      0b0000,	"VINT RMS Yüksek" },
	{ VAC_R_RMS_LW_FAULT_FC,      0b0000,	"VINR RMS Düşük" },
	{ VAC_S_RMS_LW_FAULT_FC,      0b0000,	"VINS RMS Düşük" },
	{ VAC_T_RMS_LW_FAULT_FC,      0b0000,	"VINT RMS Düşük" },
	{ VAC_R_RMS_0_FAULT_FC,       0b0000,	"VINR RMS Yok" },
	{ VAC_S_RMS_0_FAULT_FC,       0b0000,	"VINS RMS Yok" },
	{ VAC_T_RMS_0_FAULT_FC,       0b0000,	"VINT RMS Yok" },
	{ EEPROM_FAULT_FC,            0b0000,	"Kayit Sist Arz" }
};

#define NUM_FAULT_CODE_NAMES sizeof(faultList) / sizeof(faultList[0])


//typedef enum {
//	RESET,
//    SET
//} Fault_Code_Set;

uint8_t eep_hold_256bit[8] = {
0x1, 0x2, 0x3, 0x4,
0x5, 0x6, 0x7, 0x8};
uint8_t eep_test_read[8] = {0};
uint32_t eep_set_fault_threshold=1700000000;
uint32_t eep_addres_start=0x08040000;
uint32_t eep_addres_start_2=0x08040000;
uint32_t eep_addres_start_3=0x08060000;
uint32_t eep_addres_end=0x0807FFF8;
uint32_t eep_addres_after_end=0x08080000;
uint32_t eep_addres_empty=0;
uint32_t eep_data_settings[20][2]={{0, 0}};
uint8_t eep_hold_256bit_index = 0;
uint8_t eep_data_settings_index = 0;
//uint8_t eep_data_32 = 0;
volatile uint8_t EEP_reg_conf=0b10;
volatile uint8_t EEP_reg_safety=0b10;
volatile uint8_t EEP_reg_status=0b10;
volatile uint8_t EEP_reg_volatile=0b10;
#define PAGE_0_ADDRESS  0x002000  // Page 0 Address
#define CMD_WREN        0x06  // Write Enable
#define CMD_RDSR        0x05  // Read Status Register
#define CMD_WRSR 		0x01  // Write status and configuration registers
#define CMD_PGWR        0x02  // Page Write
#define CMD_RDVR        0x85  // Read Volatile Register.
#define CMD_WRVR        0x81  // Write Volatile Register. Bufen enables page write buffer.
#define CMD_RDCR  		0x15  // Read Configuration and Safety Registers Command
#define CMD_READ 		0x03  // Read Data Bytes command
#define CMD_PGPR  		0x0A  // Page Program
#define CMD_PGER  		0xDB  // Page Erase
#define CMD_SCER  		0x20  // Sector Erase
#define CMD_BKER  		0xD8  // Block Erase
#define CMD_CHER  		0xC7  // Chip Erase
uint32_t var1=0;
uint32_t var2=0;
//uint32_t var3=0;
//uint32_t var4=0;
//uint8_t var5=0;
//uint8_t var6=0;
//uint8_t var7=0;



#define NUM_FAULT_RECORD	20
uint32_t FAULT_RECORD_START_ADDRESS=0x000000;
uint32_t array_fault_data[NUM_FAULT_RECORD][2]={{0, 0}};
uint32_t flt_array_index_next = 0;
uint32_t flt_array_index_last = 0;
uint32_t disp_start = 0;
uint32_t flt_disp_index = 0;
uint32_t flt_array_index_found = 0;
uint32_t FAULT_CODES_REPORT_disp_mode = 0;





