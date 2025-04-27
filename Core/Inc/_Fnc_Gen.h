volatile void prfm(char *string);
void delay_1ms(uint32_t num1);
inline void delayA_1ms(uint32_t ms);
inline void delayA_1us(uint32_t us);
inline void delayA_100ns(uint32_t us);
void printFaultCodes(void);
void inline extern set_variables_from_EEP_fc(uint8_t scope);
void apply_state_changes_f(State_Codes state_code, uint8_t set);
void inline extern set_V_targ_con_sy(float set_val);
//void inline extern update_VDC_high_low_lim_fc(void);
void inline extern actions_after_charge_mode_change(uint8_t num);
static inline uint8_t is_state_active(State_Codes state_code);
void inline extern actions_after_charge_voltage_change();
//void inline extern blm_cancel_op_return_to_delay(void);


void compress_REL_OUT_order_to_parts(void);
void generate_REL_OUT_order_vect_from_eeprom_parts_fc(void);
void generate_rel_ord_tb_from_REL_OUT_order_vector_fc(void);
void generate_REL_OUT_order_vect_from_ord_table_fc(void);
void generate_REL_24Bit_Data_fc(void);
void change_rel_vals_in_tables_f(rel_names_t rname, uint8_t new_val);

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
    snprintf(dma_uart_buffer[buffer_head], MAX_STRING_LENGTH, "\n\r%s", string);

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

void en_uart_msg_group(uint32_t group)
{
    enabled_message_groups |= group;
}

void disb_uart_msg_group(uint32_t group)
{
    enabled_message_groups &= ~group;
}





void printBinary(uint8_t num) {
    PRF_GEN("0b");
    for (int i = 7; i >= 0; i--) { // Loop through each bit
        PRF_GEN("%d", (num >> i) & 1);
    }
    PRF_GEN("\n");
}


