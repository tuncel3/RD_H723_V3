volatile void prfm(char *string);
void extern umsg(uint32_t msg_group, char *string);
void delay_1ms(uint32_t num1);
inline void delayA_1ms(uint32_t ms);
inline void delayA_1us(uint32_t us);
inline void delayA_100ns(uint32_t us);
void printFaultCodes(void);
void inline extern get_max_min_lims_from_DEV_NOM_VOUT(void);
void apply_state_changes_f(State_Codes state_code, uint8_t set);
void inline extern set_V_targ_con_sy(float set_val);
void inline extern update_VDC_high_low_lim_fc(void);
void inline extern actions_after_charge_mode_change(uint8_t num);
void inline extern toggle_batt_inspection_direction(uint8_t num);
static inline uint8_t is_state_active(State_Codes state_code);

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

void extern umsg(uint32_t msg_group, char *string)
{
    // Check if the message group is enabled
    if (!(enabled_message_groups & msg_group)) {
        return; // Skip printing if the group is disabled
    }

    // Copy string to DMA buffer safely
    snprintf(dma_uart_buffer[buffer_head], MAX_STRING_LENGTH, "%s\n\r", string);

    // Move buffer head
    buffer_head = (buffer_head + 1) % BUFFER_SIZE;

    // Check for buffer overflow
    if (buffer_head == buffer_tail) {
        buffer_tail = (buffer_tail + 1) % BUFFER_SIZE; // Discard oldest data
    }

    // Start DMA transmission if not busy
    if (!dma_busy) {
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
    sprintf(DUB,"0b"); prfm(DUB);
    for (int i = 7; i >= 0; i--) { // Loop through each bit
        sprintf(DUB,"%d", (num >> i) & 1); prfm(DUB);
    }
    sprintf(DUB,"\n"); prfm(DUB);
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
			sprintf(DUB,"DC SH %f", IRECT_smp_sc); prfm(DUB);
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
			sprintf(DUB,"BT SH %f", IBAT_smp_sc); prfm(DUB);
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

void inline extern reset_batt_inspection_procedure_state_vars(void) {

	start_bat_inspection_req=0;
		gercek_voltaj_stable_ms=0;
	gercek_voltaj_stable_ms_Acc_cnt=0;
		check_gercek_voltaj_stable_ms=1;
	vout_sample_ready=0;
		bat_inspection_unknow_state_cnt=0;
	bat_inspection_canceled=1;
	set_V_targ_con_sy(Current_charge_voltage);
}

void inline extern end_batt_inspect_return_to_normal(uint8_t num) {

		actions_after_charge_mode_change(10+num);
		reset_batt_inspection_procedure_state_vars();

}
void inline extern batt_line_broken_fn(void) {

		actions_after_charge_mode_change(3);
		reset_batt_inspection_procedure_state_vars();

			batt_line_broken=1;
			apply_state_changes_f(BATT_LINE_BROKEN_FC, 1);

}
void inline extern batt_line_OK_fn(void) {

		actions_after_charge_mode_change(4);
		reset_batt_inspection_procedure_state_vars();

			batt_line_broken=0;
			apply_state_changes_f(BATT_LINE_BROKEN_FC, 0);
}
void inline extern toggle_batt_inspection_direction(uint8_t num) {

	if (batt_inspection_direction==1) {
		batt_inspection_direction=-1;
		sprintf(DUB," Toggled direction dir is %d num %d", batt_inspection_direction, num); umsg(pr_btln, DUB);
	} else {
		batt_inspection_direction=1;
		sprintf(DUB," Toggled direction dir is %d num %d", batt_inspection_direction, num); umsg(pr_btln, DUB);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// AKÜ HATTI KOPUK //////////////////////////////////////////////////////////////////////////////////////////////

void inline extern aku_hatti_kopuk_fc_inl(void) {

// VOUT STABILITY
if (ms_50_cnt-gercek_voltaj_smp_ms_timer_h >= gercek_voltaj_smp_ms_timer_per && check_gercek_voltaj_stable_ms==1) {
	gercek_voltaj_diff_from_sec_ms=ABS(gercek_voltaj_smp_ms-VRECT_per_avg_roll_sc);
	if (gercek_voltaj_diff_from_sec_ms <= 0.1) { // önceki sample ile aralarıdaki fark düşünce. stabil olduğu varsayılıyor.
		gercek_voltaj_stable_ms_Acc_cnt++;
//		sprintf(DUB,"diff < set val"); umsg(pr_btln, DUB);
		if (gercek_voltaj_stable_ms_Acc_cnt >= 10) {
			gercek_voltaj_stable_ms_Acc_cnt=0;
			gercek_voltaj_stable_ms=1;
		}
	} else {
		gercek_voltaj_stable_ms_Acc_cnt=0;
		gercek_voltaj_stable_ms=0;
//		sprintf(DUB,"diff > set val"); umsg(pr_btln, DUB);
	}
	gercek_voltaj_smp_ms_timer_h=ms_50_cnt;
	gercek_voltaj_smp_ms=VRECT_per_avg_roll_sc; // önceki sample
}

// ÇIKIŞ VOLTAJI TUT
if (gercek_voltaj_stable_ms==1 && vout_sample_req==1 && vout_sample_ready==0) {
	vout_sample_req=0; // voltaj stabil ise ve istek varsa o anki voltaj değeri tututluyor.
	vout_sample_ready=1;
	baslangic_voltaj_ms=VRECT_per_avg_roll_sc; // ismi baslangıc voltajı. yani inspection baslangıc voltajı.
	sprintf(DUB,"Vout sample taken"); umsg(pr_btln, DUB);
}

// BATTERY INSPECTION START TRIGGER
bat_inspection_req_timer_cnt++;
if (batt_current_detected==0 && bat_inspection_req_timer_cnt >= bat_inspection_req_timer_per) {
	bat_inspection_req_timer_cnt=0;
	start_bat_inspection_req=1; // zaman ayarlı inspection trigger. manuel de başlatılabilir.
	Batt_inspect_min=Current_charge_voltage*0.98;
	Batt_inspect_max=Current_charge_voltage*1.02;
	bat_inspection_req_timer_per=500;
} else if (batt_current_detected == 1 && bat_inspection_req_timer_cnt >= 100) {
	bat_inspection_req_timer_cnt = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BATTERY INSPECTION n013
if (start_bat_inspection_req==1) {
	bat_inspection_canceled=0;
	check_gercek_voltaj_stable_ms=0; // voltaj stabilite izlemesi durdurulabilir.
	if (ms_50_cnt-insp_loop_delay_h >= insp_loop_delay_per) {
		insp_loop_delay_h=ms_50_cnt;
		if (batt_current_detected==0) { // batarya voltajı varsa direk batarya hattı sağlam denebilir. inspection devam etmemeli.
			if (vout_sample_ready==0) { // inspection başlama voltajını tutma işlmi tamam mı.
				sprintf(DUB,"Vout sample not ready, request sample"); umsg(pr_btln, DUB);
				vout_sample_req=1;
				check_gercek_voltaj_stable_ms=1; // voltaj tutma işlemi gerçekleşmesi için voltaj stabilite testinin tamamlanması lazım.
			} else if (vout_sample_ready==1) {
				vout_sample_req=0;
				sprintf(DUB,"Vout sample ready request met"); umsg(pr_btln, DUB);
				gercek_baslangic_fark=ABS(baslangic_voltaj_ms - VRECT_per_avg_roll_sc); // voltaj kaydırma işlemi gerçekleştiriliyor.
				hedef_gercek_fark=ABS(V_targ_con_sy - VRECT_per_avg_roll_sc);	// tutulan voltaj ile hedef ve gerçek voltaj arası farklar izleniyor.
				hedef_baslangic_fark=ABS(V_targ_con_sy - baslangic_voltaj_ms);
//////////////////////////////////////////////////////////////////////////////////////////////
				if (batt_inspection_direction==-1) {
					if (gercek_baslangic_fark >= 0.5 && hedef_gercek_fark < 0.5 && hedef_baslangic_fark >= 0.5 && batt_line_broken==0) {
						batt_line_broken_fn();
						toggle_batt_inspection_direction(1);
						sprintf(DUB,"Batt line broken when dir was -1"); umsg(pr_btln, DUB);
					} else if (gercek_baslangic_fark <= 0.6 && hedef_gercek_fark > 0.6 && hedef_baslangic_fark >= 1) {
						batt_line_OK_fn();
						sprintf(DUB,"Batt line OK by cannot reduce vout. End of inspection"); umsg(pr_btln, DUB);
						toggle_batt_inspection_direction(6);
					} else {
						bat_inspection_unknow_state_cnt++;
						sprintf(DUB,"unknown state when dir was -1 %lu", bat_inspection_unknow_state_cnt); umsg(pr_btln, DUB);
						if (bat_inspection_unknow_state_cnt >= 20) {
							bat_inspection_unknow_state_cnt=0;
							sprintf(DUB,"Cancel batt insp when dir was -1, bc; too much unknown state"); umsg(pr_btln, DUB);
							end_batt_inspect_return_to_normal(1);
							toggle_batt_inspection_direction(2);
						}
					}
					if (V_targ_con_sy >= Batt_inspect_min+0.4 && bat_inspection_canceled==0) {
						set_V_targ_con_sy(V_targ_con_sy-0.3);
						sprintf(DUB,"reduce V"); umsg(pr_btln, DUB);
					}
					if (V_targ_con_sy < Batt_inspect_min+0.2) {
						min_v_reached_no_result_cnt++;
						sprintf(DUB,"min V reached and no result %lu", min_v_reached_no_result_cnt); umsg(pr_btln, DUB); // no result yet, but waiting for voltages to settle. there can be results in a few moments.
						if (min_v_reached_no_result_cnt >= 10) {
							min_v_reached_no_result_cnt=0;
							sprintf(DUB,"Cancel batt inspection, bc; min V reached"); umsg(pr_btln, DUB);
							bat_inspection_req_timer_h=ms_50_cnt-bat_inspection_req_timer_per; // fast restart inspection
							end_batt_inspect_return_to_normal(2);
							toggle_batt_inspection_direction(3);
						}
					}
				}
//////////////////////////////////////////////////////////////////////////////////////////////
				else if (batt_inspection_direction==1) {
					if (hedef_baslangic_fark >= 0.5 && gercek_baslangic_fark >= 0.5 && hedef_gercek_fark < 0.5 && batt_line_broken==0) {
						batt_line_broken_fn();
						toggle_batt_inspection_direction(4);
						sprintf(DUB,"Batt line broken when dir was 1"); umsg(pr_btln, DUB);
					} else {
						bat_inspection_unknow_state_cnt++;
						sprintf(DUB,"unknown state when dir was +1 %lu", bat_inspection_unknow_state_cnt); umsg(pr_btln, DUB);
						if (bat_inspection_unknow_state_cnt >= 20) {
							bat_inspection_unknow_state_cnt=0;
							sprintf(DUB,"Cancel batt insp when dir was +1, bc; too much unknown state"); umsg(pr_btln, DUB);
							end_batt_inspect_return_to_normal(3);
							toggle_batt_inspection_direction(5);
						}
					}
					if (V_targ_con_sy <= Batt_inspect_max-0.4 && bat_inspection_canceled==0) {
						set_V_targ_con_sy(V_targ_con_sy+0.3);
						sprintf(DUB,"increase V"); umsg(pr_btln, DUB);
					}
				}
//////////////////////////////////////////////////////////////////////////////////////////////
			}
		} else {
			batt_line_OK_fn();
			sprintf(DUB,"Batt line OK by current detection. End of inspection"); umsg(pr_btln, DUB);
		}
	}
} else if (start_bat_inspection_req==0 && bat_inspection_canceled==0) {
	end_batt_inspect_return_to_normal(4);
	sprintf(DUB,"Cancel batt inspection, bc; start_bat_inspection_req=0"); umsg(pr_btln, DUB);
}

// BATT CURRENT MONITOR
if ((IBAT_per_avg_sc > BATT_CURRENT_DETECT_THRESHOLD || IBAT_per_avg_sc < -BATT_CURRENT_DETECT_THRESHOLD)) {
	batt_current_detected_Acc_cnt++;
	if (batt_current_detected_Acc_cnt >= batt_current_detected_Acc_per && batt_current_detected==0) {
		batt_current_detected=1;
		sprintf(DUB,"batt_current_detected 1 curr %5.2f", IBAT_per_avg_sc); umsg(pr_btln, DUB);
		if (batt_line_broken==1) {
			batt_line_OK_fn();
			sprintf(DUB,"Batt line OK. batt curr det whil batt_line_brokn=1. Dir was %d", batt_inspection_direction); umsg(pr_btln, DUB);
		}
	}
} else if (IBAT_per_avg_sc <= BATT_CURRENT_DETECT_THRESHOLD || IBAT_per_avg_sc >= -BATT_CURRENT_DETECT_THRESHOLD) {
	batt_current_zero_Acc_cnt++;
	if (batt_current_zero_Acc_cnt >= batt_current_zero_Acc_per && batt_current_detected==1) {
		batt_current_detected=0;
		sprintf(DUB,"batt_current_detected 0"); umsg(pr_btln, DUB);
	}
}
////// AKÜ HATTI KOPUK //////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // void inline extern aku_hatti_kopuk_fc_inl(void) {

void inline extern get_max_min_lims_from_DEV_NOM_VOUT(void) { // n012
Vdc_float_min=EpD[DEV_NOM_VOUT][0].V1*0.9; // Normal şarj rejimi gerilim ayar aralığı
Vdc_float_max=EpD[DEV_NOM_VOUT][0].V1*1.15; // Normal şarj rejimi gerilim ayar aralığı
Vdc_boost_min=EpD[DEV_NOM_VOUT][0].V1*1.15; // Tam şarj rejimi gerilim ayar aralığı
Vdc_boost_max=EpD[DEV_NOM_VOUT][0].V1*1.3; // Tam şarj rejimi gerilim ayar aralığı

// D.A. gerilim regülasyonu (dropping diyot)
Vdc_drop_in_min=EpD[DEV_NOM_VOUT][0].V1*0.9; // D.A. gerilim regülasyonu giriş gerilimi
Vdc_drop_in_max=EpD[DEV_NOM_VOUT][0].V1*1.3; // D.A. gerilim regülasyonu giriş gerilimi
Vdc_drop_out_min=EpD[DEV_NOM_VOUT][0].V1*0.9; // D.A. gerilim regülasyonu çıkış gerilimi
Vdc_drop_out_max=EpD[DEV_NOM_VOUT][0].V1*1.15; // D.A. gerilim regülasyonu çıkış gerilimi


// D.A. çıkış akım ayarı
Irect_max=EpD[DEV_NOM_IOUT][0].V1*1.0; // Toplam çıkış
Irect_min=EpD[DEV_NOM_IOUT][0].V1*0.01; // Toplam çıkış
Ibat_max=EpD[DEV_NOM_IOUT][0].V1*1.0; // Akümülatör çıkış // release_chg -> EpD[DEV_NOM_IOUT][0].V1*1.0;
Ibat_min=EpD[DEV_NOM_IOUT][0].V1*0.1; // Akümülatör çıkış // release_chg -> EpD[DEV_NOM_IOUT][0].V1*0.1;

VAC_Hg_Lim=VAC_Nom*(1+0.1); // Giriş voltajı monitör
VAC_Lo_Lim=VAC_Nom*(1-0.12); // Giriş voltajı monitör
}
void inline extern update_VDC_high_low_lim_fc(void) {
	VRECT_DC_HIGH_LIM=V_targ_con_sy*(1+(EpD[VRECT_DC_HIGH_LIM_ADD][0].V1/100));
	VRECT_DC_HIGH_LIM_ret=V_targ_con_sy*(1+(EpD[VRECT_DC_HIGH_LIM_ADD][0].V1/100)-0.01);
	VRECT_DC_LOW_LIM=V_targ_con_sy/(1+(EpD[VRECT_DC_LOW_LIM_ADD][0].V1/100));
	VRECT_DC_LOW_LIM_ret=V_targ_con_sy/(1+(EpD[VRECT_DC_LOW_LIM_ADD][0].V1/100)-0.01);
	VLOAD_DC_HIGH_LIM=V_targ_con_sy*(1+(EpD[VLOAD_DC_HIGH_LIM_ADD][0].V1/100));
	VLOAD_DC_HIGH_LIM_ret=V_targ_con_sy*(1+(EpD[VLOAD_DC_HIGH_LIM_ADD][0].V1/100)-0.01);
	VLOAD_DC_LOW_LIM=V_targ_con_sy/(1+(EpD[VLOAD_DC_LOW_LIM_ADD][0].V1/100));
	VLOAD_DC_LOW_LIM_ret=V_targ_con_sy/(1+(EpD[VLOAD_DC_LOW_LIM_ADD][0].V1/100)-0.01);
}
void inline extern set_V_targ_con_sy(float set_val) {
	V_targ_con_sy=set_val;
	update_VDC_high_low_lim_fc();
}

// does required changes after a charge mode chage
void inline extern actions_after_charge_mode_change(uint8_t num) {
	if (EpD[SET_CHARGE_MODE][0].V1 == FLOAT) {
		Current_charge_voltage=EpD[VBAT_FLOAT][0].V1;	// şarj modu hedef voltajını geçici olarak tutan variable
		I_batt_targ_con_sy=EpD[SET_IBAT_FLOAT][0].V1;
		set_V_targ_con_sy(Current_charge_voltage);
		LED_7_Data &= ~BOOST_CHARGE_LED;
		LED_7_Data |= FLOAT_CHARGE_LED;
		switch_to_auto_mode_completed=0; // auto moddan başka bir moda geçildi.
		timed_mode_actions_do_once=0;
		charge_mode_timed_time_sec=0; // ekrandaki timed mode kalan saniye değerini kaldır
		if (num!=12 && num!=13)	{	// batt inspection numbers
			sprintf(DUB,"FLOAT charge mode %d", num); prfm(DUB);
		}
	} else if (EpD[SET_CHARGE_MODE][0].V1 == BOOST) {
		Current_charge_voltage=EpD[VBAT_BOOST][0].V1;
		I_batt_targ_con_sy=EpD[SET_IBAT_BOOST][0].V1;
		set_V_targ_con_sy(Current_charge_voltage);
		LED_7_Data &= ~FLOAT_CHARGE_LED;
		LED_7_Data |= BOOST_CHARGE_LED;
		switch_to_auto_mode_completed=0; // auto moddan başka bir moda geçildi.
		timed_mode_actions_do_once=0;
		charge_mode_timed_time_sec=0; // ekrandaki timed mode kalan saniye değerini kaldır
		if (num!=12 && num!=13)	{	// batt inspection numbers
			sprintf(DUB,"BOOST charge mode %d", num); prfm(DUB);
		}
	} else if (EpD[SET_CHARGE_MODE][0].V1 == TIMED) {
		Current_charge_voltage=EpD[VBAT_BOOST][0].V1;
		I_batt_targ_con_sy=EpD[SET_IBAT_BOOST][0].V1;
		set_V_targ_con_sy(Current_charge_voltage);
		LED_7_Data &= ~FLOAT_CHARGE_LED;
		LED_7_Data |= BOOST_CHARGE_LED;
		switch_to_auto_mode_completed=0; // auto moddan başka bir moda geçildi.
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
			sf_sta_req=0;
			sf_sta_req_ok=0;
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
		sprintf(DUB,"     state_code %d %s set %d", state_code, state_list[state_code].name, set); prfm(DUB);

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
		sprintf(DUB,"          state_code %d %s set %d", state_code, state_list[state_code].name, set); prfm(DUB);
    }

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
		sprintf(DUB,"Order %d %s", rel_ord_tb[i].rel_ord_nm, rel_ord_tb[i].rel_ord_desc); prfm(DUB);
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

