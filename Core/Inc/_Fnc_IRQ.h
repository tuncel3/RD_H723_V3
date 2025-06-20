void DMA1_Stream1_IRQHandler(void) {
    if (LL_DMA_IsActiveFlag_TC1(DMA1)) {
        LL_DMA_ClearFlag_TC1(DMA1);
        AUX1_smp = adc_buffer[0];	// AUX1 5	1
        A_16_smp = adc_buffer[1];	// A_16 16	2
        VACR_smp = adc_buffer[2];	// VINR	17	3
        VACS_smp = adc_buffer[3];	// VACS 14	4
        VACT_smp = adc_buffer[4];	// VACT 15	5
        DCKP_smp = adc_buffer[5];	// DCKP 18	6
        DCKN_smp = adc_buffer[6];	// DCKN 19	7
        VBAT_smp = adc_buffer[7];	// VBAT 8	8
        VLOAD_smp = adc_buffer[8];	// VLOAD 7	9
        IRECT_smp = adc_buffer[9];	// IRECT 4	10
        IBAT_smp = adc_buffer[10];	// IBAT 9	11
        VRECT_smp = adc_buffer[11];	// VOUT 3	12

        VRECT_smp_sc = (float) (VRECT_smp + EpD[SET_VRECT_OFFS_CAL][0].V1) * EpD[SET_VRECT_CAL][0].V1;
        VLOAD_smp_sc = (float) (VLOAD_smp + EpD[SET_VLOAD_OFFS_CAL][0].V1) * EpD[SET_VLOAD_CAL][0].V1;
		IRECT_smp_sc = (float) ((32768-IRECT_smp+EpD[SET_IRECT_OFFS_CAL][0].V1)*EpD[SET_IRECT_CAL][0].V1);
		VBAT_smp_sc= (float) ((VBAT_smp-32768+EpD[SET_VBAT_OFFS_CAL][0].V1)*EpD[SET_VBAT_CAL][0].V1);
		IBAT_smp_sc= (float) ((32768-IBAT_smp+EpD[SET_IBAT_OFFS_CAL][0].V1)*EpD[SET_IBAT_CAL][0].V1);

		error_i_rect = EpD[IRECT_LIM_RT_][0].V1 - IRECT_smp_sc;
		error_i_batt = I_batt_targ_con_sy - IBAT_smp_sc;

		integral_i_rect += error_i_rect;	if (integral_i_rect > 0) { integral_i_rect = 0; }
		integral_i_batt += error_i_batt;	if (integral_i_batt > 0) { integral_i_batt = 0; }

		pid_output_i_rect = (Kp_i_rect * error_i_rect) + (Ki_i_rect * integral_i_rect);	if (pid_output_i_rect > 0) { pid_output_i_rect = 0; }
		pid_output_i_batt = (Kp_i_batt * error_i_batt) + (Ki_i_batt * integral_i_batt);	if (pid_output_i_batt > 0) { pid_output_i_batt = 0; }

		cs_vtarg_vi=pid_output_i_rect+pid_output_i_batt+V_targ_con_sy;
		error_v = cs_vtarg_vi - VRECT_smp_sc;

		integral_v += error_v;
		if (integral_v > integral_max_v) { integral_v = integral_max_v; } else if (integral_v < 1000) { integral_v = 1000; }
		pid_output_v = (Kp_v * error_v) + (Ki_v * integral_v);

	    if (pid_output_v > 6800) {
	        pid_output_v = 6800;
	    } else if (pid_output_v < 3000) {
	        pid_output_v = 3000;
	    }

		if (thy_drv_en==1) {
			timx_trg_num=round( ((10000-pid_output_v)/10000*tim_arr_max) - zc_start_delay_300u_arr_32 ); // n001
			timx_trg_num = (uint32_t)round(timx_trg_num <= 0.0 ? 1 : timx_trg_num);
			endOfDMATransfer_f();	// set thy trig points. tim arr.
		}
		else if (thy_drv_en==0) {
			timx_trg_num=tim_arr_max - zc_start_delay_300u_arr_32;
			integral_v=0;
		}

		short_circ_monitor_f();

		timx_trg_num64=timx_trg_num64*127.0/128.0+timx_trg_num/128.0;

		IRECT_sum_sc=IRECT_sum_sc+IRECT_smp_sc;
		IBAT_sum_sc=IBAT_sum_sc+IBAT_smp_sc;
		VBAT_sum_sc=VBAT_sum_sc+VBAT_smp_sc;
		VDCKP_sum=VDCKP_sum+DCKP_smp;
		VDCKN_sum=VDCKN_sum+DCKN_smp;
		VLOAD_sum_sc=VLOAD_sum_sc+VLOAD_smp_sc;
		VRECT_sum_sc=VRECT_sum_sc+VRECT_smp_sc;
		ADC_smp_count++;

		if (rms_zc_for_DC == 1) {
			rms_zc_for_DC_cnt = 0;
			rms_zc_for_DC = 0;
			IRECT_pas.a1=IRECT_sum_sc/ADC_smp_count;
			IRECT_sum_sc=0;
			IRECT_pas.a64=IRECT_pas.a64*63.0/64.0+IRECT_pas.a1/64.0;
			IRECT_pas.a16=IRECT_pas.a16*15.0/16.0+IRECT_pas.a1/16.0;

			IBAT_pas.a1=IBAT_sum_sc/ADC_smp_count;
			IBAT_sum_sc=0;
			IBAT_pas.a64=IBAT_pas.a64*63.0/64.0+IBAT_pas.a1/64.0;
			IBAT_pas.a16=IBAT_pas.a16*15.0/16.0+IBAT_pas.a1/16.0;

			ILOAD_pas.a64=IRECT_pas.a64-IBAT_pas.a64;
			ILOAD_pas.a16=IRECT_pas.a16-IBAT_pas.a16;

			VBAT_pas.a1=VBAT_sum_sc/ADC_smp_count;
			VBAT_sum_sc=0;
			VBAT_pas.a64=VBAT_pas.a64*63.0/64.0+VBAT_pas.a1/64.0;
			VBAT_pas.a16=VBAT_pas.a16*15.0/16.0+VBAT_pas.a1/16.0;

			VDCKP_per_avg=VDCKP_sum/ADC_smp_count;
			VDCKP_sum=0;
			VDCKP_per_avg_roll=VDCKP_per_avg_roll*127.0/128.0+VDCKP_per_avg/128.0;

			VDCKN_per_avg=VDCKN_sum/ADC_smp_count;
			VDCKN_sum=0;
			VDCKN_per_avg_roll=VDCKN_per_avg_roll*127.0/128.0+VDCKN_per_avg/128.0;

			VLOAD_pas.a1=VLOAD_sum_sc/ADC_smp_count;
			VLOAD_sum_sc=0;
			VLOAD_pas.a64=VLOAD_pas.a64*63.0/64.0+VLOAD_pas.a1/64.0;
			VLOAD_pas.a16=VLOAD_pas.a16*15.0/16.0+VLOAD_pas.a1/16.0;

			VRECT_pas.a1=VRECT_sum_sc/ADC_smp_count;
			VRECT_sum_sc=0;
			VRECT_pas.a64=VRECT_pas.a64*63.0/64.0+VRECT_pas.a1/64.0;
			VRECT_pas.a16=VRECT_pas.a16*15.0/16.0+VRECT_pas.a1/16.0;
			ADC_smp_count=0;
		}

		VDCKP_per_avg_roll_perc=VDCKP_per_avg_roll/(VDCKP_per_avg_roll+VDCKN_per_avg_roll)*VRECT_pas.a64; // VDCKP nin VRECT e oranlanmış hali.
		VDCKN_per_avg_roll_perc=VDCKN_per_avg_roll/(VDCKP_per_avg_roll+VDCKN_per_avg_roll)*VRECT_pas.a64;

		VRECT_pas64_half=VRECT_pas.a64/2;

		if (VDCKP_per_avg_roll_perc >= VDCKN_per_avg_roll_perc) {
			VDCK_raw = VDCKP_per_avg_roll_perc-VRECT_pas64_half;
			VDCK_perc=VDCK_raw/VRECT_pas64_half*100;
			VDCKP_perc=VDCK_perc;
			VDCKN_perc=0;
			VDCK_side=1;
		} else if (VDCKP_per_avg_roll_perc < VDCKN_per_avg_roll_perc) {
			VDCK_raw = VDCKN_per_avg_roll_perc-VRECT_pas64_half;
			VDCK_perc=VDCK_raw/VRECT_pas64_half*100;
			VDCKP_perc=0;
			VDCKN_perc=VDCK_perc;
			VDCK_side=-1;
		}
			VDCK_sc=VDCK_raw*0.00122055;

			VACR_smp_sc=(VACR_smp-32768+EpD[SET_ACR_OFFS_CAL][0].V1)*EpD[SET_ACR_CAL][0].V1;
			VACS_smp_sc=(VACS_smp-32768+EpD[SET_ACS_OFFS_CAL][0].V1)*EpD[SET_ACS_CAL][0].V1;
			VACT_smp_sc=(VACT_smp-32768+EpD[SET_ACT_OFFS_CAL][0].V1)*EpD[SET_ACT_CAL][0].V1;

			VACR_sum_of_sqr_sc=VACR_sum_of_sqr_sc+(VACR_smp_sc*VACR_smp_sc);VACR_smp_count++;
			VACS_sum_of_sqr_sc=VACS_sum_of_sqr_sc+(VACS_smp_sc*VACS_smp_sc);VACS_smp_count++;
			VACT_sum_of_sqr_sc=VACT_sum_of_sqr_sc+(VACT_smp_sc*VACT_smp_sc);VACT_smp_count++;

			if(VAC_R_samp_end==1){
				VAC_R_samp_end=0;
				VAC_R_rms_sc.a1=sqrt(VACR_sum_of_sqr_sc/VACR_smp_count);
				VACR_smp_count=0;
				VACR_sum_of_sqr_sc=0;
				VAC_R_rms_sc.a64=VAC_R_rms_sc.a64*63.0/64.0+VAC_R_rms_sc.a1/64.0;
			}

			if(VAC_S_samp_end==1){
				VAC_S_samp_end=0;
				VAC_S_rms_sc.a1=sqrt(VACS_sum_of_sqr_sc/VACS_smp_count);
				VACS_smp_count=0;
				VACS_sum_of_sqr_sc=0;
				VAC_S_rms_sc.a64=VAC_S_rms_sc.a64*63.0/64.0+VAC_S_rms_sc.a1/64.0;
			}

			if(VAC_T_samp_end==1){
				VAC_T_samp_end=0;
				VAC_T_rms_sc.a1=sqrt(VACT_sum_of_sqr_sc/VACT_smp_count);
				VACT_smp_count=0;
				VACT_sum_of_sqr_sc=0;
				VAC_T_rms_sc.a64=VAC_T_rms_sc.a64*63.0/64.0+VAC_T_rms_sc.a1/64.0;
			}
    }
}

