volatile void prfm(char *string);
void delay_1ms(uint32_t num1);
inline void delayA_1ms(uint32_t ms);
inline void delayA_1us(uint32_t us);
inline void delayA_100ns(uint32_t us);
void printFaultCodes(void);
//void inline extern startup_get_vars_from_EEP(void);
void apply_state_changes_f(State_Codes state_code, uint8_t set);
void inline extern set_targ_DC_voltage(float set_val);
//void inline extern update_VDC_high_low_lim_fc(void);
void inline extern actions_after_charge_mode_change(uint8_t num);
uint8_t state_get(State_Codes state);
uint8_t is_state_require_stop(State_Codes state);
uint8_t is_state_a_general_fault(State_Codes state);
uint8_t is_state_require_save(State_Codes state);
void state_set(State_Codes state, uint8_t set);
void inline extern actions_after_charge_voltage_change();
void inline extern DEV_NOM_VOUT_changed_fc(void);
void print_active_states();

void save_REL_OUT_order_to_EEP(void);
void gen_SIRALI_TABLO_RELOUT_from_eep(void);
void REL_OUT_ORDER_vect_to_REL_OUT_TB(void);
void generate_REL_OUT_order_vect_from_ord_table_fc(void);

#include "_EEP_M95P32.h"
#include "_Fnc_RTC.h"
#include "_Butt_fnc.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

inline extern int8_t is_within_perc_range(int var, int target, float percent) {
	int upper_bound = target*(1+percent/100);
	int lower_bound = target*(1-percent/100);
    return (var >= lower_bound) && (var <= upper_bound);
}

inline extern void rtc_timestamp_fnc(void) {
	rtc_timestruct.tm_sec  = rtc_sec_recv;
	rtc_timestruct.tm_min  = rtc_min_recv;
	rtc_timestruct.tm_hour = rtc_hour_recv;
	rtc_timestruct.tm_mday = rtc_day_recv;
	rtc_timestruct.tm_mon  = rtc_month_recv - 1;  // Adjust month (0-based)
	rtc_timestruct.tm_year = rtc_year_recv + 100;
	rtc_timestamp = mktime(&rtc_timestruct);
}

void convert_timestamp_to_date_string(time_t timestamp, char *date_string, size_t size) {
    struct tm *tm_info;
    tm_info = localtime(&timestamp);
	if (timestamp == 0xffffffff) {
		strcpy(date_string, "-");
		return;
	} else {
		strftime(date_string, size, "%d.%m.%y %H:%M:%S", tm_info);
		return;
	}
}

// DELAY
void delay_1ms(uint32_t num1) {
	delay_1ms_cnt=0;
while (delay_1ms_cnt < num1) {asm("NOP");}
}

inline void delayA_1ms(uint32_t ms)
{
    // Adjusted iteration count for 1 ms at 550 MHz
    uint32_t iterations_per_ms = 540505;

    // Calculate total iterations needed for the given ms
    uint32_t total_iterations = iterations_per_ms * ms;

    __asm volatile (
        "1: subs %0, %0, #1 \n"  // Decrement the counter
        "   bne 1b         \n"   // If not zero, branch back to label 1
        : "+r" (total_iterations) // Input/output: total_iterations register
        :                        // No input-only operands
        : "cc"                   // Clobbers condition flags
    );
}
inline extern void delayA_1us(uint32_t us)
{
    // Adjusted iteration count for 1 ms at 550 MHz
    uint32_t iterations_per_us = 541;

    // Calculate total iterations needed for the given ms
    uint32_t total_iterations = iterations_per_us * us;

    __asm volatile (
        "1: subs %0, %0, #1 \n"  // Decrement the counter
        "   bne 1b         \n"   // If not zero, branch back to label 1
        : "+r" (total_iterations) // Input/output: total_iterations register
        :                        // No input-only operands
        : "cc"                   // Clobbers condition flags
    );
}
inline void delayA_100ns(uint32_t us)
{
    // Adjusted iteration count for 1 ms at 550 MHz
    uint32_t iterations_per_us = 50;

    // Calculate total iterations needed for the given ms
    uint32_t total_iterations = iterations_per_us * us;

    __asm volatile (
        "1: subs %0, %0, #1 \n"  // Decrement the counter
        "   bne 1b         \n"   // If not zero, branch back to label 1
        : "+r" (total_iterations) // Input/output: total_iterations register
        :                        // No input-only operands
        : "cc"                   // Clobbers condition flags
    );
}
// DELAY
////////////////////////////////////////////

void extern prfm(char *string)
{
    // Move to a new line before sending a new message
    snprintf(dma_uart_buffer[buffer_head], MAX_STRING_LENGTH, "\n%s", string);

    // Move buffer head
    buffer_head = (buffer_head + 1) % BUFFER_SIZE;

    // Check for buffer overflow
    if (buffer_head == buffer_tail)
    {
        buffer_tail = (buffer_tail + 1) % BUFFER_SIZE; // Discard oldest data
    }

    // Reset uart_debug_cnt counter when a new string is sent
    prfm_r_counter = 0;

    // Start DMA transmission if not busy
    if (!dma_busy)
    {
        dma_busy = 1;
        size_t string_length = strlen(dma_uart_buffer[buffer_tail]);
        LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, string_length);
        LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0,
                               (uint32_t)dma_uart_buffer[buffer_tail],
                               LL_USART_DMA_GetRegAddr(UART5, LL_USART_DMA_REG_DATA_TRANSMIT),
                               LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
//        set_(UART1_DE);
        LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
    }
}


void extern uart_debug_cnt()
{
    // Move cursor forward by 3 characters before writing the counter
    snprintf(dma_uart_buffer[buffer_head], MAX_STRING_LENGTH, "\033[3C%03u\033[6D", prfm_r_counter++);

    // Move buffer head
    buffer_head = (buffer_head + 1) % BUFFER_SIZE;

    // Check for buffer overflow
    if (buffer_head == buffer_tail)
    {
        buffer_tail = (buffer_tail + 1) % BUFFER_SIZE; // Discard oldest data
    }

    // Start DMA transmission if not busy
    if (!dma_busy)
    {
        dma_busy = 1;
        size_t string_length = strlen(dma_uart_buffer[buffer_tail]);
        LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, string_length);
        LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0,
                               (uint32_t)dma_uart_buffer[buffer_tail],
                               LL_USART_DMA_GetRegAddr(UART5, LL_USART_DMA_REG_DATA_TRANSMIT),
                               LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
//        set_(UART1_DE);
        LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
    }
}

//void en_uart_msg_group(uint32_t group)
//{
//    enabled_message_groups |= group;
//}

//void disb_uart_msg_group(uint32_t group)
//{
//    enabled_message_groups &= ~group;
//}

void printBinary(uint8_t num) {
    PRF_GEN("0b");
    for (int i = 7; i >= 0; i--) { // Loop through each bit
        PRF_GEN("%d", (num >> i) & 1);
    }
    PRF_GEN("\n");
}



