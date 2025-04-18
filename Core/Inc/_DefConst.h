
#define BUZZ_P GPIOE, LL_GPIO_PIN_1
#define E1_1 LL_GPIO_SetOutputPin(GPIOE, LL_GPIO_PIN_1);
#define E1_0 LL_GPIO_ResetOutputPin(GPIOE, LL_GPIO_PIN_1);
//#define E12=1; LL_GPIO_SetOutputPin(GPIOE, LL_GPIO_PIN_12);
#define E15_P GPIOE, LL_GPIO_PIN_15
#define E2_P GPIOE, LL_GPIO_PIN_2
#define E3_P GPIOE, LL_GPIO_PIN_3
#define B12_0 LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_12);

#define E1(x) ((x) ? LL_GPIO_SetOutputPin(GPIOE, LL_GPIO_PIN_1) : LL_GPIO_ResetOutputPin(GPIOE, LL_GPIO_PIN_1))


// VARIABLES
///////////////////////////////////////////////////////////////////////////
#define NUM_CHANNELS 12
volatile uint32_t adc_buffer[NUM_CHANNELS];

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
float VRECT_smp_sc = 0.0f;
float VLOAD_smp_sc = 0.0f;
//float VLOAD_pas = 0.0f;
float VDCKP = 0.0f;
float VDCKN = 0.0f;
float VOUT_max_float = 55.2f;
float VOUT_max_boost = 62.4f;

float AUX1_smp = 0.0f;
float A_16_smp = 0.0f;
float VACR_smp = 0.0f;
float VACS_smp = 0.0f;
float VACT_smp = 0.0f;
float DCKP_smp = 0.0f;
float DCKN_smp = 0.0f;
float VBAT_smp = 0.0f;
float VLOAD_smp = 0.0f;
float IRECT_smp = 0.0f;
float IBAT_smp = 0.0f;
float VRECT_smp = 0.0f;


float VLOAD_smp_gain = 0.00291575833333f;
float VRECT_smp_gain = 0.00291575833333f;
float VLOAD_smp_offst = 0.0f;
float VRECT_smp_offst = 0.0f;


float VRECT_DC_HIGH_LIM = 0.0f;
float VRECT_DC_HIGH_LIM_ret = 0.0f;
uint32_t VRECT_DC_HIGH_LIM_Acc_cnt = 0;
uint32_t VRECT_DC_HIGH_LIM_Acc_per = 200;
uint32_t VRECT_DC_HIGH_LIM_ret_Acc_cnt = 0;
uint32_t VRECT_DC_HIGH_LIM_ret_Acc_per = 20;
float VRECT_DC_LOW_LIM = 0.0f;
float VRECT_DC_LOW_LIM_ret = 0.0f;
uint32_t VRECT_DC_LOW_LIM_Acc_cnt = 0;
uint32_t VRECT_DC_LOW_LIM_Acc_per = 200;
uint32_t VRECT_DC_LOW_LIM_ret_Acc_cnt = 0;
uint32_t VRECT_DC_LOW_LIM_ret_Acc_per = 20;
float VLOAD_DC_HIGH_LIM = 0.0f;
float VLOAD_DC_HIGH_LIM_ret = 0.0f;
uint32_t VLOAD_DC_HIGH_LIM_Acc_cnt = 0;
uint32_t VLOAD_DC_HIGH_LIM_Acc_per = 200;
uint32_t VLOAD_DC_HIGH_LIM_ret_Acc_cnt = 0;
uint32_t VLOAD_DC_HIGH_LIM_ret_Acc_per = 20;
float VLOAD_DC_LOW_LIM = 0.0f;
float VLOAD_DC_LOW_LIM_ret = 0.0f;
uint32_t VLOAD_DC_LOW_LIM_Acc_cnt = 0;
uint32_t VLOAD_DC_LOW_LIM_Acc_per = 200;
uint32_t VLOAD_DC_LOW_LIM_ret_Acc_cnt = 0;
uint32_t VLOAD_DC_LOW_LIM_ret_Acc_per = 20;
float IRECT_smp_sc = 0.0f;
float ILOAD_per_sc = 0.0f;
//float IRECT_f = 0.0f;
float VBAT_smp_sc = 0.0f;
float IBAT_smp_sc = 0.0f;
//float IBAT_f = 0.0f;
float VBAT_f = 0.0f;
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

uint32_t EXTI_Zero_crossing = 0;
float IRECT_sum_sc = 0;
float IRECT_avg_sc = 0;
float IRECT_lim = 15;
uint32_t IRECT_smp_count = 0;
uint32_t IRECT_samp_end = 0;
float IRECT_zero = 32784;
//uint32_t IRECT_zero_req = 0;
//uint32_t IRECT_zero_req_start = 0;
//uint32_t IRECT_zero_req_start_cnt = 0;
//uint32_t IRECT_zero_req_ok = 1;
//uint32_t IRECT_zero_ok_cnt = 0;

//float IBAT_pas = 0.0f;
//float ILOAD_pas = 0.0f;
typedef struct {
    float a128; // 128-length rolling average
    float a64; // 64-length rolling average
    float a16; // 16-length rolling average
    float a1; // 1-length rolling average
} RollingAverage;