void TIM2_IRQHandler(void) {
	LL_TIM_DisableCounter(TIM2);
	if (LL_TIM_IsActiveFlag_UPDATE(TIM2)) {
		LL_TIM_ClearFlag_UPDATE(TIM2);
		if (en_t_thy_up_r==1) {
			en_t_thy_up_r=0;
			RU_THY=1;
//			DBG11
		}
		if (en_t_thy_dn_r==1) {
			en_t_thy_dn_r=0;
			RA_THY=1;
//			DBG11
		}
	}
}
void TIM23_IRQHandler(void) {
	LL_TIM_DisableCounter(TIM23);
	if (LL_TIM_IsActiveFlag_UPDATE(TIM23)) {
		LL_TIM_ClearFlag_UPDATE(TIM23);
		if (en_t_thy_up_s==1) {
			en_t_thy_up_s=0;
			SU_THY=1;
//			DBG11
		}
		if (en_t_thy_dn_s==1) {
			en_t_thy_dn_s=0;
			SA_THY=1;
		}
	}
}
void TIM24_IRQHandler(void) {
	LL_TIM_DisableCounter(TIM24);
	if (LL_TIM_IsActiveFlag_UPDATE(TIM24)) {
		LL_TIM_ClearFlag_UPDATE(TIM24);
		if (en_t_thy_up_t==1) {
			en_t_thy_up_t=0;
			TU_THY=1;
		}
		if (en_t_thy_dn_t==1) {
			en_t_thy_dn_t=0;
			TA_THY=1;
		}
	}
}
void TIM3_IRQHandler(void) {
	LL_TIM_DisableCounter(TIM3);
	if (LL_TIM_IsActiveFlag_UPDATE(TIM3)) {
		LL_TIM_ClearFlag_UPDATE(TIM3);
		if (en_t_dely_up_r==1 && thy_drv_en==1) { // n002
			en_t_dely_up_r=0;
			en_t_thy_up_r=1;
			en_t_thy_dn_r=0;
			RU_THY=0;
			RA_THY=0;
//			DBG10
			LL_TIM_EnableCounter(TIM2);
		}
		if (en_t_dely_dn_r==1 && thy_drv_en==1) {
			en_t_dely_dn_r=0;
			en_t_thy_up_r=0;
			en_t_thy_dn_r=1;
			RU_THY=0;
			RA_THY=0;
//			DBG10
			LL_TIM_EnableCounter(TIM2);
		}
		if (en_t_dely_up_s==1 && thy_drv_en==1) {
			en_t_dely_up_s=0;
			en_t_thy_up_s=1;
			en_t_thy_dn_s=0;
			SU_THY=0;
			SA_THY=0;
//			DBG10
			LL_TIM_EnableCounter(TIM23);
		}
		if (en_t_dely_dn_s==1 && thy_drv_en==1) {
			en_t_dely_dn_s=0;
			en_t_thy_up_s=0;
			en_t_thy_dn_s=1;
			SU_THY=0;
			SA_THY=0;
//			DBG10
			LL_TIM_EnableCounter(TIM23);
		}
		if (en_t_dely_up_t==1 && thy_drv_en==1) {
			en_t_dely_up_t=0;
			en_t_thy_up_t=1;
			en_t_thy_dn_t=0;
			TU_THY=0;
			TA_THY=0;
//			DBG10
			LL_TIM_EnableCounter(TIM24);
		}
		if (en_t_dely_dn_t==1 && thy_drv_en==1) {
			en_t_dely_dn_t=0;
			en_t_thy_up_t=0;
			en_t_thy_dn_t=1;
			TU_THY=0;
			TA_THY=0;
//			DBG10
			LL_TIM_EnableCounter(TIM24);
		}
	}
}
void EXTI9_5_IRQHandler(void){
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_7) != RESET)  {
		LL_TIM_DisableCounter(TIM2);
		LL_TIM_SetCounter(TIM2, 0);
		LL_TIM_SetCounter(TIM3, 0);
		LL_TIM_EnableCounter(TIM3);
		RU_THY=0;
		RA_THY=0;
		LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_7);
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_7);
	if (LL_EXTI_IsEnabledRisingTrig_0_31(LL_EXTI_LINE_7)) {
		en_t_dely_up_r=1;
		en_t_dely_dn_r=0;
		VAC_R_samp_end=1;
		per_r_dn_avg_m_f();
	} else if (LL_EXTI_IsEnabledFallingTrig_0_31(LL_EXTI_LINE_7)) {
		en_t_dely_up_r=0;
		en_t_dely_dn_r=1;
		per_r_up_avg_m_f();
	}
	zero_cross_timeout_R=0;
	reset_RMS_val_R=0;
    LL_EXTI_LINE_7_reenable_cnt=0;
  }
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_8) != RESET)  {
		LL_TIM_DisableCounter(TIM23);
		LL_TIM_SetCounter(TIM23, 0);
		LL_TIM_SetCounter(TIM3, 0);
		LL_TIM_EnableCounter(TIM3);
		SU_THY=0;
		SA_THY=0;
		LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_8);
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_8);
	if (LL_EXTI_IsEnabledRisingTrig_0_31(LL_EXTI_LINE_8)) {
		en_t_dely_up_s=1;
		en_t_dely_dn_s=0;
		VAC_S_samp_end=1;
		per_s_dn_avg_m_f();
	} else if (LL_EXTI_IsEnabledFallingTrig_0_31(LL_EXTI_LINE_8)) {
		en_t_dely_up_s=0;
		en_t_dely_dn_s=1;
		per_s_up_avg_m_f();
	}
	zero_cross_timeout_S=0;
	reset_RMS_val_S=0;
    LL_EXTI_LINE_8_reenable_cnt=0;
  }
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_9) != RESET)  {
		LL_TIM_DisableCounter(TIM24);
		LL_TIM_SetCounter(TIM24, 0);
		LL_TIM_SetCounter(TIM3, 0);
		LL_TIM_EnableCounter(TIM3);
		TU_THY=0;
		TA_THY=0;
		LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_9);
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_9);
	if (LL_EXTI_IsEnabledRisingTrig_0_31(LL_EXTI_LINE_9)) {
		en_t_dely_up_t=1;
		en_t_dely_dn_t=0;
		VAC_T_samp_end=1;
		per_t_dn_avg_m_f();
	} else if (LL_EXTI_IsEnabledFallingTrig_0_31(LL_EXTI_LINE_9)) {
		en_t_dely_up_t=0;
		en_t_dely_dn_t=1;
		per_t_up_avg_m_f();
	}
	zero_cross_timeout_T=0;
	reset_RMS_val_T=0;
	LL_EXTI_LINE_9_reenable_cnt=0;
  }
}

