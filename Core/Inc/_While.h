


// soft start
if (sf_sta_req && line_sgn_stable && sf_sta_req_ok==0 && ms_tick_cnt-sfst_1_t_hold >= sfst_1_step && thy_stop_fault_hold_bits==0) {
	sfst_1_t_hold=ms_tick_cnt;

	if (V_targ_con_sy <= Current_charge_voltage && (IRECT_pas.a1 < EpD[IRECT_LIM_RT_][0].V1*1.01 || IBAT_pas.a1 < I_batt_targ_con_sy*1.01)) {
		set_V_targ_con_sy(V_targ_con_sy*1.003);
		if (V_targ_con_sy >= Current_charge_voltage) {
			set_V_targ_con_sy(V_targ_con_sy);
			sf_sta_req=0;
			sf_sta_req_ok=1;
			actions_after_charge_mode_change(6);
			sprintf(DUB,"Soft start request ok"); prfm(DUB);
		}
		else if (V_targ_con_sy < VRECT_smp_sc-4) {		// vout zaten yüksekse direk hedefi oraya yaklaştırarak başla
			set_V_targ_con_sy(VRECT_smp_sc-4);
			sprintf(DUB,"sfst V_targ_con_sy=VRECT_smp_sc-4;"); prfm(DUB);
		}
		else if (V_targ_con_sy >= Current_charge_voltage && V_targ_con_sy >= VRECT_smp_sc) {	// hedef zaten vout üzerinde. hedefi vout yap.
			set_V_targ_con_sy(VRECT_smp_sc);
			sprintf(DUB,"sfst V_targ_con_sy=VRECT_smp_sc;"); prfm(DUB);
		}
	}
	else if (V_targ_con_sy > Current_charge_voltage && Current_charge_voltage >= EpD[DEV_NOM_VOUT][0].V1) {
		set_V_targ_con_sy(Current_charge_voltage);
		sf_sta_req=0;
		sf_sta_req_ok=1;
		actions_after_charge_mode_change(7);
		sprintf(DUB,"Sft strt req ok aft V_targ_con_sy > Curr_chrg_voltg"); prfm(DUB);
	}
	else if (sfst_1_unexpected_state==0) {
		sfst_1_unexpected_state=1;
		set_V_targ_con_sy(Current_charge_voltage);
		sf_sta_req=0;
		sf_sta_req_ok=1;
		sprintf(DUB,"Soft start request ok after unexpected state"); prfm(DUB);
	}
	if (sf_sta_req_ok==1) {
		set_V_targ_con_sy(Current_charge_voltage);
	}
}