static inline void handleButton(uint8_t pinState,					// n014
                                volatile uint8_t  *isHeld,
                                volatile uint16_t *releaseCnt,
                                volatile uint32_t *holdCnt,
                                volatile uint32_t *nextRepeatEdge,
                                volatile uint8_t  *fireFlag)
{
    if (!*isHeld && *releaseCnt >= RELEASE_DELAY_T) {            // idle
        if (pinState) {
            *isHeld = 1;
            *holdCnt = 0;
            *nextRepeatEdge = FIRST_REPEAT_T;
            *fireFlag = 1;
        }
    }
    else if (*isHeld) {                                          // held
        if (pinState) {
            if (++*holdCnt >= *nextRepeatEdge) {
                *nextRepeatEdge += NEXT_REPEAT_T;
                *fireFlag = 1;
            }
        } else {                                                 // released
            *isHeld = 0;
            *releaseCnt = 0;
        }
    }
    else {                                                       // lock
        (*releaseCnt)++;
    }
}


void buttonScn(void) {
	if (ButtScanDelay_cnt > 2) {
		ButtScanDelay_cnt=0;

// BLEFT
if (BLEFT == 1 && BRIGHT == 0 && BUP == 0 && BDOWN == 0 && BENTER == 0 && BESC == 0) {
	if (bleft_first_pressed == 0) {
		bleft_first_pressed = 1;
		bleft_fnc();
		bleft_down_cnt++;
	} else {
		bleft_down_cnt++;
		if (bleft_down_cnt >= 12) {
			bleft_down_cnt = 8;
			bleft_fnc();
		}
	}
} else if (BLEFT == 0) {
	bleft_first_pressed = 0;
	bleft_down_cnt = 0;
}
// BLEFT
//////////////////////////////////////////////////////////////////////////////////
// BRIGHT
if (BLEFT == 0 && BRIGHT == 1 && BUP == 0 && BDOWN == 0 && BENTER == 0 && BESC == 0) {
	if (bright_first_pressed == 0) {
		bright_first_pressed = 1;
		bright_fnc();
		bright_down_cnt++;
	} else {
		bright_down_cnt++;
		if (bright_down_cnt >= 12) {
			bright_down_cnt = 8;
			bright_fnc();
		}
	}
} else if (BRIGHT == 0) {
	bright_first_pressed = 0;
	bright_down_cnt = 0;
}
// BRIGHT
//////////////////////////////////////////////////////////////////////////////////
//// BUP
if (BLEFT == 0 && BRIGHT == 0 && BUP == 1 && BDOWN == 0 && BENTER == 0 && BESC == 0) {
	if (bup_first_pressed == 0) {
		bup_first_pressed = 1;
		bup_fnc();
		bup_down_cnt+=1;
	} else {
		bup_down_cnt+=1;
		if (bup_down_cnt >= 50) {
			bup_down_cnt = 0;
			bup_fnc();
		}
	}
} else if (BUP == 0) {
	bup_first_pressed = 0;
	bup_down_cnt = 0;
}
//// BUP
////////////////////////////////////////////////////////////////////////////////////
//// BDOWN
if (BLEFT==0 && BRIGHT==0 && BUP==0 && BDOWN==1 && BENTER==0 && BESC==0) {
	if (bdown_first_pressed == 0) {
		bdown_first_pressed = 1;
		bdown_fnc();
		bdown_down_cnt+=1;
	} else {
		bdown_down_cnt+=5;
		if (bdown_down_cnt >= 20) {
			bdown_down_cnt = 0;
			bdown_fnc();
		}
	}
}
else if (BDOWN==0) {
	bdown_first_pressed=0;
	bdown_down_cnt = 0;
}
//// BDOWN
////////////////////////////////////////////////////////////////////////////////////
//// BENTER
if (BLEFT == 0 && BRIGHT == 0 && BUP == 0 && BDOWN == 0 && BENTER == 1 && BESC == 0) {
	if (benter_first_pressed == 0) {
		benter_first_pressed = 1;
		benter_fnc();
		benter_down_cnt++;
	} else {
		benter_down_cnt++;
		if (benter_down_cnt >= 12) {
			benter_down_cnt = 8;
			benter_fnc();
		}
	}
} else if (BENTER == 0) {
	benter_first_pressed = 0;
	benter_down_cnt = 0;
}
//// BENTER
////////////////////////////////////////////////////////////////////////////////////
//// BESC
if (BLEFT == 0 && BRIGHT == 0 && BUP == 0 && BDOWN == 0 && BENTER == 0 && BESC == 1) {
	if (besc_first_pressed == 0) {
		besc_first_pressed = 1;
		besc_fnc();
		besc_down_cnt++;
	} else {
		besc_down_cnt++;
		if (besc_down_cnt >= 12) {
			besc_down_cnt = 8;
			besc_fnc();
		}
	}
} else if (BESC == 0) {
	besc_first_pressed = 0;
	besc_down_cnt = 0;
}
//// BESC
////////////////////////////////////////////////////////////////////////////////////
	}
}


inline extern void short_circ_monitor_f(void) {
	if (thy_drv_en==1 && IRECT_smp_sc > EpD[RECT_SHORT][0].V1 && !state_get(RECT_SHORT_FC)) {
		IRECT_Short_Acc_cnt++;
		IRECT_Short_Ret_Acc_cnt=0;
		if (IRECT_Short_Acc_cnt >= IRECT_Short_Acc_per) {
			IRECT_Short_Acc_cnt=0;
			apply_state_changes_f(RECT_SHORT_FC, 1);
			PRF_GEN("DC SH %f", IRECT_smp_sc);
		}
	} else { IRECT_Short_Acc_per=0; }
	if (IRECT_smp_sc <= EpD[RECT_SHORT][0].V1 && state_get(RECT_SHORT_FC)) {
		IRECT_Short_Ret_Acc_cnt++;
		IRECT_Short_Acc_cnt=0;
		if (IRECT_Short_Ret_Acc_cnt >= IRECT_Short_Ret_Acc_per) {
			IRECT_Short_Ret_Acc_cnt=0;
			apply_state_changes_f(RECT_SHORT_FC, 0);
		}
	} else { IRECT_Short_Ret_Acc_cnt=0; }
	if (thy_drv_en==1 && IBAT_smp_sc > EpD[BATT_SHORT][0].V1 && !state_get(BATT_SHORT_FC)) {
		IBAT_Short_Acc_cnt++;
		IBAT_Short_Ret_Acc_cnt=0;
		if (IBAT_Short_Acc_cnt >= IBAT_Short_Acc_per) {
			IBAT_Short_Acc_cnt=0;
			apply_state_changes_f(BATT_SHORT_FC, 1);
			PRF_GEN("BT SH %f", IBAT_smp_sc);
		}
	} else { IBAT_Short_Acc_cnt=0; }
	if (IBAT_smp_sc <= IBAT_Short_Lim && state_get(BATT_SHORT_FC)) {
		IBAT_Short_Ret_Acc_cnt++;
		IBAT_Short_Acc_cnt=0;
		if (IBAT_Short_Ret_Acc_cnt >= IBAT_Short_Ret_Acc_per) {
			IBAT_Short_Ret_Acc_cnt=0;
			apply_state_changes_f(BATT_SHORT_FC, 0);
		}
	} else { IBAT_Short_Ret_Acc_cnt=0; }
}

