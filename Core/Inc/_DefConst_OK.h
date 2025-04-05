
#define res_ LL_GPIO_ResetOutputPin
#define set_ LL_GPIO_SetOutputPin
#define tog_ LL_GPIO_TogglePin
#define isInSet_ LL_GPIO_IsInputPinSet
#define isOutSet_ LL_GPIO_IsOutputPinSet

// PIN NAMES
//#define TC_1_8_P GPIOA, LL_GPIO_PIN_15
//#define TC_2_8_P GPIOD, LL_GPIO_PIN_4

//#define RLY_TRG_BATT_P GPIOD, LL_GPIO_PIN_12

//#define DBG1_P GPIOE, LL_GPIO_PIN_10
//#define DBG2_P GPIOE, LL_GPIO_PIN_15
#define SPI4_MISO_P GPIOE, LL_GPIO_PIN_13
#define SPI4_MOSI_P GPIOE, LL_GPIO_PIN_14
#define SPI4_CLK_P GPIOE, LL_GPIO_PIN_12

#define CS_M95P32 GPIOB, LL_GPIO_PIN_14

//#define DBG11	set_(DBG1_P);
//#define DBG10	res_(DBG1_P);
//#define DBG21	set_(DBG2_P);
//#define DBG20	res_(DBG2_P);


//#define UART1_DE GPIOA, LL_GPIO_PIN_11

// FP BUTTONS
#define TC_3_1_P GPIOB, LL_GPIO_PIN_4
#define TC_3_2_P GPIOB, LL_GPIO_PIN_3
#define TC_3_3_P GPIOD, LL_GPIO_PIN_7
#define TC_3_4_P GPIOD, LL_GPIO_PIN_6
#define TC_3_5_P GPIOD, LL_GPIO_PIN_5
#define TC_3_6_P GPIOD, LL_GPIO_PIN_4

//#define GT_DC_DC_P GPIOE, LL_GPIO_PIN_5

#define BLEFT	isSet_(TC_3_1_P)
#define BRIGHT 	isSet_(TC_3_2_P)
#define BUP		isSet_(TC_3_3_P)
#define BDOWN	isSet_(TC_3_4_P)
#define BENTER	isSet_(TC_3_5_P)
#define BESC	isSet_(TC_3_6_P)
// FP SHIFT REG
#define SHFT1_SCK_P GPIOB, LL_GPIO_PIN_5
#define SHFT1_DAT_P GPIOB, LL_GPIO_PIN_7
#define SHFT1_LAT_P GPIOB, LL_GPIO_PIN_6
#define SHFT2_SCK_P GPIOB, LL_GPIO_PIN_8
#define SHFT2_DAT_P GPIOB, LL_GPIO_PIN_9
#define SHFT2_LAT_P GPIOE, LL_GPIO_PIN_0
#define SHFTR_SCK_P GPIOD, LL_GPIO_PIN_8
#define SHFTR_DAT_P GPIOD, LL_GPIO_PIN_9
#define SHFTR_LAT_P GPIOD, LL_GPIO_PIN_10
// ZCR
#define ZC_R_P GPIOE, LL_GPIO_PIN_7
#define ZC_S_P GPIOE, LL_GPIO_PIN_8
#define ZC_T_P GPIOE, LL_GPIO_PIN_9
//#define ZC_R_SEC_P GPIOB, LL_GPIO_PIN_15
//#define ZC_S_SEC_P GPIOD, LL_GPIO_PIN_8
//#define ZC_T_SEC_P GPIOD, LL_GPIO_PIN_9
// THY DRV
typedef struct {
    GPIO_TypeDef *port;   // GPIO portu
    uint32_t pin;         // Pin numarası
} GPIO_PinDef;

typedef struct {
    GPIO_PinDef A;
    GPIO_PinDef U;
} SCR_Line;

SCR_Line SCR_R = {{GPIOC, LL_GPIO_PIN_8}, {GPIOC, LL_GPIO_PIN_7}};
SCR_Line SCR_S = {{GPIOC, LL_GPIO_PIN_6}, {GPIOD, LL_GPIO_PIN_15}};
SCR_Line SCR_T = {{GPIOD, LL_GPIO_PIN_14}, {GPIOD, LL_GPIO_PIN_13}};

//#define SCR_LINE_T_A GPIOC, LL_GPIO_PIN_8
//#define SCR_LINE_T_U GPIOC, LL_GPIO_PIN_7
//#define SCR_LINE_S_A GPIOC, LL_GPIO_PIN_6
//#define SCR_LINE_S_U GPIOD, LL_GPIO_PIN_15
//#define SCR_LINE_R_A GPIOD, LL_GPIO_PIN_14
//#define SCR_LINE_R_U GPIOD, LL_GPIO_PIN_13