// Declare a variable for IBAT_per_avg_roll_sc
RollingAverage VRECT_pas; // pas period average scaled
RollingAverage VLOAD_pas;
RollingAverage IBAT_pas;
RollingAverage ILOAD_pas;
RollingAverage IRECT_pas;
RollingAverage VBAT_pas;
RollingAverage VAC_R_rms_roll_per_avg;
RollingAverage VAC_S_rms_roll_per_avg;
RollingAverage VAC_T_rms_roll_per_avg;
RollingAverage IAC_R_rms_roll_per_avg;
RollingAverage IAC_S_rms_roll_per_avg;
RollingAverage IAC_T_rms_roll_per_avg;

float blm_smp_buff_sum_ = 0;

float IBAT_sum_sc = 0;
float ILOAD_sum_sc = 0;
float IBAT_avg = 0;
float IBAT_lim = 15;
//float IBAT_per_avg_roll_sc = 0;
//float IBAT_per_avg_roll2_sc = 0;
uint32_t IBAT_smp_count = 0;
uint32_t ILOAD_smp_count = 0;
uint32_t IBAT_samp_end = 0;
float IBAT_zero = 32768;
//uint32_t IBAT_zero_req = 0;
//uint32_t IBAT_zero_req_start = 1;
//uint32_t IBAT_zero_req_start_cnt = 0;
//uint32_t IBAT_zero_req_ok = 1;
//uint32_t IBAT_zero_ok_cnt = 0;

float VBAT_sum_sc = 0;
//float VBAT_pas = 0;
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
//float VRECT_pas = 0;
float VRECT_per_avg = 0;
float VDCKP_per_avg_roll = 0;
float VDCKN_per_avg_roll = 0;
float VACR_smp_count = 0;
float VACS_smp_count = 0;
float VACT_smp_count = 0;
float VACR_sum_of_sqr_sc = 0;
float VACS_sum_of_sqr_sc = 0;
float VACT_sum_of_sqr_sc = 0;
float VACR_smp_sc = 0;
float VACS_smp_sc = 0;
float VACT_smp_sc = 0;
float VAC_R_rms_sc = 0;
float VAC_S_rms_sc = 0;
float VAC_T_rms_sc = 0;
float VRECT_per_avg_roll = 0;
float VRECT_per_avg_roll_half = 0;
float VDCKP_avg2_s = 0;
float VDCKN_avg2_s = 0;
uint32_t VDCKP_smp_count = 0;
uint32_t VDCKN_smp_count = 0;
uint32_t VAC_R_smp_count = 0;
uint32_t VAC_S_smp_count = 0;
uint32_t VAC_T_smp_count = 0;
uint32_t VRECT_smp_count = 0;
uint32_t VLOAD_smp_count = 0;
uint32_t VDCKP_samp_end = 0;
uint32_t VDCKN_samp_end = 0;
uint32_t VAC_R_samp_end = 0;
uint32_t VAC_R_samp_end2 = 0;
uint32_t VAC_S_samp_end = 0;
uint32_t VAC_T_samp_end = 0;
uint32_t VRECT_samp_end = 0;
uint32_t VLOAD_samp_end = 0;
float VDCKP_sum = 0;
float VDCKN_sum = 0;
float VLOAD_sum_sc = 0;
float VRECT_sum_sc = 0;
uint32_t VRECT_sum = 0;
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
uint32_t VDCK_accept_cnt = 0;
uint32_t VDCK_return_cnt = 0;
uint32_t VDCK_accept_per = 40;
uint32_t VDCK_return_per = 40;

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
	DROPPER_pg,
	RELAY_ORDER_pg,
	FAULT_CODES_RESET_pg,
	DEVICE_RESET_pg,
	CALIBRATION_pg,
	DATE_TIME_pg,
	BATT_REVERSE_DETECT_pg,
	CONT_SYS_pg,
    NUM_PAGES
} MenuPage;

MenuPage currentPage = HOME_PAGE_pg;
uint8_t HOME_PAGE_pg_sel = 2;

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
    "Dropper",
    "Kontak Çıkışları",	// RELAY_ORDER_pg
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
uint8_t dropper_edit_mode = 0;
uint8_t selected_DROPPER = 0;

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
	SET_VRECT_CAL,
	SET_VLOAD_CAL,
	SET_VBAT_CAL,
	SET_IRECT_CAL,
	SET_IBAT_CAL,
	SET_ACR_CAL,
	SET_ACS_CAL,
	SET_ACT_CAL,
	SET_VRECT_OFFS_CAL,
	SET_VLOAD_OFFS_CAL,
	SET_VBAT_OFFS_CAL,
	SET_IRECT_OFFS_CAL,
	SET_IBAT_OFFS_CAL,
	SET_ACR_OFFS_CAL,
	SET_ACS_OFFS_CAL,
	SET_ACT_OFFS_CAL,
	SET_UNSEEN_FLT,
	SET_BATT_REV_DET,
	SET_BATT_DISC_DET,
	SET_DROPPER_K1,
	SET_DROPPER_K2,
	SET_OVTM_ALRM_LIM,
	SET_OVTM_OPEN_DUR,
	SET_OVTM_OPEN_LIM,
	DEV_NOM_VOUT, 		// Cihaz Nom VDC
	DEV_NOM_IOUT, 		// Cihaz Nom IDC rect out
	BATT_NOM_IOUT,
	DC_KAC_POS, 			// DC kaçak pozitif
	DC_KAC_NEG, 			// DC kaçak negatif
	RECT_SHORT,
	BATT_SHORT,
	REL_OUT_1,
	REL_OUT_2,
	REL_OUT_3,
	REL_OUT_4,
	VRECT_DC_HIGH_LIM_ADD,
	VRECT_DC_LOW_LIM_ADD,
	VLOAD_DC_HIGH_LIM_ADD,
	VLOAD_DC_LOW_LIM_ADD,
    NUM_SET_ENUM            // Keeps track of total settings
} EEPROM_Setting_ID;