inline extern void endOfDMATransfer_f(void) {
	if (LL_TIM_IsEnabledCounter(TIM2)) {
		LL_TIM_DisableCounter(TIM2);
		if (LL_TIM_GetCounter(TIM2) > timx_trg_num) {
			LL_TIM_SetAutoReload(TIM2, timx_trg_num);
			LL_TIM_GenerateEvent_UPDATE(TIM2);
			LL_TIM_EnableCounter(TIM2);
		}
		else {
			LL_TIM_SetAutoReload(TIM2, timx_trg_num);
			LL_TIM_EnableCounter(TIM2);
		}
	}
	if (LL_TIM_IsEnabledCounter(TIM23)) {
		LL_TIM_DisableCounter(TIM23);
		if (LL_TIM_GetCounter(TIM23) > timx_trg_num) {
			LL_TIM_SetAutoReload(TIM23, timx_trg_num);
			LL_TIM_GenerateEvent_UPDATE(TIM23);
			LL_TIM_EnableCounter(TIM23);
		}
		else {
			LL_TIM_SetAutoReload(TIM23, timx_trg_num);
			LL_TIM_EnableCounter(TIM23);
		}
	}
	if (LL_TIM_IsEnabledCounter(TIM24)) {
		LL_TIM_DisableCounter(TIM24);
		if (LL_TIM_GetCounter(TIM24) > timx_trg_num) {
			LL_TIM_SetAutoReload(TIM24, timx_trg_num);
			LL_TIM_GenerateEvent_UPDATE(TIM24);
			LL_TIM_EnableCounter(TIM24);
		}
		else {
			LL_TIM_SetAutoReload(TIM24, timx_trg_num);
			LL_TIM_EnableCounter(TIM24);
		}
	}
}

inline extern void per_r_dn_avg_m_f(void) { // exti interrupt fnc
	tm_r_rise=LL_TIM_GetCounter(TIM5);
	per_r_dn_smp=tm_r_rise-tm_r_fall;

	per_r_dn_avg_sum=per_r_dn_avg_sum-per_r_dn_avg_vect[per_r_dn_avg_ind]+per_r_dn_smp;
	per_r_dn_avg_vect[per_r_dn_avg_ind]=per_r_dn_smp;
	per_r_dn_avg_ind=(per_r_dn_avg_ind+1) % avging_smpcnt;
		per_r_dn_avg_m=per_r_dn_avg_sum >> avging_shift;		// r dn per avg
		per_r_updn_avg_m=per_r_dn_avg_m+per_r_up_avg_m;			// r up+down per avg
		per_rst_updn_avg_m=(per_r_updn_avg_m+per_s_updn_avg_m+per_t_updn_avg_m)/3;
		per_rst_up_or_dn_avg_m=per_rst_updn_avg_m/2; tim_arr_max=per_rst_up_or_dn_avg_m;
	per_r_dn_smp_dev=(float) per_r_dn_smp/ (float) per_r_dn_avg_m;	// last per dev from avg
	per_r_dn_stable_fl = is_within_perc_range(per_r_dn_smp, per_r_dn_avg_m, per_dev_perc_max);
	per_rst_6_avg_m=per_rst_up_or_dn_avg_m/3;

	tm_r_rise_tm_s_fall=tm_r_rise-tm_s_fall;
	tm_r_rise_tm_t_fall=tm_r_rise-tm_t_fall;
}
inline extern void per_r_up_avg_m_f(void) {
	tm_r_fall=LL_TIM_GetCounter(TIM5);
	per_r_up_smp=abs(tm_r_fall-tm_r_rise);

	per_r_up_avg_sum=per_r_up_avg_sum-per_r_up_avg_vect[per_r_up_avg_ind]+per_r_up_smp;
	per_r_up_avg_vect[per_r_up_avg_ind]=per_r_up_smp;
	per_r_up_avg_ind=(per_r_up_avg_ind+1) % avging_smpcnt;
		per_r_up_avg_m=per_r_up_avg_sum >> avging_shift;		// r up per avg
		per_r_updn_avg_m=per_r_dn_avg_m+per_r_up_avg_m;			// r up+down per avg
		per_rst_updn_avg_m=(per_r_updn_avg_m+per_s_updn_avg_m+per_t_updn_avg_m)/3;
		per_rst_up_or_dn_avg_m=per_rst_updn_avg_m/2; tim_arr_max=per_rst_up_or_dn_avg_m;
	per_r_up_smp_dev=(float) per_r_up_smp/ (float) per_r_up_avg_m;
	per_r_up_stable_fl = is_within_perc_range(per_r_up_smp, per_r_up_avg_m, per_dev_perc_max);
	per_rst_6_avg_m=per_rst_up_or_dn_avg_m/3;
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
inline extern void per_s_dn_avg_m_f(void) { // exti interrupt fnc
	tm_s_rise=LL_TIM_GetCounter(TIM5);
	per_s_dn_smp=abs(tm_s_rise-tm_s_fall);

	per_s_dn_avg_sum=per_s_dn_avg_sum-per_s_dn_avg_vect[per_s_dn_avg_ind]+per_s_dn_smp;
	per_s_dn_avg_vect[per_s_dn_avg_ind]=per_s_dn_smp;
	per_s_dn_avg_ind=(per_s_dn_avg_ind+1) % avging_smpcnt;
		per_s_dn_avg_m=per_s_dn_avg_sum >> avging_shift;
		per_s_updn_avg_m=per_s_dn_avg_m+per_s_up_avg_m;
		per_rst_updn_avg_m=(per_r_updn_avg_m+per_s_updn_avg_m+per_t_updn_avg_m)/3;
		per_rst_up_or_dn_avg_m=per_rst_updn_avg_m/2; tim_arr_max=per_rst_up_or_dn_avg_m;
	per_s_dn_smp_dev=(float) per_s_dn_smp/ (float) per_s_dn_avg_m;	// last per dev from avg
	per_s_dn_stable_fl = is_within_perc_range(per_s_dn_smp, per_s_dn_avg_m, per_dev_perc_max);
	per_rst_6_avg_m=per_rst_up_or_dn_avg_m/3;
}
inline extern void per_s_up_avg_m_f(void) {
	tm_s_fall=LL_TIM_GetCounter(TIM5);
	per_s_up_smp=abs(tm_s_fall-tm_s_rise);

	per_s_up_avg_sum=per_s_up_avg_sum-per_s_up_avg_vect[per_s_up_avg_ind]+per_s_up_smp;
	per_s_up_avg_vect[per_s_up_avg_ind]=per_s_up_smp;
	per_s_up_avg_ind=(per_s_up_avg_ind+1) % avging_smpcnt;
		per_s_up_avg_m=per_s_up_avg_sum >> avging_shift;
		per_s_updn_avg_m=per_s_dn_avg_m+per_s_up_avg_m;
		per_rst_updn_avg_m=(per_r_updn_avg_m+per_s_updn_avg_m+per_t_updn_avg_m)/3;
		per_rst_up_or_dn_avg_m=per_rst_updn_avg_m/2; tim_arr_max=per_rst_up_or_dn_avg_m;
	per_s_up_smp_dev=(float) per_s_up_smp/ (float) per_s_up_avg_m;
	per_s_up_stable_fl = is_within_perc_range(per_s_up_smp, per_s_up_avg_m, per_dev_perc_max);
	per_rst_6_avg_m=per_rst_up_or_dn_avg_m/3;
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
inline extern void per_t_dn_avg_m_f(void) { // exti interrupt fnc
	tm_t_rise=LL_TIM_GetCounter(TIM5);
	per_t_dn_smp=abs(tm_t_rise-tm_t_fall);

	per_t_dn_avg_sum=per_t_dn_avg_sum-per_t_dn_avg_vect[per_t_dn_avg_ind]+per_t_dn_smp;
	per_t_dn_avg_vect[per_t_dn_avg_ind]=per_t_dn_smp;
	per_t_dn_avg_ind=(per_t_dn_avg_ind+1) % avging_smpcnt;
		per_t_dn_avg_m=per_t_dn_avg_sum >> avging_shift;
		per_t_updn_avg_m=per_t_dn_avg_m+per_t_up_avg_m;
		per_rst_updn_avg_m=(per_r_updn_avg_m+per_s_updn_avg_m+per_t_updn_avg_m)/3;
		per_rst_up_or_dn_avg_m=per_rst_updn_avg_m/2; tim_arr_max=per_rst_up_or_dn_avg_m;
	per_t_dn_smp_dev=(float) per_t_dn_smp/ (float) per_t_dn_avg_m;	// last per dev from avg
	per_t_dn_stable_fl = is_within_perc_range(per_t_dn_smp, per_t_dn_avg_m, per_dev_perc_max);
	per_rst_6_avg_m=per_rst_up_or_dn_avg_m/3;
}
inline extern void per_t_up_avg_m_f(void) {
	tm_t_fall=LL_TIM_GetCounter(TIM5);
	per_t_up_smp=abs(tm_t_fall-tm_t_rise);

	per_t_up_avg_sum=per_t_up_avg_sum-per_t_up_avg_vect[per_t_up_avg_ind]+per_t_up_smp;
	per_t_up_avg_vect[per_t_up_avg_ind]=per_t_up_smp;
	per_t_up_avg_ind=(per_t_up_avg_ind+1) % avging_smpcnt;
		per_t_up_avg_m=per_t_up_avg_sum >> avging_shift;
		per_t_updn_avg_m=per_t_dn_avg_m+per_t_up_avg_m;
		per_rst_updn_avg_m=(per_r_updn_avg_m+per_s_updn_avg_m+per_t_updn_avg_m)/3;
		per_rst_up_or_dn_avg_m=per_rst_updn_avg_m/2; tim_arr_max=per_rst_up_or_dn_avg_m;
	per_t_up_smp_dev=(float) per_t_up_smp/ (float) per_t_up_avg_m;
	per_t_up_stable_fl = is_within_perc_range(per_t_up_smp, per_t_up_avg_m, per_dev_perc_max);
	per_rst_6_avg_m=per_rst_up_or_dn_avg_m/3;
}

#include "_LCD_Pg.h"		////////////////////////////////////////////////////////


void processShiftRegister_LED_7(uint32_t data) {
	static uint32_t bit_index=0;
	static uint32_t goto_next_action=0;
	static uint32_t dely_time_cnt=0;
	static uint32_t dely_time_per=10;
	static uint32_t dely_restart_per=10000;
	if (goto_next_action == 0) {
		if (bit_index < 7) {
			if (data & (1 << bit_index)) {
				set_(SHFT1_DAT_P);
			} else {
				res_(SHFT1_DAT_P);
			}
			goto_next_action = 1;
		} else {
			res_(SHFT1_DAT_P);
			goto_next_action = 5;
		}
	} else if (goto_next_action == 1) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			set_(SHFT1_SCK_P);
			goto_next_action = 2;
		}
	} else if (goto_next_action == 2) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			res_(SHFT1_DAT_P);
			goto_next_action = 3;
		}
	} else if (goto_next_action == 3) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			res_(SHFT1_SCK_P);
			bit_index++; // Increment bit index
			goto_next_action = 0;
		}
	} else if (goto_next_action == 5) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			set_(SHFT1_LAT_P);
			goto_next_action = 6;
		}
	} else if (goto_next_action == 6) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			res_(SHFT1_LAT_P);
			bit_index = 0;
			goto_next_action = 0;
		}
	} else if (goto_next_action == 7) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_restart_per) {
			dely_time_cnt = 0;
			goto_next_action = 0;
		}
	}
}