uint8_t RA_THY=0;
uint8_t RU_THY=0;
uint8_t SA_THY=0;
uint8_t SU_THY=0;
uint8_t TA_THY=0;
uint8_t TU_THY=0;

//#define SMPS_MON GPIOH, LL_GPIO_PIN_0

#define DROPP_LOAD GPIOD, LL_GPIO_PIN_12
#define DROPP_BATT GPIOD, LL_GPIO_PIN_11
// PIN NAMES
///////////////////////////////////////////////////////////////////////////

uint32_t ms_tick_cnt=0;
uint32_t delay_1ms_cnt=0;
uint32_t ButtScanDelay_cnt=0;
uint32_t while_delay50_h=0;
uint8_t UART1_recv_char=0;

uint32_t bleft_first_pressed = 0;
uint32_t bright_first_pressed = 0;
uint32_t bup_first_pressed = 0;
uint32_t bdown_first_pressed = 0;
uint32_t benter_first_pressed = 0;
uint32_t besc_first_pressed = 0;

uint32_t bleft_down_cnt=0;
uint32_t bright_down_cnt=0;
uint32_t bup_down_cnt=0;
uint32_t bdown_down_cnt=0;
uint32_t benter_down_cnt=0;
uint32_t besc_down_cnt=0;

// LED SHIFT REGISTER
///////////////////////////////////////////////////////////////////////////
uint32_t thy_stop_fault_hold_bits=0b0;
uint16_t LED_16_Data=0b0;
uint16_t LED_7_Data=0b0;
uint32_t Relay_Board_16_Data=0b0;
uint16_t can_send_LED_16_data_fl=1;
uint8_t can_send_LED_7_data_fl = 1;
uint8_t can_send_Relay_Board_16_Data_fl = 1;
uint32_t block_R16_f=0;
uint16_t block_L16_f=0;
uint8_t block_L7_f = 0;
uint32_t bit_index_R16=0;
uint16_t bit_index_L16=0;
uint8_t bit_index_L7 = 0;
uint32_t wh_loop_R16_cnt=10;
uint32_t wh_loop_L16_cnt=10;
uint32_t wh_loop_L7_cnt = 10;
uint32_t dly_R16_cnt=0;
uint32_t dly_L16_cnt=0;
uint32_t dly_L7_cnt=0;
// LED SHIFT REGISTER
///////////////////////////////////////////////////////////////////////////

// DMA UART
#define BUFFER_SIZE 20          // Number of strings in the buffer
#define MAX_STRING_LENGTH 128  // Maximum length of each string

char dma_uart_buffer[BUFFER_SIZE][MAX_STRING_LENGTH];  // Ring buffer for strings
uint8_t buffer_head = 0;  // Write pointer
uint8_t buffer_tail = 0;  // Read pointer
uint8_t dma_busy = 0;     // DMA busy flag

char DUB[64]={0};
char UXPUB[54]={0};

#define ubl1_    (1 << 0)  // Bit 0	general message
#define pr_btln    (1 << 1)  // Bit 1 uart message batt line
#define MSG_GROUP_INFO     (1 << 2)  // Bit 2
#define MSG_GROUP_BATTERY  (1 << 3)  // Bit 3
#define MSG_GROUP_FAULTS   (1 << 4)  // Bit 4

// Default disable all
//uint32_t enabled_message_groups = 0xFFFFFFFF;
uint32_t enabled_message_groups = 0x0;


#define MCP7940N_I2C_ADDRESS (0x6F << 1)
#define RTC_RTCC_CONTROL_REG 0x07
#define RTC_OSCTRIM_REG 0x08

char rtc_sec_recv=0;
char rtc_min_recv=55;
char rtc_hour_recv=19;
char rtc_day_recv=24;
char rtc_month_recv=1;
char rtc_year_recv=25;
uint32_t rtc_timestamp=0;
struct tm rtc_timestruct = {0};
char rtc_timestring[20] = {0};

char rtc_sec_edit=0;
char rtc_min_edit=55;
char rtc_hour_edit=19;
char rtc_day_edit=24;
char rtc_month_edit=1;
char rtc_year_edit=25;

char date_time_string[64]={0};
uint8_t DATE_TIME_edit_mode=0;
uint8_t DATE_TIME_edit_digit=11;



// ZCR
uint32_t LL_EXTI_LINE_7_reenable_cnt=0;
uint32_t LL_EXTI_LINE_8_reenable_cnt=0;
uint32_t LL_EXTI_LINE_9_reenable_cnt=0;