//uint32_t SETTING_RECORD_START_ADDRESS=0x300000; // 3145728
EEPROM_Data_Type EpD[NUM_SET_ENUM][2] = {
    { {SETTING_RECORD_START_ADDRESS, 3145728.0}, {SETTING_RECORD_START_ADDRESS, 3145728.0} },
    { {SET_CHARGE_MODE, 0.0}, {SET_CHARGE_MODE, 0.0} },
    { {SET_BOOST_TIME, 4.0}, {SET_BOOST_TIME, 4.0} },
    { {VBAT_FLOAT, 52.8}, {VBAT_FLOAT, 52.8} },
    { {VBAT_BOOST, 55.2}, {VBAT_BOOST, 55.2} },
    { {SET_IBAT_FLOAT, 4.0}, {SET_IBAT_FLOAT, 4.0} },
    { {SET_IBAT_BOOST, 10.0}, {SET_IBAT_BOOST, 10.0} },
    { {IRECT_LIM_RT_, 30.0}, {IRECT_LIM_RT_, 30.0} },
    { {I_LIM_TO_FLOAT, 3.0}, {I_LIM_TO_FLOAT, 3.0} },
    { {I_LIM_TO_BOOST, 4.0}, {I_LIM_TO_BOOST, 4.0} },
    { {SET_VRECT_CAL, 0.00326439226}, {SET_VRECT_CAL, 0.00326439226} },
    { {SET_VLOAD_CAL, 0.003243}, {SET_VLOAD_CAL, 0.003243} },
    { {SET_VBAT_CAL, 0.006696}, {SET_VBAT_CAL, 0.006696} },
    { {SET_IRECT_CAL, 0.00092}, {SET_IRECT_CAL, 0.00092} },
    { {SET_IBAT_CAL, 0.001004}, {SET_IBAT_CAL, 0.001004} },
    { {SET_ACR_CAL, 0.021641}, {SET_ACR_CAL, 0.021641} },
    { {SET_ACS_CAL, 0.021758}, {SET_ACS_CAL, 0.021758} },
    { {SET_ACT_CAL, 0.021976}, {SET_ACT_CAL, 0.021976} },
    { {SET_VRECT_OFFS_CAL, 0.0}, {SET_VRECT_OFFS_CAL, 0.0} },
    { {SET_VLOAD_OFFS_CAL, 0.0}, {SET_VLOAD_OFFS_CAL, 0.0} },
    { {SET_VBAT_OFFS_CAL, -139.0}, {SET_VBAT_OFFS_CAL, -139.0} },
    { {SET_IRECT_OFFS_CAL, 0.0}, {SET_IRECT_OFFS_CAL, 0.0} },
    { {SET_IBAT_OFFS_CAL, 0.0}, {SET_IBAT_OFFS_CAL, 0.0} },
    { {SET_ACR_OFFS_CAL, -116.0}, {SET_ACR_OFFS_CAL, -116.0} },
    { {SET_ACS_OFFS_CAL, -129.0}, {SET_ACS_OFFS_CAL, -129.0} },
    { {SET_ACT_OFFS_CAL, -139.0}, {SET_ACT_OFFS_CAL, -139.0} },
    { {SET_UNSEEN_FLT, 0.0}, {SET_UNSEEN_FLT, 0.0} },
    { {SET_BATT_REV_DET, 1.0}, {SET_BATT_REV_DET, 1.0} },
    { {SET_BATT_DISC_DET, 0.0}, {SET_BATT_DISC_DET, 0.0} },
    { {SET_DROPPER_K1, 0.0}, {SET_DROPPER_K1, 0.0} },
    { {SET_DROPPER_K2, 0.0}, {SET_DROPPER_K2, 0.0} },
    { {SET_OVTM_ALRM_LIM, 80.0}, {SET_OVTM_ALRM_LIM, 80.0} },
    { {SET_OVTM_OPEN_DUR, 120.0}, {SET_OVTM_OPEN_DUR, 120.0} },
    { {SET_OVTM_OPEN_LIM, 90.0}, {SET_OVTM_OPEN_LIM, 90.0} },
    { {DEV_NOM_VOUT, 48.0}, {DEV_NOM_VOUT, 48.0} }, // Cihaz Nom VDC
    { {DEV_NOM_IOUT, 40.0}, {DEV_NOM_IOUT, 40.0} },
    { {BATT_NOM_IOUT, 40.0}, {BATT_NOM_IOUT, 40.0} }, //
    { {DC_KAC_POS, 20.0}, {DC_KAC_POS, 20.0} }, // DC kaçak pozitif
    { {DC_KAC_NEG, 20.0}, {DC_KAC_NEG, 20.0} }, // DC kaçak negatif
    { {RECT_SHORT, 20.0}, {RECT_SHORT, 20.0} }, // RECT short Amp
    { {BATT_SHORT, 20.0}, {BATT_SHORT, 20.0} }, // BATT short Amp
    { {REL_OUT_1, 657440.0}, {REL_OUT_1, 657440.0} },
    { {REL_OUT_2, 235701.0}, {REL_OUT_2, 235701.0} },
    { {REL_OUT_3, 370984.0}, {REL_OUT_3, 370984.0} },
    { {REL_OUT_4, 506284.0}, {REL_OUT_4, 506284.0} },
    { {VRECT_DC_HIGH_LIM_ADD, 10.0}, {VRECT_DC_HIGH_LIM_ADD, 10.0} },
    { {VRECT_DC_LOW_LIM_ADD, 10.0}, {VRECT_DC_LOW_LIM_ADD, 10.0} },
    { {VLOAD_DC_HIGH_LIM_ADD, 10.0}, {VLOAD_DC_HIGH_LIM_ADD, 10.0} },
    { {VLOAD_DC_LOW_LIM_ADD, 10.0}, {VLOAD_DC_LOW_LIM_ADD, 10.0} }
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
    "SET_VRECT_CAL",
    "SET_VLOAD_CAL",
    "SET_VBAT_CAL",
    "SET_IRECT_CAL",
    "SET_IBAT_CAL",
    "SET_ACR_CAL",
    "SET_ACS_CAL",
    "SET_ACT_CAL",
    "SET_VRECT_OFFS_CAL",
    "SET_VLOAD_OFFS_CAL",
    "SET_VBAT_OFFS_CAL",
    "SET_IRECT_OFFS_CAL",
    "SET_IBAT_OFFS_CAL",
    "SET_ACR_OFFS_CAL",
    "SET_ACS_OFFS_CAL",
    "SET_ACT_OFFS_CAL",
    "SET_UNSEEN_FLT",
    "SET_BATT_REV_DET",
    "SET_BATT_DISC_DET",
    "SET_DROPPER_K1",
    "SET_DROPPER_K2",
    "SET_OVTM_ALRM_LIM",
    "SET_OVTM_OPEN_DUR",
    "SET_OVTM_OPEN_LIM",
    "DEV_NOM_VOUT", // Cihaz Nom VDC
    "DEV_NOM_IOUT",
    "BATT_NOM_IOUT",
    "DC_KAC_POS",	 	// DC kaçak pozitif
    "DC_KAC_NEG", 		// DC kaçak negatif
    "RECT_SHORT",
    "BATT_SHORT",
    "REL_OUT_1",
	"REL_OUT_2",
	"REL_OUT_3",
	"REL_OUT_4",
	"VRECT_DC_HIGH_LIM_ADD",
	"VRECT_DC_LOW_LIM_ADD",
	"VLOAD_DC_HIGH_LIM_ADD",
	"VLOAD_DC_LOW_LIM_ADD"
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
{"Akü Hat Kopuk", SET_BATT_DISC_DET, 2},
{"Sıck Alrm C", SET_OVTM_ALRM_LIM, 3},
{"Sıck Açma Sur", SET_OVTM_OPEN_DUR, 2},
{"Sıck Açma C", SET_OVTM_OPEN_LIM, 3},
{"Cihaz Nom VDC", DEV_NOM_VOUT, 3},
{"I Doğrlt Max", IRECT_LIM_RT_, 3},
{"DC Kaçak +% L", DC_KAC_POS, 3},
{"DC Kaçak -% L", DC_KAC_NEG, 3},
{"K.Devr Doğr A", RECT_SHORT, 3},
{"K.Devr Bat A", BATT_SHORT, 3},
{"Doğ VDC Üst L%", VRECT_DC_HIGH_LIM_ADD, 3},
{"Doğ VDC Alt L%", VRECT_DC_LOW_LIM_ADD, 3},
{"Yük VDC Üst L%", VLOAD_DC_HIGH_LIM_ADD, 3},
{"Yük VDC Alt L%", VLOAD_DC_LOW_LIM_ADD, 3}
};
#define NUM_DEVICE_SETT_ITEMS (sizeof(DEVICE_SETT_Items) / sizeof(DEVICE_SETT_Items[0]))
uint8_t selected_DEVICE_SETT = 0;
uint8_t dev_set_disp_index =NUM_DEVICE_SETT_ITEMS-1;
uint8_t dev_set_arrow_loc = 1;
uint8_t dev_setting_edit_mode = 0;

