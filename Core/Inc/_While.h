// soft start
if (ms_tick_cnt-sfst_1_t_hold >= 5) { // soft start step. 5ms
	sfst_1_t_hold=ms_tick_cnt;

	if (sfsta_op_phase == S_SFSTA_REQ && line_sgn_stable && thy_stop_fault_hold_bits==0) {
		if (targ_DC_voltage <= temp_targ_DC_voltage) {
			set_targ_DC_voltage(targ_DC_voltage*1.003);
		}
		else if (targ_DC_voltage > temp_targ_DC_voltage) {
			set_targ_DC_voltage(temp_targ_DC_voltage);
			sfsta_op_phase=S_SFSTA_REQ_OK;
			apply_state_changes_f(SOFT_START_ST, 0);
//			actions_after_charge_mode_change(6);
			PRF_GEN("Soft start request ok");
		}
		if (targ_DC_voltage < VRECT_smp_sc-4) {		// vout zaten yüksekse direk hedefi oraya yaklaştırarak başla
			set_targ_DC_voltage(VRECT_smp_sc-4);
			PRF_GEN("Sfst OK targ_DC_voltage < VRECT_smp_sc-4;");
		}
	}
}

if (ms_tick_cnt-while_delay50_h >= 50) {	// 50ms loop. using ms counter.
	while_delay50_h=ms_tick_cnt;

if (sta_op_phase == S_STARTUP_DELAY_CNT) {	// delay bekleme state'i.
	device_start_up_delay_completed_cnt++;
	if (device_start_up_delay_completed_cnt >= 40 && device_start_up_delay_completed==0) {
		device_start_up_delay_completed=1;
		sta_op_phase=S_STARTUP_DELAY_OK;	// start up delay ok.
	}
}
// thy_drv_en_req set etmek başlatma işlemi yapmak için yeterli.
if (sta_op_phase==S_STARTUP_DELAY_OK) {		// tristör sürme başlatma
	if (thy_drv_en==0 && VRECT_pas.a64 <= temp_targ_DC_voltage*1.1 && thy_drv_en_req ==1 && line_sgn_stable && thy_stop_fault_hold_bits==0) {
		sf_sta_req_cnt++;
		if (sf_sta_req_cnt >= 20) {		// delayed soft start trigger
			set_targ_DC_voltage(5);
			apply_state_changes_f(STOP_FC, 0);
			apply_state_changes_f(START_FC, 1);
			apply_state_changes_f(SOFT_START_ST, 1);
			thy_drv_en=1;
			sfsta_op_phase=S_SFSTA_REQ;
			thy_drv_en_req=0;
			sfst_1_unexpected_state=0;
			sf_sta_req_cnt=0;
			PRF_GEN("Soft start request");
		}
	}

// Arıza durumundan çıkınca veya sistem uygunsa doğrultucuyu başlat
// burda düzenleme gerekebilir. hangi arızadan ne kadar süre sonra tekrar başlatılacak belirlemek lazım.
if (thy_stop_fault_hold_bits==0 && thy_drv_en==0 && user_wants_allows_thy_drv==1 && !thy_drv_en_req) { // bütün thy stop gerektiren arızalar deaktif durumunda ise.
	thy_drv_en_req = 1; // bu durumda thy drv en req gönder.
	PRF_GEN("thy_stop_fault_hold_bits 0. bütün arızalar sıfırlandı. starting rectf");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// MANAGE CHARGE MODE AUTO //////////////////////////////////////////////////////////////////////////////////////
if (EpD[SET_CH_CONT_MODE][0].V1 == AUTO) {
	if (switch_to_auto_mode_completed==0) {
		switch_to_auto_mode_completed=1; // başka bir moddan auto moda geçildi.
		boost_of_auto_mode_active=0; // şimdi aşağıdaki iki moddan birini seç. float ya da boost.
		float_of_auto_mode_active=0; // şimdi aşağıdaki iki moddan birini seç. float ya da boost.
		PRF_GEN("switch_to_auto_mode_completed");
	}
	else if (IBAT_pas.a1 > EpD[I_LIM_TO_BOOST][0].V1 && boost_of_auto_mode_active==0) {
		float_of_auto_mode_active=0;
		boost_of_auto_mode_active=1;
//		actions_after_charge_mode_change(33);
		temp_targ_DC_voltage=EpD[VBAT_BOOST][0].V1;
		targ_DC_current=EpD[SET_IBAT_BOOST][0].V1;
		set_targ_DC_voltage(temp_targ_DC_voltage);
		apply_state_changes_f(ST_FLOAT_CHARGE, 0);
		apply_state_changes_f(BOOST_CHARGE_FC, 1);
		LED_7_Data &= ~FLOAT_CHARGE_LED;
		LED_7_Data |= BOOST_CHARGE_LED;
//		switch_to_auto_mode_completed=0;
		timed_mode_actions_do_once=0;
		charge_mode_timed_time_sec=0; // ekrandaki timed mode kalan saniye değerini kaldır
		PRF_GEN("BOOST charge mode");

		PRF_GEN("AUTO switch to BOOST %f", IBAT_pas.a1);
	}
	else if (IBAT_pas.a1 < EpD[I_LIM_TO_FLOAT][0].V1 && float_of_auto_mode_active==0) {
		float_of_auto_mode_active=1;
		boost_of_auto_mode_active=0;
//		actions_after_charge_mode_change(34);
		temp_targ_DC_voltage=EpD[VBAT_FLOAT][0].V1;
		targ_DC_current=EpD[SET_IBAT_FLOAT][0].V1;
		set_targ_DC_voltage(temp_targ_DC_voltage);
		apply_state_changes_f(ST_FLOAT_CHARGE, 1);
		apply_state_changes_f(BOOST_CHARGE_FC, 0);
		LED_7_Data &= ~BOOST_CHARGE_LED;
		LED_7_Data |= FLOAT_CHARGE_LED;
//		switch_to_auto_mode_completed=0;
		timed_mode_actions_do_once=0;
		charge_mode_timed_time_sec=0; // ekrandaki timed mode kalan saniye değerini kaldır
		PRF_GEN("FLOAT charge mode");

		PRF_GEN("AUTO switch to FLOAT %f", IBAT_pas.a1);
	}
}
////// MANAGE CHARGE MODE AUTO //////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// MANAGE CHARGE MODE TIMED /////////////////////////////////////////////////////////////////////////////////////
//if (EpD[SET_CH_CONT_MODE][0].V1 == TIMED && EpD[SET_CHARGE_MODE][0].V1 == FLOAT || EpD[SET_CHARGE_MODE][0].V1 == BOOST) {
//	if (charge_mode_timed_time_cnt > 0) {
//		charge_mode_timed_float=0;
//		charge_mode_timed_boost=1;
//		charge_mode_timed_time_cnt--;
//		charge_mode_timed_time_sec=charge_mode_timed_time_cnt/20;
//		PRF_GEN("TIMED mode selected minutes %f %lu", EpD[SET_BOOST_TIME][0].V1, charge_mode_timed_time_cnt);
//	}
//
//	if (charge_mode_timed_time_cnt == 0 && timed_mode_time_ended==0) {
//		timed_mode_time_ended=1;
//		charge_mode_timed_float=1;
//		charge_mode_timed_boost=0;
//		temp_targ_DC_voltage=EpD[VBAT_FLOAT][0].V1;
//		targ_DC_current=EpD[SET_IBAT_FLOAT][0].V1;
//		set_targ_DC_voltage(temp_targ_DC_voltage);
//		apply_state_changes_f(ST_FLOAT_CHARGE, 1);
//		apply_state_changes_f(BOOST_CHARGE_FC, 0);
//		LED_7_Data &= ~BOOST_CHARGE_LED;
//		LED_7_Data |= FLOAT_CHARGE_LED;
//
//		actions_after_charge_mode_change(8);
//		PRF_GEN("Timer mode count down ended. Switch to FLOAT mode settings");
//	}
//}
////// MANAGE CHARGE MODE TIMED /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// MANAGE CHARGE MODE AUTO //////////////////////////////////////////////////////////////////////////////////////
if (EpD[SET_CHARGE_MODE][0].V1 == AUTO) {

}
////// MANAGE CHARGE MODE AUTO //////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//SW_LINE_OFF=isInSet_(SW_LINE_P);
//SW_BATT_OFF=isInSet_(SW_BATT_P);
//SW_LOAD_OFF=isInSet_(SW_LOAD_P);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// MCCB MONITORING //////////////////////////////////////////////////////////////////////////////////////////////
state_set(ST_LINE_MCCB_OFF, !isInSet_(SW_LINE_P)); // LINE FUSE OFF MONITORING
state_set(ST_BATT_MCCB_OFF, !isInSet_(SW_BATT_P)); // BATT FUSE OFF MONITORING
state_set(ST_LOAD_MCCB_OFF, !isInSet_(SW_LOAD_P)); // LOAD FUSE OFF MONITORING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// BATTERY_FAULT_FC decide //////////////////////////////////////////////////////////////////////////////////////
if (!state_get(BATTERY_FAULT_FC)) {
	if (state_get(ST_BATT_MCCB_OFF) || state_get(ST_BATT_LINE_BROKEN) || state_get(BATT_REVERSE_FC) || state_get(BATT_SHORT_FC)) {
		state_set(BATTERY_FAULT_FC, 1);
	}
} else if (state_get(BATTERY_FAULT_FC)) {
	if (!state_get(ST_BATT_MCCB_OFF) && !state_get(ST_BATT_LINE_BROKEN) && !state_get(BATT_REVERSE_FC) && !state_get(BATT_SHORT_FC)) {
		state_set(BATTERY_FAULT_FC, 0);
	}
}
////// BATTERY_FAULT_FC decide //////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// BATT REVERSE MONITORING //////////////////////////////////////////////////////////////////////////////////////
if (VBAT_pas.a1 < -10 && EpD[SET_BATT_REV_DET][0].V1==1 && !state_get(BATT_REVERSE_FC)) {
	batt_reverse_Acc_cnt++;
	if (batt_reverse_Acc_cnt >= batt_reverse_Acc_per) {
		batt_reverse_Acc_cnt=0;
		apply_state_changes_f(BATT_REVERSE_FC, 1);
	}
} else if (VBAT_pas.a1 >= -0.5 && state_get(BATT_REVERSE_FC)) {
	batt_reverse_return_Acc_cnt++;
	if (batt_reverse_return_Acc_cnt >= batt_reverse_return_Acc_per) {
		batt_reverse_return_Acc_cnt=0;
		apply_state_changes_f(BATT_REVERSE_FC, 0);
	}
}
////// BATT REVERSE MONITORING //////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

if (sfsta_op_phase == S_SFSTA_REQ_OK) {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// DCK FAULT MON ////////////////////////////////////////////////////////////////////////////////////////////////

DCK_mon_start_cnt++;
if (DCK_mon_start_cnt >= DCK_mon_start_per) {
	DCK_mon_start_cnt=DCK_mon_start_per; // start delay passed.
	if (VDCKP_perc >= EpD[DC_KAC_POS][0].V1 && !state_get(DC_LEAK_POS_FC)) {
		VDCK_accept_cnt++;
		if (VDCK_accept_cnt >= VDCK_accept_per) {
			VDCK_accept_cnt=0;
			apply_state_changes_f(DC_LEAK_POS_FC, 1);
			PRF_GEN("DC leak above pos lim");
		 }
	} else if (VDCKP_perc < EpD[DC_KAC_POS][0].V1 && state_get(DC_LEAK_POS_FC)) {
		VDCK_return_cnt++;
		if (VDCK_return_cnt >= VDCK_return_per) {
			VDCK_return_cnt=0;
			apply_state_changes_f(DC_LEAK_POS_FC, 0);
			PRF_GEN("DC leak above pos lim removed");
		}
	} else if (VDCKN_perc >= EpD[DC_KAC_NEG][0].V1 && !state_get(DC_LEAK_NEG_FC)) {
		VDCK_accept_cnt++;
		if (VDCK_accept_cnt >= VDCK_accept_per) {
			VDCK_accept_cnt=0;
			apply_state_changes_f(DC_LEAK_NEG_FC, 1);
			PRF_GEN("DC leak below neg lim");
		}
	} else if (VDCKN_perc < EpD[DC_KAC_NEG][0].V1 && state_get(DC_LEAK_NEG_FC)) {
		VDCK_return_cnt++;
		if (VDCK_return_cnt >= VDCK_return_per) {
			VDCK_return_cnt=0;
			apply_state_changes_f(DC_LEAK_NEG_FC, 0);
			PRF_GEN("DC leak below neg lim removed");
		}
	} else {
		VDCK_return_cnt=0;
	}
}

////// DCK FAULT MON ////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// MANAGE DROPPER ///////////////////////////////////////////////////////////////////////////////////////////////

	if (EpD[SET_DROPPER_MANOTO][0].V1==1) { // 0 manuel 1 auto
		if (VLOAD_pas.a1+dropper_test_var_1 > set_dropper_l_hg_V && !state_get(ST_DROPPER_K2) && !state_get(ST_DROPPER_K1)) {
			actvate_drop_cnt++;
			if (actvate_drop_cnt >= actvate_drop_per) {
				actvate_drop_cnt=0;
				EpD[SET_DROPPER_K1][0].V1=1; EpD[SET_DROPPER_K1][1].V1=1; // kullanıcı harici değişim olduğu için bu değişiklik yapılmalı.
				DROPP_BATT_CTRL(EpD[SET_DROPPER_K1][0].V1); // dropper ı kayıtlı değere göre değiştir.
				apply_state_changes_f(ST_DROPPER_K2, EpD[SET_DROPPER_K1][0].V1); // burası ledleri değiştiriyor.
			}
		} else if (VLOAD_pas.a1+dropper_test_var_1 > set_dropper_l_hg_V && state_get(ST_DROPPER_K2) && !state_get(ST_DROPPER_K1)) {
			actvate_drop_cnt++;
			if (actvate_drop_cnt >= actvate_drop_per) {
				actvate_drop_cnt=0;
				EpD[SET_DROPPER_K2][0].V1=1; EpD[SET_DROPPER_K2][1].V1=1;
				DROPP_LOAD_CTRL(EpD[SET_DROPPER_K2][0].V1);
				apply_state_changes_f(ST_DROPPER_K1, EpD[SET_DROPPER_K2][0].V1);
			}
		} else if (VLOAD_pas.a1+dropper_test_var_1 > set_dropper_l_hg_V && !state_get(ST_DROPPER_K2) && state_get(ST_DROPPER_K1)) {
			actvate_drop_cnt++;
			if (actvate_drop_cnt >= actvate_drop_per) {
				actvate_drop_cnt=0;
				EpD[SET_DROPPER_K1][0].V1=1; EpD[SET_DROPPER_K1][1].V1=1;
				DROPP_LOAD_CTRL(EpD[SET_DROPPER_K1][0].V1);
				apply_state_changes_f(ST_DROPPER_K2, EpD[SET_DROPPER_K1][0].V1);
			}
		} else if (VLOAD_pas.a1+dropper_test_var_1 < set_dropper_l_lw_V && state_get(ST_DROPPER_K2) && state_get(ST_DROPPER_K1)) {
			actvate_drop_cnt++;
			if (actvate_drop_cnt >= actvate_drop_per) { // ikisi de devredeyse önce 2. dropperi kapat.
				actvate_drop_cnt=0;
				EpD[SET_DROPPER_K2][0].V1=0; EpD[SET_DROPPER_K2][1].V1=0;
				DROPP_LOAD_CTRL(EpD[SET_DROPPER_K2][0].V1);
				apply_state_changes_f(ST_DROPPER_K1, EpD[SET_DROPPER_K2][0].V1);
			}
		} else if (VLOAD_pas.a1+dropper_test_var_1 < set_dropper_l_lw_V && state_get(ST_DROPPER_K2) && !state_get(ST_DROPPER_K1)) {
			actvate_drop_cnt++;
			if (actvate_drop_cnt >= actvate_drop_per) { // 2. kapalıysa 1. dropperi kapat.
				actvate_drop_cnt=0;
				EpD[SET_DROPPER_K1][0].V1=0; EpD[SET_DROPPER_K1][1].V1=0;
				DROPP_BATT_CTRL(EpD[SET_DROPPER_K1][0].V1);
				apply_state_changes_f(ST_DROPPER_K2, EpD[SET_DROPPER_K1][0].V1);
			}
		} else if (VLOAD_pas.a1+dropper_test_var_1 < set_dropper_l_lw_V && !state_get(ST_DROPPER_K2) && state_get(ST_DROPPER_K1)) {
			actvate_drop_cnt++;
			if (actvate_drop_cnt >= actvate_drop_per) { // 1. kapalıysa 2. dropperi kapat.
				actvate_drop_cnt=0;
				EpD[SET_DROPPER_K2][0].V1=0; EpD[SET_DROPPER_K2][1].V1=0;
				DROPP_LOAD_CTRL(EpD[SET_DROPPER_K2][0].V1);
				apply_state_changes_f(ST_DROPPER_K1, EpD[SET_DROPPER_K2][0].V1);
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// V LOAD DC HIGH/LOW MON ///////////////////////////////////////////////////////////////////////////////////////
//	dropper high limit vload_dc_high_lim olarak kabul ediliyor
		if (VLOAD_pas.a1+load_dcv_test_var_1 > set_dropper_l_hg_V && state_get(ST_DROPPER_K2) && state_get(ST_DROPPER_K1)
				&& !state_get(LOAD_DC_HG_FC)) {
			vload_dc_high_lim_Acc_cnt++; // dropper kademeleri devreye alınmasına rağmen limitin üzerine çıkmış
			vload_dc_high_lim_ret_Acc_cnt=0;
			if (vload_dc_high_lim_Acc_cnt >= vload_dc_high_lim_Acc_per) {
				vload_dc_high_lim_Acc_cnt=0;
				apply_state_changes_f(LOAD_DC_HG_FC, 1);
				PRF_GEN("LOAD DC High");
			}
		} else { vload_dc_high_lim_Acc_cnt=0; }

		if (VLOAD_pas.a1+load_dcv_test_var_1 <= set_dropper_l_hg_V && state_get(LOAD_DC_HG_FC)) {
			vload_dc_high_lim_ret_Acc_cnt++;
			vload_dc_high_lim_Acc_cnt=0;
			if (vload_dc_high_lim_ret_Acc_cnt >= vload_dc_high_lim_ret_Acc_per) {
				vload_dc_high_lim_ret_Acc_cnt=0;
				apply_state_changes_f(LOAD_DC_HG_FC, 0);
				PRF_GEN("LOAD DC High Return");
			}
		} else { vload_dc_high_lim_ret_Acc_cnt=0; }

		if (VLOAD_pas.a1+load_dcv_test_var_1 < set_dropper_l_lw_V && !state_get(ST_DROPPER_K2) && !state_get(ST_DROPPER_K1)
				&& !state_get(LOAD_DC_LW_FC)) {
			vload_dc_low_lim_Acc_cnt++; // dropper kademeleri devrede olmamasına rağmen limitin altına inilmiş
			vload_dc_low_lim_ret_Acc_cnt=0;
			if (vload_dc_low_lim_Acc_cnt >= VLOAD_DC_LOW_LIM_Acc_per) {
				vload_dc_low_lim_Acc_cnt=0;
				apply_state_changes_f(LOAD_DC_LW_FC, 1);
				PRF_GEN("LOAD DC Low");
			}
		} else { vload_dc_low_lim_Acc_cnt=0; }

		if (VLOAD_pas.a1+load_dcv_test_var_1 >= set_dropper_l_lw_V && state_get(LOAD_DC_LW_FC)) {
			vload_dc_low_lim_ret_Acc_cnt++;
			vload_dc_low_lim_Acc_cnt=0;
			if (vload_dc_low_lim_ret_Acc_cnt >= vload_dc_low_lim_ret_Acc_per) {
				vload_dc_low_lim_ret_Acc_cnt=0;
				apply_state_changes_f(LOAD_DC_LW_FC, 0);
				PRF_GEN("LOAD DC Low Return");
			}
		} else { vload_dc_low_lim_ret_Acc_cnt=0; }

////// V LOAD DC HIGH/LOW MON ///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// MANAGE DROPPER ///////////////////////////////////////////////////////////////////////////////////////////////

} // if (sfsta_op_phase == S_SFSTA_REQ_OK) {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// V RECT DC HIGH/LOW MON ///////////////////////////////////////////////////////////////////////////////////////
if (VRECT_pas.a1 > vrect_dc_high_lim && !state_get(RECT_DC_HG_FC) && sfsta_op_phase == S_SFSTA_REQ_OK) {
	vrect_dc_high_lim_Acc_cnt++;
	vrect_dc_high_lim_ret_Acc_cnt=0;
	if (vrect_dc_high_lim_Acc_cnt >= vrect_dc_high_lim_Acc_per) {
		vrect_dc_high_lim_Acc_cnt=0;
		apply_state_changes_f(RECT_DC_HG_FC, 1);
		PRF_GEN("RECT DC High");
	}
} else {
	vrect_dc_high_lim_Acc_cnt=0;
}
if (VRECT_pas.a1 <= vrect_dc_high_lim_ret && state_get(RECT_DC_HG_FC)) {
	vrect_dc_high_lim_ret_Acc_cnt++;
	vrect_dc_high_lim_Acc_cnt=0;
	if (vrect_dc_high_lim_ret_Acc_cnt >= vrect_dc_high_lim_ret_Acc_per) {
		vrect_dc_high_lim_ret_Acc_cnt=0;
		apply_state_changes_f(RECT_DC_HG_FC, 0);
		PRF_GEN("RECT DC High Return");
	}
} else {
	vrect_dc_high_lim_ret_Acc_cnt=0;
}
if (VRECT_pas.a1 < vrect_dc_low_lim && !state_get(RECT_DC_LW_FC) && sfsta_op_phase == S_SFSTA_REQ_OK) {
	vrect_dc_low_lim_Acc_cnt++;
	vrect_dc_low_lim_ret_Acc_cnt=0;
	if (vrect_dc_low_lim_Acc_cnt >= vrect_dc_low_lim_Acc_per) {
		vrect_dc_low_lim_Acc_cnt=0;
		apply_state_changes_f(RECT_DC_LW_FC, 1);
		PRF_GEN("RECT DC Low");
	}
} else {
	vrect_dc_low_lim_Acc_cnt=0;
}
if (VRECT_pas.a1 >= vrect_dc_low_lim_ret && state_get(RECT_DC_LW_FC)) {
	vrect_dc_low_lim_ret_Acc_cnt++;
	vrect_dc_low_lim_Acc_cnt=0;
	if (vrect_dc_low_lim_ret_Acc_cnt >= vrect_dc_low_lim_ret_Acc_per) {
		vrect_dc_low_lim_ret_Acc_cnt=0;
		apply_state_changes_f(RECT_DC_LW_FC, 0);
		PRF_GEN("RECT DC Low Return");
	}
} else {
	vrect_dc_low_lim_ret_Acc_cnt=0;
}
////// V RECT DC HIGH/LOW MON ///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// AC VOLTAGE MONITORING >>>>>>>>>>>>>>>>>>>
// RRRRRRRRRRRRRRRRRRR
if (VAC_R_rms_sc.a1 < VAC_0_LIM && VAC_R_Off_fc==0) {
	VAC_R_0_Acc_cnt++; // S 0
	if (VAC_R_0_Acc_cnt >= VAC_R_0_Acc_per) {
		VAC_R_0_Acc_cnt=0;
		VAC_R_Off_fc=1;
	}
} else {VAC_R_0_Acc_cnt=0;}

if (VAC_R_rms_sc.a1 > VAC_Hg_Lim && VAC_R_Hg_fc==0) {
	VAC_R_Hg_Acc_cnt++; // S HIGH
	if (VAC_R_Hg_Acc_cnt >= VAC_R_Hg_Acc_per) {
		VAC_R_Hg_Acc_cnt=0;
		VAC_R_Hg_fc=1;
	}
} else {VAC_R_Hg_Acc_cnt=0;}

if (VAC_R_rms_sc.a1 < VAC_Lo_Lim && VAC_R_Lo_fc==0 && VAC_R_Off_fc==0) {
	VAC_R_Lo_Acc_cnt++; // S LOW
	if (VAC_R_Lo_Acc_cnt >= VAC_R_Lo_Acc_per) {
		VAC_R_Lo_Acc_cnt=0;
		VAC_R_Lo_fc=1;
	}
} else {VAC_R_Lo_Acc_cnt=0;}

if (VAC_R_rms_sc.a1 >= VAC_0_RET_LIM && VAC_R_Off_fc == 1) {
	VAC_R_0_Ret_Acc_cnt++; // S 0 RET
	if (VAC_R_0_Ret_Acc_cnt >= VAC_R_0_Ret_Acc_per) {
		VAC_R_0_Ret_Acc_cnt=0;
		VAC_R_Off_fc=0;
	}
} else {VAC_R_0_Ret_Acc_cnt=0;}

if (VAC_R_rms_sc.a1 <= VAC_HG_RET_LIM && VAC_R_Hg_fc == 1) {
	VAC_R_Hg_Ret_Acc_cnt++; // S HIGH RET
	if (VAC_R_Hg_Ret_Acc_cnt >= VAC_R_Hg_Ret_Acc_per) {
		VAC_R_Hg_Ret_Acc_cnt=0;
		VAC_R_Hg_fc=0;
	}
} else {VAC_R_Hg_Ret_Acc_cnt=0;}

if (VAC_R_rms_sc.a1 >= VAC_LW_RET_LIM && VAC_R_Lo_fc == 1) {
	VAC_R_Lo_Ret_Acc_cnt++; // S LOW RET
	if (VAC_R_Lo_Ret_Acc_cnt >= VAC_R_Lo_Ret_Acc_per) {
		VAC_R_Lo_Ret_Acc_cnt=0;
		VAC_R_Lo_fc=0;
	}
} else {VAC_R_Lo_Ret_Acc_cnt=0;}
// RRRRRRRRRRRRRRRRRRR
// SSSSSSSSSSSSSSSSSSS
if (VAC_S_rms_sc.a1 < VAC_0_LIM && VAC_S_Off_fc==0) {
	VAC_S_0_Acc_cnt++; // S 0
	if (VAC_S_0_Acc_cnt >= VAC_S_0_Acc_per) {
		VAC_S_0_Acc_cnt=0;
		VAC_S_Off_fc=1;
	}
} else {VAC_S_0_Acc_cnt=0;}

if (VAC_S_rms_sc.a1 > VAC_Hg_Lim && VAC_S_Hg_fc==0) {
	VAC_S_Hg_Acc_cnt++; // S HIGH
	if (VAC_S_Hg_Acc_cnt >= VAC_S_Hg_Acc_per) {
		VAC_S_Hg_Acc_cnt=0;
		VAC_S_Hg_fc=1;
	}
} else {VAC_S_Hg_Acc_cnt=0;}

if (VAC_S_rms_sc.a1 < VAC_Lo_Lim && VAC_S_Lo_fc==0 && VAC_S_Off_fc==0) {
	VAC_S_Lo_Acc_cnt++; // S LOW
	if (VAC_S_Lo_Acc_cnt >= VAC_S_Lo_Acc_per) {
		VAC_S_Lo_Acc_cnt=0;
		VAC_S_Lo_fc=1;
	}
} else {VAC_S_Lo_Acc_cnt=0;}

if (VAC_S_rms_sc.a1 >= VAC_0_RET_LIM && VAC_S_Off_fc == 1) {
	VAC_S_0_Ret_Acc_cnt++; // S 0 RET
	if (VAC_S_0_Ret_Acc_cnt >= VAC_S_0_Ret_Acc_per) {
		VAC_S_0_Ret_Acc_cnt=0;
		VAC_S_Off_fc=0;
	}
} else {VAC_S_0_Ret_Acc_cnt=0;}

if (VAC_S_rms_sc.a1 <= VAC_HG_RET_LIM && VAC_S_Hg_fc == 1) {
	VAC_S_Hg_Ret_Acc_cnt++; // S HIGH RET
	if (VAC_S_Hg_Ret_Acc_cnt >= VAC_S_Hg_Ret_Acc_per) {
		VAC_S_Hg_Ret_Acc_cnt=0;
		VAC_S_Hg_fc=0;
	}
} else {VAC_S_Hg_Ret_Acc_cnt=0;}

if (VAC_S_rms_sc.a1 >= VAC_LW_RET_LIM && VAC_S_Lo_fc == 1) {
	VAC_S_Lo_Ret_Acc_cnt++; // S LOW RET
	if (VAC_S_Lo_Ret_Acc_cnt >= VAC_S_Lo_Ret_Acc_per) {
		VAC_S_Lo_Ret_Acc_cnt=0;
		VAC_S_Lo_fc=0;
	}
} else {VAC_S_Lo_Ret_Acc_cnt=0;}
// SSSSSSSSSSSSSSSSSSS
// TTTTTTTTTTTTTTTTTTT
if (VAC_T_rms_sc.a1 < VAC_0_LIM && VAC_T_Off_fc==0) {
	VAC_T_0_Acc_cnt++; // S 0
	if (VAC_T_0_Acc_cnt >= VAC_T_0_Acc_per) {
		VAC_T_0_Acc_cnt=0;
		VAC_T_Off_fc=1;
	}
} else {VAC_T_0_Acc_cnt=0;}

if (VAC_T_rms_sc.a1 > VAC_Hg_Lim && VAC_T_Hg_fc==0) {
	VAC_T_Hg_Acc_cnt++; // S HIGH
	if (VAC_T_Hg_Acc_cnt >= VAC_T_Hg_Acc_per) {
		VAC_T_Hg_Acc_cnt=0;
		VAC_T_Hg_fc=1;
	}
} else {VAC_T_Hg_Acc_cnt=0;}

if (VAC_T_rms_sc.a1 < VAC_Lo_Lim && VAC_T_Lo_fc==0 && VAC_T_Off_fc==0) {
	VAC_T_Lo_Acc_cnt++; // S LOW
	if (VAC_T_Lo_Acc_cnt >= VAC_T_Lo_Acc_per) {
		VAC_T_Lo_Acc_cnt=0;
		VAC_T_Lo_fc=1;
	}
} else {VAC_T_Lo_Acc_cnt=0;}

if (VAC_T_rms_sc.a1 >= VAC_0_RET_LIM && VAC_T_Off_fc == 1) {
	VAC_T_0_Ret_Acc_cnt++; // S 0 RET
	if (VAC_T_0_Ret_Acc_cnt >= VAC_T_0_Ret_Acc_per) {
		VAC_T_0_Ret_Acc_cnt=0;
		VAC_T_Off_fc=0;
	}
} else {VAC_T_0_Ret_Acc_cnt=0;}

if (VAC_T_rms_sc.a1 <= VAC_HG_RET_LIM && VAC_T_Hg_fc == 1) {
	VAC_T_Hg_Ret_Acc_cnt++; // S HIGH RET
	if (VAC_T_Hg_Ret_Acc_cnt >= VAC_T_Hg_Ret_Acc_per) {
		VAC_T_Hg_Ret_Acc_cnt=0;
		VAC_T_Hg_fc=0;
	}
} else {VAC_T_Hg_Ret_Acc_cnt=0;}

if (VAC_T_rms_sc.a1 >= VAC_LW_RET_LIM && VAC_T_Lo_fc == 1) {
	VAC_T_Lo_Ret_Acc_cnt++; // S LOW RET
	if (VAC_T_Lo_Ret_Acc_cnt >= VAC_T_Lo_Ret_Acc_per) {
		VAC_T_Lo_Ret_Acc_cnt=0;
		VAC_T_Lo_fc=0;
	}
} else {VAC_T_Lo_Ret_Acc_cnt=0;}
// TTTTTTTTTTTTTTTTTTT
// RST RST RST RST RST RST RST
if ((VAC_R_Off_fc == 0 && VAC_S_Off_fc == 0 && VAC_T_Off_fc == 0) && !state_get(VAC_ON_FC)) {
	apply_state_changes_f(VAC_ON_FC, 1);
	PRF_GEN("VAC_ON_FC 1");
}
if ((VAC_R_Off_fc == 1 && VAC_S_Off_fc == 1 && VAC_T_Off_fc == 1) && state_get(VAC_ON_FC)) {
	apply_state_changes_f(VAC_ON_FC, 0);
	PRF_GEN("VAC_ON_FC 0");
}
if ((VAC_R_Off_fc == 1 || VAC_S_Off_fc == 1 || VAC_T_Off_fc == 1) && !state_get(VAC_OFF_FC)) {
	apply_state_changes_f(VAC_OFF_FC, 1);
	PRF_GEN("VAC_OFF_FC 1");
}
if ((VAC_R_Off_fc == 0 && VAC_S_Off_fc == 0 && VAC_T_Off_fc == 0) && state_get(VAC_OFF_FC)) {
	apply_state_changes_f(VAC_OFF_FC, 0);
	PRF_GEN("VAC_OFF_FC 0");
}
if ((VAC_R_Hg_fc == 1 || VAC_S_Hg_fc == 1 || VAC_T_Hg_fc == 1) && !state_get(VAC_HG_FC)) {
	apply_state_changes_f(VAC_HG_FC, 1);	// VAC Hg
	PRF_GEN("VAC HG");
}
if ((VAC_R_Hg_fc == 0 && VAC_S_Hg_fc == 0 && VAC_T_Hg_fc == 0) && state_get(VAC_HG_FC)) {
	apply_state_changes_f(VAC_HG_FC, 0);	// VAC Hg RET
	PRF_GEN("VAC HG RETURNED");
}
if ((VAC_R_Lo_fc == 1 || VAC_S_Lo_fc == 1 || VAC_T_Lo_fc == 1) && !state_get(VAC_LO_FC)) {
	apply_state_changes_f(VAC_LO_FC, 1);	// VAC Lo
	apply_state_changes_f(VAC_ON_FC, 0);
	PRF_GEN("VAC LO");
}
if ((VAC_R_Lo_fc == 0 && VAC_S_Lo_fc == 0 && VAC_T_Lo_fc == 0) && state_get(VAC_LO_FC)) {
	apply_state_changes_f(VAC_LO_FC, 0);	// VAC Lo RET
	apply_state_changes_f(VAC_ON_FC, 1);
	PRF_GEN("VAC LO RETURNED");
}
// RST RST RST RST RST RST RST
// AC VOLTAGE MONITORING <<<<<<<<<<<<<<<<<<<<
// CURRENT LIMIT STATES >>>>>>>>>>>>>>>>>>>
	if (pid_output_i_rect < 0 && !state_get(RECTIFIER_CURRENT_LIMIT_FC)) {
		rectifier_current_limit_Acc_cnt++;
		rectifier_current_limit_return_Acc_cnt=0;
		if (rectifier_current_limit_Acc_cnt >= rectifier_current_limit_Acc_per) {
			apply_state_changes_f(RECTIFIER_CURRENT_LIMIT_FC, 1);
			rectifier_current_limit_Acc_cnt=0;
			rectifier_current_limit_accepted=1;
		}
	} else if (pid_output_i_rect >= 0 && state_get(RECTIFIER_CURRENT_LIMIT_FC)) {
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
	if (pid_output_i_batt < 0 && !state_get(BATTERY_CURRENT_LIMIT_FC)) {
		battery_current_limit_Acc_cnt++;
		battery_current_limit_return_Acc_cnt=0;
		if (battery_current_limit_Acc_cnt >= battery_current_limit_Acc_per) {
			apply_state_changes_f(BATTERY_CURRENT_LIMIT_FC, 1);
			battery_current_limit_Acc_cnt=0;
			battery_current_limit_accepted=1;
		}
	} else if(pid_output_i_batt >= 0 && state_get(BATTERY_CURRENT_LIMIT_FC)) {
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
// CURRENT LIMIT STATES <<<<<<<<<<<<<<<<<<<<<<

}	// if (sta_op_phase==S_STARTUP_DELAY_OK) {

// FREQUENCY (50ms loop)
if (line_sgn_stable) {
	frq_rst_updn_avg_m=frq_cal_k / per_rst_updn_avg_m;
	exti_int_endely_per=(1/frq_rst_updn_avg_m)/40e-6*0.90;
}

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
//		PRF_GEN("t cnt per %lu %lu", ovtmp_open_cnt, ovtmp_open_per);

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
		if (tmp_dat_C[0]+temp_test_thy_1 > EpD[SET_OVERTEMP_ALARM][0].V1 && !state_get(OVERTEMP_ALARM_FC)) { // overtemp alarm enable
			apply_state_changes_f(OVERTEMP_ALARM_FC, 1);
		} else if (tmp_dat_C[0]+temp_test_thy_1 < EpD[SET_OVERTEMP_ALARM][0].V1-5 && state_get(OVERTEMP_ALARM_FC)) { // overtemp alarm disable
			apply_state_changes_f(OVERTEMP_ALARM_FC, 0);
		}

		if (tmp_dat_C[0]+temp_test_thy_1 > EpD[SET_OVERTEMP_OPEN][0].V1 && !state_get(OVERTEMP_OPEN_FC)) { // overtemp open count
			ovtmp_open_cnt++;
		} else if (tmp_dat_C[0]+temp_test_thy_1 < EpD[SET_OVERTEMP_OPEN][0].V1-5 && state_get(OVERTEMP_OPEN_FC)) { // overtemp open cancel
			apply_state_changes_f(OVERTEMP_OPEN_FC, 0);
			PRF_GEN("apply_state_changes_f(OVERTEMP_OPEN_FC, 0);");
			ovtmp_open_cnt=0;
		} else if (ovtmp_open_cnt >= ovtmp_open_per && !state_get(OVERTEMP_OPEN_FC)) { // overtemp alarm enable
			ovtmp_open_cnt=0;
			apply_state_changes_f(OVERTEMP_OPEN_FC, 1);
			PRF_GEN("apply_state_changes_f(OVERTEMP_OPEN_FC, 1);");
		} else {
			ovtmp_open_cnt=0;
		}

		if (tmp_dat_C[0]+temp_test_thy_1 > EpD[SET_COOL_FAN_TEMP][0].V1 && !state_get(THY_FAN1_REL)) { // fan enable
			apply_state_changes_f(THY_FAN1_REL, 1);
		} else if (tmp_dat_C[0]+temp_test_thy_1 < EpD[SET_COOL_FAN_TEMP][0].V1-10 && state_get(THY_FAN1_REL)) { // fan disable
			apply_state_changes_f(THY_FAN1_REL, 0);
		}

		if (tmp_dat_C[1]+temp_test_trf_2 > EpD[SET_TRANSF_FAN_TEMP][0].V1 && !state_get(TRF_FAN2_REL)) { // fan enable
			apply_state_changes_f(TRF_FAN2_REL, 1);
		} else if (tmp_dat_C[0]+temp_test_trf_2 < EpD[SET_TRANSF_FAN_TEMP][0].V1-10 && state_get(TRF_FAN2_REL)) { // fan disable
			apply_state_changes_f(TRF_FAN2_REL, 0);
		}
	}

//// AKÜ ////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (batt_sensor_exists) {
		if (tmp_dat_C[2]+temp_test_trf_2 <= 0 && !state_get(BAT_TEMP_ZERO_FC)) { // BATT TEMP ZERO disable
			apply_state_changes_f(BAT_TEMP_ZERO_FC, 1);
		} else if (tmp_dat_C[2]+temp_test_trf_2 >= 50 && !state_get(BAT_TEMP_50_FC)) { // BATT TEMP 50 disable
			apply_state_changes_f(BAT_TEMP_50_FC, 1);
		} else if (tmp_dat_C[2]+temp_test_trf_2 > 5 && state_get(BAT_TEMP_ZERO_FC)) { // BATT TEMP ZERO enable
			apply_state_changes_f(BAT_TEMP_ZERO_FC, 0);
		} else if (tmp_dat_C[2]+temp_test_trf_2 < 45 && state_get(BAT_TEMP_50_FC)) { // BATT TEMP 50 enable
			apply_state_changes_f(BAT_TEMP_50_FC, 0);
		}
	}

////// TEMP READ ////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// BATT LINE MONITORING /////////////////////////////////////////////////////////////////////////////////////////
if (fabs(IBAT_pas.a16) >= blm_I_step_075perc && !batt_current_detected) {
	batt_curr_detected_cnt++;
	batt_curr_not_detected_cnt=0;
	if (batt_curr_detected_cnt >= batt_current_detected_per) {
		batt_curr_detected_cnt=0;
		batt_current_detected=1;														// CURRENT DETECTED
	}
} else if (fabs(IBAT_pas.a16) < blm_I_step_075perc && batt_current_detected) {
	batt_curr_not_detected_cnt++;
	batt_curr_detected_cnt=0;
	if (batt_curr_not_detected_cnt >= batt_current_not_detected_per) {
		batt_curr_not_detected_cnt=0;
		batt_current_detected=0;														// CURRENT NOT DETECTED
	}
} else {
	batt_curr_detected_cnt=0;
	batt_curr_not_detected_cnt=0;
}

if (sfsta_op_phase == S_SFSTA_REQ_OK) {
// soft start tamamlanmış. tristör devreden çıkaran yerler bu değişkeni de değiştiriyor.
// böylece tristörler kapatıldığında blm ye girilmiyor.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	stability_vrect_fc();	// vrect_stable 1 0 yapıyor
	stability_irect_fc();	// irect_stable 1 0 yapıyor
	stability_ibat_fc();	// ibat_stable ve batt_current_detected 1 0 yapıyor.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// WHAT STOPS AND RESETS BATT LINE MONITORING
	if (state_get(ST_BATT_MCCB_OFF) && !state_get(ST_BATT_LINE_BROKEN) && EpD[SET_BATT_DISC_CHK_EN][0].V1==1 && blm_pause==0) {
		apply_state_changes_f(ST_BATT_LINE_BROKEN, 1);										// BATT SWITCH OFF
			blm_op_phase = B_RESTRT_AFTR_DELAY;									// bring_vtarg_back_goto_delay
			fast_restart_blm_after_bat_switch_on = 1;
			blm_corr_op_start_delay_cnt = 0;
			blm_enable_collect_samples = 0;
			blm_corr_buf_index = 0;
		PRF_BLM("blm SW off. batt broken set");
	}
	if (!state_get(ST_BATT_MCCB_OFF) && !state_get(ST_BATT_LINE_BROKEN) && EpD[SET_BATT_DISC_CHK_EN][0].V1==1 && blm_pause==0 && fast_restart_blm_after_bat_switch_on) {
			blm_op_phase = B_RESTRT_AFTR_DELAY;
			fast_restart_blm_after_bat_switch_on = 0;
			blm_corr_op_start_delay_cnt = blm_corr_op_start_delay_per-10;
			blm_corr_op_start_delay_cnt = 0;										// BATT SWITCH ON
			blm_enable_collect_samples = 0;
			blm_corr_buf_index = 0;
		PRF_BLM("blm SW on. start inspection now");
	}
	if (VBAT_pas.a16 <= Vbat_flt && !batt_current_detected && !state_get(ST_BATT_LINE_BROKEN) && EpD[SET_BATT_DISC_CHK_EN][0].V1==1 && blm_pause==0) {
		apply_state_changes_f(ST_BATT_LINE_BROKEN, 1);										// VBAT LOW
			blm_op_phase = B_RESTRT_AFTR_DELAY;									// bring_vtarg_back_goto_delay
			blm_corr_op_start_delay_cnt = 0;
			blm_enable_collect_samples = 0;
			blm_corr_buf_index = 0;
			PRF_BLM("blm Vbat too low. batt broken set");
	}
	if (batt_current_detected && state_get(ST_BATT_LINE_BROKEN)) {
		apply_state_changes_f(ST_BATT_LINE_BROKEN, 0);									// CURRENT DETECTED
			blm_op_phase = B_RESTRT_AFTR_DELAY;									// bring_vtarg_back_goto_delay
			blm_corr_op_start_delay_cnt = 0;
			blm_enable_collect_samples = 0;
			blm_corr_buf_index = 0;
			PRF_BLM("current detected. batt line connected");
	}
	if ((EpD[SET_BATT_DISC_CHK_EN][0].V1==0 || blm_pause==1) && state_get(ST_BATT_LINE_BROKEN)) {	// batt line broken fault durumu var. kullanıcı batt kontrlü devreden çıkarıyor.
		apply_state_changes_f(ST_BATT_LINE_BROKEN, 0);
		blm_op_phase = B_RESTRT_AFTR_DELAY;										// bring_vtarg_back_goto_delay
		blm_corr_op_start_delay_cnt = 0;
		blm_enable_collect_samples = 0;
		blm_corr_buf_index = 0;
		PRF_BLM("user disabled batt mon");
	}
	if (!irect_stable && blm_op_phase != B_SKIP_DELAY_RESTART) {	// rectifier akımındaki oynama bat akımında oynamaya neden olup operasyonu bozabiliyor.
			blm_op_phase = B_SKIP_DELAY_RESTART;					// irect stable değilse incelemeyi iptal edip baştan başla.
			blm_enable_collect_samples = 0;
			blm_corr_buf_index = 0;
//			PRF_BLM("blm !irect_stable");
	}
	if (!ibat_stable && blm_op_phase != B_SKIP_DELAY_RESTART) {		// ibat akımının stabil olması. bataryanın iç yapısının stabil olduğu anlamına geliyor. yüklemeden yeni çıktığında iç yapısı stabil olmuyor ve voltaj ile akım corr olmuyor.
			blm_op_phase = B_SKIP_DELAY_RESTART;
			blm_enable_collect_samples = 0;
			PRF_BLM("blm !ibat_stable");
	}
	if (!user_wants_allows_thy_drv) {
			blm_op_phase = B_RESTRT_AFTR_DELAY;
			blm_enable_collect_samples = 0;
			PRF_BLM("blm delay restart load dc low");
	}
//	if (state_get(LOAD_DC_LW_FC)) {
//			blm_op_phase = B_RESTRT_AFTR_DELAY;
//			blm_enable_collect_samples = 0;
//			PRF_BLM("blm delay restart load dc low");
//	}
//	if (state_get(RECT_DC_LW_FC)) {
//			blm_op_phase = B_RESTRT_AFTR_DELAY;
//			blm_enable_collect_samples = 0;
//			PRF_BLM("blm delay restart rect dc low");
//	}

/// WHAT STOPS AND RESETS BATT LINE MONITORING  && state_get(ST_BATT_LINE_BROKEN)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// blm başlatılmasını engelleyen durumlar yoksa devam et.
// Switch off ise inspection yapmanın anlamı yok. zaten kopuk.
// bat voltajı çok düşükse zaten inspection a gerek yok direk bat bağlı değil denebilir.
// bat akımı varsa zaten bat bağlı demek oluyor, inspection a gerek yok.
// irect stable a1 yani bir örneklik periyot ortalaması
	if (blm_op_phase == B_SKIP_DELAY_RESTART) {
		bring_vtarg_back_to_chrgV(0);
	}
	if (blm_op_phase == B_RESTRT_AFTR_DELAY) {
		bring_vtarg_back_to_chrgV(9);	// iptal edilen optan sonra buraya geliniyor.
	}
	if (!state_get(ST_BATT_MCCB_OFF) && VBAT_pas.a16 > Vbat_flt && !batt_current_detected && blm_op_phase==0) {
		blm_op_phase=1;
	}
	if (blm_op_phase == 1 && EpD[SET_BATT_DISC_CHK_EN][0].V1==1 && blm_pause==0 && vrect_stable) {		// BATT MONITORING ENABLED OR DISABLED
		PRF_BLM("vrect stable. ");
		blm_op_phase=2;
	} else if (blm_op_phase == 2) { // Başlatma. vrect stable değilse başlama. sakin durumda iken yap.
		blm_corr_buf_index = 0;
		blm_enable_collect_samples = 1;
		check_vrect_vtarg_e_asagi_gitti =1;									// will check in phase 5
		check_vrect_vtarg_e_yukari_gitti =1;								// will check in phase 7
		blm_move_up_down_vtarg_limits();
		PRF_BLM("start changing voltage");
		blm_op_phase = 3;
	} else if (blm_op_phase == 3) { // Vtarg’ı düşür
		if (targ_DC_voltage > blm_vtarg_move_dn_targ) {
			set_targ_DC_voltage(targ_DC_voltage / (1 + blm_vi_change_mult));
		} else {
			blm_phase_switch_delay_cnt = 0;
			blm_op_phase = 4;
		}
	} else if (blm_op_phase == 4) { // Bekle dn
		blm_phase_switch_delay_cnt++;
		if (blm_phase_switch_delay_cnt >= blm_phase_switch_delay_dn_per) {
			blm_op_phase = 5;
		}
	} else if (blm_op_phase == 5) {
		if (check_vrect_vtarg_e_asagi_gitti) {
			check_vrect_vtarg_e_asagi_gitti=0;
			hedef_suan_fark=fabs(targ_DC_voltage-VRECT_pas.a16); // çıkarılmak istenen değer ile şu anki gerçek değer arası fark.
			hedef_baslng_fark=fabs(targ_DC_voltage-baslangic_v_stbl); // çıkarılmak istenen değer ile başlangıç arası fark.
			hedefe_yaklasma_yuzde_dn=hedef_suan_fark/hedef_baslng_fark;
			hedef_baslngc_fark=fabs(baslangic_v_stbl-targ_DC_voltage); // başlangıç volt ile hedef volt farkı
			PRF_BLM("asag hdf-suan hdf-basl hdf_yakl_yuzde. %f %f %f", hedef_suan_fark, hedef_baslngc_fark, hedefe_yaklasma_yuzde_dn); // hedef voltaj ile stabil voltaj arası fark
		}
		if (targ_DC_voltage < blm_vtarg_move_up_targ) { // Vtarg’ı artır
			set_targ_DC_voltage(targ_DC_voltage * (1 + blm_vi_change_mult));
		} else {
			blm_phase_switch_delay_cnt = 0;
			blm_op_phase = 6;
		}
	} else if (blm_op_phase == 6) { // Bekle up
		blm_phase_switch_delay_cnt++;
		if (blm_phase_switch_delay_cnt >= blm_phase_switch_delay_up_per) {
			blm_op_phase = 7;
		}
	} else if (blm_op_phase == 7) {
		if (check_vrect_vtarg_e_yukari_gitti) {
			check_vrect_vtarg_e_yukari_gitti=0;
			hedef_suan_fark=fabs(targ_DC_voltage-VRECT_pas.a16); // indirilmek istenen değer ile şu anki gerçek değer arası fark.
			hedef_baslng_fark=fabs(targ_DC_voltage-baslangic_v_stbl); // indirilmek istenen değer ile başlangıç arası fark.
			hedefe_yaklasma_yuzde_up=hedef_suan_fark/hedef_baslngc_fark;
			PRF_BLM("yukr hdf-suan hdf-basl hdf_yakl_yuzde. %f %f %f", hedef_suan_fark, hedef_baslngc_fark, hedefe_yaklasma_yuzde_up); // hedef voltaj ile stabil voltaj arası fark
		}
		if (targ_DC_voltage > temp_targ_DC_voltage) {
			set_targ_DC_voltage(targ_DC_voltage * (1 - blm_vi_change_mult));
		} else {
			blm_phase_switch_delay_cnt = 0;
			blm_op_phase = 71;
		}
	} else if (blm_op_phase == 71) {
		bring_vtarg_back_to_chrgV(8);
	} else if (blm_op_phase == 8) { // Bekle
		blm_phase_switch_delay_cnt++;
		if (blm_phase_switch_delay_cnt >= blm_phase_switch_delay_bck_per) {
			blm_enable_collect_samples = 0;
			blm_corr = calculate_blm_op();	// şimdiki corr

			PRF_BLM("  blm_corr %f", blm_corr);
			if (discard_corr_result == 0) {
				if (blm_corr >= 0.90) {	// bir tanesi 0.9 üstü ise corr ok.
					blm_req_corr_batt_connected=1;
					PRF_BLM("corr good. 0.90 batt connected.");
				} else if (blm_corr >= 0.85 && blm_corr_p >= 0.85) { // son ikisi 0.85 üstü ise corr ok
					blm_req_corr_batt_connected=1;
					PRF_BLM("corr good. 2x0.85 batt connected.");
				} else if (blm_corr < 0.6 && blm_corr_p < 0.6 && !state_get(ST_BATT_LINE_BROKEN) && hedefe_yaklasma_yuzde_dn <= 0.4) { // iki kez üst üste 0.6 altı olmuşsa corr yok.
					blm_req_corr_batt_connected=0; 												// hedefe_yaklasma_yuzde_dn, ne kadar düşükse o kadar vrect vtarg ı takip etmiş
					PRF_BLM("corr low. batt broken.");
				} else if (blm_corr < 0.25 && !state_get(ST_BATT_LINE_BROKEN) && hedefe_yaklasma_yuzde_dn <= 0.4) { // 0.25 altı olmuşsa corr yok.
					blm_req_corr_batt_connected=0;
					PRF_BLM("corr low. batt broken.");
				} else  {
					PRF_BLM("corr results requires no action this time.");
				}
				blm_corr_p = blm_corr;			// bir önceki corr
			} else {
		    	discard_corr_result=0;
				PRF_BLM("discard_corr_result %f", blm_corr);
			}
			blm_corr_results[blm_corr_results_index]=blm_corr;	// corr ları kaydet. bir yerde kullanılmıyor.
			blm_corr_results_index=(blm_corr_results_index+1) % BLM_CORR_RESULTS_SIZE;
			blm_corr_op_start_delay_cnt = 0;
			blm_op_phase = 9;
		}
	} else if (blm_op_phase == 9) {
		blm_corr_op_start_delay_cnt++;
		if (blm_corr_op_start_delay_cnt >= blm_corr_op_start_delay_per) {
			blm_op_phase = 0;
			blm_corr_op_start_delay_cnt = 0;
			discard_corr_result = 0;
		}
	}
	if (blm_enable_collect_samples && blm_corr_buf_index < CORR_BUF_SIZE) {
		vrect_buf[blm_corr_buf_index] = VRECT_pas.a16;
		ibat_buf[blm_corr_buf_index] = IBAT_pas.a16;
		vtarg_buf[blm_corr_buf_index] = targ_DC_voltage;
		blm_corr_buf_index++;
	}
	if (blm_req_corr_batt_connected == 1) { // corr sonucuna göre batt line broken state uygulaması buraya koyuldu, çünkü başka durumlar da izlenerek sonuç çıkarılmaya çalışılacak. örneğin vtarg düşürüldü ama vrect düşmedi.
		blm_req_corr_batt_connected=99;
		apply_state_changes_f(ST_BATT_LINE_BROKEN, 0);
	} else if (blm_req_corr_batt_connected == 0) {
		blm_req_corr_batt_connected=99;
		apply_state_changes_f(ST_BATT_LINE_BROKEN, 1);
	}
} // if (sfsta_op_phase == S_SFSTA_REQ_OK) {
//baslangic_v_stbl						|
//											|
//VRECT_pas.a16		  |		hedef_baslngc_fark |
//	hedef_suan_fark  |						|
//targ_DC_voltage		  |						|
////// BATT LINE MONITORING /////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////


// LED_7_Data SET CLEAR
LED_7_Data |= (state_get(ST_LOAD_MCCB_OFF) << (ST_LOAD_MCCB_OFF-16));
LED_7_Data |= (state_get(ST_DROPPER_K1) << (ST_DROPPER_K1-16));
LED_7_Data |= (state_get(ST_DROPPER_K2) << (ST_DROPPER_K2-16));
LED_7_Data |= (state_get(ST_BATT_MCCB_OFF) << (ST_BATT_MCCB_OFF-16));
LED_7_Data |= (state_get(BOOST_CHARGE_FC) << (BOOST_CHARGE_FC-16));
LED_7_Data |= (state_get(ST_FLOAT_CHARGE) << (ST_FLOAT_CHARGE-16));
LED_7_Data |= (state_get(ST_LINE_MCCB_OFF) << (ST_LINE_MCCB_OFF-16));

LED_7_Data &= ~(!state_get(ST_LOAD_MCCB_OFF) << (ST_LOAD_MCCB_OFF-16));
LED_7_Data &= ~(!state_get(ST_DROPPER_K1) << (ST_DROPPER_K1-16));
LED_7_Data &= ~(!state_get(ST_DROPPER_K2) << (ST_DROPPER_K2-16));
LED_7_Data &= ~(!state_get(ST_BATT_MCCB_OFF) << (ST_BATT_MCCB_OFF-16));
LED_7_Data &= ~(!state_get(BOOST_CHARGE_FC) << (BOOST_CHARGE_FC-16));
LED_7_Data &= ~(!state_get(ST_FLOAT_CHARGE) << (ST_FLOAT_CHARGE-16));
LED_7_Data &= ~(!state_get(ST_LINE_MCCB_OFF) << (ST_LINE_MCCB_OFF-16));
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LED_16_Data |= (state_get(GENERAL_FAULT_FC) << (GENERAL_FAULT_FC - 0));
LED_16_Data |= (state_get(BATTERY_FAULT_FC) << (BATTERY_FAULT_FC - 0));
LED_16_Data |= (state_get(OVERTEMP_ALARM_FC) << (OVERTEMP_ALARM_FC - 0));
LED_16_Data |= (state_get(OVERTEMP_OPEN_FC) << (OVERTEMP_OPEN_FC - 0));
LED_16_Data |= (state_get(BATTERY_CURRENT_LIMIT_FC) << (BATTERY_CURRENT_LIMIT_FC - 0));
LED_16_Data |= (state_get(RECTIFIER_CURRENT_LIMIT_FC) << (RECTIFIER_CURRENT_LIMIT_FC - 0));
LED_16_Data |= (state_get(DC_LEAK_NEG_FC) << (DC_LEAK_NEG_FC - 0));
LED_16_Data |= (state_get(DC_LEAK_POS_FC) << (DC_LEAK_POS_FC - 0));
LED_16_Data |= (state_get(RECT_DC_LW_FC) << (RECT_DC_LW_FC - 0));
LED_16_Data |= (state_get(RECT_DC_HG_FC) << (RECT_DC_HG_FC - 0));
LED_16_Data |= (state_get(VAC_LO_FC) << (VAC_LO_FC - 0));
LED_16_Data |= (state_get(VAC_HG_FC) << (VAC_HG_FC - 0));
LED_16_Data |= (state_get(STOP_FC) << (STOP_FC - 0));
LED_16_Data |= (state_get(START_FC) << (START_FC - 0));
LED_16_Data |= (state_get(VAC_OFF_FC) << (VAC_OFF_FC - 0));
LED_16_Data |= (state_get(VAC_ON_FC) << (VAC_ON_FC - 0));

LED_16_Data &= ~(!state_get(GENERAL_FAULT_FC) << (GENERAL_FAULT_FC-0));
LED_16_Data &= ~(!state_get(BATTERY_FAULT_FC) << (BATTERY_FAULT_FC-0));
LED_16_Data &= ~(!state_get(OVERTEMP_ALARM_FC) << (OVERTEMP_ALARM_FC-0));
LED_16_Data &= ~(!state_get(OVERTEMP_OPEN_FC) << (OVERTEMP_OPEN_FC-0));
LED_16_Data &= ~(!state_get(BATTERY_CURRENT_LIMIT_FC) << (BATTERY_CURRENT_LIMIT_FC-0));
LED_16_Data &= ~(!state_get(RECTIFIER_CURRENT_LIMIT_FC) << (RECTIFIER_CURRENT_LIMIT_FC-0));
LED_16_Data &= ~(!state_get(DC_LEAK_NEG_FC) << (DC_LEAK_NEG_FC-0));
LED_16_Data &= ~(!state_get(DC_LEAK_POS_FC) << (DC_LEAK_POS_FC-0));
LED_16_Data &= ~(!state_get(RECT_DC_LW_FC) << (RECT_DC_LW_FC-0));
LED_16_Data &= ~(!state_get(RECT_DC_HG_FC) << (RECT_DC_HG_FC-0));
LED_16_Data &= ~(!state_get(VAC_LO_FC) << (VAC_LO_FC-0));
LED_16_Data &= ~(!state_get(VAC_HG_FC) << (VAC_HG_FC-0));
LED_16_Data &= ~(!state_get(STOP_FC) << (STOP_FC-0));
LED_16_Data &= ~(!state_get(START_FC) << (START_FC-0));
LED_16_Data &= ~(!state_get(VAC_OFF_FC) << (VAC_OFF_FC-0));
LED_16_Data &= ~(!state_get(VAC_ON_FC) << (VAC_ON_FC-0));

//rel_out_16Bit_Data REL_OUT_TB[0].rel_out_tb_nm


for (int i = 0; i < sizeof(state_list) / sizeof(state_list[0]); i++) {
    if (state_list[i].rel_ord == 3) { // Check if the 4th column is 3
        if (state_list[i].action & (1 << 3)) { // Check if the 3rd bit of the 2nd column is 1
            rel_out_16Bit_Data |= (1 << 2); // Set the 3rd bit from the end of rel_out_16Bit_Data
        }
        else if (state_list[i].action & (1 << 3)) { // Check if the 3rd bit of the 2nd column is 1
            rel_out_16Bit_Data &= ~(1 << 2); // Set the 3rd bit from the end of rel_out_16Bit_Data
        }
    }
}


} // if (ms_tick_cnt-while_delay50_h >= 50) {



if (ms_tick_cnt-while_RTC_delay_h >= while_RTC_delay_per) {
	while_RTC_delay_h=ms_tick_cnt;
    Read_Register_0x00_to_0x06();
    rtc_timestamp_fnc();
}

if (ms_tick_cnt-while_LCD_delay_h >= while_LCD_delay_per) {
	while_LCD_delay_h=ms_tick_cnt;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// ESTIMATE AC INPUT CURRENT
	VAC_avg   = (VAC_R_rms_sc.a1 + VAC_S_rms_sc.a1 + VAC_T_rms_sc.a1) / 3.0f;   /* ort. hat-hat RMS  */
	P_DC    = VRECT_pas.a16*IRECT_pas.a16;                                                   /* cikis DC gerilimi */
	IAC=P_DC/VAC_avg;
	IAC_est =
	      0.00369463 * IAC * IAC * IAC
	    - 0.05588933 * IAC * IAC
	    + 1.19303552 * IAC
	    + 0.27978617;
// ESTIMATE AC INPUT CURRENT
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

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
        case FANS_TEMP_pg:
        	FANS_TEMP_pg_disp();
            break;
        case TEST_pg:
        	TEST_pg_disp();
            break;
        case RELAY_ORDER_pg:
        	RELAY_ORDER_pg_disp();
            break;
        case FAULT_CODES_RESET_pg:
        	FAULT_CODES_RESET_pg_disp();
            break;
        case RECTF_ACTIVE_AT_STARTUP_pg:
        	RECTF_ACTIVE_AT_STARTUP_pg_disp();
            break;
        case DEVICE_RESET_pg:
        	DEVICE_RESET_pg_disp();
            break;
        case CALIBRATION_1_pg:
        	CALIBRATION_1_pg_disp();
            break;
        case CALIBRATION_2_pg:
        	CALIBRATION_2_pg_disp();
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

//	PRF_GEN("%3.2f %3.2f %3.2f %3.2f %3.2f", VAC_R_rms_sc.a64, VAC_S_rms_sc.a64, VAC_T_rms_sc.a64, VRECT_pas.a64, IRECT_pas.a64);


//	print_active_states();

	if (dbg_disp==1) {
		dbg_disp=0;
		print_REL_OUT_Table();
	}
	if (dbg_disp==2) {
		dbg_disp=0;
		print_active_states();
	}
}

BUZZ_P(buzzer_override);

if (leds_rels_override && leds_rels_override_returned) {
	LED_16_Data_h=LED_16_Data;
	LED_7_Data_h=LED_7_Data;
	REL_24Bit_Data_h=REL_24Bit_Data;
	LED_16_Data=65535;
	LED_7_Data=255;
	REL_24Bit_Data=16777215;
	leds_rels_override_returned=0;
} else if (!leds_rels_override && !leds_rels_override_returned) {
	leds_rels_override_returned=1;
	LED_16_Data=LED_16_Data_h;
	LED_7_Data=LED_7_Data_h;
	REL_24Bit_Data=REL_24Bit_Data_h;
}

processShiftRegister_LED_16(LED_16_Data);
processShiftRegister_LED_7(LED_7_Data);
processShiftRegister_Relay_Board_16(REL_24Bit_Data);

if (req_reset_db==1) {
	req_reset_db=0;
	NVIC_SystemReset();
}

if (upFireFlag) { upFireFlag = 0;  bup_fnc();    }
if (dnFireFlag) { dnFireFlag = 0;  bdown_fnc();  }
if (lfFireFlag) { lfFireFlag = 0;  bleft_fnc();  }
if (rtFireFlag) { rtFireFlag = 0;  bright_fnc(); }
if (enFireFlag) { enFireFlag = 0;  benter_fnc(); }
if (esFireFlag) { esFireFlag = 0;  besc_fnc();   }

