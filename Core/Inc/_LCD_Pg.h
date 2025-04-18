




inline extern void DROPPER_pg_disp(void) {
    GLCD_PrintString(0, 0, "Dropper");
	char L[32]; static uint8_t lnhg=9;
	sprintf(L, " Kademe 1 %s", AKTFPAS_SEL_Items[(uint32_t)EpD[SET_DROPPER_K1][dropper_edit_mode].V1]); 		GLCD_PrintString(0, 1*lnhg, L);
	sprintf(L, " Kademe 2 %s", AKTFPAS_SEL_Items[(uint32_t)EpD[SET_DROPPER_K2][dropper_edit_mode].V1]); 		GLCD_PrintString(0, 2*lnhg, L);
    sprintf(L, ">");
	if (selected_DROPPER==0) {
        GLCD_PrintString(0, 1*lnhg, L);
    } else if (selected_DROPPER==1) {
        GLCD_PrintString(0, 2*lnhg, L);
    }
	if (dropper_edit_mode) {
		GLCD_Rect_E(69,(selected_DROPPER+1)*9-2,101,(selected_DROPPER+2)*9-1); // batt rect
	}
}

inline extern void CALIBRATION_pg_disp(void) {
    GLCD_PrintString(0, 0, "Kalibrasyon");
	char L[32]; static uint8_t lnhg=11;
    sprintf(L, "VR%7.2f", VRECT_pas.a64); 		GLCD_PrintString(0, 1*lnhg, L);
    sprintf(L, "VL%7.2f", VLOAD_pas.a64); 		GLCD_PrintString(0, 2*lnhg, L);
    sprintf(L, "VB%7.2f", VBAT_pas.a64); 		GLCD_PrintString(0, 3*lnhg, L);
    sprintf(L, "IR%7.2f", IRECT_pas.a64); 		GLCD_PrintString(0, 4*lnhg, L);
    sprintf(L, "IB%7.2f", IBAT_pas.a64); 		GLCD_PrintString(0, 5*lnhg, L);
    sprintf(L, "CR%7.2f", VAC_R_rms_roll_per_avg.a64); 		GLCD_PrintString(64, 1*lnhg, L);
    sprintf(L, "CS%7.2f", VAC_S_rms_roll_per_avg.a64); 		GLCD_PrintString(64, 2*lnhg, L);
    sprintf(L, "CT%7.2f", VAC_T_rms_roll_per_avg.a64); 		GLCD_PrintString(64, 3*lnhg, L);

    uint8_t x0=0; uint8_t y0=0; uint8_t w=0; uint8_t h=10;


    if (cal_sel_edit_mode==cal_none) {
		if (cal_sel_col==0) {
			x0=13; y0=9+(cal_sel_item_left*11); w=41;
			GLCD_Rect_E(x0,y0,x0+w,y0+h);
		} else if (cal_sel_col==1) {
			x0=77; y0=9+(cal_sel_item_right*11); w=41;
			GLCD_Rect_E(x0,y0,x0+w,y0+h);
		}
    }

	if (cal_sel_col == 0) {
		if (cal_sel_item_left==0) {
		    if (cal_sel_edit_mode != cal_none) {y0=19;}
		    sprintf(L, "%9.8f", EpD[SET_VRECT_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_VRECT_OFFS_CAL][0].V1); 	GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_sel_item_left==1) {
		    if (cal_sel_edit_mode != cal_none) {y0=30;}
			sprintf(L, "%9.8f", EpD[SET_VLOAD_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_VLOAD_OFFS_CAL][0].V1); 	GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_sel_item_left==2) {
		    if (cal_sel_edit_mode != cal_none) {y0=41;}
			sprintf(L, "%9.8f", EpD[SET_VBAT_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_VBAT_OFFS_CAL][0].V1); 		GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_sel_item_left==3) {
		    if (cal_sel_edit_mode != cal_none) {y0=52;}
			sprintf(L, "%9.8f", EpD[SET_IRECT_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_IRECT_OFFS_CAL][0].V1); 	GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_sel_item_left==4) {
		    if (cal_sel_edit_mode != cal_none) {y0=63;}
			sprintf(L, "%9.8f", EpD[SET_IBAT_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_IBAT_OFFS_CAL][0].V1); 		GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_sel_digit==0) {
		    if (cal_sel_edit_mode != cal_none) {x0=48; w=4;}

		}
		if (cal_sel_digit==1) {
		    if (cal_sel_edit_mode != cal_none) {x0=42; w=4;}
		}
	}
	else if (cal_sel_col == 1) {
		if (cal_sel_item_right==0) {
		    if (cal_sel_edit_mode != cal_none) {y0=19;}
			sprintf(L, "%9.8f", EpD[SET_ACR_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_ACR_OFFS_CAL][0].V1); 		GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_sel_item_right==1) {
		    if (cal_sel_edit_mode != cal_none) {y0=30;}
			sprintf(L, "%9.8f", EpD[SET_ACS_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_ACS_OFFS_CAL][0].V1); 		GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_sel_item_right==2) {
		    if (cal_sel_edit_mode != cal_none) {y0=41;}
			sprintf(L, "%9.8f", EpD[SET_ACT_CAL][0].V1); 			GLCD_PrintString(64, 4*lnhg, L);
			sprintf(L, "%6.0f", EpD[SET_ACT_OFFS_CAL][0].V1); 		GLCD_PrintString(64, 5*lnhg, L);
		}
		if (cal_sel_digit==0) {
		    if (cal_sel_edit_mode != cal_none) {x0=48+64; w=4;}
		}
		if (cal_sel_digit==1) {
		    if (cal_sel_edit_mode != cal_none) {x0=42+64; w=4;}
		}
	}
	GLCD_Line(x0, y0, x0+w, y0);



    if (cal_sel_edit_mode==cal_gain) {
    	GLCD_PrintString(96, 0, "GAIN");
    } else if (cal_sel_edit_mode==cal_offset) {
    	GLCD_PrintString(91, 0, "OFFST");
	}

}