const char* MANAGEMENT_Items[] = {
"Arıza Kaytları Sil",
"Cihaz Yeniden Başlat",
"Kalibrasyon"
};
#define NUM_MANAGEMENT_ITEMS (sizeof(MANAGEMENT_Items) / sizeof(MANAGEMENT_Items[0]))
uint8_t selected_MANAGEMENT = 0;

typedef enum {
	cal_none,
	cal_gain,
	cal_offset
} cal_edit_mode;

uint32_t cal_sel_col=0;
uint32_t cal_sel_item_left=0;
uint32_t cal_sel_item_right=0;
uint32_t cal_sel_digit=0;
uint32_t cal_sel_edit_mode=0;



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
float Vbat_flt=0; // updated at startup and when DEV_NOM_VOUT changed
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

float VAC_Nom=398.0f;
volatile float VAC_Hg_Lim=0.0f;
float VAC_Lo_Lim=0.0f;


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
uint32_t bat_inspection_allowed = 1;
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
uint32_t device_start_up_delay_completed_cnt = 0;
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

//#define LOAD_MCB_OFF_LED     	 	(1U << 0)   // bit 0
//#define DROPPER_2_BYPASS_LED	    (1U << 1)   // bit 1
//#define DROPPER_1_BYPASS_LED  	    (1U << 2)   // bit 2
//#define BATT_MCB_OFF_LED		    (1U << 3)   // bit 3
#define BOOST_CHARGE_LED  			(1U << 4)   // bit 4
#define FLOAT_CHARGE_LED			(1U << 5)   // bit 5
//#define INPUT_MCB_OFF_LED	    	(1U << 6)   // bit 6