// PER AVERAGING
#define 		  avging_smpcnt 16
#define			  avging_shift 4

int32_t  timx_trg_num = 2800000;

uint8_t per_dev_perc_max = 10;

uint32_t per_stable_all_val = 0;
float	  per_rst_updn_avg_m = 0.0;
float	  per_rst_up_or_dn_avg_m = 0.0;
float	  per_rst_6_avg_m = 0.0;

uint8_t per_r_up_stable_fl = 0;
uint8_t per_r_dn_stable_fl = 0;
uint8_t per_s_up_stable_fl = 0;
uint8_t per_s_dn_stable_fl = 0;
uint8_t per_t_up_stable_fl = 0;
uint8_t per_t_dn_stable_fl = 0;

uint32_t en_t_dely_dn_r = 0;
uint32_t en_t_thy_dn_r = 0;
uint32_t en_t_dely_up_r = 0;
uint32_t en_t_thy_up_r = 0;
uint32_t en_t_dely_dn_s = 0;
uint32_t en_t_thy_dn_s = 0;
uint32_t en_t_dely_up_s = 0;
uint32_t en_t_thy_up_s = 0;
uint32_t en_t_dely_dn_t = 0;
uint32_t en_t_thy_dn_t = 0;
uint32_t en_t_dely_up_t = 0;
uint32_t en_t_thy_up_t = 0;
uint32_t zc_start_delay_300u_arr_32 = 82499;
uint32_t zc_start_delay_300u_psc_16 = 4;
uint32_t zc_start_delay_300u_arr_16 = 16499;
uint32_t tim_arr_max = 0;

int32_t per_r_dn_smp = 0;
uint32_t per_r_dn_avg_sum = 0;
uint32_t per_r_dn_avg_vect[avging_smpcnt]={0};
uint32_t per_r_dn_avg_ind = 0;
int32_t per_r_dn_avg_m = 0;
float	  per_r_dn_smp_dev = 0.0;
float    frq_r_updn_avg_m = 0.0;
int32_t per_r_up_smp = 0;
uint32_t per_r_up_avg_sum = 0;
uint32_t per_r_up_avg_vect[avging_smpcnt]={0};
uint32_t per_r_up_avg_ind = 0;
int32_t per_r_up_avg_m = 0;
float	  per_r_up_smp_dev = 0.0;
uint32_t per_r_updn_avg_m = 0;

int32_t per_s_dn_smp = 0;
uint32_t per_s_dn_avg_sum = 0;
uint32_t per_s_dn_avg_vect[avging_smpcnt]={0};
uint32_t per_s_dn_avg_ind = 0;
int32_t per_s_dn_avg_m = 0;
float	  per_s_dn_smp_dev = 0.0;
float    frq_s_updn_avg_m = 0.0;
int32_t per_s_up_smp = 0;
uint32_t per_s_up_avg_sum = 0;
uint32_t per_s_up_avg_vect[avging_smpcnt]={0};
uint32_t per_s_up_avg_ind = 0;
int32_t per_s_up_avg_m = 0;
float	  per_s_up_smp_dev = 0.0;
uint32_t per_s_updn_avg_m = 0;

int32_t per_t_dn_smp = 0;
uint32_t per_t_dn_avg_sum = 0;
uint32_t per_t_dn_avg_vect[avging_smpcnt]={0};
uint32_t per_t_dn_avg_ind = 0;
int32_t per_t_dn_avg_m = 0;
float	  per_t_dn_smp_dev = 0.0;
float    frq_t_updn_avg_m = 0.0;
int32_t per_t_up_smp = 0;
uint32_t per_t_up_avg_sum = 0;
uint32_t per_t_up_avg_vect[avging_smpcnt]={0};
uint32_t per_t_up_avg_ind = 0;
int32_t per_t_up_avg_m = 0;
float	  per_t_up_smp_dev = 0.0;
uint32_t per_t_updn_avg_m = 0;

int tm_r_rise_tm_t_fall = 0;
int tm_r_rise_tm_s_fall = 0;

uint32_t tm_r_rise = 0;
uint32_t tm_r_fall = 0;
uint32_t tm_s_rise = 0;
uint32_t tm_s_fall = 0;
uint32_t tm_t_rise = 0;
uint32_t tm_t_fall = 0;

uint8_t phase_sequence = 0;
uint32_t line_sgn_stable_Acc_cnt = 0;
uint8_t line_sgn_stable = 0;

float tm_r_rise_tm_s_fall_1 = 0;
float tm_r_rise_tm_t_fall_1 = 0;
float tm_r_rise_tm_s_fall_2 = 0;
float tm_r_rise_tm_t_fall_2 = 0;
float per6_dist_int_sum = 0;