void TIM16_IRQHandler(void) {
    if (LL_TIM_IsActiveFlag_UPDATE(TIM16)) {
        LL_TIM_ClearFlag_UPDATE(TIM16);
        if (RA_THY == 1) set_(SCR_R.A.port, SCR_R.A.pin);
        if (RU_THY == 1) set_(SCR_R.U.port, SCR_R.U.pin);
        if (SA_THY == 1) set_(SCR_S.A.port, SCR_S.A.pin);
        if (SU_THY == 1) set_(SCR_S.U.port, SCR_S.U.pin);
        if (TA_THY == 1) set_(SCR_T.A.port, SCR_T.A.pin);
        if (TU_THY == 1) set_(SCR_T.U.port, SCR_T.U.pin);
    }
    if (LL_TIM_IsActiveFlag_CC1(TIM16)) {
        LL_TIM_ClearFlag_CC1(TIM16);
        res_(SCR_R.A.port, SCR_R.A.pin);
        res_(SCR_R.U.port, SCR_R.U.pin);
        res_(SCR_S.A.port, SCR_S.A.pin);
        res_(SCR_S.U.port, SCR_S.U.pin);
        res_(SCR_T.A.port, SCR_T.A.pin);
        res_(SCR_T.U.port, SCR_T.U.pin);
    }
}