void processShiftRegister_LED_16(uint32_t data) {
	static uint32_t bit_index=0;
	static uint32_t goto_next_action=0;
	static uint32_t dely_time_cnt=0;
	static uint32_t dely_time_per=10;
	static uint32_t dely_restart_per=10000;
	if (goto_next_action == 0) {
		if (bit_index < 16) {
			if (data & (1 << bit_index)) {
				set_(SHFT2_DAT_P);
			} else {
				res_(SHFT2_DAT_P);
			}
			goto_next_action = 22;
		} else {
			res_(SHFT2_DAT_P);
			goto_next_action = 5;
		}
	} else if (goto_next_action == 22) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
//			set_(SHFT2_SCK_P);
			goto_next_action = 1;
		}
	} else if (goto_next_action == 1) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			set_(SHFT2_SCK_P);
			goto_next_action = 2;
		}
	} else if (goto_next_action == 2) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			res_(SHFT2_DAT_P);
			goto_next_action = 3;
		}
	} else if (goto_next_action == 3) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			res_(SHFT2_SCK_P);
			bit_index++; // Increment bit index
			goto_next_action = 0;
		}
	} else if (goto_next_action == 5) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			set_(SHFT2_LAT_P);
			goto_next_action = 6;
		}
	} else if (goto_next_action == 6) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			res_(SHFT2_LAT_P);
			bit_index = 0;
			goto_next_action = 0;
		}
	} else if (goto_next_action == 7) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_restart_per) {
			dely_time_cnt = 0;
			goto_next_action = 0;
		}
	}
}

void processShiftRegister_Relay_Board_16(uint32_t data) {
	static uint32_t bit_index=0;
	static uint32_t goto_next_action=0;
	static uint32_t dely_time_cnt=0;
	static uint32_t dely_time_per=20;
	static uint32_t dely_restart_per=10000;
	if (goto_next_action == 0) {
		if (bit_index < 24) {
			if (data & (1 << bit_index)) {
				set_(SHFTR_DAT_P);
			} else {
				res_(SHFTR_DAT_P);
			}
			goto_next_action = 1;
		} else {
			res_(SHFTR_DAT_P);
			goto_next_action = 5;
		}
	} else if (goto_next_action == 1) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			set_(SHFTR_SCK_P);
			goto_next_action = 2;
		}
	} else if (goto_next_action == 2) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			res_(SHFTR_SCK_P);
			bit_index++; // Increment bit index
			goto_next_action = 3;
		}
	} else if (goto_next_action == 3) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			res_(SHFTR_DAT_P);
			goto_next_action = 0;
		}
	} else if (goto_next_action == 5) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			set_(SHFTR_LAT_P);
			goto_next_action = 6;
		}
	} else if (goto_next_action == 6) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_time_per) {
			dely_time_cnt = 0;
			res_(SHFTR_LAT_P);
			bit_index = 0;
			goto_next_action = 7;
		}
	} else if (goto_next_action == 7) {
		dely_time_cnt++;
		if (dely_time_cnt > dely_restart_per) {
			dely_time_cnt = 0;
			goto_next_action = 0;
		}
	}
}

