
inline extern void HOME_PAGE_pg_disp(void) {
	if (HOME_PAGE_pg_sel==1) {
		char L[32]; char R[32]; char M[32];
//		sprintf(M, "NORMAL"); 					GLCD_PrintString(0, 0, M);
		sprintf(L, "VL%6.1f V", VLOAD_per_avg_sc); 			GLCD_PrintString(0, 9+1+1, L);
		sprintf(L, "IT%6.1f A", IRECT_per_sc); 			GLCD_PrintString(0, 18+1+1, L);
		sprintf(L, "VB%6.1f V", VBAT_per_avg_sc); 				GLCD_PrintString(0, 27+3+1, L);
		sprintf(L, "IB%6.1f A", IBAT_per_sc); 			GLCD_PrintString(0, 36+3+1, L);
		if (VDCK_side==1) {
		sprintf(L, "KP%6.1f %%", VDCK_perc); 		GLCD_PrintString(0, 45+3+4+1, L);
		}
		if (VDCK_side==-1) {
		sprintf(L, "KN%6.1f %%", VDCK_perc); 		GLCD_PrintString(0, 45+3+4+1, L);
		}

		sprintf(R, "VR%6.1f", VAC_R_roll_per_avg);	GLCD_PrintString(76, 9, R);
		sprintf(R, "VS%6.1f", VAC_S_roll_per_avg);	GLCD_PrintString(76, 18, R);
		sprintf(R, "VT%6.1f", VAC_T_roll_per_avg);	GLCD_PrintString(76, 27, R);
		sprintf(R, "IR 400.2");						GLCD_PrintString(76, 27+3+8, R);
		sprintf(R, "IS 400.5");						GLCD_PrintString(76, 36+3+8, R);
		sprintf(R, "IT 400.5");						GLCD_PrintString(76, 45+3+8, R);

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
			sprintf(M, "ARZK"); 		GLCD_PrintString(100, 0, M);
		}
	}
	else if (HOME_PAGE_pg_sel==2) {
		char b[8][32];
		sprintf(b[0], "V %5.1f >-D->%5.1f", VRECT_smp_sc, VLOAD_per_avg_sc);
		sprintf(b[1], "A %5.1f   |  %5.1f", IRECT_per_sc, ILOAD_per_sc);
		sprintf(b[2], "          | ");
		sprintf(b[3], "       BAT");
		sprintf(b[4], "     %5.1f V", VBAT_per_avg_sc);
		sprintf(b[5], "     %5.1f A", IBAT_per_sc);
		sprintf(b[6], " ");
		sprintf(b[7], "KÇK %5.1f", VDCK_sc);
		for (int i = 0; i < 8; ++i) {
			GLCD_DisplayString(0, i, b[i]);
		}
			uint32_t x0 = 0; uint32_t y0 = 0;
			uint32_t x1 = 0; uint32_t y1 = 0;

			x0 = 32;		y0 = 22;
			x1 = x0+50;		y1 = y0+26;
			GLCD_Line(x0, y0, x1, y0); // bat ust
			GLCD_Line(x0, y1, x1, y1); // bat alt
			GLCD_Line(x0, y0, x0, y1); // bat sol
			GLCD_Line(x1, y0, x1, y1); // bat sağ
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
		if (DEVICE_SETT_Items[dev_set_disp_index_].type == 99) {
			sprintf(M, " ");
			GLCD_PrintString(96, (i + 1) * 9, M);
		}
		else if (DEVICE_SETT_Items[dev_set_disp_index_].V1 == SET_BATT_REV_DET) {
			sprintf(M, "%s", AKTFPAS_SEL_Items[(uint32_t)EpD[SET_BATT_REV_DET][dev_set_disp_index_].V1]);
			GLCD_PrintString(86, (i + 1) * 9, M);
		}
		else if (DEVICE_SETT_Items[dev_set_disp_index_].type == 3) {
			sprintf(M, "%05.1f", EpD[DEVICE_SETT_Items[dev_set_disp_index_].V1][dev_setting_edit_mode].V1);
			GLCD_PrintString(98, (i + 1) * 9, M);
		}
		dev_set_disp_index_=(dev_set_disp_index_+1+NUM_DEVICE_SETT_ITEMS) % NUM_DEVICE_SETT_ITEMS;
		GLCD_PrintString(0, (i + 1) * 9, L);
	}
	GLCD_PrintString(0, (dev_set_arrow_loc+1) * 9, ">");
	GLCD_PrintString(90, (dev_set_arrow_loc+1) * 9, ">");

if (dev_setting_edit_mode) {
	GLCD_Line(88, (dev_set_arrow_loc+1)*9-1, 88, (dev_set_arrow_loc+2)*9-2);
	GLCD_Line(125, (dev_set_arrow_loc+1)*9-1, 125, (dev_set_arrow_loc+2)*9-2);
	GLCD_Line(88, (dev_set_arrow_loc+1)*9-2, 125, (dev_set_arrow_loc+1)*9-2);
	GLCD_Line(88, (dev_set_arrow_loc+2)*9-1, 125, (dev_set_arrow_loc+2)*9-1);
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

    	    if (array_fault_data[disp_index_][1] < NUM_FAULT_CODE_NAMES) {
        		sprintf(L, "%03d %s", disp_index_+1, faultList[array_fault_data[disp_index_][1]].name);
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