void TIM7_IRQHandler(void)
{
	if (LL_TIM_IsActiveFlag_UPDATE(TIM7)){
		LL_TIM_ClearFlag_UPDATE(TIM7);
		ms_tick_cnt++;
		delay_1ms_cnt++;
		ButtScanDelay_cnt++;

		zero_cross_timeout_R++;
		zero_cross_timeout_S++;
		zero_cross_timeout_T++;

		rms_zc_for_DC_cnt++;
		if (rms_zc_for_DC_cnt >= EpD[SET_WORK_FREQ][0].V1) {
			rms_zc_for_DC=1;
		}

		if (zero_cross_timeout_R > 500) {
			zero_cross_timeout_R=501;
			VAC_R_rms_sc.a64=0;	// zero cross olmadığı için takılı kalmış olan RMS değeri sıfırlanmalı.
		}
		if (zero_cross_timeout_S > 500) {
			zero_cross_timeout_S=501;
			VAC_S_rms_sc.a64=0;	// zero cross olmadığı için takılı kalmış olan RMS değeri sıfırlanmalı.
		}
		if (zero_cross_timeout_T > 500) {
			zero_cross_timeout_T=501;
			VAC_T_rms_sc.a64=0;	// zero cross olmadığı için takılı kalmış olan RMS değeri sıfırlanmalı.
		}
	}
}
void TIM1_UP_IRQHandler(void)
{
	if (LL_TIM_IsActiveFlag_UPDATE(TIM1)){
		LL_TIM_ClearFlag_UPDATE(TIM1);

	    uint8_t up_raw = ( BUP && !BDOWN && !BLEFT && !BRIGHT && !BENTER && !BESC);
	    uint8_t dn_raw = (!BUP &&  BDOWN && !BLEFT && !BRIGHT && !BENTER && !BESC);
	    uint8_t lf_raw = (!BUP && !BDOWN &&  BLEFT && !BRIGHT && !BENTER && !BESC);
	    uint8_t rt_raw = (!BUP && !BDOWN && !BLEFT &&  BRIGHT && !BENTER && !BESC);
	    uint8_t en_raw = (!BUP && !BDOWN && !BLEFT && !BRIGHT &&  BENTER && !BESC);
	    uint8_t es_raw = (!BUP && !BDOWN && !BLEFT && !BRIGHT && !BENTER &&  BESC);

	    handleButton(up_raw, &upIsHeld, &upReleaseCnt, &upHoldCnt, &upNextRepeatEdge, &upFireFlag);
	    handleButton(dn_raw, &dnIsHeld, &dnReleaseCnt, &dnHoldCnt, &dnNextRepeatEdge, &dnFireFlag);
	    handleButton(lf_raw, &lfIsHeld, &lfReleaseCnt, &lfHoldCnt, &lfNextRepeatEdge, &lfFireFlag);
	    handleButton(rt_raw, &rtIsHeld, &rtReleaseCnt, &rtHoldCnt, &rtNextRepeatEdge, &rtFireFlag);
	    handleButton(en_raw, &enIsHeld, &enReleaseCnt, &enHoldCnt, &enNextRepeatEdge, &enFireFlag);
	    handleButton(es_raw, &esIsHeld, &esReleaseCnt, &esHoldCnt, &esNextRepeatEdge, &esFireFlag);
	}
}