//void SPI_Send24Bits(uint32_t data) {
//    for (int bit = 32; bit >= 0; bit--) {
//        if (data & (1 << bit)) {
//            set_(SHFTR_DAT_P);
//        } else {
//            res_(SHFTR_DAT_P);
//        }
//        delayA_1us(10);
//
//        set_(SHFTR_SCK_P);
//        delayA_1us(10);
//        res_(SHFTR_SCK_P);
//        delayA_1us(10);
//    }
//
//    delayA_1us(100);
//    set_(SHFTR_LAT_P);
//    delayA_1us(10);
//    res_(SHFTR_LAT_P);
//}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// AKÜ HATTI KOPUK //////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// AKÜ HATTI KOPUK //////////////////////////////////////////////////////////////////////////////////////////////

void inline extern aku_hatti_kopuk_fc_inl(void) {

} // void inline extern aku_hatti_kopuk_fc_inl(void) {

////// AKÜ HATTI KOPUK //////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void inline extern DEV_NOM_VOUT_changed_fc(void) {
	EpD[VBAT_FLOAT][0].V1=EpD[DEV_NOM_VOUT][0].V1*1.1;	// şarj voltajlarını yeni belirlenen cihaz voltajına göre ayarla
	EpD[VBAT_BOOST][0].V1=EpD[DEV_NOM_VOUT][0].V1*1.15;
	Vbat_flt = EpD[DEV_NOM_VOUT][0].V1 * 0.1;
	Vdc_float_min=EpD[DEV_NOM_VOUT][0].V1*0.9; // Normal şarj rejimi gerilim ayar aralığı
	Vdc_float_max=EpD[DEV_NOM_VOUT][0].V1*1.15; // Normal şarj rejimi gerilim ayar aralığı
	Vdc_boost_min=EpD[DEV_NOM_VOUT][0].V1*1.15; // Tam şarj rejimi gerilim ayar aralığı
	Vdc_boost_max=EpD[DEV_NOM_VOUT][0].V1*1.3; // Tam şarj rejimi gerilim ayar aralığı
	actions_after_charge_voltage_change();
	blm_op_phase = B_SKIP_DELAY_RESTART;					// cancel op. bring_vtarg_back_skip_delay
	blm_enable_collect_samples = 0;
	blm_corr_buf_index = 0;


    blm_V_step_05perc  = EpD[DEV_NOM_VOUT][0].V1 * 0.005;
    blm_V_step_15perc  = EpD[DEV_NOM_VOUT][0].V1 * 0.015;
    blm_V_move_up_set  = EpD[DEV_NOM_VOUT][0].V1 * 0.02;
    blm_V_move_dn_set  = EpD[DEV_NOM_VOUT][0].V1 * 0.02;

	set_dropper_l_hg_V = EpD[DEV_NOM_VOUT][0].V1 * (1 + (EpD[SET_DROPP_L_HG_PERC][0].V1 / 100)); // voltaj olarak değer
	set_dropper_l_lw_V  = EpD[DEV_NOM_VOUT][0].V1 * (1 - (EpD[SET_DROPP_L_LW_PERC][0].V1 / 100));
	set_dropper_l_hg_perc=EpD[SET_DROPP_L_HG_PERC][0].V1 / 100; // yüzde olarak değer
	set_dropper_l_lw_perc=EpD[SET_DROPP_L_LW_PERC][0].V1 / 100;
}


void inline extern set_targ_DC_voltage(float set_val) {
	targ_DC_voltage=set_val;
//	targ_DC_voltage=temp_targ_DC_voltage;
	vrect_dc_high_lim=targ_DC_voltage*(1+(EpD[VRECT_DC_HIGH_LIM_add][0].V1/100));
	vrect_dc_high_lim_ret=targ_DC_voltage*(1+(EpD[VRECT_DC_HIGH_LIM_add][0].V1/100)-0.01);
	vrect_dc_low_lim=targ_DC_voltage/(1+(EpD[VRECT_DC_LOW_LIM_add][0].V1/100));
	vrect_dc_low_lim_ret=targ_DC_voltage/(1+(EpD[VRECT_DC_LOW_LIM_add][0].V1/100)-0.01);
}

void inline extern actions_after_charge_voltage_change() {
	if (EpD[SET_CHARGE_MODE][0].V1 == FLOAT) {
		temp_targ_DC_voltage=EpD[VBAT_FLOAT][0].V1;
		set_targ_DC_voltage(temp_targ_DC_voltage);
	} else if (EpD[SET_CHARGE_MODE][0].V1 == BOOST) {
		temp_targ_DC_voltage=EpD[VBAT_BOOST][0].V1;
		set_targ_DC_voltage(temp_targ_DC_voltage);
	}
//	else if (EpD[SET_CHARGE_MODE][0].V1 == TIMED) {
//		temp_targ_DC_voltage=EpD[VBAT_BOOST][0].V1;
//		set_targ_DC_voltage(temp_targ_DC_voltage);
//	}
}

