


inline extern void TEST_pg_disp(void) {
    uint8_t x0=0; uint8_t y0=0; uint8_t w=0;
	char L[32]; lnhg=9;
    GLCD_PrintString(0, 0, "Test");

	sprintf(L, " %s %3.1f C", TEST_Items[0], tmp_dat_C[0]+temp_test_thy_1); 	GLCD_PrintString(0, 1*lnhg, L);
	sprintf(L, " %s %3.1f C", TEST_Items[1], tmp_dat_C[1]+temp_test_trf_2); 	GLCD_PrintString(0, 2*lnhg, L);
	sprintf(L, " %s", TEST_Items[2]); 	GLCD_PrintString(0, 3*lnhg, L);
	sprintf(L, " %s", TEST_Items[3]); 	GLCD_PrintString(0, 4*lnhg, L);

	GLCD_PrintString(0, (selected_TEST_PG_line+1)*lnhg, ">");

	static uint8_t testLnPic[6] = { 0, 1, 3};

	if (test_edit_mode) {
		if ((test_edit_blink^=1)==0 && selected_TEST_PG_line <= 1) {
			x0=48+(testLnPic[test_dig]*6); y0=(selected_TEST_PG_line+2)*lnhg-1; w=4;
		}
	}
	GLCD_Line(x0, y0, x0+w, y0);

	sprintf(L, "%s", AKTFPAS_SEL_Items[buzzer_override]); 	GLCD_PrintString(0+6*7, 3*lnhg, L);
	sprintf(L, "%s", AKTFPAS_SEL_Items[leds_rels_override]); 	GLCD_PrintString(0+6*7, 4*lnhg, L);
}