void SysTick_Handler(void) {	// n009

	zcrendely1=zcrendely;
//	zcrendely2=zcrendely;
//	zcrendely3=zcrendely;
//	if (zcrendely1==zcrendely1 && zcrendely1==zcrendely2 && zcrendely2==zcrendely3) {
		zcrendely_ok=zcrendely1;
//		zcrendely_ok_cnt++;
//	} else {
//		zcrendely_ok_no_cnt++;
//	}
	if (LL_EXTI_LINE_7_reenable_cnt < zcrendely_ok) {
		LL_EXTI_LINE_7_reenable_cnt++;
		if (LL_EXTI_LINE_7_reenable_cnt >= zcrendely_ok) {
			LL_EXTI_LINE_7_reenable_cnt=zcrendely_ok << 3;
			if (LL_EXTI_IsEnabledRisingTrig_0_31(LL_EXTI_LINE_7)) {
				LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_7);
				LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_7);
				LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_7);
			}
			else if (LL_EXTI_IsEnabledFallingTrig_0_31(LL_EXTI_LINE_7)) {
				LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_7);
				LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_7);
				LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_7);
			}
		}
	}
	if (LL_EXTI_LINE_8_reenable_cnt < ZCRENDELY) {
		LL_EXTI_LINE_8_reenable_cnt++;
		if (LL_EXTI_LINE_8_reenable_cnt == ZCRENDELY) {
			LL_EXTI_LINE_8_reenable_cnt=ZCRENDELY+1;
			if (LL_EXTI_IsEnabledRisingTrig_0_31(LL_EXTI_LINE_8)) {
				LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_8);
				LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_8);
				LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_8);
			}
			else if (LL_EXTI_IsEnabledFallingTrig_0_31(LL_EXTI_LINE_8)) {
				LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_8);
				LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_8);
				LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_8);
			}
		}
	}
	if (LL_EXTI_LINE_9_reenable_cnt < ZCRENDELY) {
		LL_EXTI_LINE_9_reenable_cnt++;
		if (LL_EXTI_LINE_9_reenable_cnt == ZCRENDELY) {
			LL_EXTI_LINE_9_reenable_cnt=ZCRENDELY+1;
			if (LL_EXTI_IsEnabledRisingTrig_0_31(LL_EXTI_LINE_9)) {
				LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_9);
				LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_9);
				LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_9);
			}
			else if (LL_EXTI_IsEnabledFallingTrig_0_31(LL_EXTI_LINE_9)) {
				LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_9);
				LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_9);
				LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_9);
			}
		}
	}



}