// does required changes after a charge mode chage
void inline extern actions_after_charge_mode_change(uint8_t num) {
	if (EpD[SET_CHARGE_MODE][0].V1 == FLOAT) {
		temp_targ_DC_voltage=EpD[VBAT_FLOAT][0].V1;	// şarj modu hedef voltajını geçici olarak tutan variable
		targ_DC_current=EpD[SET_IBAT_FLOAT][0].V1;
		set_targ_DC_voltage(temp_targ_DC_voltage);
		state_set(ST_FLOAT_CHARGE, 1);
		state_set(BOOST_CHARGE_FC, 0);
		switch_to_auto_mode_completed=0;
		timed_mode_actions_do_once=0;
		charge_mode_timed_time_sec=0; // ekrandaki timed mode kalan saniye değerini kaldır
		PRF_GEN("FLOAT charge mode %d", num);
	} else if (EpD[SET_CHARGE_MODE][0].V1 == BOOST) {
		temp_targ_DC_voltage=EpD[VBAT_BOOST][0].V1;
		targ_DC_current=EpD[SET_IBAT_BOOST][0].V1;
		set_targ_DC_voltage(temp_targ_DC_voltage);
		state_set(ST_FLOAT_CHARGE, 0);
		state_set(BOOST_CHARGE_FC, 1);
		switch_to_auto_mode_completed=0;
		timed_mode_actions_do_once=0;
		charge_mode_timed_time_sec=0; // ekrandaki timed mode kalan saniye değerini kaldır
		PRF_GEN("BOOST charge mode %d", num);
	}
//	else if (EpD[SET_CHARGE_MODE][0].V1 == TIMED_FLOAT_CHARGE_FC) {
//		temp_targ_DC_voltage=EpD[VBAT_FLOAT][0].V1;
//		targ_DC_current=EpD[SET_IBAT_FLOAT][0].V1;
//		set_targ_DC_voltage(temp_targ_DC_voltage);
//		apply_state_changes_f(ST_FLOAT_CHARGE, 1);
//		apply_state_changes_f(BOOST_CHARGE_FC, 0);
//		apply_state_changes_f(TIMED_FLOAT_CHARGE_FC, 1);
//		apply_state_changes_f(TIMED_BOOST_CHARGE_FC, 0);
//		apply_state_changes_f(MANUAL_FLOAT_CHARGE_FC, 0);
//		apply_state_changes_f(MANUAL_BOOST_CHARGE_FC, 0);
//		apply_state_changes_f(AUTO_FLOAT_CHARGE_FC, 0);
//		apply_state_changes_f(AUTO_BOOST_CHARGE_FC, 0);
//		LED_7_Data |= FLOAT_CHARGE_LED;
//		LED_7_Data &= ~BOOST_CHARGE_LED;
//		PRF_GEN("timed mode selected, boost charge values loaded");
//		switch_to_auto_mode_completed=0;
//		if (timed_mode_actions_do_once==0) {
//			timed_mode_actions_do_once=1; // timed a geçiş yapıldığında bir kez uygulanacak. tekrar uygulanabilmesi için başka moda geçilmesi lazım.
//			timed_mode_time_ended=0; // timed mod sayacı sıfırla. sayaç sonunda float a geçilecek
//			charge_mode_timed_time_cnt=(uint32_t) (EpD[SET_BOOST_TIME][0].V1*60*1000/50);
//		}
//	}
//	else if (EpD[SET_CHARGE_MODE][0].V1 == TIMED_BOOST_CHARGE_FC) {
//		temp_targ_DC_voltage=EpD[VBAT_BOOST][0].V1;
//		targ_DC_current=EpD[SET_IBAT_BOOST][0].V1;
//		set_targ_DC_voltage(temp_targ_DC_voltage);
//		apply_state_changes_f(ST_FLOAT_CHARGE, 0);
//		apply_state_changes_f(BOOST_CHARGE_FC, 1);
//		apply_state_changes_f(TIMED_FLOAT_CHARGE_FC, 0);
//		apply_state_changes_f(TIMED_BOOST_CHARGE_FC, 1);
//		apply_state_changes_f(MANUAL_FLOAT_CHARGE_FC, 0);
//		apply_state_changes_f(MANUAL_BOOST_CHARGE_FC, 0);
//		apply_state_changes_f(AUTO_FLOAT_CHARGE_FC, 0);
//		apply_state_changes_f(AUTO_BOOST_CHARGE_FC, 0);
//		LED_7_Data &= ~FLOAT_CHARGE_LED;
//		LED_7_Data |= BOOST_CHARGE_LED;
//		PRF_GEN("timed mode selected, boost charge values loaded");
//		switch_to_auto_mode_completed=0;
//		if (timed_mode_actions_do_once==0) {
//			timed_mode_actions_do_once=1; // timed a geçiş yapıldığında bir kez uygulanacak. tekrar uygulanabilmesi için başka moda geçilmesi lazım.
//			timed_mode_time_ended=0; // timed mod sayacı sıfırla. sayaç sonunda float a geçilecek
//			charge_mode_timed_time_cnt=(uint32_t) (EpD[SET_BOOST_TIME][0].V1*60*1000/50);
//		}
//	}
//else if (EpD[SET_CHARGE_MODE][0].V1 == AUTO) {
//		apply_state_changes_f(TIMED_FLOAT_CHARGE_FC, 0);
//		apply_state_changes_f(TIMED_BOOST_CHARGE_FC, 0);
//		apply_state_changes_f(MANUAL_FLOAT_CHARGE_FC, 0);
//		apply_state_changes_f(MANUAL_BOOST_CHARGE_FC, 0);
//		apply_state_changes_f(AUTO_FLOAT_CHARGE_FC, 0);
//		apply_state_changes_f(AUTO_BOOST_CHARGE_FC, 0);
//	}
}

void apply_state_changes_f(State_Codes state_code, uint8_t set) {
    if (set) {
		state_list[state_code].action |= (1 << ACTIVE_enum); // set active flag in fault action bits
        if (is_state_require_stop(state_code)) {  // bu state durma gerektiriyor diye istenmiş mi
        	thy_drv_en=0;
        	sfsta_op_phase = S_SFSTA_NONE;
        	blm_op_phase = B_RESTRT_AFTR_DELAY;
        	state_set(START_FC, 0);
        	state_set(STOP_FC, 1);
        }
		if (is_state_require_save(state_code)) { // eğer save biti 1 ise hafızaya kaydet
			Record_Fault_Code(state_code);
		}
    }
	else if (!set) {
		state_list[state_code].action &= ~(1U << ACTIVE_enum); // reset active flag in fault action bits

	}





//// toplu state inceleme
    uint8_t thystop_faults_bit_all_=0;    // tristörlerin kapatılmasını gerektirecek hiçbir arıza yok.
    for (uint8_t i = 0; i < NUM_STATE_NAMES; i++) {
        if (is_state_require_stop(i)) {
        	thystop_faults_bit_all_++;
        }
    } 	thystop_faults_bit_all=thystop_faults_bit_all_;

    uint8_t general_thystop_faults_bit_all_=0;    // genel arıza gerektirecek hiçbir arıza yok.
    for (uint8_t i = 0; i < NUM_STATE_NAMES; i++) {
        if (is_state_a_general_fault(i)) {
        	general_thystop_faults_bit_all_++;
        }
    } 	general_thystop_faults_bit_all=general_thystop_faults_bit_all_;
		PRF_GEN("     state_code %d %s set %d", state_code, state_list[state_code].name, set);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}


void swap_scr_lines(SCR_Line *line1, SCR_Line *line2) {
    SCR_Line temp = *line1;
    *line1 = *line2;
    *line2 = temp;
}


void USART10_SendByte(uint8_t data)
{
    while (!LL_USART_IsActiveFlag_TXE(USART10)) {
        // Optionally place a timeout here in production code
    }
    LL_USART_TransmitData8(USART10, data);
    while (!LL_USART_IsActiveFlag_TC(USART10)) {
    }
}

//int read_data(uint8_t temp_sens_count, uint16_t *raw_vals)
//{
//
//	U10_rxCount = 0;
//    for (int i = 0; i < U10_RX_BUFFER_SIZE; i++) {
//    	U10_rxBuf[i] = 0;
//    }
//
//    USART10_SendByte(0x55);
//    delayA_1us(10);
//    USART10_SendByte(0xF1);
//
//    for (int i = 0; i < temp_sens_count; i++) {
//        // ham değer = LSB + (MSB << 8)
//        uint8_t lsb = U10_rxBuf[2 + i*2];
//        uint8_t msb = U10_rxBuf[3 + i*2];
//        raw_vals[i] = (msb << 8) | lsb;
//    }
//
//    return 0;
//}