// delayed soft start trigger
if (ms_tick_cnt-while_delay50_h >= 50) {
	while_delay50_h=ms_tick_cnt;
	ms_50_cnt++;

	device_start_up_delay_completed_cnt++;
if (device_start_up_delay_completed_cnt >= 40 && device_start_up_delay_completed==0) {
	device_start_up_delay_completed=1;
}

if (device_start_up_delay_completed==1) {
	if (thy_drv_en==0 && thy_drv_en_req ==1 && line_sgn_stable && thy_stop_fault_hold_bits==0) {
		sf_sta_req_cnt++;
		if (sf_sta_req_cnt >= 20) {
			set_V_targ_con_sy(5);
			thy_drv_en=1;
			apply_state_changes_f(STOP_FC, 0);
			apply_state_changes_f(START_FC, 1);
			sf_sta_req=1;
			sf_sta_req_ok=0;
			thy_drv_en_req=0;
			sfst_1_unexpected_state=0;
			sf_sta_req_cnt=0;
			sprintf(DUB,"Soft start request"); prfm(DUB);
		}
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// MANAGE DROPPER ///////////////////////////////////////////////////////////////////////////////////////////////

		if (VLOAD_pas.a1+dropper_test_var_1 > Vdc_drop_out_max && !is_state_active(DROPPER1_BYP_FC) && !is_state_active(DROPPER2_BYP_FC)) {
			actvate_drop_cnt++;
			if (actvate_drop_cnt >= actvate_drop_per) {
				actvate_drop_cnt=0;
				EpD[SET_DROPPER_K1][0].V1=1; EpD[SET_DROPPER_K1][1].V1=1; // kullanıcı harici değişim olduğu için bu değişiklik yapılmalı.
				DROPP_BATT_CTRL(EpD[SET_DROPPER_K1][0].V1); // dropper ı kayıtlı değere göre değiştir.
				apply_state_changes_f(DROPPER1_BYP_FC, EpD[SET_DROPPER_K1][0].V1); // burası ledleri değiştiriyor.
			}
		} else if (VLOAD_pas.a1+dropper_test_var_1 > Vdc_drop_out_max && is_state_active(DROPPER1_BYP_FC) && !is_state_active(DROPPER2_BYP_FC)) {
			actvate_drop_cnt++;
			if (actvate_drop_cnt >= actvate_drop_per) {
				actvate_drop_cnt=0;
				EpD[SET_DROPPER_K2][0].V1=1; EpD[SET_DROPPER_K2][1].V1=1;
				DROPP_LOAD_CTRL(EpD[SET_DROPPER_K2][0].V1);
				apply_state_changes_f(DROPPER2_BYP_FC, EpD[SET_DROPPER_K2][0].V1);
			}
		} else if (VLOAD_pas.a1+dropper_test_var_1 < Vdc_drop_out_min && is_state_active(DROPPER1_BYP_FC) && is_state_active(DROPPER2_BYP_FC)) {
			actvate_drop_cnt++;
			if (actvate_drop_cnt >= actvate_drop_per) { // ikisi de devredeyse önce 2. dropperi kapat.
				actvate_drop_cnt=0;
				EpD[SET_DROPPER_K2][0].V1=0; EpD[SET_DROPPER_K2][1].V1=0;
				DROPP_LOAD_CTRL(EpD[SET_DROPPER_K2][0].V1);
				apply_state_changes_f(DROPPER2_BYP_FC, EpD[SET_DROPPER_K2][0].V1);
			}
		} else if (VLOAD_pas.a1+dropper_test_var_1 < Vdc_drop_out_min && is_state_active(DROPPER1_BYP_FC) && !is_state_active(DROPPER2_BYP_FC)) {
			actvate_drop_cnt++;
			if (actvate_drop_cnt >= actvate_drop_per) { // 2. kapalıysa 1. dropperi kapat.
				actvate_drop_cnt=0;
				EpD[SET_DROPPER_K1][0].V1=0; EpD[SET_DROPPER_K1][1].V1=0;
				DROPP_BATT_CTRL(EpD[SET_DROPPER_K1][0].V1);
				apply_state_changes_f(DROPPER1_BYP_FC, EpD[SET_DROPPER_K1][0].V1);
			}
		} else if (VLOAD_pas.a1+dropper_test_var_1 < Vdc_drop_out_min && !is_state_active(DROPPER1_BYP_FC) && is_state_active(DROPPER2_BYP_FC)) {
			actvate_drop_cnt++;
			if (actvate_drop_cnt >= actvate_drop_per) { // 1. kapalıysa 2. dropperi kapat.
				actvate_drop_cnt=0;
				EpD[SET_DROPPER_K2][0].V1=0; EpD[SET_DROPPER_K2][1].V1=0;
				DROPP_LOAD_CTRL(EpD[SET_DROPPER_K2][0].V1);
				apply_state_changes_f(DROPPER2_BYP_FC, EpD[SET_DROPPER_K2][0].V1);
			}
		}

////// MANAGE DROPPER ///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// MANAGE CHARGE MODE AUTO //////////////////////////////////////////////////////////////////////////////////////
if (EpD[SET_CHARGE_MODE][0].V1 == AUTO) {
	if (switch_to_auto_mode_completed==0) {
		switch_to_auto_mode_completed=1; // başka bir moddan auto moda geçildi.
		boost_of_auto_mode_active=0; // şimdi aşağıdaki iki moddan birini seç. float ya da boost.
		float_of_auto_mode_active=0; // şimdi aşağıdaki iki moddan birini seç. float ya da boost.
		Current_charge_voltage=EpD[VBAT_FLOAT][0].V1;
		I_batt_targ_con_sy=EpD[SET_IBAT_FLOAT][0].V1;
		set_V_targ_con_sy(Current_charge_voltage);
		LED_7_Data &= !BOOST_CHARGE_LED;
		LED_7_Data |= FLOAT_CHARGE_LED;
		sprintf(DUB,"switch_to_auto_mode_completed"); prfm(DUB);
	}
	else if (IBAT_pas.a1 > EpD[I_LIM_TO_BOOST][0].V1 && boost_of_auto_mode_active==0) {
		float_of_auto_mode_active=0;
		boost_of_auto_mode_active=1;
		Current_charge_voltage=EpD[VBAT_BOOST][0].V1;
		I_batt_targ_con_sy=EpD[SET_IBAT_BOOST][0].V1;
		set_V_targ_con_sy(Current_charge_voltage);
		LED_7_Data &= !FLOAT_CHARGE_LED;
		LED_7_Data |= BOOST_CHARGE_LED;
		sprintf(DUB,"AUTO switch to BOOST %f", IBAT_pas.a1); prfm(DUB);
	}
	else if (IBAT_pas.a1 < EpD[I_LIM_TO_FLOAT][0].V1 && float_of_auto_mode_active==0) {
		float_of_auto_mode_active=1;
		boost_of_auto_mode_active=0;
		Current_charge_voltage=EpD[VBAT_FLOAT][0].V1;
		I_batt_targ_con_sy=EpD[SET_IBAT_FLOAT][0].V1;
		set_V_targ_con_sy(Current_charge_voltage);
		LED_7_Data &= !BOOST_CHARGE_LED;
		LED_7_Data |= FLOAT_CHARGE_LED;
		sprintf(DUB,"AUTO switch to FLOAT %f", IBAT_pas.a1); prfm(DUB);
	}
}
////// MANAGE CHARGE MODE AUTO //////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// MANAGE CHARGE MODE TIMED /////////////////////////////////////////////////////////////////////////////////////
if (EpD[SET_CHARGE_MODE][0].V1 == TIMED) {
	if (charge_mode_timed_time_cnt > 0) {
		charge_mode_timed_time_cnt--;
		charge_mode_timed_time_sec=charge_mode_timed_time_cnt/20;
//		sprintf(DUB,"TIMED mode selected minutes %f %lu", EpD[SET_BOOST_TIME][0].V1, charge_mode_timed_time_cnt); prfm(DUB);
	}

	if (charge_mode_timed_time_cnt == 0 && timed_mode_time_ended==0) {
		timed_mode_time_ended=1;
		EpD[SET_CHARGE_MODE][0].V1=FLOAT; EpD[SET_CHARGE_MODE][1].V1=FLOAT; // button yukarı aşağı seçeneği dışında değiştirildiği için hem [0] hem de [1] olanı değiştiriliyor.
		actions_after_charge_mode_change(8);
		sprintf(DUB,"Timer mode count down ended. Switch to FLOAT mode"); prfm(DUB);
	}
}
////// MANAGE CHARGE MODE TIMED /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// MANAGE CHARGE MODE AUTO //////////////////////////////////////////////////////////////////////////////////////
if (EpD[SET_CHARGE_MODE][0].V1 == AUTO) {

}
////// MANAGE CHARGE MODE AUTO //////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Arıza durumundan çıkınca veya sistem uygunsa doğrultucuyu başlat
if (thy_stop_fault_hold_bits==0 && thy_drv_en==0 && user_wants_thy_drv==1) { // bütün thy stop gerektiren arızalar deaktif durumunda ise.
	thy_drv_en_req = 1; // bu durumda thy drv en req gönder.
	if (start_bat_inspection_req==1) { // devam etmekte olan bir batt inspection varsa iptal et. Bu istisnai bir durum. Düşük olasılıklı çakışma durumu.
		end_batt_inspect_return_to_normal(5);
	}
}


SW_LINE_OFF=!isInSet_(SW_LINE_P);
SW_BATT_OFF=!isInSet_(SW_BATT_P);
SW_LOAD_OFF=!isInSet_(SW_LOAD_P);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// MCCB MONITORING //////////////////////////////////////////////////////////////////////////////////////////////
////// LINE FUSE OFF MONITORING /////////////////////////////////////////////////////////////////////////////////////
if (SW_LINE_OFF && !is_state_active(LINE_FUSE_OFF_FC)) {
	apply_state_changes_f(LINE_FUSE_OFF_FC, 1);
} else if (!SW_LINE_OFF && is_state_active(LINE_FUSE_OFF_FC)) {
	apply_state_changes_f(LINE_FUSE_OFF_FC, 0);
}
////// BATT FUSE OFF MONITORING /////////////////////////////////////////////////////////////////////////////////////
if (SW_BATT_OFF && !is_state_active(BATT_FUSE_OFF_FC)) {
	apply_state_changes_f(BATT_FUSE_OFF_FC, 1);
} else if (!SW_BATT_OFF && is_state_active(BATT_FUSE_OFF_FC)) {
	apply_state_changes_f(BATT_FUSE_OFF_FC, 0);
}
////// LOAD FUSE OFF MONITORING /////////////////////////////////////////////////////////////////////////////////////
if (SW_LOAD_OFF && !is_state_active(LOAD_FUSE_OFF_FC)) {
	apply_state_changes_f(LOAD_FUSE_OFF_FC, 1);
} else if (!SW_LOAD_OFF && is_state_active(LOAD_FUSE_OFF_FC)) {
	apply_state_changes_f(LOAD_FUSE_OFF_FC, 0);
}
////// MCCB MONITORING //////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// BATTERY_FAULT_FC decide //////////////////////////////////////////////////////////////////////////////////////
if (!is_state_active(BATTERY_FAULT_FC)) {
	if (is_state_active(BATT_FUSE_OFF_FC) || is_state_active(BATT_LINE_BROKEN_FC) || is_state_active(BATT_REVERSE_FC) || is_state_active(BATT_SHORT_FC)) {
		apply_state_changes_f(BATTERY_FAULT_FC, 1);
	}
} else if (is_state_active(BATTERY_FAULT_FC)) {
	if (!is_state_active(BATT_FUSE_OFF_FC) && !is_state_active(BATT_LINE_BROKEN_FC) && !is_state_active(BATT_REVERSE_FC) && !is_state_active(BATT_SHORT_FC)) {
		apply_state_changes_f(BATTERY_FAULT_FC, 0);
	}
}
////// BATTERY_FAULT_FC decide //////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// BATT REVERSE MONITORING //////////////////////////////////////////////////////////////////////////////////////
if (VBAT_pas.a1 < -10 && EpD[SET_BATT_REV_DET][0].V1==1 && !is_state_active(BATT_REVERSE_FC)) {
	batt_reverse_Acc_cnt++;
	if (batt_reverse_Acc_cnt >= batt_reverse_Acc_per) {
		batt_reverse_Acc_cnt=0;
		apply_state_changes_f(BATT_REVERSE_FC, 1);
		apply_state_changes_f(BATT_LINE_BROKEN_FC, 0);
	}
} else if (VBAT_pas.a1 >= -0.5 && is_state_active(BATT_REVERSE_FC)) {
	batt_reverse_return_Acc_cnt++;
	if (batt_reverse_return_Acc_cnt >= batt_reverse_return_Acc_per) {
		batt_reverse_return_Acc_cnt=0;
		apply_state_changes_f(BATT_REVERSE_FC, 0);
	}
}
////// BATT REVERSE MONITORING //////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

if (sf_sta_req_ok==1) {
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// AKÜ HATTI KOPUK //////////////////////////////////////////////////////////////////////////////////////////////
if (thy_drv_en==1 && bat_inspection_allowed==1 && EpD[SET_BATT_DISC_DET][0].V1==1 && VBAT_pas.a1 > 10 && !is_state_active(BATT_FUSE_OFF_FC) && !is_state_active(BATT_REVERSE_FC)) {
	aku_hatti_kopuk_fc_inl();
} else if (start_bat_inspection_req==1) {
	end_batt_inspect_return_to_normal(6);
}
////// AKÜ HATTI KOPUK //////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// DCK FAULT MON ////////////////////////////////////////////////////////////////////////////////////////////////
// && sf_sta_req_ok==0 && batt_line_broken==1
DCK_mon_start_cnt++;
if (DCK_mon_start_cnt >= DCK_mon_start_per) {
	DCK_mon_start_cnt=DCK_mon_start_per; // start delay passed.
	if (VDCKP_perc >= EpD[DC_KAC_POS][0].V1 && !is_state_active(DC_LEAK_POSITIVE_FC)) {
		VDCK_accept_cnt++;
		if (VDCK_accept_cnt >= VDCK_accept_per) {
			VDCK_accept_cnt=0;
			apply_state_changes_f(DC_LEAK_POSITIVE_FC, 1);
			sprintf(DUB,"DC leak above pos lim"); prfm(DUB);
		 }
	} else if (VDCKP_perc < EpD[DC_KAC_POS][0].V1 && is_state_active(DC_LEAK_POSITIVE_FC)) {
		VDCK_return_cnt++;
		if (VDCK_return_cnt >= VDCK_return_per) {
			VDCK_return_cnt=0;
			apply_state_changes_f(DC_LEAK_POSITIVE_FC, 0);
			sprintf(DUB,"DC leak above pos lim removed"); prfm(DUB);
		}
	} else if (VDCKN_perc >= EpD[DC_KAC_NEG][0].V1 && !is_state_active(DC_LEAK_NEGATIVE_FC)) {
		VDCK_accept_cnt++;
		if (VDCK_accept_cnt >= VDCK_accept_per) {
			VDCK_accept_cnt=0;
			apply_state_changes_f(DC_LEAK_NEGATIVE_FC, 1);
			sprintf(DUB,"DC leak below neg lim"); prfm(DUB);
		}
	} else if (VDCKN_perc < EpD[DC_KAC_NEG][0].V1 && is_state_active(DC_LEAK_NEGATIVE_FC)) {
		VDCK_return_cnt++;
		if (VDCK_return_cnt >= VDCK_return_per) {
			VDCK_return_cnt=0;
			apply_state_changes_f(DC_LEAK_NEGATIVE_FC, 0);
			sprintf(DUB,"DC leak below neg lim removed"); prfm(DUB);
		}
	} else {
		VDCK_return_cnt=0;
	}
}

////// DCK FAULT MON ////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// V RECT DC HIGH/LOW MON ///////////////////////////////////////////////////////////////////////////////////////
if (VRECT_pas.a1 > VRECT_DC_HIGH_LIM && !is_state_active(RECT_DC_HG_FC)) {
	VRECT_DC_HIGH_LIM_Acc_cnt++;
	VRECT_DC_HIGH_LIM_ret_Acc_cnt=0;
	if (VRECT_DC_HIGH_LIM_Acc_cnt >= VRECT_DC_HIGH_LIM_Acc_per) {
		VRECT_DC_HIGH_LIM_Acc_cnt=0;
		apply_state_changes_f(RECT_DC_HG_FC, 1);
		sprintf(DUB,"RECT DC High"); prfm(DUB);
	}
} else {
	VRECT_DC_HIGH_LIM_Acc_cnt=0;
}
if (VRECT_pas.a1 <= VRECT_DC_HIGH_LIM_ret && is_state_active(RECT_DC_HG_FC)) {
	VRECT_DC_HIGH_LIM_ret_Acc_cnt++;
	VRECT_DC_HIGH_LIM_Acc_cnt=0;
	if (VRECT_DC_HIGH_LIM_ret_Acc_cnt >= VRECT_DC_HIGH_LIM_ret_Acc_per) {
		VRECT_DC_HIGH_LIM_ret_Acc_cnt=0;
		apply_state_changes_f(RECT_DC_HG_FC, 0);
		sprintf(DUB,"RECT DC High Return"); prfm(DUB);
	}
} else {
	VRECT_DC_HIGH_LIM_ret_Acc_cnt=0;
}
if (VRECT_pas.a1 < VRECT_DC_LOW_LIM && !is_state_active(RECT_DC_LW_FC)) {
	VRECT_DC_LOW_LIM_Acc_cnt++;
	VRECT_DC_LOW_LIM_ret_Acc_cnt=0;
	if (VRECT_DC_LOW_LIM_Acc_cnt >= VRECT_DC_LOW_LIM_Acc_per) {
		VRECT_DC_LOW_LIM_Acc_cnt=0;
		apply_state_changes_f(RECT_DC_LW_FC, 1);
		sprintf(DUB,"RECT DC Low"); prfm(DUB);
	}
} else {
	VRECT_DC_LOW_LIM_Acc_cnt=0;
}
if (VRECT_pas.a1 >= VRECT_DC_LOW_LIM_ret && is_state_active(RECT_DC_LW_FC)) {
	VRECT_DC_LOW_LIM_ret_Acc_cnt++;
	VRECT_DC_LOW_LIM_Acc_cnt=0;
	if (VRECT_DC_LOW_LIM_ret_Acc_cnt >= VRECT_DC_LOW_LIM_ret_Acc_per) {
		VRECT_DC_LOW_LIM_ret_Acc_cnt=0;
		apply_state_changes_f(RECT_DC_LW_FC, 0);
		sprintf(DUB,"RECT DC Low Return"); prfm(DUB);
	}
} else {
	VRECT_DC_LOW_LIM_ret_Acc_cnt=0;
}
////// V RECT DC HIGH/LOW MON ///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// V LOAD DC HIGH/LOW MON ///////////////////////////////////////////////////////////////////////////////////////
if (VLOAD_pas.a1 > VLOAD_DC_HIGH_LIM && !is_state_active(LOAD_DC_HG_FC)) {
	VLOAD_DC_HIGH_LIM_Acc_cnt++;
	VLOAD_DC_HIGH_LIM_ret_Acc_cnt=0;
	if (VLOAD_DC_HIGH_LIM_Acc_cnt >= VLOAD_DC_HIGH_LIM_Acc_per) {
		VLOAD_DC_HIGH_LIM_Acc_cnt=0;
		apply_state_changes_f(LOAD_DC_HG_FC, 1);
		sprintf(DUB,"LOAD DC High"); prfm(DUB);
	}
} else {
	VLOAD_DC_HIGH_LIM_Acc_cnt=0;
}
if (VLOAD_pas.a1 <= VLOAD_DC_HIGH_LIM_ret && is_state_active(LOAD_DC_HG_FC)) {
	VLOAD_DC_HIGH_LIM_ret_Acc_cnt++;
	VLOAD_DC_HIGH_LIM_Acc_cnt=0;
	if (VLOAD_DC_HIGH_LIM_ret_Acc_cnt >= VLOAD_DC_HIGH_LIM_ret_Acc_per) {
		VLOAD_DC_HIGH_LIM_ret_Acc_cnt=0;
		apply_state_changes_f(LOAD_DC_HG_FC, 0);
		sprintf(DUB,"LOAD DC High Return"); prfm(DUB);
	}
} else {
	VLOAD_DC_HIGH_LIM_ret_Acc_cnt=0;
}
if (VLOAD_pas.a1 < VLOAD_DC_LOW_LIM && !is_state_active(LOAD_DC_LW_FC)) {
	VLOAD_DC_LOW_LIM_Acc_cnt++;
	VLOAD_DC_LOW_LIM_ret_Acc_cnt=0;
	if (VLOAD_DC_LOW_LIM_Acc_cnt >= VLOAD_DC_LOW_LIM_Acc_per) {
		VLOAD_DC_LOW_LIM_Acc_cnt=0;
		apply_state_changes_f(LOAD_DC_LW_FC, 1);
		sprintf(DUB,"LOAD DC Low"); prfm(DUB);
	}
} else {
	VLOAD_DC_LOW_LIM_Acc_cnt=0;
}
if (VLOAD_pas.a1 >= VLOAD_DC_LOW_LIM_ret && is_state_active(LOAD_DC_LW_FC)) {
	VLOAD_DC_LOW_LIM_ret_Acc_cnt++;
	VLOAD_DC_LOW_LIM_Acc_cnt=0;
	if (VLOAD_DC_LOW_LIM_ret_Acc_cnt >= VLOAD_DC_LOW_LIM_ret_Acc_per) {
		VLOAD_DC_LOW_LIM_ret_Acc_cnt=0;
		apply_state_changes_f(LOAD_DC_LW_FC, 0);
		sprintf(DUB,"LOAD DC Low Return"); prfm(DUB);
	}
} else {
	VLOAD_DC_LOW_LIM_ret_Acc_cnt=0;
}
////// V LOAD DC HIGH/LOW MON ///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} // if (sf_sta_req_ok) {

// AC VOLTAGE MONITORING >>>>>>>>>>>>>>>>>>>
// RRRRRRRRRRRRRRRRRRR
if (VAC_R_rms_sc < VAC_0_LIM && VAC_R_Off_fc==0) {
	VAC_R_0_Acc_cnt++; // S 0
	if (VAC_R_0_Acc_cnt >= VAC_R_0_Acc_per) {
		VAC_R_0_Acc_cnt=0;
		VAC_R_Off_fc=1;
	}
} else {VAC_R_0_Acc_cnt=0;}

if (VAC_R_rms_sc > VAC_Hg_Lim && VAC_R_Hg_fc==0) {
	VAC_R_Hg_Acc_cnt++; // S HIGH
	if (VAC_R_Hg_Acc_cnt >= VAC_R_Hg_Acc_per) {
		VAC_R_Hg_Acc_cnt=0;
		VAC_R_Hg_fc=1;
	}
} else {VAC_R_Hg_Acc_cnt=0;}

if (VAC_R_rms_sc < VAC_Lo_Lim && VAC_R_Lo_fc==0 && VAC_R_Off_fc==0) {
	VAC_R_Lo_Acc_cnt++; // S LOW
	if (VAC_R_Lo_Acc_cnt >= VAC_R_Lo_Acc_per) {
		VAC_R_Lo_Acc_cnt=0;
		VAC_R_Lo_fc=1;
	}
} else {VAC_R_Lo_Acc_cnt=0;}

if (VAC_R_rms_sc >= VAC_0_RET_LIM && VAC_R_Off_fc == 1) {
	VAC_R_0_Ret_Acc_cnt++; // S 0 RET
	if (VAC_R_0_Ret_Acc_cnt >= VAC_R_0_Ret_Acc_per) {
		VAC_R_0_Ret_Acc_cnt=0;
		VAC_R_Off_fc=0;
	}
} else {VAC_R_0_Ret_Acc_cnt=0;}

if (VAC_R_rms_sc >= VAC_HG_RET_LIM && VAC_R_Hg_fc == 1) {
	VAC_R_Hg_Ret_Acc_cnt++; // S HIGH RET
	if (VAC_R_Hg_Ret_Acc_cnt >= VAC_R_Hg_Ret_Acc_per) {
		VAC_R_Hg_Ret_Acc_cnt=0;
		VAC_R_Hg_fc=0;
	}
} else {VAC_R_Hg_Ret_Acc_cnt=0;}

if (VAC_R_rms_sc >= VAC_LW_RET_LIM && VAC_R_Lo_fc == 1) {
	VAC_R_Lo_Ret_Acc_cnt++; // S LOW RET
	if (VAC_R_Lo_Ret_Acc_cnt >= VAC_R_Lo_Ret_Acc_per) {
		VAC_R_Lo_Ret_Acc_cnt=0;
		VAC_R_Lo_fc=0;
	}
} else {VAC_R_Lo_Ret_Acc_cnt=0;}
// RRRRRRRRRRRRRRRRRRR
// SSSSSSSSSSSSSSSSSSS
if (VAC_S_rms_sc < VAC_0_LIM && VAC_S_Off_fc==0) {
	VAC_S_0_Acc_cnt++; // S 0
	if (VAC_S_0_Acc_cnt >= VAC_S_0_Acc_per) {
		VAC_S_0_Acc_cnt=0;
		VAC_S_Off_fc=1;
	}
} else {VAC_S_0_Acc_cnt=0;}

if (VAC_S_rms_sc > VAC_Hg_Lim && VAC_S_Hg_fc==0) {
	VAC_S_Hg_Acc_cnt++; // S HIGH
	if (VAC_S_Hg_Acc_cnt >= VAC_S_Hg_Acc_per) {
		VAC_S_Hg_Acc_cnt=0;
		VAC_S_Hg_fc=1;
	}
} else {VAC_S_Hg_Acc_cnt=0;}

if (VAC_S_rms_sc < VAC_Lo_Lim && VAC_S_Lo_fc==0 && VAC_S_Off_fc==0) {
	VAC_S_Lo_Acc_cnt++; // S LOW
	if (VAC_S_Lo_Acc_cnt >= VAC_S_Lo_Acc_per) {
		VAC_S_Lo_Acc_cnt=0;
		VAC_S_Lo_fc=1;
	}
} else {VAC_S_Lo_Acc_cnt=0;}

if (VAC_S_rms_sc >= VAC_0_RET_LIM && VAC_S_Off_fc == 1) {
	VAC_S_0_Ret_Acc_cnt++; // S 0 RET
	if (VAC_S_0_Ret_Acc_cnt >= VAC_S_0_Ret_Acc_per) {
		VAC_S_0_Ret_Acc_cnt=0;
		VAC_S_Off_fc=0;
	}
} else {VAC_S_0_Ret_Acc_cnt=0;}

if (VAC_S_rms_sc >= VAC_HG_RET_LIM && VAC_S_Hg_fc == 1) {
	VAC_S_Hg_Ret_Acc_cnt++; // S HIGH RET
	if (VAC_S_Hg_Ret_Acc_cnt >= VAC_S_Hg_Ret_Acc_per) {
		VAC_S_Hg_Ret_Acc_cnt=0;
		VAC_S_Hg_fc=0;
	}
} else {VAC_S_Hg_Ret_Acc_cnt=0;}

if (VAC_S_rms_sc >= VAC_LW_RET_LIM && VAC_S_Lo_fc == 1) {
	VAC_S_Lo_Ret_Acc_cnt++; // S LOW RET
	if (VAC_S_Lo_Ret_Acc_cnt >= VAC_S_Lo_Ret_Acc_per) {
		VAC_S_Lo_Ret_Acc_cnt=0;
		VAC_S_Lo_fc=0;
	}
} else {VAC_S_Lo_Ret_Acc_cnt=0;}
// SSSSSSSSSSSSSSSSSSS
// TTTTTTTTTTTTTTTTTTT
if (VAC_T_rms_sc < VAC_0_LIM && VAC_T_Off_fc==0) {
	VAC_T_0_Acc_cnt++; // S 0
	if (VAC_T_0_Acc_cnt >= VAC_T_0_Acc_per) {
		VAC_T_0_Acc_cnt=0;
		VAC_T_Off_fc=1;
	}
} else {VAC_T_0_Acc_cnt=0;}

if (VAC_T_rms_sc > VAC_Hg_Lim && VAC_T_Hg_fc==0) {
	VAC_T_Hg_Acc_cnt++; // S HIGH
	if (VAC_T_Hg_Acc_cnt >= VAC_T_Hg_Acc_per) {
		VAC_T_Hg_Acc_cnt=0;
		VAC_T_Hg_fc=1;
	}
} else {VAC_T_Hg_Acc_cnt=0;}

if (VAC_T_rms_sc < VAC_Lo_Lim && VAC_T_Lo_fc==0 && VAC_T_Off_fc==0) {
	VAC_T_Lo_Acc_cnt++; // S LOW
	if (VAC_T_Lo_Acc_cnt >= VAC_T_Lo_Acc_per) {
		VAC_T_Lo_Acc_cnt=0;
		VAC_T_Lo_fc=1;
	}
} else {VAC_T_Lo_Acc_cnt=0;}

if (VAC_T_rms_sc >= VAC_0_RET_LIM && VAC_T_Off_fc == 1) {
	VAC_T_0_Ret_Acc_cnt++; // S 0 RET
	if (VAC_T_0_Ret_Acc_cnt >= VAC_T_0_Ret_Acc_per) {
		VAC_T_0_Ret_Acc_cnt=0;
		VAC_T_Off_fc=0;
	}
} else {VAC_T_0_Ret_Acc_cnt=0;}

if (VAC_T_rms_sc >= VAC_HG_RET_LIM && VAC_T_Hg_fc == 1) {
	VAC_T_Hg_Ret_Acc_cnt++; // S HIGH RET
	if (VAC_T_Hg_Ret_Acc_cnt >= VAC_T_Hg_Ret_Acc_per) {
		VAC_T_Hg_Ret_Acc_cnt=0;
		VAC_T_Hg_fc=0;
	}
} else {VAC_T_Hg_Ret_Acc_cnt=0;}

if (VAC_T_rms_sc >= VAC_LW_RET_LIM && VAC_T_Lo_fc == 1) {
	VAC_T_Lo_Ret_Acc_cnt++; // S LOW RET
	if (VAC_T_Lo_Ret_Acc_cnt >= VAC_T_Lo_Ret_Acc_per) {
		VAC_T_Lo_Ret_Acc_cnt=0;
		VAC_T_Lo_fc=0;
	}
} else {VAC_T_Lo_Ret_Acc_cnt=0;}
// TTTTTTTTTTTTTTTTTTT
// RST RST RST RST RST RST RST
if ((VAC_R_Off_fc == 0 && VAC_S_Off_fc == 0 && VAC_T_Off_fc == 0) && !is_state_active(VAC_ON_FC)) {
	apply_state_changes_f(VAC_ON_FC, 1);
	sprintf(DUB,"VAC_ON_FC 1"); prfm(DUB);
}
if ((VAC_R_Off_fc == 1 && VAC_S_Off_fc == 1 && VAC_T_Off_fc == 1) && is_state_active(VAC_ON_FC)) {
	apply_state_changes_f(VAC_ON_FC, 0);
	sprintf(DUB,"VAC_ON_FC 0"); prfm(DUB);
}
if ((VAC_R_Off_fc == 1 || VAC_S_Off_fc == 1 || VAC_T_Off_fc == 1) && !is_state_active(VAC_OFF_FC)) {
	apply_state_changes_f(VAC_OFF_FC, 1);
	sprintf(DUB,"VAC_OFF_FC 1"); prfm(DUB);
}
if ((VAC_R_Off_fc == 0 && VAC_S_Off_fc == 0 && VAC_T_Off_fc == 0) && is_state_active(VAC_OFF_FC)) {
	apply_state_changes_f(VAC_OFF_FC, 0);
	sprintf(DUB,"VAC_OFF_FC 0"); prfm(DUB);
}
if ((VAC_R_Hg_fc == 1 || VAC_S_Hg_fc == 1 || VAC_T_Hg_fc == 1) && !is_state_active(VAC_HG_FC)) {
	apply_state_changes_f(VAC_HG_FC, 1);	// VAC Hg
	sprintf(DUB,"VAC HG"); prfm(DUB);
}
if ((VAC_R_Hg_fc == 0 && VAC_S_Hg_fc == 0 && VAC_T_Hg_fc == 0) && is_state_active(VAC_HG_FC)) {
	apply_state_changes_f(VAC_HG_FC, 0);	// VAC Hg RET
	sprintf(DUB,"VAC HG RETURNED"); prfm(DUB);
}
if ((VAC_R_Lo_fc == 1 || VAC_S_Lo_fc == 1 || VAC_T_Lo_fc == 1) && !is_state_active(VAC_LO_FC)) {
	apply_state_changes_f(VAC_LO_FC, 1);	// VAC Lo
	apply_state_changes_f(VAC_ON_FC, 0);
	sprintf(DUB,"VAC LO"); prfm(DUB);
}
if ((VAC_R_Lo_fc == 0 && VAC_S_Lo_fc == 0 && VAC_T_Lo_fc == 0) && is_state_active(VAC_LO_FC)) {
	apply_state_changes_f(VAC_LO_FC, 0);	// VAC Lo RET
	apply_state_changes_f(VAC_ON_FC, 1);
	sprintf(DUB,"VAC LO RETURNED"); prfm(DUB);
}
// RST RST RST RST RST RST RST
// AC VOLTAGE MONITORING <<<<<<<<<<<<<<<<<<<<
// CURRENT LIMIT STATES >>>>>>>>>>>>>>>>>>>
	if (pid_output_i_rect < 0 && !is_state_active(RECTIFIER_CURRENT_LIMIT_FC)) {
		rectifier_current_limit_Acc_cnt++;
		rectifier_current_limit_return_Acc_cnt=0;
		if (rectifier_current_limit_Acc_cnt >= rectifier_current_limit_Acc_per) {
			apply_state_changes_f(RECTIFIER_CURRENT_LIMIT_FC, 1);
			rectifier_current_limit_Acc_cnt=0;
			rectifier_current_limit_accepted=1;
		}
	} else if (pid_output_i_rect >= 0 && is_state_active(RECTIFIER_CURRENT_LIMIT_FC)) {
		rectifier_current_limit_return_Acc_cnt++;
		rectifier_current_limit_Acc_cnt=0;
		if (rectifier_current_limit_return_Acc_cnt >= rectifier_current_limit_return_Acc_per) {
			apply_state_changes_f(RECTIFIER_CURRENT_LIMIT_FC, 0);
			rectifier_current_limit_return_Acc_cnt=0;
			rectifier_current_limit_accepted=0;
		}
	} else {
		rectifier_current_limit_Acc_cnt=0;
		rectifier_current_limit_return_Acc_cnt=0;
	}
	if (pid_output_i_batt < 0 && !is_state_active(BATTERY_CURRENT_LIMIT_FC)) {
		battery_current_limit_Acc_cnt++;
		battery_current_limit_return_Acc_cnt=0;
		if (battery_current_limit_Acc_cnt >= battery_current_limit_Acc_per) {
			apply_state_changes_f(BATTERY_CURRENT_LIMIT_FC, 1);
			battery_current_limit_Acc_cnt=0;
			battery_current_limit_accepted=1;
		}
	} else if(pid_output_i_batt >= 0 && is_state_active(BATTERY_CURRENT_LIMIT_FC)) {
		battery_current_limit_return_Acc_cnt++;
		battery_current_limit_Acc_cnt=0;
		if (battery_current_limit_return_Acc_cnt >= battery_current_limit_return_Acc_per) {
			apply_state_changes_f(BATTERY_CURRENT_LIMIT_FC, 0);
			battery_current_limit_return_Acc_cnt=0;
			battery_current_limit_accepted=0;
		}
		} else {
			battery_current_limit_Acc_cnt=0;
			battery_current_limit_return_Acc_cnt=0;
		}
}	// if (device_start_up_delay_completed==1) {
// CURRENT LIMIT STATES <<<<<<<<<<<<<<<<<<<<<<


// FREQUENCY (50ms loop)
	frq_r_updn_avg_m=275e6 / (float) per_r_updn_avg_m;
	frq_s_updn_avg_m=275e6 / (float) per_s_updn_avg_m;
	frq_t_updn_avg_m=275e6 / (float) per_t_updn_avg_m;

// half cycle duration stability
	per_stable_all_val=(per_r_dn_stable_fl<<5)+(per_r_up_stable_fl<<4)+(per_s_dn_stable_fl<<3)+
		(per_s_up_stable_fl<<2)+(per_t_dn_stable_fl<<1)+(per_t_up_stable_fl); // ==63 stable

	tm_r_rise_tm_s_fall_1=(float) tm_r_rise_tm_s_fall/per_rst_6_avg_m; // n003
	tm_r_rise_tm_t_fall_1=(float) tm_r_rise_tm_t_fall/per_rst_6_avg_m;
	per6_dist_int_sum=round(tm_r_rise_tm_s_fall_1)+round(tm_r_rise_tm_t_fall_1); // n004

// phase sequence. predicted from input mesurement connections.
	if (tm_r_rise_tm_s_fall < per_rst_6_avg_m*1.1 && tm_r_rise_tm_s_fall > per_rst_6_avg_m*0.8) { // n005
		phase_sequence=1; // R S T
		tm_r_rise_tm_s_fall_2=ABS((1-tm_r_rise_tm_s_fall_1)*100); // n006
		tm_r_rise_tm_t_fall_2=ABS((1-tm_r_rise_tm_t_fall_1/5)*100);
	}
	else if (tm_r_rise_tm_s_fall < per_rst_6_avg_m*5*1.1 && tm_r_rise_tm_s_fall > per_rst_6_avg_m*5*0.8) {
		phase_sequence=2; // R T S
		tm_r_rise_tm_s_fall_2=ABS((1-tm_r_rise_tm_s_fall_1/5)*100);
		tm_r_rise_tm_t_fall_2=ABS((1-tm_r_rise_tm_t_fall_1)*100);
	}
	else {
		phase_sequence=0; // unknown
	}

// define a stability condition for line signal
	if ((phase_sequence==1 || phase_sequence==2) && per_stable_all_val==63 &&
		per6_dist_int_sum == 6 && tm_r_rise_tm_s_fall_2 < 10 && tm_r_rise_tm_t_fall_2 < 10) { // n007
		if (line_sgn_stable==0) {
			line_sgn_stable_Acc_cnt++;
			if (line_sgn_stable_Acc_cnt>=5) { // n008
				line_sgn_stable=1;
				line_sgn_stable_Acc_cnt=0;
			}
		}
	} else {
		line_sgn_stable=0;
		line_sgn_stable_Acc_cnt=0;
	}

//	if (line_sgn_stable && f_line_in_stable_cnt_reset_start==1) { // line in unstable fault counter reset at start
//		f_line_in_stable_cnt_reset_start=0;
//		f_line_in_stable_cnt=0;
//	}
//
//    if (f_line_in_stable==0) {
//    	f_line_in_stable_cnt++;
//    	f_line_in_stable_accepted=1;
//    	f_line_in_stable=1;
//    }

//	if (LL_USART_IsActiveFlag_RXNE(USART1)) {
//		UART1_recv_char=LL_USART_ReceiveData8(USART1);
//		if (UART1_recv_char=='z') {
//			UART1_recv_char=0;
//		}
//	}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// TEMP READ ////////////////////////////////////////////////////////////////////////////////////////////////////
	read_temp_dat_from_rx_buffer_cnt++;
	if (read_temp_dat_from_rx_buffer_cnt >= 20) {
		read_temp_dat_from_rx_buffer_cnt=0;
//		sprintf(DUB,"t cnt per %lu %lu", ovtmp_open_cnt, ovtmp_open_per); prfm(DUB);

		for (int i = 0; i < 3; i++) { // read rx buffer
			tmp_dat_C[i] = tmp144_convert_temperature((U10_rxBuf[2*i+3] << 8) | U10_rxBuf[2*i+2]);

		}

		U10_rxCount=0; // clear rx buffer cnt
		for (int i = 0; i < U10_RX_BUFFER_SIZE; i++) { // clear rx buffer
		    U10_rxBuf[i] = 0;
		}
		USART10_SendByte(0x55); // request temperature data
		delayA_1us(10);
		USART10_SendByte(0xF1);
	}
//// SOĞUTUCU ///////////////////////////////////////////////////////////////////////////////////////////////////////
	if (sogut_sensor_exists) {
		if (tmp_dat_C[0]+temp_test_var_1 > EpD[SET_OVTM_ALRM_LIM][0].V1 && !is_state_active(OVERTEMP_ALARM_FC)) { // overtemp alarm enable
			apply_state_changes_f(OVERTEMP_ALARM_FC, 1);
			ovtmp_open_per=(uint32_t) (EpD[SET_OVTM_OPEN_DUR][0].V1*1000/50); // calculate alarm to open duration in 50ms
		} else if (tmp_dat_C[0]+temp_test_var_1 > EpD[SET_OVTM_OPEN_LIM][0].V1 && !is_state_active(OVERTEMP_OPEN_FC)) { // overtemp open enable
			apply_state_changes_f(OVERTEMP_OPEN_FC, 1);
		} else if (tmp_dat_C[0]+temp_test_var_1 < EpD[SET_OVTM_ALRM_LIM][0].V1-5 && is_state_active(OVERTEMP_ALARM_FC)) { // overtemp alarm disable
			apply_state_changes_f(OVERTEMP_ALARM_FC, 0);
			ovtmp_open_cnt=0; // reset open count only when alarm is deactivated
		} else if (tmp_dat_C[0]+temp_test_var_1 < EpD[SET_OVTM_ALRM_LIM][0].V1-5 && is_state_active(OVERTEMP_OPEN_FC)) { // overtemp open disable
			apply_state_changes_f(OVERTEMP_OPEN_FC, 0);
		}
		if (is_state_active(OVERTEMP_ALARM_FC) && !is_state_active(OVERTEMP_OPEN_FC)) {
			ovtmp_open_cnt++;
			if (ovtmp_open_cnt >= ovtmp_open_per) {
				apply_state_changes_f(OVERTEMP_OPEN_FC, 1);
			}
		}
	}

//// AKÜ ////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (batt_sensor_exists) {
		if (tmp_dat_C[2]+temp_test_var_2 <= 0 && !is_state_active(BAT_TEMP_ZERO_FC)) { // BATT TEMP ZERO disable
			apply_state_changes_f(BAT_TEMP_ZERO_FC, 1);
		} else if (tmp_dat_C[2]+temp_test_var_2 >= 50 && !is_state_active(BAT_TEMP_50_FC)) { // BATT TEMP 50 disable
			apply_state_changes_f(BAT_TEMP_50_FC, 1);
		} else if (tmp_dat_C[2]+temp_test_var_2 > 5 && is_state_active(BAT_TEMP_ZERO_FC)) { // BATT TEMP ZERO enable
			apply_state_changes_f(BAT_TEMP_ZERO_FC, 0);
		} else if (tmp_dat_C[2]+temp_test_var_2 < 45 && is_state_active(BAT_TEMP_50_FC)) { // BATT TEMP 50 enable
			apply_state_changes_f(BAT_TEMP_50_FC, 0);
		}
	}

////// TEMP READ ////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	stability_vrect_fc();	// vrect_stable 1 0 yapıyor
	stability_ibat_fc();	// ibat_stable ve batt_current_detected 1 0 yapıyor


if (SW_BATT_OFF && blm_batt_connected) {
	blm_batt_connected=0;													// BATT SWITCH OFF
	blm_cancel_op_return_normal();
	sprintf(DUB,"SW off"); prfm(DUB);
} else if (VBAT_pas.a16 <= Vbat_flt && blm_batt_connected) {
	blm_batt_connected=0;													// V BATT LOW
	blm_cancel_op_return_normal();
	sprintf(DUB,"Vbat too low"); prfm(DUB);
}
if (batt_current_detected && !blm_batt_connected) {
	blm_batt_connected=1;													// CURRENT DETECTED
	blm_cancel_op_return_normal();
}
// BURAYA KADAR Kİ KISIM TAMAM
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

if (vrect_stable && !batt_current_detected && !blm_req_up_down_vtarg_limits) {
	blm_req_up_down_vtarg_limits=1;									// REQUEST UP DOWN VTARG LIMITS
}
if (blm_req_up_down_vtarg_limits && !blm_set_up_down_vtarg_limits_completed) {
	blm_set_up_down_vtarg_limits();									// SET UP DOWN VTARG LIMITS
	blm_set_up_down_vtarg_limits_completed=1;						// SET UP DOWN VTARG LIMITS COMPLETED
	blm_req_reduce_vtarg=1;										// REQUEST REDUCE VTARG
}

if (blm_req_reduce_vtarg) {
	if (V_targ_con_sy > blm_down_vtarg_limit_2) {
		set_V_targ_con_sy(V_targ_con_sy*(1-blm_vi_change_mult));
		blm_reducing_vtarg_cnt++;		 							// REDUCING VTARG HERE
		blm_reducing_vtarg=1;	//💌💌💌
	} else {blm_reducing_vtarg=0;}
	if (V_targ_con_sy <= blm_down_vtarg_limit_2) {
		blm_req_reduce_vtarg=0;
		blm_reducing_vtarg=0;	//💌💌💌
		blm_reducing_vtarg_completed=1; 							// REDUCING VTARG COMPLETED
	}
}

if (blm_reducing_vtarg_completed) {
	blm_wait_at_low_lim_cnt++;
	if (blm_wait_at_low_lim_cnt >= blm_wait_at_low_lim_per && !blm_wait_at_low_lim_completed) {
		blm_req_return_voltage_to_normal=1; 						// REQ RETURN TO NORMAL VTARG LEVEL
		blm_reducing_vtarg_completed=0;
		blm_wait_at_low_lim_cnt=0;
		blm_wait_at_low_lim_completed=1;
	}
} else {
	blm_wait_at_low_lim_cnt=0;
}

if (blm_req_return_voltage_to_normal) {
	if (V_targ_con_sy < Current_charge_voltage) {
		set_V_targ_con_sy(V_targ_con_sy*(1+blm_vi_change_mult));
		blm_increasing_vtarg_back_cnt++;							// RETURNING VOLTAGE TO NORMAL HERE;
		blm_increasing_vtarg_back=1;	//❤❤❤
	} else {blm_increasing_vtarg_back=0;}
	if (V_targ_con_sy >= Current_charge_voltage && !blm_return_voltage_to_normal_completed) {
		blm_req_return_voltage_to_normal=0;
		blm_increasing_vtarg_back=0;	//❤❤❤
		blm_return_voltage_to_normal_completed=1;					// RETURNING VOLTAGE TO NORMAL COMPLETED
		blm_req_return_state_to_normal=1;
	}
}

if (blm_req_return_state_to_normal) {
	blm_req_return_state_to_normal=0;
	set_V_targ_con_sy(Current_charge_voltage);
}

flagları live expressinos a ekle ve ekrar çalıştır.
şu anki haliyle voltaj indirip çıkarıyor.




if (blm_reducing_vtarg || blm_reducing_vtarg_completed) {			// REDUCING VTARG OR REDUCING VTARG COMPLETED
	vtarg_VRECT_diff_high=fabs(V_targ_con_sy-VRECT_pas.a16) > blm_V_step_05perc*3;
}


//	    sprintf(DUB,"vs %d %d %d %d %d", vrect_stable_lv1, vrect_stable_lv1_cnt, vrect_stable_lv2_cnt, vrect_stable_lv3_cnt, vrect_stable); prfm(DUB);
//	    sprintf(DUB,"mx %f", v_max); prfm(DUB);
//	    sprintf(DUB,"vn %f", VRECT_pas.a16); prfm(DUB);
//	    sprintf(DUB,"mn %f", v_min); prfm(DUB);



//
//    sprintf(DUB, "vs %d %d",
//            vrect_stable_cnt,
//            vrect_stable);
//    prfm(DUB);
//    sprintf(DUB, "mx %f", v_max); prfm(DUB);
//    sprintf(DUB, "vn %f", VRECT_pas.a16); prfm(DUB);
//    sprintf(DUB, "mn %f", v_min); prfm(DUB);








//			sprintf(DUB,"bb1 %.2f %.2f %.2f %.2f", VRECT_per_avg_sc_old, VBAT_per_avg_sc_old, IBAT_per_avg_roll_sc_old, V_targ_con_sy_old); prfm(DUB);
//			sprintf(DUB,"bb1 %.2f %.2f %.2f %.2f", VRECT_pas, VBAT_pas, IBAT_per_avg_roll_sc, V_targ_con_sy); prfm(DUB);
//			sprintf(DUB,"bb1 %.2f %.2f %.2f %.2f", VRECT_old_diff, VBAT_old_diff, 0.0f, V_targ_con_sy_old_diff); prfm(DUB);












} // if (ms_tick_cnt-while_delay50_h >= 50) {










if (ms_tick_cnt-while_RTC_delay_h >= while_RTC_delay_per) {
	while_RTC_delay_h=ms_tick_cnt;
    Read_Register_0x00_to_0x06();
    rtc_timestamp_fnc();
}

if (ms_tick_cnt-while_LCD_delay_h >= while_LCD_delay_per) {
	while_LCD_delay_h=ms_tick_cnt;

	if (ms_tick_cnt-while_LCD_reinit_h >= while_LCD_reinit_per) {
		while_LCD_reinit_per=ms_tick_cnt;
		GLCD_Init_while();

	}

	GLCD_ClearScreen(0x00);

    switch (currentPage) {
        case HOME_PAGE_pg:
        	HOME_PAGE_pg_disp();
            break;
        case MAIN_MENU_pg:
        	MAIN_MENU_pg_disp();
            break;
        case CHARGE_SETT_pg:
        	CHARGE_SETT_pg_disp();
            break;
        case DEVICE_SETT_pg:
        	DEVICE_SETT_pg_disp();
            break;
        case MANAGEMENT_pg:
        	MANAGEMENT_pg_disp();
            break;
        case FAULT_CODES_REPORT_pg:
        	FAULT_CODES_REPORT_pg_disp();
            break;
        case DROPPER_pg:
        	DROPPER_pg_disp();
            break;
        case RELAY_ORDER_pg:
        	RELAY_ORDER_pg_disp();
            break;
        case FAULT_CODES_RESET_pg:
        	FAULT_CODES_RESET_pg_disp();
            break;
        case DEVICE_RESET_pg:
        	DEVICE_RESET_pg_disp();
            break;
        case CALIBRATION_pg:
        	CALIBRATION_pg_disp();
            break;
        case DATE_TIME_pg:
        	DATE_TIME_pg_disp();
            break;
        default:
            break;
    }
	GLCD_RefreshGRAM();
}

if (ms_tick_cnt-UART_Debg_t_h >= 1000) {
	UART_Debg_t_h=ms_tick_cnt;

//	uart_debug_cnt();

	if (unexpected_program_state==1) {	// if else koşulları içinde takılma durumu. olmayan koşula gelme durumu.
		sprintf(DUB,"%lu %s\033[A", unexpected_program_state, UXPUB); prfm(DUB);
	}
//	if (var1==3) {
//		var1=0;
//		apply_state_changes_f(TRF_FAN2_REL,1);
//		calc_REL_24Bit_Data_f();
//	}
//	if (var1==4) {
//		var1=0;
//		apply_state_changes_f(TRF_FAN2_REL,0);
//		calc_REL_24Bit_Data_f();
//	}
//	if (var1==1) {
//		var1=0;
//		rel_ord_tb[var2].rel_sel_nm=rel_dat_tb[var3].rel_sel_nm;
//		rel_ord_tb[var2].rel_sel_desc=rel_dat_tb[var3].rel_sel_desc;
//		calc_REL_24Bit_Data_f();
//		print_REL_OUT_Table();
//	}
//	if (var1==6) {
//		var1=0;
//		REL_OUT_order_chng_f(OVERTEMP_OPEN_FC_REL,4);
//		calc_REL_24Bit_Data_f();
//		print_REL_OUT_Table();
//	}
}

buttonScn();
processShiftRegister_LED_16(LED_16_Data);
processShiftRegister_LED_7(LED_7_Data);
processShiftRegister_Relay_Board_16(REL_24Bit_Data);

if (req_reset_db==1) {
	req_reset_db=0;
	NVIC_SystemReset();
}