uint8_t SW_LINE_OFF=0;
uint8_t SW_BATT_OFF=0;
uint8_t SW_LOAD_OFF=0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// STATE CHANGES ////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum {
	GENERAL_FAULT_FC,
	BATTERY_FAULT_FC,
	OVERTEMP_ALARM_FC,
	OVERTEMP_OPEN_FC,
	BATTERY_CURRENT_LIMIT_FC,
	RECTIFIER_CURRENT_LIMIT_FC,
	DC_LEAK_NEGATIVE_FC,
	DC_LEAK_POSITIVE_FC,
	RECT_DC_LW_FC,
	RECT_DC_HG_FC,
	VAC_LO_FC,
	VAC_HG_FC,
	STOP_FC,
	START_FC,
	VAC_OFF_FC,
	VAC_ON_FC, // led 16
	LOAD_FUSE_OFF_FC,
	DROPPER2_BYP_FC,
	DROPPER1_BYP_FC,
	BATT_FUSE_OFF_FC,
	BOOST_CHARGE_FC,
	FLOAT_CHARGE_FC,
	LINE_FUSE_OFF_FC, // led 7
	RECT_SHORT_FC,
	BATT_SHORT_FC,
	BATT_REVERSE_FC,
	BATT_LINE_BROKEN_FC,
	BAT_TEMP_ZERO_FC,
	BAT_TEMP_50_FC,
	AUX_REL1_REL,
    THY_FAN1_REL,
    TRF_FAN2_REL,
    DROP_FAN3_REL,
    AC_CON1_REL,
    AC_CON2_REL,
    DROP_CON1_REL,
    DROP_CON2_REL,
	LOAD_DC_HG_FC,
	LOAD_DC_LW_FC,
	EEPROM_FAULT_FC,
	RTC_FAULT_FC,
	VAC_R_RMS_HG_FAULT_FC,
	VAC_S_RMS_HG_FAULT_FC,
	VAC_T_RMS_HG_FAULT_FC,
	VAC_R_RMS_LW_FAULT_FC,
	VAC_S_RMS_LW_FAULT_FC,
	VAC_T_RMS_LW_FAULT_FC,
	VAC_R_RMS_0_FAULT_FC,
	VAC_S_RMS_0_FAULT_FC,
	VAC_T_RMS_0_FAULT_FC
} State_Codes;

typedef enum {
	SET_GEN_F_LED_enum,
	SAVE_enum,
	THYSTOP_enum,
	ACTIVE_enum,
	REL_ACTV_enum
} State_Code_Action_Bits;

typedef enum {
    START_STOP_REL,
    VAC_HG_FC_REL,
    VAC_LO_FC_REL,
    LOAD_DC_HG_FC_REL,
	LOAD_DC_LW_FC_REL,
    DC_LEAK_POSITIVE_FC_REL,
    DC_LEAK_NEGATIVE_FC_REL,
    LINE_FUSE_OFF_FC_REL,
    BATT_FUSE_OFF_FC_REL,
    LOAD_FUSE_OFF_FC_REL,
    OVERTEMP_ALARM_FC_REL,
    FAN_FAULT_FC_REL,
    BATT_LINE_BROKEN_FC_REL,
    BATT_REVERSE_FC_REL,
    BATTERY_FAULT_FC_REL,
    GENERAL_FAULT_FC_REL,
    VAC_OFF_FC_REL,
    VAC_ON_FC_REL,
    BOOST_CHARGE_FC_REL,
    FLOAT_CHARGE_FC_REL,
    RECT_DC_HG_FC_REL,
    RECT_DC_LW_FC_REL,
	OVERTEMP_OPEN_FC_REL,
	BAT_TEMP_ZERO_FC_REL,
	BAT_TEMP_50_FC_REL,
	EMPTY_REL,
    NUM_REL_CODES
} rel_names_t;