void DMA1_Stream0_IRQHandler(void)
{
    if (LL_DMA_IsActiveFlag_TC0(DMA1))  // Transfer Complete interrupt
    {
        LL_DMA_ClearFlag_TC0(DMA1);  // Clear the interrupt flag

        // Move to the next string in the buffer
        buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;

        if (buffer_tail != buffer_head)  // More strings in the buffer
        {
            size_t string_length = strlen(dma_uart_buffer[buffer_tail]);
            LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, string_length);
            LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0,
                                   (uint32_t)dma_uart_buffer[buffer_tail],
                                   LL_USART_DMA_GetRegAddr(UART5, LL_USART_DMA_REG_DATA_TRANSMIT),
                                   LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
            LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);  // Start next transmission
        }
        else
        {
            dma_busy = 0;  // Buffer is empty, DMA is idle
        }
    }
}


void TIM6_DAC_IRQHandler(void) {
	if (LL_TIM_IsActiveFlag_UPDATE(TIM6)) {
		LL_TIM_ClearFlag_UPDATE(TIM6);
	}
}


void USART10_IRQHandler(void)
{
    if (LL_USART_IsActiveFlag_RXNE(USART10)) {

        if (U10_rxCount < U10_RX_BUFFER_SIZE) {
        	U10_rxBuf[U10_rxCount++] = LL_USART_ReceiveData8(USART10);
        }

    }

}