void buttonScn(void) {
	if (ButtScanDelay_cnt > 30) {
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
		bup_down_cnt++;
	} else {
		bup_down_cnt++;
		if (bup_down_cnt >= 12) {
			bup_down_cnt = 8;
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
		bdown_down_cnt++;
	} else {
		bdown_down_cnt++;
		if (bdown_down_cnt >= 12) {
			bdown_down_cnt = 8;
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
	if (thy_drv_en==1 && IRECT_smp_sc > EpD[RECT_SHORT][0].V1 && !is_state_active(RECT_SHORT_FC)) {
		IRECT_Short_Acc_cnt++;
		IRECT_Short_Ret_Acc_cnt=0;
		if (IRECT_Short_Acc_cnt >= IRECT_Short_Acc_per) {
			IRECT_Short_Acc_cnt=0;
			apply_state_changes_f(RECT_SHORT_FC, 1);
			PRF_GEN("DC SH %f", IRECT_smp_sc);
		}
	} else { IRECT_Short_Acc_per=0; }
	if (IRECT_smp_sc <= EpD[RECT_SHORT][0].V1 && is_state_active(RECT_SHORT_FC)) {
		IRECT_Short_Ret_Acc_cnt++;
		IRECT_Short_Acc_cnt=0;
		if (IRECT_Short_Ret_Acc_cnt >= IRECT_Short_Ret_Acc_per) {
			IRECT_Short_Ret_Acc_cnt=0;
			apply_state_changes_f(RECT_SHORT_FC, 0);
		}
	} else { IRECT_Short_Ret_Acc_cnt=0; }
	if (thy_drv_en==1 && IBAT_smp_sc > EpD[BATT_SHORT][0].V1 && !is_state_active(BATT_SHORT_FC)) {
		IBAT_Short_Acc_cnt++;
		IBAT_Short_Ret_Acc_cnt=0;
		if (IBAT_Short_Acc_cnt >= IBAT_Short_Acc_per) {
			IBAT_Short_Acc_cnt=0;
			apply_state_changes_f(BATT_SHORT_FC, 1);
			PRF_GEN("BT SH %f", IBAT_smp_sc);
		}
	} else { IBAT_Short_Acc_cnt=0; }
	if (IBAT_smp_sc <= IBAT_Short_Lim && is_state_active(BATT_SHORT_FC)) {
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

void inline extern set_variables_from_EEP_fc(uint8_t scope) { // n012
    if (scope & SCOPE_VOLTAGE_LIMITS_FROM_EEP || scope == SCOPE_VAR_ALL_FROM_EEP) {
    	Vdc_float_min=EpD[DEV_NOM_VOUT][0].V1*0.9; // Normal şarj rejimi gerilim ayar aralığı
    	Vdc_float_max=EpD[DEV_NOM_VOUT][0].V1*1.15; // Normal şarj rejimi gerilim ayar aralığı
    	Vdc_boost_min=EpD[DEV_NOM_VOUT][0].V1*1.15; // Tam şarj rejimi gerilim ayar aralığı
    	Vdc_boost_max=EpD[DEV_NOM_VOUT][0].V1*1.3; // Tam şarj rejimi gerilim ayar aralığı
    }
    if (scope & SCOPE_DROPPER_LIMITS_FROM_EEP || scope == SCOPE_VAR_ALL_FROM_EEP) {
    	// D.A. gerilim regülasyonu (dropping diyot)
    	// limitler şartnamede anma geriliminin yüzdesi olarak belirtilmiş.
    	//if (scope==dropper_limits) {
    	//Vdc_drop_in_min=EpD[DEV_NOM_VOUT][0].V1*0.9; // D.A. gerilim regülasyonu giriş gerilimi
    	//Vdc_drop_in_max=EpD[DEV_NOM_VOUT][0].V1*1.3; // D.A. gerilim regülasyonu giriş gerilimi
        dropp_reg_high_lim = EpD[DEV_NOM_VOUT][0].V1 * (1 + (EpD[dropp_reg_high_lim_add][0].V1 / 100));
        dropp_reg_low_lim  = EpD[DEV_NOM_VOUT][0].V1 * (1 - (EpD[dropp_reg_low_lim_sub][0].V1 / 100));
    }
    if (scope & SCOPE_CURRENT_LIMITS_FROM_EEP || scope == SCOPE_VAR_ALL_FROM_EEP) {
        Irect_max = EpD[DEV_NOM_IOUT][0].V1 * 1.0;
        Irect_min = EpD[DEV_NOM_IOUT][0].V1 * 0.01;
        Ibat_max  = EpD[DEV_NOM_IOUT][0].V1 * 1.0;
        Ibat_min  = EpD[DEV_NOM_IOUT][0].V1 * 0.1;
    }
    if (scope & SCOPE_BLM_LIMITS_FROM_EEP || scope == SCOPE_VAR_ALL_FROM_EEP) {
        blm_I_step_05perc  = EpD[DEV_NOM_IOUT][0].V1 * 0.005;
        blm_I_step_075perc = EpD[DEV_NOM_IOUT][0].V1 * 0.0075;
        blm_I_step_10perc  = EpD[DEV_NOM_IOUT][0].V1 * 0.010;
        blm_V_step_05perc  = EpD[DEV_NOM_VOUT][0].V1 * 0.005;
        blm_V_step_15perc  = EpD[DEV_NOM_VOUT][0].V1 * 0.015;
    }
    if (scope == SCOPE_VAR_ALL_FROM_EEP) {
        Vbat_flt = EpD[DEV_NOM_VOUT][0].V1 * 0.1;
        VAC_Hg_Lim = VAC_Nom * (1 + 0.1); // Giriş voltajı monitör
        VAC_Lo_Lim = VAC_Nom * (1 - 0.12); // Giriş voltajı monitör
    }
//
    if (scope == SCOPE_VRECT_DC_HIGH_LOW_LIM_EEP) {
    	vrect_dc_high_lim=V_targ_con_sy*(1+(EpD[VRECT_DC_HIGH_LIM_add][0].V1/100));
    	vrect_dc_high_lim_ret=V_targ_con_sy*(1+(EpD[VRECT_DC_HIGH_LIM_add][0].V1/100)-0.01);
    	vrect_dc_low_lim=V_targ_con_sy/(1+(EpD[VRECT_DC_LOW_LIM_add][0].V1/100));
    	vrect_dc_low_lim_ret=V_targ_con_sy/(1+(EpD[VRECT_DC_LOW_LIM_add][0].V1/100)-0.01);
    }
}

void inline extern set_V_targ_con_sy(float set_val) {
	V_targ_con_sy=set_val;
	set_variables_from_EEP_fc(SCOPE_VRECT_DC_HIGH_LOW_LIM_EEP);
}

void inline extern actions_after_charge_voltage_change() {
	if (EpD[SET_CHARGE_MODE][0].V1 == FLOAT) {
		Current_charge_voltage=EpD[VBAT_FLOAT][0].V1;
		set_V_targ_con_sy(Current_charge_voltage);
	} else if (EpD[SET_CHARGE_MODE][0].V1 == BOOST) {
		Current_charge_voltage=EpD[VBAT_BOOST][0].V1;
		set_V_targ_con_sy(Current_charge_voltage);
	} else if (EpD[SET_CHARGE_MODE][0].V1 == TIMED) {
		Current_charge_voltage=EpD[VBAT_BOOST][0].V1;
		set_V_targ_con_sy(Current_charge_voltage);
	}
}
// does required changes after a charge mode chage
void inline extern actions_after_charge_mode_change(uint8_t num) {
	if (EpD[SET_CHARGE_MODE][0].V1 == FLOAT) {
		Current_charge_voltage=EpD[VBAT_FLOAT][0].V1;	// şarj modu hedef voltajını geçici olarak tutan variable
		I_batt_targ_con_sy=EpD[SET_IBAT_FLOAT][0].V1;
		set_V_targ_con_sy(Current_charge_voltage);
		LED_7_Data &= ~BOOST_CHARGE_LED;
		LED_7_Data |= FLOAT_CHARGE_LED;
		switch_to_auto_mode_completed=0;
		timed_mode_actions_do_once=0;
		charge_mode_timed_time_sec=0; // ekrandaki timed mode kalan saniye değerini kaldır
		PRF_GEN("FLOAT charge mode %d", num);
	} else if (EpD[SET_CHARGE_MODE][0].V1 == BOOST) {
		Current_charge_voltage=EpD[VBAT_BOOST][0].V1;
		I_batt_targ_con_sy=EpD[SET_IBAT_BOOST][0].V1;
		set_V_targ_con_sy(Current_charge_voltage);
		LED_7_Data &= ~FLOAT_CHARGE_LED;
		LED_7_Data |= BOOST_CHARGE_LED;
		switch_to_auto_mode_completed=0;
		timed_mode_actions_do_once=0;
		charge_mode_timed_time_sec=0; // ekrandaki timed mode kalan saniye değerini kaldır
		PRF_GEN("BOOST charge mode %d", num);
	} else if (EpD[SET_CHARGE_MODE][0].V1 == TIMED) {
		Current_charge_voltage=EpD[VBAT_BOOST][0].V1;
		I_batt_targ_con_sy=EpD[SET_IBAT_BOOST][0].V1;
		set_V_targ_con_sy(Current_charge_voltage);
		LED_7_Data &= ~FLOAT_CHARGE_LED;
		LED_7_Data |= BOOST_CHARGE_LED;
		switch_to_auto_mode_completed=0;
		if (timed_mode_actions_do_once==0) {
			timed_mode_actions_do_once=1; // timed a geçiş yapıldığında bir kez uygulanacak. tekrar uygulanabilmesi için başka moda geçilmesi lazım.
			timed_mode_time_ended=0; // timed mod sayacı sıfırla. sayaç sonunda float a geçilecek
			charge_mode_timed_time_cnt=(uint32_t) (EpD[SET_BOOST_TIME][0].V1*60*1000/50);
		}
	}
}

void apply_state_changes_f(State_Codes state_code, uint8_t set) {
	uint32_t fault_bit = (1U << state_code);
	uint32_t led7_bit = (1U << (state_code-16));
	uint32_t REL8_bit = (1U << (state_code-29));
    if (set) {
        if (state_list[state_code].code < 16) {
        	LED_16_Data |= fault_bit; }  // activate LED if required
        if (state_list[state_code].code >= 16 && state_list[state_code].code < 23) {
        	LED_7_Data |= led7_bit; }  // activate LED 7 if required
        if (state_list[state_code].code >= 29 && state_list[state_code].code < 46) {
        	REL_MB_8Bit_Data |= REL8_bit; }  // activate REL 8 if required
        if (!!(state_list[state_code].action & (1 << SET_GEN_F_LED_enum))) {
        	LED_16_Data |= (1U << GENERAL_FAULT_FC); } // activate general fault LED if associated
        if (!!(state_list[state_code].action & (1 << THYSTOP_enum))) {  // stop thy drv if fault requires
        	thy_drv_en=0;
        	sfsta_op_phase = S_SFSTA_NONE;
            thy_stop_fault_hold_bits |= fault_bit;
        	LED_16_Data |= (1U << STOP_FC);
        	LED_16_Data &= ~(1U << START_FC); }
        if (state_code == START_FC) {
        	change_rel_vals_in_tables_f(START_STOP_REL, 1);
        }
        if (state_code == STOP_FC) {
        	change_rel_vals_in_tables_f(START_STOP_REL, 0);
        }
		state_list[state_code].action |= (1 << ACTIVE_enum); // set active flag in fault action bits

		if (!!(state_list[state_code].action & (1 << SAVE_enum)) == 1 ) { // eğer save biti 1 ise hafızaya kaydet
			Record_Fault_Code(state_code); }
    } else {
        if (state_list[state_code].code < 16) {
        	LED_16_Data &= ~fault_bit; }  // deactivate LED if resetting a fault with LED requirement
        if (state_list[state_code].code >= 16 && state_list[state_code].code < 23) {
        	LED_7_Data &= ~led7_bit; }  // deactivate LED 7 if required
        if (state_list[state_code].code >= 29 && state_list[state_code].code < 46) {
        	REL_MB_8Bit_Data &= ~REL8_bit; }  // deactivate REL 8 if required
        if (!!(state_list[state_code].action & (1 << SET_GEN_F_LED_enum))) { // deactivate general fault LED if associated
        	LED_16_Data &= ~(1U << GENERAL_FAULT_FC); }
        if (!!(state_list[state_code].action & (1 << THYSTOP_enum))) { // thy stop gerektiren bir arıza reset ediliyor
            thy_stop_fault_hold_bits &= ~fault_bit; // bu variable'ı güncelle. deactive edilen fault'un bit'inin resetlenmesi gerekiyor.
        }
		state_list[state_code].action &= ~(1U << ACTIVE_enum); // reset active flag in fault action bits
    }
		PRF_GEN("     state_code %d %s set %d", state_code, state_list[state_code].name, set);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// OUT RELAY ACTIVATE DEACTIVATE ////////////////////////////////////////////////////////////////////////////////
    rel_names_t relCode = state_list[state_code].rel_dat_nm;  // 4. sütunda tanımlı enum
    // ACTIVE_enum set mi değil mi
    uint8_t activeBitSet = (state_list[state_code].action & (1 << ACTIVE_enum)) ? 1 : 0;
    change_rel_vals_in_tables_f(relCode, activeBitSet);
////// OUT RELAY ACTIVATE DEACTIVATE ////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

static inline uint8_t is_state_active(State_Codes state_code) {
    	return (!!(state_list[state_code].action & (1U << ACTIVE_enum))) == 1;
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

int read_data(uint8_t temp_sens_count, uint16_t *raw_vals)
{

	U10_rxCount = 0;
    for (int i = 0; i < U10_RX_BUFFER_SIZE; i++) {
    	U10_rxBuf[i] = 0;
    }

    USART10_SendByte(0x55);
    delayA_1us(10);
    USART10_SendByte(0xF1);

    for (int i = 0; i < temp_sens_count; i++) {
        // ham değer = LSB + (MSB << 8)
        uint8_t lsb = U10_rxBuf[2 + i*2];
        uint8_t msb = U10_rxBuf[3 + i*2];
        raw_vals[i] = (msb << 8) | lsb;
    }

    return 0;
}

static float tmp144_convert_temperature(uint16_t raw16) {
  int16_t val12 = (raw16 >> 4) & 0x0FFF;
  if (val12 & 0x0800) {
    val12 |= 0xF000;
  }
  return (float)val12 * 0.0625f;
}

int tmp144_init_and_assign(void)
{
	U10_rxCount = 0;
    for (int i = 0; i < U10_RX_BUFFER_SIZE; i++) {
    	U10_rxBuf[i] = 0;
    }

    USART10_SendByte(0x55);
    delayA_1us(10);
    USART10_SendByte(0x8C);
    delayA_1us(10);
    USART10_SendByte(0x90);
    delay_1ms(1000);

    uint8_t dev_count_raw = U10_rxBuf[2];
    uint8_t temp_sens_count = dev_count_raw & 0x0F;

    if (temp_sens_count < 1 || temp_sens_count > 16) {
        return 0;
    }
    return temp_sens_count;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// OUT RELAY OPERATION //////////////////////////////////////////////////////////////////////////////////////////


void print_REL_OUT_Table() {
    for (int i = 0; i < 16; i++) {
		PRF_GEN("Order %d %s", rel_ord_tb[i].rel_ord_nm, rel_ord_tb[i].rel_ord_desc);
		delay_1ms(10);
    }
}

void calc_REL_24Bit_Data_f(void) {
	REL_24Bit_Data=(uint32_t)(REL_MB_8Bit_Data << 16) | (rel_out_16Bit_Data);
}


void compress_REL_OUT_order_to_parts(void) {
    REL_OUT_order_part1 = 0;
    REL_OUT_order_part2 = 0;
    REL_OUT_order_part3 = 0;
    REL_OUT_order_part4 = 0;

    for (int i = 0; i < 16; i++) {
        uint32_t val = REL_OUT_order[i] & 0x1F;  // 5-bit

        if (i < 4) {
            REL_OUT_order_part1 |= (val << (i * 5));
        } else if (i < 8) {
            REL_OUT_order_part2 |= (val << ((i - 4) * 5));
        } else if (i < 12) {
            REL_OUT_order_part3 |= (val << ((i - 8) * 5));
        } else {
            REL_OUT_order_part4 |= (val << ((i - 12) * 5));
        }
    }

    EpD[REL_OUT_1][0].V1 = REL_OUT_order_part1; EpD[REL_OUT_1][1].V1 = REL_OUT_order_part1;
    EpD[REL_OUT_2][0].V1 = REL_OUT_order_part2; EpD[REL_OUT_2][1].V1 = REL_OUT_order_part2;
    EpD[REL_OUT_3][0].V1 = REL_OUT_order_part3; EpD[REL_OUT_3][1].V1 = REL_OUT_order_part3;
    EpD[REL_OUT_4][0].V1 = REL_OUT_order_part4; EpD[REL_OUT_4][1].V1 = REL_OUT_order_part4;

    Rec_Dat_to_EEp_f(REL_OUT_1);
    Rec_Dat_to_EEp_f(REL_OUT_2);
    Rec_Dat_to_EEp_f(REL_OUT_3);
    Rec_Dat_to_EEp_f(REL_OUT_4);
}


void generate_REL_OUT_order_vect_from_eeprom_parts_fc(void) {

    REL_OUT_order_part1 = EpD[REL_OUT_1][0].V1;
    REL_OUT_order_part2 = EpD[REL_OUT_2][0].V1;
    REL_OUT_order_part3 = EpD[REL_OUT_3][0].V1;
    REL_OUT_order_part4 = EpD[REL_OUT_4][0].V1;

    for (int i = 0; i < 16; i++) {
        uint32_t val;

        if (i < 4) {
            val = (REL_OUT_order_part1 >> (i * 5)) & 0x1F;
        } else if (i < 8) {
            val = (REL_OUT_order_part2 >> ((i - 4) * 5)) & 0x1F;
        } else if (i < 12) {
            val = (REL_OUT_order_part3 >> ((i - 8) * 5)) & 0x1F;
        } else {
            val = (REL_OUT_order_part4 >> ((i - 12) * 5)) & 0x1F;
        }
        REL_OUT_order[i] = (rel_names_t)val;
    }
}

void generate_rel_ord_tb_from_REL_OUT_order_vector_fc(void) {
    for (int i = 0; i < 16; i++) {
    	rel_ord_tb[i].rel_ord_nm=REL_OUT_order[i];
    	rel_ord_tb[i].rel_ord_desc=rel_dat_tb[rel_ord_tb[i].rel_ord_nm].rel_dat_desc;
    }
}

void generate_REL_OUT_order_vect_from_ord_table_fc(void) {
    for (int i = 0; i < 16; ++i) {
        REL_OUT_order[i] = rel_ord_tb[i].rel_ord_nm;
    }
}
void generate_REL_24Bit_Data_fc(void) {
    rel_out_16Bit_Data = 0; // Clear current value

    for (int i = 0; i < 16; ++i) {
        uint8_t order = rel_ord_tb[i].rel_ord_order;
        uint8_t val = rel_ord_tb[i].rel_ord_val;

        if (order < 16) {
            if (val) {
                rel_out_16Bit_Data |= (1 << order);
            } else {
                rel_out_16Bit_Data &= ~(1 << order);
            }
        	REL_24Bit_Data=(uint32_t)(REL_MB_8Bit_Data << 16) | (rel_out_16Bit_Data);
        }
    }
}

void change_rel_vals_in_tables_f(rel_names_t rname, uint8_t new_val)
{
    // Update rel_dat_tb
    for (int i = 0; i < rel_dat_tb_size; i++) {
        if (rel_dat_tb[i].rel_dat_nm == rname) {
            rel_dat_tb[i].rel_dat_val = new_val;
            break;  // Found the matching enum, so we can stop searching
        }
    }
    // Update rel_ord_tb
    for (int j = 0; j < rel_ord_tb_size; j++) {
        if (rel_ord_tb[j].rel_ord_nm == rname) {
            rel_ord_tb[j].rel_ord_val = new_val;
            generate_REL_24Bit_Data_fc(); // röle değerleri update edildiği için 24 bit değer de güncelleniyor.
            break;  // Found and updated, so we can stop searching
        }
    }
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


//void inline extern blm_discard_corr_restart_skip_delay(void) {
//	blm_op_phase = 100;
//	blm_enable_collect_samples = 0;
//	blm_corr_buf_index = 0;
//}
//void inline extern blm_cancel_op_return_to_delay(void) {
//	blm_op_phase = 101;
//	blm_corr_op_start_delay_cnt = 0;
//	blm_enable_collect_samples = 0;
//	blm_corr_buf_index = 0;
//}

void inline extern bring_vtarg_back_skip_delay(void) {
	if (V_targ_con_sy < Current_charge_voltage - blm_V_step_05perc) {
		set_V_targ_con_sy(V_targ_con_sy + blm_V_step_05perc);
	} else if (V_targ_con_sy > Current_charge_voltage + blm_V_step_05perc) {
		set_V_targ_con_sy(V_targ_con_sy - blm_V_step_05perc);
	} else {
		set_V_targ_con_sy(Current_charge_voltage); // hedefe ulaşınca sabitle
		blm_op_phase = 0;
	}
}

void inline extern bring_vtarg_back_goto_delay(void) {
	if (V_targ_con_sy < Current_charge_voltage - blm_V_step_05perc) {
		set_V_targ_con_sy(V_targ_con_sy + blm_V_step_05perc);
	} else if (V_targ_con_sy > Current_charge_voltage + blm_V_step_05perc) {
		set_V_targ_con_sy(V_targ_con_sy - blm_V_step_05perc);
	} else {
		set_V_targ_con_sy(Current_charge_voltage); // hedefe ulaşınca sabitle
		blm_op_phase = 9;
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
void stability_ibat_fc(void) {
    if (IBAT_pas.a64 > i_bat_max_stb) {
        i_bat_max_stb = IBAT_pas.a64 + blm_I_step_05perc/2;
        i_bat_min_stb = IBAT_pas.a64 - blm_I_step_05perc/2;
        ibat_stable_cnt = (ibat_stable_cnt > 4) ? ibat_stable_cnt - 4 : 0;
    } else if (IBAT_pas.a64 < i_bat_min_stb) {
        i_bat_max_stb = IBAT_pas.a64 + blm_I_step_05perc/2;
        i_bat_min_stb = IBAT_pas.a64 - blm_I_step_05perc/2;
        ibat_stable_cnt = (ibat_stable_cnt > 4) ? ibat_stable_cnt - 4 : 0;
    } else if (ibat_stable_cnt < 150) {
        ibat_stable_cnt++;
    }
    ibat_stable = (ibat_stable_cnt >= 150);
}


void blm_set_up_down_vtarg_limits(void) {
	blm_stable_v_vrect=VRECT_pas.a16;	// vrect stabil iken bu fonksiyon çağırılıyor ve istenen değerler belirleniyor.
	blm_vtarg_move_up_targ=blm_stable_v_vrect+blm_V_step_05perc*5;
	blm_vtarg_move_dn_targ=blm_stable_v_vrect-blm_V_step_05perc*5;
	blm_vtarg_move_up_max=V_targ_con_sy*(1+(EpD[VRECT_DC_HIGH_LIM_add][0].V1/100)-0.01);
	blm_vtarg_move_dn_min=V_targ_con_sy/(1+(EpD[VRECT_DC_LOW_LIM_add][0].V1/100)-0.01);

	if (blm_vtarg_move_up_max <= blm_vtarg_move_up_targ) {
		blm_vtarg_move_up_max = blm_vtarg_move_up_targ;
	}
	if (blm_vtarg_move_dn_min >= blm_vtarg_move_dn_targ) {
		blm_vtarg_move_dn_min = blm_vtarg_move_dn_targ;
	}
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
    	float vt = vtarg_buf[i];
    	float ib = ibat_buf[i];

        sum_v += v;
        sum_i += ib;
        sum_v2 += v * v;
        sum_i2 += ib * ib;
        sum_vi += v * ib;

		if (v > blm_VRECT_pas_max) {
			blm_VRECT_pas_max = v;
			blm_VRECT_pas_max_ind = i;
		}
		if (v < blm_VRECT_pas_min) {
			blm_VRECT_pas_min = v;
			blm_VRECT_pas_min_ind = i;
		}
		if (vt > V_targ_con_sy_max) {
			V_targ_con_sy_max = vt;
			V_targ_con_sy_max_ind = i;
		}
		if (vt < V_targ_con_sy_min) {
			V_targ_con_sy_min = vt;
			V_targ_con_sy_min_ind = i;
		}
    }

    float mean_v = sum_v / blm_corr_buf_index;
    float mean_i = sum_i / blm_corr_buf_index;

    float var_v = sum_v2 - blm_corr_buf_index * mean_v * mean_v;
    float var_i = sum_i2 - blm_corr_buf_index * mean_i * mean_i;
    float cov_vi = sum_vi - blm_corr_buf_index * mean_v * mean_i;


		if (fabs(blm_VRECT_pas_max-blm_VRECT_pas_min) > blm_V_step_15perc) {
			blm_VRECT_changed = 1;
		} else {
			blm_VRECT_changed = 0;
		}

		blm_vrect_vtarg_min_diff=fabs(blm_VRECT_pas_min-V_targ_con_sy_min);
		blm_vrect_vtarg_max_diff=fabs(blm_VRECT_pas_min-V_targ_con_sy_min);

    	PRF_BLM("  blm_corr_buf_index var_v var_i %d %f %f", blm_corr_buf_index, var_v, var_i);
    	PRF_BLM("  vrmx vrmxi vrmn vrmni %f %d %f %d", blm_VRECT_pas_max, blm_VRECT_pas_max_ind, blm_VRECT_pas_min, blm_VRECT_pas_min_ind);
    	PRF_BLM("  vtmx vtmxi vtmn vtmni %f %d %f %d", V_targ_con_sy_max, V_targ_con_sy_max_ind, V_targ_con_sy_min, V_targ_con_sy_min_ind);
    	PRF_BLM("  blm_vrect_vtarg_min_diff blm_vrect_vtarg_max_diff %f %f", blm_vrect_vtarg_min_diff, blm_vrect_vtarg_max_diff);






    	blm_VRECT_pas_max = 0;
    	blm_VRECT_pas_min = 1000;
    	V_targ_con_sy_max = 0;
    	V_targ_con_sy_min = 1000;


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