typedef struct {
	State_Codes code;
    uint8_t action;
    const char *name;
	rel_names_t rel_dat_nm;
} State_Info;
//  status   action   action  action
//  ACTIVE   THYSTOP  SAVE    SET_GEN_F_LED
State_Info state_list[] = {
	// LED 16 BIT0 enum 0
    { GENERAL_FAULT_FC,           0b0010,	"Genel Arıza",         GENERAL_FAULT_FC_REL }, // 4. sütun out relay enum u
	{ BATTERY_FAULT_FC,           0b0000,	"Akü Arızası",         BATTERY_FAULT_FC_REL },
	{ OVERTEMP_ALARM_FC,          0b0010,	"Aşrı Sıckl Uyar",     OVERTEMP_ALARM_FC_REL },
	{ OVERTEMP_OPEN_FC,           0b0111,	"Aşrı Sıckl Açık",     OVERTEMP_OPEN_FC_REL },
	{ BATTERY_CURRENT_LIMIT_FC,   0b0000,	"Akü Akım Sınırı",     NUM_REL_CODES },
	{ RECTIFIER_CURRENT_LIMIT_FC, 0b0000,	"Doğrltc Akm Sınr",    NUM_REL_CODES },
	{ DC_LEAK_NEGATIVE_FC,        0b0010,	"DC Kaçak Negatif",    DC_LEAK_NEGATIVE_FC_REL },
	{ DC_LEAK_POSITIVE_FC,        0b0010,	"DC Kaçak Pozitif",    DC_LEAK_POSITIVE_FC_REL },
	{ RECT_DC_LW_FC,              0b0011,	"Doğr VDC Düşük",      RECT_DC_LW_FC_REL },
	{ RECT_DC_HG_FC,              0b0111,	"Doğr VDC Yüksk",      RECT_DC_HG_FC_REL },
	{ VAC_LO_FC,                  0b0110,	"Şebeke Düşük",        VAC_LO_FC_REL },
	{ VAC_HG_FC,                  0b0110,	"Şebeke Yüksek",       VAC_HG_FC_REL },
	{ STOP_FC,                    0b0010,	"Manuel Durdur",       NUM_REL_CODES },
	{ START_FC,                   0b0000,	"Başlat",              NUM_REL_CODES },
	{ VAC_OFF_FC,                 0b0110,	"VAC OFF",             VAC_OFF_FC_REL },
	{ VAC_ON_FC,                  0b0000,	"VAC ON",              VAC_ON_FC_REL },
	// LED 16 BIT15	enum 15
	// LED 7 BIT0 enum 16
	{ LOAD_FUSE_OFF_FC,      	  0b0010,	"Çıkış Sigrt Atık",    LOAD_FUSE_OFF_FC_REL },
	{ DROPPER2_BYP_FC,      	  0b0000,	"Dropper 2 Bypass",    NUM_REL_CODES },
	{ DROPPER1_BYP_FC,      	  0b0000,	"Dropper 1 Bypass",    NUM_REL_CODES },
	{ BATT_FUSE_OFF_FC,           0b0010,	"Akü Sigorta Atık",    BATT_FUSE_OFF_FC_REL },
	{ BOOST_CHARGE_FC,      	  0b0000,	"Hızlı Şarj",          BOOST_CHARGE_FC_REL },
	{ FLOAT_CHARGE_FC,      	  0b0000,	"Normal Şarj",         FLOAT_CHARGE_FC_REL },
	{ LINE_FUSE_OFF_FC,      	  0b0110,	"Giriş Sigrt Atık",    LINE_FUSE_OFF_FC_REL },
	// LED 7 BIT6 enum 22
	{ RECT_SHORT_FC,              0b0110,	"DC Kısa Devre",       NUM_REL_CODES },
	{ BATT_SHORT_FC,              0b0110,	"Akü Kısa Devre",      NUM_REL_CODES },
	{ BATT_REVERSE_FC,            0b0010,	"Akü Ters",            BATT_REVERSE_FC_REL },
	{ BATT_LINE_BROKEN_FC,        0b0010,	"Akü Hattı Kopuk",     BATT_LINE_BROKEN_FC_REL },
	{ BAT_TEMP_ZERO_FC,           0b0011,	"Akü Sıcaklk Sıfr",    BAT_TEMP_ZERO_FC_REL },
	{ BAT_TEMP_50_FC,          	  0b0111,	"Akü Sıcaklk 50 C",    BAT_TEMP_50_FC_REL },
	// REL MB 8 BIT0 enum 31
	{ AUX_REL1_REL,      		  0b0000,	"Aux Rel 1",           NUM_REL_CODES },
	{ THY_FAN1_REL,     		  0b0000,	"Tristör Fan 1",       NUM_REL_CODES },
	{ TRF_FAN2_REL,    			  0b0000,	"Trafo Fan 2",         NUM_REL_CODES },
	{ DROP_FAN3_REL,     		  0b0000,	"Dropper Fan 3",       NUM_REL_CODES },
	{ AC_CON1_REL,    			  0b0000,	"AC KONT1 RÖL",        NUM_REL_CODES },
	{ AC_CON2_REL,    			  0b0000,	"AC KONT2 RÖL",        NUM_REL_CODES },
	{ DROP_CON1_REL,    		  0b0000,	"Dropper 1 Bypass",    NUM_REL_CODES },
	{ DROP_CON2_REL,    		  0b0000,	"Dropper 2 Bypass",    NUM_REL_CODES },
	// REL MB 8 BIT7 enum 36
	{ LOAD_DC_HG_FC,              0b0111,	"Yük VDC Yüksk",       LOAD_DC_HG_FC_REL },
	{ LOAD_DC_LW_FC,              0b0011,	"Yük VDC Düşük",       LOAD_DC_LW_FC_REL },
	{ EEPROM_FAULT_FC,            0b0000,	"Kayit Sist Arz",      NUM_REL_CODES },
	{ RTC_FAULT_FC,               0b0000,	"RTC Arz",             NUM_REL_CODES },
	{ VAC_R_RMS_HG_FAULT_FC,      0b0000,	"VINR RMS Yüksek",     NUM_REL_CODES },
	{ VAC_S_RMS_HG_FAULT_FC,      0b0000,	"VINS RMS Yüksek",     NUM_REL_CODES },
	{ VAC_T_RMS_HG_FAULT_FC,      0b0000,	"VINT RMS Yüksek",     NUM_REL_CODES },
	{ VAC_R_RMS_LW_FAULT_FC,      0b0000,	"VINR RMS Düşük",      NUM_REL_CODES },
	{ VAC_S_RMS_LW_FAULT_FC,      0b0000,	"VINS RMS Düşük",      NUM_REL_CODES },
	{ VAC_T_RMS_LW_FAULT_FC,      0b0000,	"VINT RMS Düşük",      NUM_REL_CODES },
	{ VAC_R_RMS_0_FAULT_FC,       0b0000,	"VINR RMS Yok",        NUM_REL_CODES },
	{ VAC_S_RMS_0_FAULT_FC,       0b0000,	"VINS RMS Yok",        NUM_REL_CODES },
	{ VAC_T_RMS_0_FAULT_FC,       0b0000,	"VINT RMS Yok",        NUM_REL_CODES },
};