static float tmp144_convert_temperature(uint16_t raw16) {
  int16_t val12 = (raw16 >> 4) & 0x0FFF;
  if (val12 & 0x0800) {
    val12 |= 0xF000;
  }
  return (float)val12 * 0.0625f;
}

void tmp144_init_and_assign(void) {
USART10_SendByte(0x55); // Calibration Byte (55h)
delayA_1us(10);
USART10_SendByte(0xB4); // Global software reset
delay_1ms(100);

	U10_rxCount = 0;
    for (int i = 0; i < U10_RX_BUFFER_SIZE; i++) {
    	U10_rxBuf[i] = 0;
    }

    USART10_SendByte(0x55);
    delayA_1us(10);
    USART10_SendByte(0x8C);
    delayA_1us(10);
    USART10_SendByte(0x90);
    delay_1ms(100);

    uint8_t dev_count_raw = U10_rxBuf[2];
    uint8_t temp_sens_count_g = dev_count_raw & 0x0F;

    temp_sens_count=temp_sens_count_g;
    if (temp_sens_count_g < 1 || temp_sens_count_g > 16) {
    	temp_sens_count=0;
    }

    if (temp_sens_count == 0) { // Alınan sensor bilgilerine göre sensör isimlendirmelerini yap.
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
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// OUT RELAY OPERATION //////////////////////////////////////////////////////////////////////////////////////////



void gen_SIRALI_TABLO_RELOUT_from_eep(void) {
    REL_OUT_order_part1 = EpD[REL_OUT_1][0].V1;
    REL_OUT_order_part2 = EpD[REL_OUT_2][0].V1;
    REL_OUT_order_part3 = EpD[REL_OUT_3][0].V1;
    REL_OUT_order_part4 = EpD[REL_OUT_4][0].V1;

    uint32_t val;
	for (int i = 0; i < SIRALI_TABLO_SIZE; i++) {
		if (i < 4) {
			val = (REL_OUT_order_part1 >> (i * 5)) & 0x1F;
		} else if (i < 8) {
			val = (REL_OUT_order_part2 >> ((i - 4) * 5)) & 0x1F;
		} else if (i < 12) {
			val = (REL_OUT_order_part3 >> ((i - 8) * 5)) & 0x1F;
		} else {
			val = (REL_OUT_order_part4 >> ((i - 12) * 5)) & 0x1F;
		}
		SIRALI_TABLO_RELOUT[i].tbl_code = (State_Codes)val;
	}
}

void save_REL_OUT_order_to_EEP(void) {
    REL_OUT_order_part1 = 0; // sıralama bu 4 parça değişkene kaydediliyor.
    REL_OUT_order_part2 = 0;
    REL_OUT_order_part3 = 0;
    REL_OUT_order_part4 = 0;
	for (int i = 0; i < SIRALI_TABLO_SIZE; i++) {
		uint8_t state_code_ = SIRALI_TABLO_RELOUT[i].tbl_code & 0x1F;
		if (i < 4) {
			REL_OUT_order_part1 |= (state_code_ << ((i - 0) * 5)); // i 4 5 6 7, parantez içi 0 1 2 3 oluyor.
		} else if (i < 8) {
			REL_OUT_order_part2 |= (state_code_ << ((i - 4) * 5)); // i 4 5 6 7, parantez içi 0 1 2 3 oluyor.
		} else if (i < 12) {
			REL_OUT_order_part3 |= (state_code_ << ((i - 8) * 5)); // i 8 9 10 11, parantez içi 0 1 2 3 oluyor.
		} else {
			REL_OUT_order_part4 |= (state_code_ << ((i - 12) * 5));
		}
	}

	EpD[REL_OUT_1][0].V1 = REL_OUT_order_part1;
	EpD[REL_OUT_2][0].V1 = REL_OUT_order_part2;
	EpD[REL_OUT_3][0].V1 = REL_OUT_order_part3;
	EpD[REL_OUT_4][0].V1 = REL_OUT_order_part4;

	Rec_Dat_to_EEp_f(REL_OUT_1);
	Rec_Dat_to_EEp_f(REL_OUT_2);
	Rec_Dat_to_EEp_f(REL_OUT_3);
	Rec_Dat_to_EEp_f(REL_OUT_4);
}

////// OUT RELAY OPERATION //////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float calculate_corr_from_sums(float sum_x, float sum_y, float sum_x2, float sum_y2, float sum_xy, uint16_t n) {
    float numerator = n * sum_xy - sum_x * sum_y;
    float denominator = sqrtf((n * sum_x2 - sum_x * sum_x) * (n * sum_y2 - sum_y * sum_y));
    if (denominator == 0.0f) return 0.0f;
    return numerator / denominator;
}

void inline extern bring_vtarg_back_to_chrgV(uint8_t num) {
	if (targ_DC_voltage < temp_targ_DC_voltage - blm_V_step_05perc) {
		set_targ_DC_voltage(targ_DC_voltage + blm_V_step_05perc);
	} else if (targ_DC_voltage > temp_targ_DC_voltage + blm_V_step_05perc) {
		set_targ_DC_voltage(targ_DC_voltage - blm_V_step_05perc);
	} else {
		set_targ_DC_voltage(temp_targ_DC_voltage); // hedefe ulaşınca sabitle
		blm_op_phase = num;
	}
}


void stability_vrect_fc(void) {
		if (VRECT_pas.a16 > v_max_stb) {
			v_max_stb = VRECT_pas.a16 + blm_V_step_15perc;
			v_min_stb = VRECT_pas.a16 - blm_V_step_15perc;
			vrect_stable_cnt = (vrect_stable_cnt > 4) ? vrect_stable_cnt - 4 : 0; // koşul sağlanmıyorsa sayacı 4 geri çek.
		} else if (VRECT_pas.a16 < v_min_stb) {
			v_max_stb = VRECT_pas.a16 + blm_V_step_15perc;
			v_min_stb = VRECT_pas.a16 - blm_V_step_15perc;
			vrect_stable_cnt = (vrect_stable_cnt > 4) ? vrect_stable_cnt - 4 : 0;
		} else if (vrect_stable_cnt < 150) {
			vrect_stable_cnt++;
		}
		vrect_stable = (vrect_stable_cnt >= 150);
}
void stability_irect_fc(void) {
		if (IRECT_pas.a16 > i_rec_max_stb) {
			i_rec_max_stb = IRECT_pas.a16 + blm_I_step_075perc;
			i_rec_min_stb = IRECT_pas.a16 - blm_I_step_075perc;
			irect_stable_cnt = (irect_stable_cnt > 4) ? irect_stable_cnt - 4 : 0;
		} else if (IRECT_pas.a16 < i_rec_min_stb) {
			i_rec_max_stb = IRECT_pas.a16 + blm_I_step_075perc;
			i_rec_min_stb = IRECT_pas.a16 - blm_I_step_075perc;
			irect_stable_cnt = (irect_stable_cnt > 4) ? irect_stable_cnt - 4 : 0;
		} else if (irect_stable_cnt < 150) {
			irect_stable_cnt++;
		}
		irect_stable = (irect_stable_cnt >= 150);
}
void stability_ibat_fc(void) { // ibat_stable variable ı burda belirleniyor.
    if (IBAT_pas.a64 > i_bat_max_stb) {
        i_bat_max_stb = IBAT_pas.a64 + blm_I_step_025perc;
        i_bat_min_stb = IBAT_pas.a64 - blm_I_step_025perc;
        ibat_stable_cnt = (ibat_stable_cnt > 4) ? ibat_stable_cnt - 4 : 0; // koşul sağlanmıyorsa sayacı 4 geri çek.
    } else if (IBAT_pas.a64 < i_bat_min_stb) {
        i_bat_max_stb = IBAT_pas.a64 + blm_I_step_025perc;
        i_bat_min_stb = IBAT_pas.a64 - blm_I_step_025perc;
        ibat_stable_cnt = (ibat_stable_cnt > 4) ? ibat_stable_cnt - 4 : 0;
    } else if (ibat_stable_cnt < 150) {
        ibat_stable_cnt++;
    }
    ibat_stable = (ibat_stable_cnt >= 150);
}


void blm_move_up_down_vtarg_limits(void) {
	baslangic_v_stbl=VRECT_pas.a16;	// vrect stabil iken bu fonksiyon çağırılıyor ve istenen değerler belirleniyor.
	blm_vtarg_move_up_targ=baslangic_v_stbl+blm_V_move_up_set;
	blm_vtarg_move_dn_targ=baslangic_v_stbl-blm_V_move_dn_set;
	blm_vtarg_move_up_max=targ_DC_voltage*(1+(EpD[VRECT_DC_HIGH_LIM_add][0].V1/100)-0.01); // ayarlanan vdc rect max değerinden yüzde 1 aşağısı hesaplanıyor burada
	blm_vtarg_move_dn_min=targ_DC_voltage/(1+(EpD[VRECT_DC_LOW_LIM_add][0].V1/100)+0.01); // VRECT_DC_LOW_LIM_add demek ayarlanan dc rect voltajından ne kadar aşağıya inmesine izin veriliyor rect voltajın

	if (blm_vtarg_move_up_targ > blm_vtarg_move_up_max) {// targete kadar gidebilir ancak target minimumun altındaysa minimuma kadar gidebilir.
		blm_vtarg_move_up_targ = blm_vtarg_move_up_max;
	}
	if (blm_vtarg_move_dn_targ < blm_vtarg_move_dn_min) {
		blm_vtarg_move_dn_targ = blm_vtarg_move_dn_min;
	}
	blm_V_move_up_dist = blm_vtarg_move_up_targ-baslangic_v_stbl; // for debug only
	blm_V_move_dn_dist = baslangic_v_stbl-blm_vtarg_move_dn_targ;
}

float calculate_blm_op(void);
float calculate_blm_op2(void);
float calculate_blm_op(void) {
    if (blm_corr_buf_index < 2) {
    	discard_corr_result=1;
        return -1;
    }

    float sum_v = 0.0f, sum_i = 0.0f;
    float sum_v2 = 0.0f, sum_i2 = 0.0f;
    float sum_vi = 0.0f;

    for (uint16_t i = 0; i < blm_corr_buf_index; i++) {
    	float v = vrect_buf[i];
    	float ib = ibat_buf[i];

        sum_v += v;
        sum_i += ib;
        sum_v2 += v * v;
        sum_i2 += ib * ib;
        sum_vi += v * ib;
    }

    float mean_v = sum_v / blm_corr_buf_index;
    float mean_i = sum_i / blm_corr_buf_index;

    float var_v = sum_v2 - blm_corr_buf_index * mean_v * mean_v;
    float var_i = sum_i2 - blm_corr_buf_index * mean_i * mean_i;
    float cov_vi = sum_vi - blm_corr_buf_index * mean_v * mean_i;


    	if (var_v <= 0.0f || var_i <= 0.0f) {
    		discard_corr_result=1;
			return -2;
    	}
    float corr = cov_vi / sqrtf(var_v * var_i);

    if (corr > 1.0f) corr = 1.0f;
    else if (corr < -1.0f) corr = -1.0f;

    blm_corr_buf_index=0;
    return corr;
}

uint8_t are_all_equal_fc(uint8_t phase) {
    uint32_t ref = zcr_record_vals[phase][0];
    for (int i = 1; i < 10; i++) {
        if (zcr_record_vals[phase][i] != ref) {
            return 0;
        }
    }
    return 1;
}

//void print_active_states(void) {
//	for (int i = 0; i < NUM_STATE_NAMES; i++) {
//		if (state_list[i].action & (1 << LCD_roll_enum) && state_list[i].action & (1 << ACTIVE_enum)) {
//			PRF_GEN("%s", state_list[i].name);
//		}
//	}
//}

//#include <math.h>

// alpha_deg: derece cinsinden tetikleme açısı
// i_dc: çıkış DC akımı (IRECT_pas.a64)
// dönüş: giriş faz RMS akımı
float calc_phase_rms_current(float i_dc, float alpha_deg)
{
    float alpha_rad = alpha_deg * (M_PI / 10000.0f); // derece → rad
    float factor = (M_PI - alpha_rad + 0.5f * sinf(2.0f * alpha_rad)) / M_PI;
    float i_ac_rms = (i_dc / sqrtf(3.0f)) * sqrtf(factor);
    return i_ac_rms;
}

void print_active_states() {
    for (int i = 0; i < NUM_STATE_NAMES; i++) {
        if (state_list[i].action & (1 << ACTIVE_enum)) {
            PRF_GEN("%s", state_list[i].name);
        }
    }
}
void state_set(State_Codes state, uint8_t set) {
    if (set) {
        state_list[state].action |= (1 << ACTIVE_enum); // Set the ACTIVE_enum bit
    } else {
        state_list[state].action &= ~(1 << ACTIVE_enum); // Clear the ACTIVE_enum bit
    }
}
//void set_state_active(State_Codes state) {
//	state_list[state].action |= (1 << ACTIVE_enum);
//}
//void set_state_deactive(State_Codes state) {
//    state_list[state].action &= ~(1 << ACTIVE_enum);
//}
//uint8_t state_get(State_Codes state) {
//	return (state_list[state].action & (1 << ACTIVE_enum)) != 0;
//}
uint8_t state_get(State_Codes state) {
    return ((state_list[state].action & (1 << ACTIVE_enum)) != 0) ? 1 : 0;
}
uint8_t is_state_require_stop(State_Codes state) {
    return ((state_list[state].action & (1 << THYSTOP_enum)) != 0) ? 1 : 0;
}
uint8_t is_state_a_general_fault(State_Codes state) {
    return ((state_list[state].action & (1 << GEN_F_LED_enum)) != 0) ? 1 : 0;
}
uint8_t is_state_require_save(State_Codes state) {
    return ((state_list[state].action & (1 << SAVE_enum)) != 0) ? 1 : 0;
}