inline extern void FANS_TEMP_pg_disp(void) {
    uint8_t x0=0; uint8_t y0=0; uint8_t w=0;
	char L[32]; lnhg=9;
    GLCD_PrintString(0, 0, "Fanlar Sıcakl Koruma");

	sprintf(L, " %s  %5.0f C", FANS_TEMP_Items[0], EpD[SET_COOL_FAN_TEMP][fan_temp_edit_mode].V1);	GLCD_PrintString(0, 1*lnhg, L);
	sprintf(L, " %s    %5.0f C", FANS_TEMP_Items[1], EpD[SET_TRANSF_FAN_TEMP][fan_temp_edit_mode].V1);	GLCD_PrintString(0, 2*lnhg, L);
	sprintf(L, " %s%5.0f C", FANS_TEMP_Items[2], EpD[SET_OVERTEMP_ALARM][fan_temp_edit_mode].V1);	GLCD_PrintString(0, 3*lnhg, L);
	sprintf(L, " %s %5.0f C", FANS_TEMP_Items[3], EpD[SET_OVERTEMP_OPEN][fan_temp_edit_mode].V1);	GLCD_PrintString(0, 4*lnhg, L);
	sprintf(L, " %s   %03lu s", FANS_TEMP_Items[4], (uint32_t)EpD[SET_OVT_OPEN_DELAY][fan_temp_edit_mode].V1);	GLCD_PrintString(0, 5*lnhg, L);
	GLCD_PrintString(0, (selected_FAN_TEMP_PG_line+1)*lnhg, ">");

	if (fan_temp_edit_mode) {
		if ((fan_temp_edit_blink^=1)==0) {
			x0=102+(fan_temp_dig*6); y0=(selected_FAN_TEMP_PG_line+2)*lnhg-1; w=4;
		}
	}
	GLCD_Line(x0, y0, x0+w, y0);
}
inline extern void DROPPER_pg_disp(void) {
    uint8_t x0=0; uint8_t y0=0; uint8_t w=0;
    GLCD_PrintString(0, 0, "Dropper");
	char L[32]; lnhg=9;
	sprintf(L, " Kontrol   %s", MANUOTO_SEL_Items[(uint32_t)EpD[SET_DROPPER_MANOTO][dropper_edit_mode].V1]); 	GLCD_PrintString(0, 1*lnhg, L);
	sprintf(L, " Kademe 1 %s", DROPNORM_SEL_Items[(uint32_t)EpD[SET_DROPPER_K1][dropper_edit_mode].V1]); 		GLCD_PrintString(0, 2*lnhg, L);
	sprintf(L, " Kademe 2 %s", DROPNORM_SEL_Items[(uint32_t)EpD[SET_DROPPER_K2][dropper_edit_mode].V1]); 		GLCD_PrintString(0, 3*lnhg, L);

	if (!dropper_edit_mode) {
		sprintf(L, " Üst Lm %5.1fV %4.1f%%", set_dropper_l_hg_V, EpD[SET_DROPP_L_HG_PERC][dropper_edit_mode].V1); 	GLCD_PrintString(0, 4*lnhg, L);
		sprintf(L, " Alt Lm %5.1fV %4.1f%%", set_dropper_l_lw_V, EpD[SET_DROPP_L_LW_PERC][dropper_edit_mode].V1); 	GLCD_PrintString(0, 5*lnhg, L);
	} else {
		sprintf(L, " Üst Lm %5.1fV %4.1f%%", set_dropper_l_hg_V_h, EpD[SET_DROPP_L_HG_PERC][dropper_edit_mode].V1); 	GLCD_PrintString(0, 4*lnhg, L);
		sprintf(L, " Alt Lm %5.1fV %4.1f%%", set_dropper_l_lw_V_h, EpD[SET_DROPP_L_LW_PERC][dropper_edit_mode].V1); 	GLCD_PrintString(0, 5*lnhg, L);
	}

	GLCD_PrintString(0, (selected_DROPPER_PG_line+1)*lnhg, ">");
	if ((selected_DROPPER_PG_line >= 0 && selected_DROPPER_PG_line <= 2) && dropper_edit_mode) {
		GLCD_Rect_E(69,(selected_DROPPER_PG_line+1)*9-2,108,(selected_DROPPER_PG_line+2)*9-1); // edit rectangle
	}
	static uint8_t dropperLnPic[6] = { 0, 1, 3, 6, 7, 9};
	if (dropper_edit_mode) {
		if ((selected_DROPPER_PG_line==3 || selected_DROPPER_PG_line==4) && (dropper_edit_blink^=1)==0) {
			x0=54+(dropperLnPic[drop_set_dig]*6); y0=(selected_DROPPER_PG_line+2)*lnhg-1; w=4;
		}
	}
	GLCD_Line(x0, y0, x0+w, y0);
// DIODE ANIMATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static const char *dropperPic[2] = { "->-", "---" };   // 0 ise “->-”, 1 ise “---”
	GLCD_PrintString(110, 2 * lnhg, dropperPic[EpD[SET_DROPPER_K1][0].V1 == 0]); // EpD[SET_DROPPER_K1][0].V1==0 ise sonuç 1 oluyor ve dropperPic[1]="---" yazıyor
	GLCD_PrintString(110, 3 * lnhg, dropperPic[EpD[SET_DROPPER_K2][0].V1 == 0]);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

inline extern void CALIBRATION_2_pg_disp(void) {
	GLCD_PrintString(0, 0, "Kalibrasyon pg2");
	char L[32]; lnhg=11;
	sprintf(L, "frq %7.3f", frq_rst_updn_avg_m); 		GLCD_PrintString(0, 1*lnhg, L);
//	sprintf(L, "frqS%7.3f", frq_s_updn_avg_m); 		GLCD_PrintString(0, 2*lnhg, L);
//	sprintf(L, "frqT%7.3f", frq_t_updn_avg_m); 		GLCD_PrintString(0, 3*lnhg, L);

	uint8_t x0=0; uint8_t y0=0; uint8_t w=0; uint8_t h=10;

	if (cal_pg2_sel_edit_mode==cal_none) {
		if (cal_pg2_sel_col==0) {
			x0=26; y0=9+(cal_pg2_sel_item_left*11); w=41;
			GLCD_Rect_E(x0,y0,x0+w,y0+h);
		} else if (cal_pg2_sel_col==1) {
			x0=77; y0=9+(cal_pg2_sel_item_right*11); w=41;
			GLCD_Rect_E(x0,y0,x0+w,y0+h);
		}
	}

	if (cal_pg2_sel_col == 0) {
		if (cal_pg2_sel_item_left==0) {
		    if (cal_pg2_sel_edit_mode != cal_none) {y0=19;}
		    sprintf(L, "%9.8f", EpD[SET_FRQ_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
		}
		if (cal_pg2_sel_digit==0) {
		    if (cal_pg2_sel_edit_mode != cal_none) {x0=54; w=4;}

		}
		if (cal_pg2_sel_digit==1) {
		    if (cal_pg2_sel_edit_mode != cal_none) {x0=48; w=4;}
		}
	}
	GLCD_Line(x0, y0, x0+w, y0);

    if (cal_pg2_sel_edit_mode==cal_gain) {
    	GLCD_PrintString(96, 0, "GAIN");
    } else if (cal_pg2_sel_edit_mode==cal_offset) {
    	GLCD_PrintString(91, 0, "OFFST");
	}
}
inline extern void CALIBRATION_1_pg_disp(void) {
    GLCD_PrintString(0, 0, "Kalibrasyon pg1");
	char L[32]; lnhg=11;
    sprintf(L, "VR%7.2f", VRECT_pas.a64); 		GLCD_PrintString(0, 1*lnhg, L);
    sprintf(L, "VL%7.2f", VLOAD_pas.a64); 		GLCD_PrintString(0, 2*lnhg, L);
    sprintf(L, "VB%7.2f", VBAT_pas.a64); 		GLCD_PrintString(0, 3*lnhg, L);
    sprintf(L, "IR%7.2f", IRECT_pas.a64); 		GLCD_PrintString(0, 4*lnhg, L);
    sprintf(L, "IB%7.2f", IBAT_pas.a64); 		GLCD_PrintString(0, 5*lnhg, L);
    sprintf(L, "VR%7.2f", VAC_R_rms_sc.a64); 		GLCD_PrintString(64, 1*lnhg, L);
    sprintf(L, "VS%7.2f", VAC_S_rms_sc.a64); 		GLCD_PrintString(64, 2*lnhg, L);
    sprintf(L, "VT%7.2f", VAC_T_rms_sc.a64); 		GLCD_PrintString(64, 3*lnhg, L);

    uint8_t x0=0; uint8_t y0=0; uint8_t w=0; uint8_t h=10;


    if (cal_pg1_sel_edit_mode==cal_none) {
		if (cal_pg1_sel_col==0) {
			x0=13; y0=9+(cal_pg1_sel_item_left*11); w=41;
			GLCD_Rect_E(x0,y0,x0+w,y0+h);
		} else if (cal_pg1_sel_col==1) {
			x0=77; y0=9+(cal_pg1_sel_item_right*11); w=41;
			GLCD_Rect_E(x0,y0,x0+w,y0+h);
		}
    }

	if (cal_pg1_sel_col == 0) {
		if (cal_pg1_sel_item_left==0) {
		    if (cal_pg1_sel_edit_mode != cal_none) {y0=19;}
		    sprintf(L, "%9.8f", EpD[SET_VRECT_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_VRECT_OFFS_CAL][0].V1); 	GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_pg1_sel_item_left==1) {
		    if (cal_pg1_sel_edit_mode != cal_none) {y0=30;}
			sprintf(L, "%9.8f", EpD[SET_VLOAD_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_VLOAD_OFFS_CAL][0].V1); 	GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_pg1_sel_item_left==2) {
		    if (cal_pg1_sel_edit_mode != cal_none) {y0=41;}
			sprintf(L, "%9.8f", EpD[SET_VBAT_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_VBAT_OFFS_CAL][0].V1); 		GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_pg1_sel_item_left==3) {
		    if (cal_pg1_sel_edit_mode != cal_none) {y0=52;}
			sprintf(L, "%9.8f", EpD[SET_IRECT_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_IRECT_OFFS_CAL][0].V1); 	GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_pg1_sel_item_left==4) {
		    if (cal_pg1_sel_edit_mode != cal_none) {y0=63;}
			sprintf(L, "%9.8f", EpD[SET_IBAT_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_IBAT_OFFS_CAL][0].V1); 		GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_pg1_sel_digit==0) {
		    if (cal_pg1_sel_edit_mode != cal_none) {x0=48; w=4;}

		}
		if (cal_pg1_sel_digit==1) {
		    if (cal_pg1_sel_edit_mode != cal_none) {x0=42; w=4;}
		}
	}
	else if (cal_pg1_sel_col == 1) {
		if (cal_pg1_sel_item_right==0) {
		    if (cal_pg1_sel_edit_mode != cal_none) {y0=19;}
			sprintf(L, "%9.8f", EpD[SET_ACR_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_ACR_OFFS_CAL][0].V1); 		GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_pg1_sel_item_right==1) {
		    if (cal_pg1_sel_edit_mode != cal_none) {y0=30;}
			sprintf(L, "%9.8f", EpD[SET_ACS_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_ACS_OFFS_CAL][0].V1); 		GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_pg1_sel_item_right==2) {
		    if (cal_pg1_sel_edit_mode != cal_none) {y0=41;}
			sprintf(L, "%9.8f", EpD[SET_ACT_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_ACT_OFFS_CAL][0].V1); 		GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_pg1_sel_digit==0) {
		    if (cal_pg1_sel_edit_mode != cal_none) {x0=48+64; w=4;}
		}
		if (cal_pg1_sel_digit==1) {
		    if (cal_pg1_sel_edit_mode != cal_none) {x0=42+64; w=4;}
		}
	}
	GLCD_Line(x0, y0, x0+w, y0);

    if (cal_pg1_sel_edit_mode==cal_gain) {
    	GLCD_PrintString(96, 0, "GAIN");
    } else if (cal_pg1_sel_edit_mode==cal_offset) {
    	GLCD_PrintString(91, 0, "OFFST");
	}

}

inline extern void HOME_PAGE_pg_disp(void) {
	if (EpD[HOME_PG_SEL][0].V1==1) {
		char L[32]; char R[32];
//		sprintf(M, "NORMAL"); 					GLCD_PrintString(0, 0, M);
		sprintf(L, "Vo%6.1f V", VLOAD_pas.a16); 			GLCD_PrintString(0, 11, L);
		sprintf(L, "VB%6.1f V", VBAT_pas.a16); 				GLCD_PrintString(0, 20, L);
		sprintf(L, "IT%6.1f A", IRECT_pas.a16); 			GLCD_PrintString(0, 31, L);
		sprintf(L, "IB%6.1f A", IBAT_pas.a16); 			GLCD_PrintString(0, 40, L);

		if (VDCK_side==1) {
		sprintf(L, "K+%6.1f %%", VDCK_perc); 		GLCD_PrintString(0, 53, L);
		}
		if (VDCK_side==-1) {
		sprintf(L, "K-%6.1f %%", VDCK_perc); 		GLCD_PrintString(0, 53, L);
		}

		static char timed_charge_cnt[12];
		if (charge_mode_timed_time_sec > 0) {
			sprintf(timed_charge_cnt, "%04lu", charge_mode_timed_time_sec);
			GLCD_PrintString(104, 43, timed_charge_cnt);
		}
		sprintf(date_time_string, "%02u:%02u:%02u", rtc_hour_recv, rtc_min_recv, rtc_sec_recv); GLCD_PrintString(80, 53, date_time_string);


static char start_stop_str[7];
if (state_list[START_FC].action & (1 << ACTIVE_enum)) {
	sprintf(start_stop_str, "%s", state_list[START_FC].name); GLCD_PrintString(98, 0, start_stop_str);
	if (sfsta_op_phase==S_SFSTA_REQ) {
		GLCD_PrintString(98, 0, "SoftS");
	}
} else if (state_list[STOP_FC].action & (1 << ACTIVE_enum)) {
	sprintf(start_stop_str, "%s", state_list[STOP_FC].name); GLCD_PrintString(98, 0, start_stop_str);
}

		static uint8_t rolling_disp_VAC_cnt=0;
		static uint8_t disp_VAC_phase_wait =12;
		static uint8_t disp_VAC_phase_wait_slice =4;
		rolling_disp_VAC_cnt=(rolling_disp_VAC_cnt+1) % disp_VAC_phase_wait;

		if (rolling_disp_VAC_cnt < disp_VAC_phase_wait_slice) {
			sprintf(R, "VR%6.1f", VAC_R_rms_sc.a64);	GLCD_PrintString(74, 11, R);
			sprintf(R, "IR%6.1f", IAC_est);	GLCD_PrintString(74, 20, R);
		} else if (rolling_disp_VAC_cnt < (2 * disp_VAC_phase_wait_slice)) {
			sprintf(R, "VS%6.1f", VAC_S_rms_sc.a64);	GLCD_PrintString(74, 11, R);
			sprintf(R, "IS%6.1f", IAC_est);	GLCD_PrintString(74, 20, R);
		} else if (rolling_disp_VAC_cnt < (4 * disp_VAC_phase_wait_slice)) {
			sprintf(R, "VT%6.1f", VAC_T_rms_sc.a64);	GLCD_PrintString(74, 11, R);
			sprintf(R, "IT%6.1f", IAC_est);	GLCD_PrintString(74, 20, R);
		}

	static uint8_t rolling_disp_TEMP_cnt=0;
	static uint8_t disp_TEMP_phase_wait =12;
	static uint8_t disp_TEMP_phase_wait_slice =4;

if (temp_sens_count==3) {
	disp_TEMP_phase_wait =12;
	disp_TEMP_phase_wait_slice =4;
	rolling_disp_TEMP_cnt=(rolling_disp_TEMP_cnt+1) % disp_TEMP_phase_wait;
	if (rolling_disp_VAC_cnt < disp_VAC_phase_wait_slice) {
		sprintf(L, "Soğt%5.1f", tmp_dat_C[0]); 		GLCD_PrintString(74, 31+2, L);
	} else if (rolling_disp_VAC_cnt < 2 * disp_VAC_phase_wait_slice) {
		sprintf(L, "Traf%5.1f", tmp_dat_C[1]); 		GLCD_PrintString(74, 31+2, L);
	} else if (rolling_disp_VAC_cnt < 4 * disp_VAC_phase_wait_slice) {
		sprintf(L, "Akü %5.1f", tmp_dat_C[2]); 		GLCD_PrintString(74, 31+2, L);
	}
} else if (temp_sens_count==2) {
	disp_TEMP_phase_wait =8;
	disp_TEMP_phase_wait_slice =4;
	rolling_disp_TEMP_cnt=(rolling_disp_TEMP_cnt+1) % disp_TEMP_phase_wait;
	if (rolling_disp_TEMP_cnt < disp_TEMP_phase_wait_slice) {
		sprintf(L, "Soğt%5.1f", tmp_dat_C[0]); 		GLCD_PrintString(74, 31+2, L);
	} else if (rolling_disp_TEMP_cnt < 2 * disp_TEMP_phase_wait_slice) {
		sprintf(L, "Traf%5.1f", tmp_dat_C[1]); 		GLCD_PrintString(74, 31+2, L);
	}
}
///////////////////////////////////////////////////////////////
// STATE AÇIKLAMALRINI DOLANDIRARAK GÖSTER
static uint8_t tabl_dolas=0, tabl_dolas_delay=4;
static uint8_t son_kal=0;
static uint8_t tabl_dolas_delay_cnt=0;
static char RollBuf[32];
if (tabl_dolas_delay_cnt==0) {
//		if (tabl_dolas == NUM_STATE_NAMES) {
//			tabl_dolas=0;
//			son_kal=0;
//		}
	for (tabl_dolas = son_kal; tabl_dolas <= NUM_STATE_NAMES; tabl_dolas++) {
		if (state_list[tabl_dolas].action & (1 << LCD_roll_enum) && state_list[tabl_dolas].action & (1 << ACTIVE_enum)) {
			if ((state_list[tabl_dolas].code==ST_FLOAT_CHARGE || state_list[tabl_dolas].code==BOOST_CHARGE_FC) && EpD[SET_CHARGE_MODE][0].V1 == AUTO) {
				sprintf(RollBuf, "%s%s", state_list[tabl_dolas].name, "(Oto)");
			} else if ((state_list[tabl_dolas].code==1 || state_list[tabl_dolas].code==1)) {
				sprintf(RollBuf, "%s%s", state_list[tabl_dolas].name, "(Zmn)");
			} else if ((state_list[tabl_dolas].code==ST_FLOAT_CHARGE || state_list[tabl_dolas].code==BOOST_CHARGE_FC)) {
				sprintf(RollBuf, "%s%s", state_list[tabl_dolas].name, "(Man)");
			} else {
				sprintf(RollBuf, "%s", state_list[tabl_dolas].name);
			}
			son_kal=tabl_dolas+1;
			break; // ilk gösterilecek eleman bulundu gösterildi.
		}
	}
	if (tabl_dolas == NUM_STATE_NAMES) {	// tablo sonuna gelirsek başa dön tara bulursan göster.
		tabl_dolas=0;
		son_kal=0;

		for (tabl_dolas = son_kal; tabl_dolas < NUM_STATE_NAMES; tabl_dolas++) { // üstteki for döngüsü ile aynı. yazma işlemi üstteki loopta yapılıyor. üstteki loop bir şey bulamayıp tablo sonuna geldi diyelim. bu durumda birşey bulunamadığı için bu döngüde boş satır yazılacak. bunu engellemek için aynı loop buraya da koyuluyor.
			if (state_list[tabl_dolas].action & (1 << LCD_roll_enum) && state_list[tabl_dolas].action & (1 << ACTIVE_enum)) {
				if ((state_list[tabl_dolas].code==ST_FLOAT_CHARGE || state_list[tabl_dolas].code==BOOST_CHARGE_FC) && EpD[SET_CHARGE_MODE][0].V1 == AUTO) {
					sprintf(RollBuf, "%s%s", state_list[tabl_dolas].name, "(Oto)");
				} else if ((state_list[tabl_dolas].code==1 || state_list[tabl_dolas].code==1)) {
					sprintf(RollBuf, "%s%s", state_list[tabl_dolas].name, "(Zmn)");
				} else if ((state_list[tabl_dolas].code==ST_FLOAT_CHARGE || state_list[tabl_dolas].code==BOOST_CHARGE_FC)) {
					sprintf(RollBuf, "%s%s", state_list[tabl_dolas].name, "(Man)");
				} else {
					sprintf(RollBuf, "%s", state_list[tabl_dolas].name);
				}
				son_kal=tabl_dolas+1;
				break;
			}
		}
	}
}
GLCD_PrintString(0, 0, RollBuf);
tabl_dolas_delay_cnt=(tabl_dolas_delay_cnt+1) % tabl_dolas_delay;
// STATE AÇIKLAMALRINI DOLANDIRARAK GÖSTER
///////////////////////////////////////////////////////////////

////////////////////////////////////////////////
// HOMEPAGE HÜCRELEME ÇİZGİLERİ
	uint32_t x0 = 65; uint32_t y0 = 9;
	uint32_t x1 = 65; uint32_t y1 = 63;
	GLCD_Line(x0, y0, x1, y1);
	GLCD_Line(x0+3, y0, x1+3, y1);
	x0 = 0; y0 = 9;
	x1 = 65; y1 = 9;
	GLCD_Line(x0, y0, x1, y1);
//	x0 = 95; y0 = 0;
//	x1 = 95; y1 = 9;
//	GLCD_Line(x0, y0, x1, y1);
	x0 = 68; y0 = 9;
	x1 = 127; y1 = 9;
	GLCD_Line(x0, y0, x1, y1);
	x0 = 0; y0 = 27+2;
	x1 = 65; y1 = 27+2;
	GLCD_Line(x0, y0, x1, y1);
	x0 = 68; y0 = 27+2;
	x1 = 127; y1 = 27+2;
	GLCD_Line(x0, y0, x1, y1);
	x0 = 0; y0 = 48+1;
	x1 = 65; y1 = 48+1;
	GLCD_Line(x0, y0, x1, y1);
	x0 = 0; y0 = 63;
	x1 = 65; y1 = 63;
	GLCD_Line(x0, y0, x1, y1);
////////////////////////////////////////////////



//		if (EpD[SET_UNSEEN_FLT][0].V1==1) {
//			sprintf(M, "A"); 		GLCD_PrintString(118, 0, M);
//		}
	}
	else if (EpD[HOME_PG_SEL][0].V1==2) {
		char b[8][32];
		sprintf(b[0], "%5.1f >-D->%5.1f V", VRECT_pas.a16, VLOAD_pas.a16);
		sprintf(b[1], "%5.1f   |  %5.1f A", IRECT_pas.a16, ILOAD_pas.a16);
		sprintf(b[2], "        | ");
		sprintf(b[3], "    BAT");
		sprintf(b[4], "  %5.1f V", VBAT_pas.a16);
		sprintf(b[5], "  %5.1f A", IBAT_pas.a16);
		char L[32];
		if (VDCK_side==1) {
		sprintf(L, "K+%5.1f%%", VDCK_perc); 		GLCD_PrintString(4, 45+3+4+1, L);
		}
		if (VDCK_side==-1) {
		sprintf(L, "K-%5.1f%%", VDCK_perc); 		GLCD_PrintString(4, 45+3+4+1, L);
		}

		sprintf(L, "Sıcaklık C"); 						GLCD_PrintString(66, 24, L);
		sprintf(L, "Soğt%6.1f", tmp_dat_C[0]); 		GLCD_PrintString(66, 34, L);
		sprintf(L, "Traf%6.1f", tmp_dat_C[1]); 		GLCD_PrintString(66, 44, L);
		sprintf(L, "Akü %6.1f", tmp_dat_C[2]); 		GLCD_PrintString(66, 54, L);


		for (int i = 0; i < 6; ++i) {
			GLCD_DisplayString(0, i, b[i]);
		}
			uint32_t x0 = 0; uint32_t y0 = 0;
			uint32_t w = 0; uint32_t h = 0;

			x0 = 3;		y0 = 22;
			w = 53;		h = 26;
			GLCD_Rect_E(x0,y0,x0+w,y0+h); // batt rect
			x0 = 62;		y0 = 22;
			w = 65;		h = 41;
			GLCD_Rect_E(x0,y0,x0+w,y0+h); // sıcaklık rect
	}




}

inline extern void RELAY_ORDER_pg_disp(void) {

	char L[32];
    if (rel_disp_mode == 1) {
		GLCD_PrintString(0, 0, "Kontak Sıralama");
		uint8_t sirali_tablo_disp_start_ind_h=sirali_tablo_disp_start_ind; // aşağıdaki for döngüsü bu indexten başlayarak ilerleyecek ve 6 elemanı gösterecek.
		for (uint8_t i = 0; i < 6; i++) {

		    sprintf(L, " %02d", sirali_tablo_disp_start_ind_h+1);
			GLCD_PrintString(0, (i + 1) * 9, L);
			sprintf(L, "%s ", state_list[(SIRALI_TABLO_RELOUT[sirali_tablo_disp_start_ind_h].rel_out_code)].name);
			GLCD_PrintString(22, (i + 1) * 9, L);
			sprintf(L, "%d ", (state_get(SIRALI_TABLO_RELOUT[sirali_tablo_disp_start_ind_h].rel_out_code)));
			GLCD_PrintString(122, (i + 1) * 9, L);
			sirali_tablo_disp_start_ind_h=(sirali_tablo_disp_start_ind_h+1+rel_ord_tb_size) % rel_ord_tb_size;

		}

		GLCD_PrintString(0, (sirali_tablo_arrow_ind+1) * 9, ">");

	} else if (rel_edit_mode == 1) {
		sprintf(L, "Kontak %02d İçin Seçim", SIRALI_TABLO_RELOUT[sirali_tablo_disp_start_ind].rel_out_tb_ind);
		GLCD_PrintString(0, 0, L);
//		uint8_t tam_tablo_disp_start_ind_=tam_tablo_disp_start_ind;
//		for (uint8_t i = 0; i < 6; i++) {
//
//			sprintf(L, " %s ", TAM_TABLO_RELOUT[tam_tablo_disp_start_ind_].name);
//			GLCD_PrintString(3, (i + 1) * 9, L);
//			sprintf(L, "%d ", TAM_TABLO_RELOUT[tam_tablo_disp_start_ind_].rel_out_tb_val);
//			GLCD_PrintString(122, (i + 1) * 9, L);
//			tam_tablo_disp_start_ind_=(tam_tablo_disp_start_ind_+1+tam_tablo_size) % tam_tablo_size;
//
//		}

//		GLCD_PrintString(0, (tam_tablo_arrow_ind+1) * 9, ">");
//
//		uint8_t x0=0; uint8_t y0=0; uint8_t w=100; uint8_t h=10;
//		if (cal_pg1_sel_edit_mode==cal_none) {
//			if (cal_pg1_sel_col==0) {
//				x0=6; y0=(tam_tablo_arrow_ind+1) * 9-2;
//				GLCD_Rect_E(x0,y0,x0+w,y0+h);
//			}
//		}
    }
}

inline extern void CHARGE_SETT_pg_disp(void) {
    GLCD_PrintString(0, 0, "Şarj Ayarları");
	char L[32]; char M[32];

	if (charge_mode_timed_time_sec > 0) {
		sprintf(M, "t%04lu", charge_mode_timed_time_sec);
		GLCD_PrintString(96, 0, M);
	}

	uint8_t chg_set_disp_index_=chg_set_disp_index;
    for (uint8_t i = 0; i < 6; i++) {

	sprintf(L, " %s ", CHARGE_SETT_Items[chg_set_disp_index_].setting_id);
	GLCD_PrintString(0, (i + 1) * 9, L);

	if (CHARGE_SETT_Items[chg_set_disp_index_].V1 == SET_CHARGE_MODE) {
		sprintf(M, "%s", CH_MOD_SEL_Items[(uint32_t)EpD[SET_CHARGE_MODE][chg_setting_edit_mode].V1]);
		GLCD_PrintString(80, (i + 1) * 9, M);
	}
	else if (CHARGE_SETT_Items[chg_set_disp_index_].V1 == SET_BOOST_TIME) {
	sprintf(M, "%02lu dk", (uint32_t) EpD[SET_BOOST_TIME][chg_setting_edit_mode].V1);
	GLCD_PrintString(96, (i + 1) * 9, M);
	}
	else if (CHARGE_SETT_Items[chg_set_disp_index_].type == 0) {
	sprintf(M, "%05.1f", EpD[CHARGE_SETT_Items[chg_set_disp_index_].V1][chg_setting_edit_mode].V1);
	GLCD_PrintString(96, (i + 1) * 9, M);
	}
chg_set_disp_index_=(chg_set_disp_index_+1+NUM_CHARGE_SETT_ITEMS) % NUM_CHARGE_SETT_ITEMS;
    }
	GLCD_PrintString(0, (chg_set_arrow_loc+1) * 9, ">");
	if (selected_CHARGE_SETT==(SET_CHARGE_MODE-1)) {
		GLCD_PrintString(78, (chg_set_arrow_loc+1) * 9, ">");
	} else {
		GLCD_PrintString(90, (chg_set_arrow_loc+1) * 9, ">");
	}

if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1 == SET_CHARGE_MODE && chg_setting_edit_mode) {
	GLCD_Line(83, (chg_set_arrow_loc+1)*9-1, 83, (chg_set_arrow_loc+2)*9-2);
	GLCD_Line(127, (chg_set_arrow_loc+1)*9-1, 127, (chg_set_arrow_loc+2)*9-2);
	GLCD_Line(83, (chg_set_arrow_loc+1)*9-2, 127, (chg_set_arrow_loc+1)*9-2);
	GLCD_Line(83, (chg_set_arrow_loc+2)*9-1, 127, (chg_set_arrow_loc+2)*9-1);
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1 == SET_BOOST_TIME && chg_setting_edit_mode) {
	GLCD_Line(94, (chg_set_arrow_loc+1)*9-1, 94, (chg_set_arrow_loc+2)*9-2);
	GLCD_Line(127, (chg_set_arrow_loc+1)*9-1, 127, (chg_set_arrow_loc+2)*9-2);
	GLCD_Line(94, (chg_set_arrow_loc+1)*9-2, 127, (chg_set_arrow_loc+1)*9-2);
	GLCD_Line(94, (chg_set_arrow_loc+2)*9-1, 127, (chg_set_arrow_loc+2)*9-1);
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].type == 0 && chg_setting_edit_mode) {
	GLCD_Line(94, (chg_set_arrow_loc+1)*9-1, 94, (chg_set_arrow_loc+2)*9-2);
	GLCD_Line(127, (chg_set_arrow_loc+1)*9-1, 127, (chg_set_arrow_loc+2)*9-2);
	GLCD_Line(94, (chg_set_arrow_loc+1)*9-2, 127, (chg_set_arrow_loc+1)*9-2);
	GLCD_Line(94, (chg_set_arrow_loc+2)*9-1, 127, (chg_set_arrow_loc+2)*9-1);
}
}

inline extern void MAIN_MENU_pg_disp(void) {
    GLCD_PrintString(0, 0, "Ana Menü");
    char L[32];
//    char M[32];
    uint8_t main_menu_disp_index_=main_menu_disp_index;

    for (uint8_t i = 0; i < 6; i++) {
		sprintf(L, " %s ", MAIN_MENU_Items[main_menu_disp_index_]);
    	if (main_menu_disp_index_!=6) {
    		GLCD_PrintString(0, (i + 1) * 9, L);
    	}
    	else if (main_menu_disp_index_==6 && management_menu_en) {
    		GLCD_PrintString(0, (i + 1) * 9, L);
    	}
		GLCD_PrintString(0, (main_menu_arrow_loc+1) * 9, ">");
		main_menu_disp_index_=(main_menu_disp_index_+1+NUM_MAIN_MENU_ITEMS) % NUM_MAIN_MENU_ITEMS;
    }

    static char timed_charge_cnt[12];
    if (charge_mode_timed_time_sec > 0) {
    	sprintf(timed_charge_cnt, "t%04lu", charge_mode_timed_time_sec);
    	GLCD_PrintString(96, 0, timed_charge_cnt);
    }

}
inline extern void DEVICE_SETT_pg_disp(void) {
    GLCD_PrintString(0, 0, "Cihaz Ayarları");
        char L[32];
        char M[32];
    uint8_t dev_set_disp_index_=dev_set_disp_index;

	for (uint8_t i = 0; i < 6; i++) {
		sprintf(L, " %s ", DEVICE_SETT_Items[dev_set_disp_index_].setting_id);
		GLCD_PrintString(0, (i + 1) * 9, L);
		if (DEVICE_SETT_Items[dev_set_disp_index_].V1 == SET_BATT_REV_DET) {
			sprintf(M, "%s", AKTFPAS_SEL_Items[(uint32_t)EpD[SET_BATT_REV_DET][dev_setting_edit_mode].V1]);
			GLCD_PrintString(86, (i + 1) * 9, M);
		}
		else if (DEVICE_SETT_Items[dev_set_disp_index_].V1 == SET_BATT_DISC_CHK_EN) {
			sprintf(M, "%s", AKTFPAS_SEL_Items[(uint32_t)EpD[SET_BATT_DISC_CHK_EN][dev_setting_edit_mode].V1]);
			GLCD_PrintString(86, (i + 1) * 9, M);
		}
//		else if (DEVICE_SETT_Items[dev_set_disp_index_].V1 == SET_OVTM_OPEN_DUR) {
//			sprintf(M, "%03lusn", (uint32_t) EpD[SET_OVTM_OPEN_DUR][dev_setting_edit_mode].V1);
//			GLCD_PrintString(97, (i + 1) * 9, M);
//		}
		else if (DEVICE_SETT_Items[dev_set_disp_index_].type == 3) {
			sprintf(M, "%05.1f", EpD[DEVICE_SETT_Items[dev_set_disp_index_].V1][dev_setting_edit_mode].V1);
			GLCD_PrintString(98, (i + 1) * 9, M);
		}
		dev_set_disp_index_=(dev_set_disp_index_+1+NUM_DEVICE_SETT_ITEMS) % NUM_DEVICE_SETT_ITEMS;
	}
	GLCD_PrintString(0, (dev_set_arrow_loc+1) * 9, ">");
	GLCD_PrintString(91, (dev_set_arrow_loc+1) * 9, ">");

if (dev_setting_edit_mode) {
	GLCD_Rect_E(95,(dev_set_arrow_loc+1)*9-2,127,(dev_set_arrow_loc+2)*9-1); // batt rect
}
}

inline extern void MANAGEMENT_pg_disp(void) {
    GLCD_PrintString(0, 0, "Yönetim");

    for (uint8_t i = 0; i < NUM_MANAGEMENT_ITEMS; i++) {
        char L[32];
        if (i == selected_MANAGEMENT) {
            sprintf(L, ">%s", MANAGEMENT_Items[i]);

        } else {
            sprintf(L, " %s", MANAGEMENT_Items[i]);
        }
        GLCD_PrintString(0, (i + 1) * 9, L);
    }
}

inline extern void FAULT_CODES_REPORT_pg_disp(void) {
	uint8_t line_num = 0;
	uint8_t disp_index_ = flt_disp_index;
    char L[32];
    GLCD_PrintString(0, 0, "Arıza Kod Raporu");

    for (uint8_t j = 0; j < 6; j++) {
    	if (!FAULT_CODES_REPORT_disp_mode) {

    	    if (array_fault_data[disp_index_][1] < NUM_STATE_NAMES) {
        		sprintf(L, "%03d %s", disp_index_+1, state_list[array_fault_data[disp_index_][1]].name);
    	    } else {
        		sprintf(L, "%03d -", disp_index_+1);
    	    }

    	} else {
    		convert_timestamp_to_date_string(array_fault_data[disp_index_][0], rtc_timestring, sizeof(rtc_timestring));
    		sprintf(L, "%03d %s", disp_index_+1, rtc_timestring);
    	}
		GLCD_PrintString(0, (line_num + 1) * 9, L);
		line_num++;
    	disp_index_=(disp_index_+1+NUM_FAULT_RECORD) % NUM_FAULT_RECORD;

// LAST FAULT OVER UNDER LINE >>>>>>>>>>>>>>>>>>>>
		int line1_y = ((((flt_array_index_last+NUM_FAULT_RECORD)-flt_disp_index+1)%NUM_FAULT_RECORD) * 9)-2;
		int line2_y = line1_y+9+1;

		if (line1_y < 0 && line2_y < 0) {
		} else if (line1_y < 0 && line2_y >= 0 && line2_y < 64) {
			GLCD_Line(0, line2_y, 127, line2_y);
		} else if (line1_y >= 0 && line1_y < 64 && line2_y >= 0 && line2_y < 64) {
			GLCD_Line(0, line1_y, 127, line1_y);
			GLCD_Line(0, line2_y, 127, line2_y);
		} else if (line1_y >= 0 && line1_y < 64 && line2_y > 64) {
			GLCD_Line(0, line1_y, 127, line1_y);
		} else if (line1_y > 64 && line2_y > 64) {
		}
// LAST FAULT OVER UNDER LINE <<<<<<<<<<<<<<<<<<<<

	}
}

inline extern void FAULT_CODES_RESET_pg_disp(void) {
    GLCD_PrintString(0, 18, ">Arıza Kayıtları Sil");

	if (fault_codes_reset_req) {
		GLCD_PrintString(0, 36, "   İptal    Onayla");
		uint8_t x0=6+2*6, y0=44;
		if (fault_codes_reset_req_right == 0) {
			GLCD_Line(x0, y0, x0+4+4*6, y0);
		} else {
			x0 = x0+9*6;
			GLCD_Line(x0, y0, x0+4+5*6, y0);
		}
		if (fault_codes_reset_completed == 1) {
			GLCD_PrintString(0, 48, "   Kayıtlar Silindi");
		}
	}
}

inline extern void RECTF_ACTIVE_AT_STARTUP_pg_disp(void) {
		GLCD_PrintString(0, 36, "   Pasif    Aktif");
		uint8_t x0=6+2*6, y0=44;
		if (rectf_active_at_startup_req_right == 0) {
		    GLCD_PrintString(0, 18, ">Doğrltc Açlşta Pasif");
			GLCD_Line(x0, y0, x0+4+4*6, y0);
		} else {
			x0 = x0+9*6;
		    GLCD_PrintString(0, 18, ">Doğrltc Açlşta Aktif");
			GLCD_Line(x0, y0, x0+4+4*6, y0);
		}
}

inline extern void DEVICE_RESET_pg_disp(void) {
    GLCD_PrintString(0, 27, ">Cihaz Yeniden Başlat");

	if (device_reset_req) {
		GLCD_PrintString(0, 45, "   İptal    Onayla");
		uint8_t x0=6+2*6, y0=53;
		if (device_reset_req_right == 0) {
			GLCD_Line(x0, y0, x0+4+4*6, y0);
		} else {
			x0 = x0+9*6;
			GLCD_Line(x0, y0, x0+4+5*6, y0);
		}
	}
}


inline extern void DATE_TIME_pg_disp(void) {
    GLCD_PrintString(0, 0, "Tarih Saat Ayarı");

    if (DATE_TIME_edit_mode == 1) {
	    sprintf(date_time_string, " %02u.%02u.20%02u %02u:%02u:%02u", rtc_day_edit, rtc_month_edit, rtc_year_edit, rtc_hour_edit, rtc_min_edit, rtc_sec_edit);
	    GLCD_PrintString(0, 9, date_time_string);
		if (DATE_TIME_edit_digit == 0) {
			GLCD_Line(6*1, 16, 6*1+5, 16);
		} else if (DATE_TIME_edit_digit == 1) {
			GLCD_Line(6*2, 16, 6*2+5, 16);
		} else if (DATE_TIME_edit_digit == 2) {
			GLCD_Line(6*4, 16, 6*4+5, 16);
		} else if (DATE_TIME_edit_digit == 3) {
			GLCD_Line(6*5, 16, 6*5+5, 16);
		} else if (DATE_TIME_edit_digit == 4) {
			GLCD_Line(6*9, 16, 6*9+5, 16);
		} else if (DATE_TIME_edit_digit == 5) {
			GLCD_Line(6*10, 16, 6*10+5, 16);
		} else if (DATE_TIME_edit_digit == 6) {
			GLCD_Line(6*12, 16, 6*12+5, 16);
		} else if (DATE_TIME_edit_digit == 7) {
			GLCD_Line(6*13, 16, 6*13+5, 16);
		} else if (DATE_TIME_edit_digit == 8) {
			GLCD_Line(6*15, 16, 6*15+5, 16);
		} else if (DATE_TIME_edit_digit == 9) {
			GLCD_Line(6*16, 16, 6*16+5, 16);
		} else if (DATE_TIME_edit_digit == 10) {
			GLCD_Line(6*18, 16, 6*18+5, 16);
		} else if (DATE_TIME_edit_digit == 11) {
			GLCD_Line(6*19, 16, 6*19+5, 16);
		}
	} else {
	    sprintf(date_time_string, " %02d.%02d.20%02d %02d:%02d:%02d", rtc_day_recv, rtc_month_recv, rtc_year_recv, rtc_hour_recv, rtc_min_recv, rtc_sec_recv);
	    GLCD_PrintString(0, 9, date_time_string);
	}
}