#define NUM_STATE_NAMES sizeof(state_list) / sizeof(state_list[0])



rel_names_t REL_OUT_order[16]; // sıralama buna kaydediliyor
uint32_t REL_OUT_order_part1 = 0;  // 20 bit. float olarak kaydedildiği için 24 bit max
uint32_t REL_OUT_order_part2 = 0;  // 20 bit
uint32_t REL_OUT_order_part3 = 0;  // 20 bit
uint32_t REL_OUT_order_part4 = 0;  // 20 bit



typedef enum {
	REL_NO,
	REL_NC
} NO_NC;

typedef struct {
	rel_names_t rel_dat_nm;
    uint8_t rel_dat_val;
    uint8_t rel_dat_NO_NC;
    char* rel_dat_desc;
} rel_dat_stru;

rel_dat_stru rel_dat_tb[] = {
	{ START_STOP_REL,            0, REL_NO, "Başlat-Durdur" },
	{ VAC_HG_FC_REL,             0, REL_NO, "Şebeke Yüksek" },
	{ VAC_LO_FC_REL,             0, REL_NO, "Şebeke Düşük" },
	{ LOAD_DC_HG_FC_REL,         0, REL_NO, "Yük VDC Yüksk" },
	{ LOAD_DC_LW_FC_REL,         0, REL_NO, "Yük VDC Düşük" },
	{ DC_LEAK_POSITIVE_FC_REL,   0, REL_NO, "DC Kaçak Poztf" },
	{ DC_LEAK_NEGATIVE_FC_REL,   0, REL_NO, "DC Kaçak Negtf" },
	{ LINE_FUSE_OFF_FC_REL,      0, REL_NO, "Giriş Sigr Off" },
	{ BATT_FUSE_OFF_FC_REL,      0, REL_NO, "Akü Sigrta Off" },
	{ LOAD_FUSE_OFF_FC_REL,      0, REL_NO, "Yük Sigrt Atık" },
	{ OVERTEMP_ALARM_FC_REL,     0, REL_NO, "Aşrı Sıck Uyar" },
	{ FAN_FAULT_FC_REL,          0, REL_NO, "Fan Arızası" },
	{ BATT_LINE_BROKEN_FC_REL,   0, REL_NO, "Akü Hattı Kopuk" },
	{ BATT_REVERSE_FC_REL,       0, REL_NO, "Akü Ters" },
	{ BATTERY_FAULT_FC_REL,      0, REL_NO, "Akü Arızası" },
	{ GENERAL_FAULT_FC_REL,      0, REL_NO, "Genel Arıza" },
	{ VAC_OFF_FC_REL,            0, REL_NO, "Şebeke Yok" },
	{ VAC_ON_FC_REL,             0, REL_NO, "Şebeke Var" },
	{ BOOST_CHARGE_FC_REL,       0, REL_NO, "Hızlı Şarj" },
	{ FLOAT_CHARGE_FC_REL,       0, REL_NO, "Normal Şarj" },
	{ RECT_DC_HG_FC_REL,         0, REL_NO, "Doğr VDC Yüksk" },
	{ RECT_DC_LW_FC_REL,         0, REL_NO, "Doğr VDC Düşük" },
	{ OVERTEMP_OPEN_FC_REL,      0, REL_NO, "Aşrı Sıck Açma" },
	{ BAT_TEMP_ZERO_FC_REL,      0, REL_NO, "Akü Sıck Sıfr C" },
	{ BAT_TEMP_50_FC_REL,        0, REL_NO, "Akü Sıck 50 C" },
	{ EMPTY_REL,        		 0, REL_NO, "     Boş" }
};

typedef struct {
	rel_names_t rel_ord_nm;
    uint8_t rel_ord_order;
    uint8_t rel_ord_val;
    uint8_t rel_dat_NO_NC;
    char* rel_ord_desc;
} rel_ord_st;