inline extern void HOME_PAGE_pg_disp(void) {
	if (HOME_PAGE_pg_sel==1) {
		char L[32]; char R[32]; char M[32];
//		sprintf(M, "NORMAL"); 					GLCD_PrintString(0, 0, M);
		sprintf(L, "VL%6.1f V", VLOAD_pas.a16); 			GLCD_PrintString(0, 9+1+1, L);
		sprintf(L, "IT%6.1f A", IRECT_pas.a16); 			GLCD_PrintString(0, 18+1+1, L);
		sprintf(L, "VB%6.1f V", VBAT_pas.a16); 				GLCD_PrintString(0, 27+3+1, L);
		sprintf(L, "IB%6.1f A", IBAT_pas.a16); 			GLCD_PrintString(0, 36+3+1, L);
		if (VDCK_side==1) {
		sprintf(L, "K+%6.1f %%", VDCK_perc); 		GLCD_PrintString(0, 45+3+4+1, L);
		}
		if (VDCK_side==-1) {
		sprintf(L, "K-%6.1f %%", VDCK_perc); 		GLCD_PrintString(0, 45+3+4+1, L);
		}

		sprintf(R, "VR%6.1f", VAC_R_rms_roll_per_avg.a64);	GLCD_PrintString(76, 9, R);
		sprintf(R, "VS%6.1f", VAC_S_rms_roll_per_avg.a64);	GLCD_PrintString(76, 18, R);
		sprintf(R, "VT%6.1f", VAC_T_rms_roll_per_avg.a64);	GLCD_PrintString(76, 27, R);
		sprintf(R, "IR%6.1f", IAC_R_rms_roll_per_avg.a64);						GLCD_PrintString(76, 27+3+8, R);
		sprintf(R, "IS%6.1f", IAC_S_rms_roll_per_avg.a64);						GLCD_PrintString(76, 36+3+8, R);
		sprintf(R, "IT%6.1f", IAC_T_rms_roll_per_avg.a64);						GLCD_PrintString(76, 45+3+8, R);

		uint32_t x0 = 65; uint32_t y0 = 9;
		uint32_t x1 = 65; uint32_t y1 = 63;
		GLCD_Line(x0, y0, x1, y1);
		GLCD_Line(x0+3, y0, x1+3, y1);
		x0 = 0; y0 = 9;
		x1 = 65; y1 = 9;
		GLCD_Line(x0, y0, x1, y1);
		GLCD_Line(x0, y0, x1, y1);
		x0 = 0; y0 = 27+2;
		x1 = 65; y1 = 27+2;
		GLCD_Line(x0, y0, x1, y1);
		x0 = 0; y0 = 48+1;
		x1 = 65; y1 = 48+1;
		GLCD_Line(x0, y0, x1, y1);
		x0 = 0; y0 = 63;
		x1 = 65; y1 = 63;
		GLCD_Line(x0, y0, x1, y1);

		if (device_start_up_delay_completed==0) {
			sprintf(M, "BAŞLANGIÇ"); GLCD_PrintString(0, 0, M);
		} else {

			if (batt_line_broken==1) {
				if (batt_switch_status==0) {
					sprintf(M, "AKÜ ANAHTAR OFF"); 		GLCD_PrintString(0, 0, M);
				} else if (batt_switch_status==1) {
					sprintf(M, "AKÜ HATTI KOPUK"); 		GLCD_PrintString(0, 0, M);
				}
			}
			else if (thy_drv_en==1) {
				if (sf_sta_req==1 && !rectifier_current_limit_accepted && !battery_current_limit_accepted) {
					sprintf(M, "SOFT START"); 		GLCD_PrintString(0, 0, M);
				} else if (sf_sta_req==0 || rectifier_current_limit_accepted || battery_current_limit_accepted) {
					sprintf(M, "DOĞRLT AKTİF"); 			GLCD_PrintString(0, 0, M);
				}
			}
			else if (thy_drv_en==0) {
				if (thy_drv_en_req == 1) {
					if (sf_sta_req==0) {
						sprintf(M, "AKTİF EDİLECEK"); // sf_sta_req_cnt++ sayacının sayıldığı anda bu yazıyı yazdır.
						GLCD_PrintString(0, 0, M);
					}
					else if (sf_sta_req == 1) {
						sprintf(M, "SOFT START");
						GLCD_PrintString(0, 0, M);
					}
				} else if (thy_drv_en_req == 0) {
					sprintf(M, "DOĞRLT KAPALI");
					GLCD_PrintString(0, 0, M);
				}
			}
		}
		if (EpD[SET_UNSEEN_FLT][0].V1==1) {
			sprintf(M, "A"); 		GLCD_PrintString(118, 0, M);
		}
	}
	else if (HOME_PAGE_pg_sel==2) {
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

inline extern void MAIN_MENU_pg_disp(void) {
    GLCD_PrintString(0, 0, "Ana Menü");

    for (uint8_t i = 0; i < NUM_MAIN_MENU_ITEMS; i++) {
        char L[32];
        if (i == selected_MAIN_MENU) {
            sprintf(L, ">%s", MAIN_MENU_Items[i]);
        } else {
            sprintf(L, " %s", MAIN_MENU_Items[i]);
        }
        GLCD_PrintString(0, (i + 1) * 9, L);
    }
}
inline extern void RELAY_ORDER_pg_disp(void) {

	char L[32];
    if (rel_disp_mode == 1) {
		GLCD_PrintString(0, 0, "Kontak Sıralama");
		uint8_t rel_ord_disp_index_=rel_ord_disp_index;
		for (uint8_t i = 0; i < 6; i++) {

		    if (rel_ord_disp_index_ < 16) {
		    	sprintf(L, " %02d", rel_ord_tb[rel_ord_disp_index_].rel_ord_order);
				GLCD_PrintString(0, (i + 1) * 9, L);
		    }
			sprintf(L, "%s ", rel_ord_tb[rel_ord_disp_index_].rel_ord_desc);
			GLCD_PrintString(22, (i + 1) * 9, L);
			sprintf(L, "%d ", rel_ord_tb[rel_ord_disp_index_].rel_ord_val);
			GLCD_PrintString(122, (i + 1) * 9, L);

			rel_ord_disp_index_=(rel_ord_disp_index_+1+rel_ord_tb_size) % rel_ord_tb_size;

		}

		GLCD_PrintString(0, (rel_ord_arrow_loc+1) * 9, ">");

	} else if (rel_edit_mode == 1) {
		sprintf(L, "Kontak %02d İçin Seçim", rel_ord_tb[rel_ord_tb_sel].rel_ord_order);
		GLCD_PrintString(0, 0, L);
		uint8_t rel_dat_disp_index_=rel_dat_disp_index;
		for (uint8_t i = 0; i < 6; i++) {

			sprintf(L, " %s ", rel_dat_tb[rel_dat_disp_index_].rel_dat_desc);
			GLCD_PrintString(3, (i + 1) * 9, L);
			sprintf(L, "%d ", rel_dat_tb[rel_dat_disp_index_].rel_dat_val);
			GLCD_PrintString(122, (i + 1) * 9, L);
			rel_dat_disp_index_=(rel_dat_disp_index_+1+rel_dat_tb_size) % rel_dat_tb_size;

		}

		GLCD_PrintString(0, (rel_dat_arrow_loc+1) * 9, ">");

		uint8_t x0=0; uint8_t y0=0; uint8_t w=100; uint8_t h=10;
		if (cal_sel_edit_mode==cal_none) {
			if (cal_sel_col==0) {
				x0=6; y0=(rel_dat_arrow_loc+1) * 9-2;
				GLCD_Rect_E(x0,y0,x0+w,y0+h);
			}
		}
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
		else if (DEVICE_SETT_Items[dev_set_disp_index_].V1 == SET_BATT_DISC_DET) {
			sprintf(M, "%s", AKTFPAS_SEL_Items[(uint32_t)EpD[SET_BATT_DISC_DET][dev_setting_edit_mode].V1]);
			GLCD_PrintString(86, (i + 1) * 9, M);
		}
		else if (DEVICE_SETT_Items[dev_set_disp_index_].V1 == SET_OVTM_OPEN_DUR) {
			sprintf(M, "%03lusn", (uint32_t) EpD[SET_OVTM_OPEN_DUR][dev_setting_edit_mode].V1);
			GLCD_PrintString(97, (i + 1) * 9, M);
		}
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
//		if (fault_codes_reset_completed == 1) {
//			GLCD_PrintString(0, 48, "   Kayıtlar Silindi");
//		}
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