rel_ord_st rel_ord_tb[] = {
    { START_STOP_REL,            1,  0, REL_NO, "Başlat-Durdur" },
    { VAC_HG_FC_REL,             2,  0, REL_NO, "Şebeke Yüksek" },
    { VAC_LO_FC_REL,             3,  0, REL_NO, "Şebeke Düşük" },
    { LOAD_DC_HG_FC_REL,         4,  0, REL_NO, "Yük VDC Yüksk" },
    { LOAD_DC_LW_FC_REL,         5,  0, REL_NO, "Yük VDC Düşük" },
    { DC_LEAK_POSITIVE_FC_REL,   6,  0, REL_NO, "DC Kaçak Poztf" },
    { DC_LEAK_NEGATIVE_FC_REL,   7,  0, REL_NO, "DC Kaçak Negtf" },
    { LINE_FUSE_OFF_FC_REL,      8,  0, REL_NO, "Giriş Sigr Off" },
    { BATT_FUSE_OFF_FC_REL,      9,  0, REL_NO, "Akü Sigrta Off" },
    { LOAD_FUSE_OFF_FC_REL,      10, 0, REL_NO, "Yük Sigrt Atık" },
    { OVERTEMP_ALARM_FC_REL,     11, 0, REL_NO, "Aşrı Sıck Uyar" },
    { FAN_FAULT_FC_REL,          12, 0, REL_NO, "Fan Arızası" },
    { BATT_LINE_BROKEN_FC_REL,   13, 0, REL_NO, "Akü Hattı Kopuk" },
    { BATT_REVERSE_FC_REL,       14, 0, REL_NO, "Akü Ters" },
    { BATTERY_FAULT_FC_REL,      15, 0, REL_NO, "Akü Arızası" },
    { GENERAL_FAULT_FC_REL,      16, 0, REL_NO, "Genel Arıza" }
};

uint8_t rel_dat_tb_size = NUM_REL_CODES;
uint8_t rel_dat_tb_sel = 1;
uint8_t rel_dat_disp_index =0;
uint8_t rel_dat_arrow_loc = 1;
uint8_t rel_disp_mode = 1;
uint8_t rel_edit_mode = 0;
uint8_t rel_ord_tb_size = 16;
uint8_t rel_ord_tb_sel = 1;
uint8_t rel_ord_disp_index =0;
uint8_t rel_ord_arrow_loc = 1;

////// STATE CHANGES ////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
int var1=0;
int var2=0;
int var3=0;
//char var4[]="ü";
//char var5[]="ş";
//uint32_t var5=0;
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

char lcdd[32];


#define U10_RX_BUFFER_SIZE 14
volatile uint8_t U10_rxBuf[U10_RX_BUFFER_SIZE];
volatile uint8_t U10_rxCount = 0;
int temp_sens_count = 1;
uint32_t read_temp_dat_from_rx_buffer_cnt = 0;
float tmp_dat_C[6]={0};
uint32_t ovtmp_open_cnt = 0;
uint32_t ovtmp_open_per = 0; // 1200*50 ms 1dk
uint8_t temp_test_var_1 = 0;
uint8_t temp_test_var_2 = 0;
uint8_t sogut_sensor_exists = 0;
uint8_t trafo_sensor_exists = 0;
uint8_t batt_sensor_exists = 0;
int dropper_test_var_1 = 0;



uint8_t blm_batt_connected=0;
uint32_t sync_diff_det_cnt=0;
uint32_t sync_diff_det_per=40;
int V_targ_change_dir=-1;
int V_targ_change_cnt=0;
int V_targ_change_per=0;



uint8_t blm_balance_accepted=0;
uint32_t blm_current_detected_cnt=0;
uint32_t blm_balance_accept_cnt=0;
uint32_t blm_balance_accept_per=60;
uint32_t change_v_targ_cnt=0;
uint32_t change_v_targ_per=20;
uint32_t blm_batt_check_timer_cnt=0;
uint32_t blm_batt_check_per=20;
uint32_t blm_wait_at_low_lim_cnt=0;
uint32_t blm_wait_at_low_lim_per=40;

uint8_t V_targ_con_sy_returned_fl=0;
uint8_t blm_req_wait_at_low_lim_fl=0;
uint8_t gercV_yuks_contsV_fl=0;
uint8_t gercV_contsV_fark_dusuk_fl=0;
uint8_t gercV_dusuk_balncV_step1_fl=0;

uint8_t blm_req_monitor_stability=0;
uint8_t blm_req_reduce_vtarg=0;
uint8_t blm_req_rect_cur_lim_reduce=0;
uint32_t blm_voltage_increasing_cnt=0;
uint8_t blm_req_return_voltage_to_normal=0;
uint8_t blm_return_voltage_to_normal_completed=0;



float blm_V_step_05perc=0.2f;
float blm_I_step_05perc=0.2f;
float v_max_stb=0;
float v_min_stb=0;
float i_max_stb=0;
float i_min_stb=0;
float blm_stable_v_vrect=0;

uint32_t vrect_stable_cnt = 0;
uint8_t  vrect_stable     = 0;
uint32_t ibat_stable_cnt = 0;
uint8_t  ibat_stable     = 0;
uint8_t  blm_can_start_inspection = 0;
uint8_t  blm_reducing_vtarg_completed = 0;
uint8_t  blm_req_up_down_vtarg_limits = 0;
uint8_t  blm_set_up_down_vtarg_limits_completed = 0;
uint8_t  blm_wait_at_low_lim_completed = 0;
uint8_t  blm_req_return_state_to_normal = 0;
uint8_t  vtarg_VRECT_diff_high = 0;
uint8_t  blm_reducing_vtarg = 0;
uint32_t  blm_reducing_vtarg_cnt = 0;
uint8_t  blm_increasing_vtarg_back = 0;
uint32_t  blm_increasing_vtarg_back_cnt = 0;

volatile uint8_t batt_connect_test_start    = 0;
volatile uint8_t batt_connected      = 0;
volatile uint8_t batt_inspecting     = 0;
volatile float blm_down_vtarg_limit_1   = 0.0f;
volatile float blm_down_vtarg_limit_2   = 0.0f;
float blm_vi_change_mult = 0.0003f;




