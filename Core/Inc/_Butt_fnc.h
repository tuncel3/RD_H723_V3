
#include "_benter.h"

void bleft_fnc(void) {
    if (currentPage == HOME_PAGE_pg) {
    	if (thy_drv_en==0 && user_wants_allows_thy_drv==0) {
    		user_wants_allows_thy_drv=1;
    		thy_drv_en_req=1;
    		PRF_GEN("User req START rectf");
    	}
    }
    else if (currentPage == CHARGE_SETT_pg) {
    	if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_CHARGE_MODE) {
    		EpD[SET_CHARGE_MODE][1].V1=((uint32_t)EpD[SET_CHARGE_MODE][1].V1+1+NUM_CH_MOD_SEL_ITEMS) % NUM_CH_MOD_SEL_ITEMS;
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_BOOST_TIME) {
    		EpD[SET_BOOST_TIME][1].V1=(uint32_t)EpD[SET_BOOST_TIME][1].V1-5;
    		if (EpD[SET_BOOST_TIME][1].V1 <= BOOST_CHARGE_TIME_MIN) {
    			EpD[SET_BOOST_TIME][1].V1=BOOST_CHARGE_TIME_MIN;
    		}
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==VBAT_FLOAT) {
    		if (EpD[VBAT_FLOAT][1].V1 >= Vdc_float_min+1.0) {
    			EpD[VBAT_FLOAT][1].V1=EpD[VBAT_FLOAT][1].V1-1.0;
    		}
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==VBAT_BOOST) {
    		if (EpD[VBAT_BOOST][1].V1 >= Vdc_boost_min+1.0) {
    			EpD[VBAT_BOOST][1].V1=EpD[VBAT_BOOST][1].V1-1.0;
    		}
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IBAT_FLOAT) {
    		EpD[SET_IBAT_FLOAT][1].V1=EpD[SET_IBAT_FLOAT][1].V1-1.0;
    		if (EpD[SET_IBAT_FLOAT][1].V1 <= Ibat_min) {
    			EpD[SET_IBAT_FLOAT][1].V1=Ibat_min;
    		}
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IBAT_BOOST) {
    		EpD[SET_IBAT_BOOST][1].V1=EpD[SET_IBAT_BOOST][1].V1-1.0;
    		if (EpD[SET_IBAT_BOOST][1].V1 <= Ibat_min) {
    			EpD[SET_IBAT_BOOST][1].V1=Ibat_min;
    		}
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==IRECT_LIM_RT_) {
    		EpD[IRECT_LIM_RT_][1].V1=EpD[IRECT_LIM_RT_][1].V1-1.0;
    		if (EpD[IRECT_LIM_RT_][1].V1 <= Irect_min) {
    			EpD[IRECT_LIM_RT_][1].V1=Irect_min;
    		}
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==I_LIM_TO_FLOAT) {
    		EpD[I_LIM_TO_FLOAT][1].V1=EpD[I_LIM_TO_FLOAT][1].V1-1.0;
    		if (EpD[I_LIM_TO_FLOAT][1].V1 <= 0) {
    			EpD[I_LIM_TO_FLOAT][1].V1=0;
    		}
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==I_LIM_TO_BOOST) {
    		if (EpD[I_LIM_TO_BOOST][1].V1 >= 1.3) {
    			EpD[I_LIM_TO_BOOST][1].V1=EpD[I_LIM_TO_BOOST][1].V1-1;
    		}
    		if (EpD[I_LIM_TO_BOOST][1].V1 <= EpD[I_LIM_TO_FLOAT][1].V1+0.1) {
    			EpD[I_LIM_TO_FLOAT][1].V1=EpD[I_LIM_TO_BOOST][1].V1-0.1;
    		}
    	}
    }
    else if (currentPage == DEVICE_SETT_pg) {

    	if (!dev_setting_edit_mode) {
    		if (dev_set_arrow_loc > 1) {
    			dev_set_arrow_loc--;
    		} else if (dev_set_arrow_loc == 1) {
    			dev_set_disp_index=(dev_set_disp_index-1+NUM_DEVICE_SETT_ITEMS) % NUM_DEVICE_SETT_ITEMS;
    		}
    		selected_DEVICE_SETT=(dev_set_disp_index+dev_set_arrow_loc) % NUM_DEVICE_SETT_ITEMS;
    	} else {
			if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_REV_DET) {
				if (EpD[SET_BATT_REV_DET][1].V1==1) {
					EpD[SET_BATT_REV_DET][1].V1=0;
				} else { EpD[SET_BATT_REV_DET][1].V1=1;}
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_DISC_DET) {
				if (EpD[SET_BATT_DISC_DET][1].V1==1) {
					EpD[SET_BATT_DISC_DET][1].V1=0;
				} else { EpD[SET_BATT_DISC_DET][1].V1=1;}
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==DEV_NOM_VOUT) {
				EpD[DEV_NOM_VOUT][1].V1=EpD[DEV_NOM_VOUT][1].V1-1.0;
				if (EpD[DEV_NOM_VOUT][1].V1 <= 0)
					EpD[DEV_NOM_VOUT][1].V1=0;
	    	} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==IRECT_LIM_RT_) {
	    		EpD[IRECT_LIM_RT_][1].V1=EpD[IRECT_LIM_RT_][1].V1-1.0;
	    		if (EpD[IRECT_LIM_RT_][1].V1 <= Irect_min)
	    			EpD[IRECT_LIM_RT_][1].V1=Irect_min;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == DC_KAC_POS) {
			    EpD[DC_KAC_POS][1].V1=EpD[DC_KAC_POS][1].V1-1.0;
				if (EpD[DC_KAC_POS][1].V1 <= 0)
					EpD[DC_KAC_POS][1].V1=0;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == DC_KAC_NEG) {
			    EpD[DC_KAC_NEG][1].V1=EpD[DC_KAC_NEG][1].V1-1.0;
				if (EpD[DC_KAC_NEG][1].V1 <= 0)
					EpD[DC_KAC_NEG][1].V1=0;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == RECT_SHORT) {
			    EpD[RECT_SHORT][1].V1=EpD[RECT_SHORT][1].V1-1.0;
				if (EpD[RECT_SHORT][1].V1 <= 0)
					EpD[RECT_SHORT][1].V1=0;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == BATT_SHORT) {
			    EpD[BATT_SHORT][1].V1=EpD[BATT_SHORT][1].V1-1.0;
				if (EpD[BATT_SHORT][1].V1 <= 0)
					EpD[BATT_SHORT][1].V1=0;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == VRECT_DC_HIGH_LIM_add) {
			    EpD[VRECT_DC_HIGH_LIM_add][1].V1=EpD[VRECT_DC_HIGH_LIM_add][1].V1-1.0;
				if (EpD[VRECT_DC_HIGH_LIM_add][1].V1 <= 0)
					EpD[VRECT_DC_HIGH_LIM_add][1].V1=0;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == VRECT_DC_LOW_LIM_add) {
			    EpD[VRECT_DC_LOW_LIM_add][1].V1=EpD[VRECT_DC_LOW_LIM_add][1].V1-1.0;
				if (EpD[VRECT_DC_LOW_LIM_add][1].V1 <= 0)
					EpD[VRECT_DC_LOW_LIM_add][1].V1=0;
			}
    	}
    }
    else if (currentPage == DROPPER_pg) { // LEFT
    	if (!dropper_edit_mode) {
    		selected_DROPPER_PG_line=(selected_DROPPER_PG_line+1+5) % 5;
    	} else if (dropper_edit_mode && (selected_DROPPER_PG_line==3 || selected_DROPPER_PG_line==4)) {
    		drop_set_dig=(drop_set_dig-1+6) % 6;
    	}
    }
    else if (currentPage == FANS_TEMP_pg) { // LEFT
    	if (!fan_temp_edit_mode) {
    		selected_FAN_TEMP_PG_line=(selected_FAN_TEMP_PG_line-1+NUM_FANS_TEMP_ITEMS) % NUM_FANS_TEMP_ITEMS;
    	} else if (fan_temp_edit_mode) {
    		fan_temp_dig=(fan_temp_dig-1+3) % 3;
    	}
    }
    else if (currentPage == MANAGEMENT_pg) {
    	selected_MANAGEMENT=(selected_MANAGEMENT-1+NUM_MANAGEMENT_ITEMS) % NUM_MANAGEMENT_ITEMS;
    }
    else if (currentPage == CALIBRATION_pg) {
    	if (cal_sel_edit_mode == cal_none) {
			if (cal_sel_col==0) {
				cal_sel_col=1;
			}
			else if (cal_sel_col==1) {
				cal_sel_col=0;
			}
    	} else if (cal_sel_edit_mode != cal_none) {
    		cal_sel_digit=(cal_sel_digit+1)%2;
    	}

		if (cal_sel_edit_mode == cal_none) {
			if (cal_sel_col==0) {	// üstte cal sel col belirleniyor. burada da ona uygun olarak dikdörgenin yeri belirleniyor.
				cal_sel_item_left=cal_sel_item_right;	// soldaki neyse sağa geçince de o olmasını sağlıyor.
			}
			else if (cal_sel_col==1 && cal_sel_item_left < 3) {
				cal_sel_item_right=cal_sel_item_left;
			}
			else if (cal_sel_col==1 && cal_sel_item_left >= 3) {
				cal_sel_item_right=2;
			}
		}

    }
    else if (currentPage == FAULT_CODES_REPORT_pg) {
    	flt_disp_index=(flt_disp_index-5+NUM_FAULT_RECORD) % NUM_FAULT_RECORD;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1) {
    		DATE_TIME_edit_digit = ((DATE_TIME_edit_digit-1+12) % 12); // date time ayar satırı yatay hareket.
    }
    else if (currentPage == FAULT_CODES_RESET_pg && fault_codes_reset_req == 1 && !fault_codes_reset_completed) {
		if (fault_codes_reset_req_right == 0) {
			fault_codes_reset_req_right = 1;
		} else {
			fault_codes_reset_req_right = 0;
		}
    }
    else if (currentPage == RECTF_ACTIVE_AT_STARTUP_pg) {
    	rectf_active_at_startup_req_right^=1;
    }
    else if (currentPage == DEVICE_RESET_pg && device_reset_req == 1) {
		if (device_reset_req_right == 0) {
			device_reset_req_right = 1;
		} else {
			device_reset_req_right = 0;
		}
    }
    SHOW_MENU_NOW
}

void bright_fnc(void) {
    if (currentPage == HOME_PAGE_pg) {
    	if (thy_drv_en == 1 && user_wants_allows_thy_drv==1) {
    		thy_drv_en=0;
        	sfsta_op_phase = S_SFSTA_NONE;
        	blm_op_phase = B_RESTRT_AFTR_DELAY;
    		user_wants_allows_thy_drv=0;
    		apply_state_changes_f(STOP_FC, 1);
    		apply_state_changes_f(START_FC, 0);
    		PRF_GEN("User req STOP rectf");
    	}
    }
    else if (currentPage == CHARGE_SETT_pg) {
    	if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_CHARGE_MODE) {
    		EpD[SET_CHARGE_MODE][1].V1=((uint32_t)EpD[SET_CHARGE_MODE][1].V1-1+NUM_CH_MOD_SEL_ITEMS) % NUM_CH_MOD_SEL_ITEMS;
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_BOOST_TIME) {
    		EpD[SET_BOOST_TIME][1].V1=(uint32_t)EpD[SET_BOOST_TIME][1].V1+5;
    		if (EpD[SET_BOOST_TIME][1].V1 >= BOOST_CHARGE_TIME_MAX)
    			EpD[SET_BOOST_TIME][1].V1=BOOST_CHARGE_TIME_MAX;
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==VBAT_FLOAT) {
    		if (EpD[VBAT_FLOAT][1].V1 <= Vdc_float_max-1.1)
    			EpD[VBAT_FLOAT][1].V1=EpD[VBAT_FLOAT][1].V1+1.0;
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==VBAT_BOOST) {
    		if (EpD[VBAT_BOOST][1].V1 <= Vdc_boost_max-1.0)
    			EpD[VBAT_BOOST][1].V1=EpD[VBAT_BOOST][1].V1+1.0;
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IBAT_FLOAT) {
    		EpD[SET_IBAT_FLOAT][1].V1=EpD[SET_IBAT_FLOAT][1].V1+1.0;
    		if (EpD[SET_IBAT_FLOAT][1].V1 >= Ibat_max)
    			EpD[SET_IBAT_FLOAT][1].V1=Ibat_max;
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IBAT_BOOST) {
    		EpD[SET_IBAT_BOOST][1].V1=EpD[SET_IBAT_BOOST][1].V1+1.0;
    		if (EpD[SET_IBAT_BOOST][1].V1 >= Ibat_max)
    			EpD[SET_IBAT_BOOST][1].V1=Ibat_max;
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==I_LIM_TO_FLOAT) {
    		if (EpD[I_LIM_TO_FLOAT][1].V1 <= EpD[DEV_NOM_IOUT][1].V1-1.2) { // 0.2 altında tut. boost bunun 0.1 üstü olacak.
    			EpD[I_LIM_TO_FLOAT][1].V1=EpD[I_LIM_TO_FLOAT][1].V1+1;
    		}
    	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==I_LIM_TO_BOOST) {
    		EpD[I_LIM_TO_BOOST][1].V1=EpD[I_LIM_TO_BOOST][1].V1+1.0;
    		if (EpD[I_LIM_TO_BOOST][1].V1 >= EpD[DEV_NOM_IOUT][1].V1)
    			EpD[I_LIM_TO_BOOST][1].V1=EpD[DEV_NOM_IOUT][1].V1;
    	}
    }
    else if (currentPage == DEVICE_SETT_pg) {

    	if (!dev_setting_edit_mode) {
    		if (dev_set_arrow_loc > 1) {
    			dev_set_arrow_loc--;
    		} else if (dev_set_arrow_loc == 1) {
    			dev_set_disp_index=(dev_set_disp_index-1+NUM_DEVICE_SETT_ITEMS) % NUM_DEVICE_SETT_ITEMS;
    		}
    		selected_DEVICE_SETT=(dev_set_disp_index+dev_set_arrow_loc) % NUM_DEVICE_SETT_ITEMS;
    	} else {
			if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_REV_DET) {
				if (EpD[SET_BATT_REV_DET][1].V1==1) {
					EpD[SET_BATT_REV_DET][1].V1=0;
				} else { EpD[SET_BATT_REV_DET][1].V1=1;}
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_DISC_DET) {
				if (EpD[SET_BATT_DISC_DET][1].V1==1) {
					EpD[SET_BATT_DISC_DET][1].V1=0;
				} else { EpD[SET_BATT_DISC_DET][1].V1=1;}
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==DEV_NOM_VOUT) {
				EpD[DEV_NOM_VOUT][1].V1=EpD[DEV_NOM_VOUT][1].V1+1.0;
				if (EpD[DEV_NOM_VOUT][1].V1 >= VDC_NOM_MAX)
					EpD[DEV_NOM_VOUT][1].V1=VDC_NOM_MAX;
	    	} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==IRECT_LIM_RT_) {
	    		EpD[IRECT_LIM_RT_][1].V1=EpD[IRECT_LIM_RT_][1].V1+1.0;
	    		if (EpD[IRECT_LIM_RT_][1].V1 >= Irect_max)
	    			EpD[IRECT_LIM_RT_][1].V1=Irect_max;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == DC_KAC_POS) {
			    EpD[DC_KAC_POS][1].V1=EpD[DC_KAC_POS][1].V1+1.0;
				if (EpD[DC_KAC_POS][1].V1 >= DC_KAC_POS_MAX)
					EpD[DC_KAC_POS][1].V1=DC_KAC_POS_MAX;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == DC_KAC_NEG) {
			    EpD[DC_KAC_NEG][1].V1=EpD[DC_KAC_NEG][1].V1+1.0;
				if (EpD[DC_KAC_NEG][1].V1 >= DC_KAC_NEG_MAX)
					EpD[DC_KAC_NEG][1].V1=DC_KAC_NEG_MAX;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == RECT_SHORT) {
			    EpD[RECT_SHORT][1].V1=EpD[RECT_SHORT][1].V1+1.0;
				if (EpD[RECT_SHORT][1].V1 >= 999)
					EpD[RECT_SHORT][1].V1=999;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == BATT_SHORT) {
			    EpD[BATT_SHORT][1].V1=EpD[BATT_SHORT][1].V1+1.0;
				if (EpD[BATT_SHORT][1].V1 >= 999)
					EpD[BATT_SHORT][1].V1=999;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == VRECT_DC_HIGH_LIM_add) {
			    EpD[VRECT_DC_HIGH_LIM_add][1].V1=EpD[VRECT_DC_HIGH_LIM_add][1].V1+1.0;
				if (EpD[VRECT_DC_HIGH_LIM_add][1].V1 >= 50)
					EpD[VRECT_DC_HIGH_LIM_add][1].V1=50;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == VRECT_DC_LOW_LIM_add) {
			    EpD[VRECT_DC_LOW_LIM_add][1].V1=EpD[VRECT_DC_LOW_LIM_add][1].V1+1.0;
				if (EpD[VRECT_DC_LOW_LIM_add][1].V1 >= 50)
					EpD[VRECT_DC_LOW_LIM_add][1].V1=50;
			}
    	}
    }
    else if (currentPage == DROPPER_pg) { // RIGHT
    	if (!dropper_edit_mode) {
    		selected_DROPPER_PG_line=(selected_DROPPER_PG_line-1+5) % 5;
    	} else if (dropper_edit_mode && (selected_DROPPER_PG_line==3 || selected_DROPPER_PG_line==4)) {
    		drop_set_dig=(drop_set_dig+1+6) % 6;
    	}
    }
    else if (currentPage == FANS_TEMP_pg) { // RIGHT
    	if (!fan_temp_edit_mode) {
    		selected_FAN_TEMP_PG_line=(selected_FAN_TEMP_PG_line+1+NUM_FANS_TEMP_ITEMS) % NUM_FANS_TEMP_ITEMS;
    	} else if (fan_temp_edit_mode) {
    		fan_temp_dig=(fan_temp_dig+1+3) % 3;
    	}
    }
    else if (currentPage == MANAGEMENT_pg) {
    	selected_MANAGEMENT=(selected_MANAGEMENT-1+NUM_MANAGEMENT_ITEMS) % NUM_MANAGEMENT_ITEMS;
    }
    else if (currentPage == CALIBRATION_pg) {
    	if (cal_sel_edit_mode == cal_none) {
			if (cal_sel_col==0) {
				cal_sel_col=1;
			}
			else if (cal_sel_col==1) {
				cal_sel_col=0;
			}
    	} else if (cal_sel_edit_mode != cal_none) {
    		cal_sel_digit=(cal_sel_digit-1+2)%2;
    	}

		if (cal_sel_edit_mode == cal_none) {
			if (cal_sel_col==0) {	// üstte cal sel col belirleniyor. burada da ona uygun olarak dikdörgenin yeri belirleniyor.
				cal_sel_item_left=cal_sel_item_right;	// soldaki neyse sağa geçince de o olmasını sağlıyor.
			}
			else if (cal_sel_col==1 && cal_sel_item_left < 3) {
				cal_sel_item_right=cal_sel_item_left;
			}
			else if (cal_sel_col==1 && cal_sel_item_left >= 3) {
				cal_sel_item_right=2;
			}
		}

    }
    else if (currentPage == FAULT_CODES_REPORT_pg) {
    	flt_disp_index=(flt_disp_index+5+NUM_FAULT_RECORD) % NUM_FAULT_RECORD;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1) {
    	DATE_TIME_edit_digit = ((DATE_TIME_edit_digit+1+12) % 12);
    }
    else if (currentPage == FAULT_CODES_RESET_pg && fault_codes_reset_req == 1 && !fault_codes_reset_completed) {
		if (fault_codes_reset_req_right == 0) {
			fault_codes_reset_req_right = 1;
		} else {
			fault_codes_reset_req_right = 0;
		}
    }
    else if (currentPage == RECTF_ACTIVE_AT_STARTUP_pg) {
    	rectf_active_at_startup_req_right^=1;
    }
    else if (currentPage == DEVICE_RESET_pg && device_reset_req == 1) {
		if (device_reset_req_right == 0) {
			device_reset_req_right = 1;
		} else {
			device_reset_req_right = 0;
		}
    }
    SHOW_MENU_NOW
}

void bup_fnc(void) {
    if (currentPage == HOME_PAGE_pg && HOME_PAGE_pg_sel == 1) {
    	asm("NOP");
    }
    else if (currentPage == MAIN_MENU_pg) {
		main_menu_disp_index=(main_menu_disp_index-1+NUM_MAIN_MENU_ITEMS) % NUM_MAIN_MENU_ITEMS;
    	selected_MAIN_MENU=(main_menu_disp_index+main_menu_arrow_loc) % NUM_MAIN_MENU_ITEMS;
    }
    else if (currentPage == DEVICE_SETT_pg) {

    	if (!dev_setting_edit_mode) {
    		if (dev_set_arrow_loc > 1) {
    			dev_set_arrow_loc--;
    		} else if (dev_set_arrow_loc == 1) {
    			dev_set_disp_index=(dev_set_disp_index-1+NUM_DEVICE_SETT_ITEMS) % NUM_DEVICE_SETT_ITEMS;
    		}
    		selected_DEVICE_SETT=(dev_set_disp_index+dev_set_arrow_loc) % NUM_DEVICE_SETT_ITEMS;
    	} else {
			if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_REV_DET) {
				if (EpD[SET_BATT_REV_DET][1].V1==1) {
					EpD[SET_BATT_REV_DET][1].V1=0;
				} else { EpD[SET_BATT_REV_DET][1].V1=1;}
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_DISC_DET) {
				if (EpD[SET_BATT_DISC_DET][1].V1==1) {
					EpD[SET_BATT_DISC_DET][1].V1=0;
				} else { EpD[SET_BATT_DISC_DET][1].V1=1;}
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==DEV_NOM_VOUT) {
				EpD[DEV_NOM_VOUT][1].V1=EpD[DEV_NOM_VOUT][1].V1+0.1;
				if (EpD[DEV_NOM_VOUT][1].V1 >= VDC_NOM_MAX)
					EpD[DEV_NOM_VOUT][1].V1=VDC_NOM_MAX;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==IRECT_LIM_RT_) {
				EpD[IRECT_LIM_RT_][1].V1=EpD[IRECT_LIM_RT_][1].V1+0.1;
				if (EpD[IRECT_LIM_RT_][1].V1 >= Irect_max)
					EpD[IRECT_LIM_RT_][1].V1=Irect_max;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == DC_KAC_POS) {
			    EpD[DC_KAC_POS][1].V1=EpD[DC_KAC_POS][1].V1+0.1;
				if (EpD[DC_KAC_POS][1].V1 >= DC_KAC_POS_MAX)
					EpD[DC_KAC_POS][1].V1=DC_KAC_POS_MAX;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == DC_KAC_NEG) {
			    EpD[DC_KAC_NEG][1].V1=EpD[DC_KAC_NEG][1].V1+0.1;
				if (EpD[DC_KAC_NEG][1].V1 >= DC_KAC_NEG_MAX)
					EpD[DC_KAC_NEG][1].V1=DC_KAC_NEG_MAX;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == RECT_SHORT) {
			    EpD[RECT_SHORT][1].V1=EpD[RECT_SHORT][1].V1+0.1;
				if (EpD[RECT_SHORT][1].V1 >= 999)
					EpD[RECT_SHORT][1].V1=999;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == BATT_SHORT) {
			    EpD[BATT_SHORT][1].V1=EpD[BATT_SHORT][1].V1+0.1;
				if (EpD[BATT_SHORT][1].V1 >= 999)
					EpD[BATT_SHORT][1].V1=999;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == VRECT_DC_HIGH_LIM_add) {
			    EpD[VRECT_DC_HIGH_LIM_add][1].V1=EpD[VRECT_DC_HIGH_LIM_add][1].V1+0.1;
				if (EpD[VRECT_DC_HIGH_LIM_add][1].V1 >= 50)
					EpD[VRECT_DC_HIGH_LIM_add][1].V1=50;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == VRECT_DC_LOW_LIM_add) {
			    EpD[VRECT_DC_LOW_LIM_add][1].V1=EpD[VRECT_DC_LOW_LIM_add][1].V1+0.1;
				if (EpD[VRECT_DC_LOW_LIM_add][1].V1 >= 50)
					EpD[VRECT_DC_LOW_LIM_add][1].V1=50;
			}
    	}
    }
    else if (currentPage == CHARGE_SETT_pg) {
if (!chg_setting_edit_mode) {
	if (chg_set_arrow_loc > 1) {
		chg_set_arrow_loc--;
	} else if (chg_set_arrow_loc == 1) {
		chg_set_disp_index=(chg_set_disp_index-1+NUM_CHARGE_SETT_ITEMS) % NUM_CHARGE_SETT_ITEMS;
	}
	selected_CHARGE_SETT=(chg_set_disp_index+chg_set_arrow_loc) % NUM_CHARGE_SETT_ITEMS;
} else {
	if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_CHARGE_MODE) {
		EpD[SET_CHARGE_MODE][1].V1=((uint32_t)EpD[SET_CHARGE_MODE][1].V1-1+NUM_CH_MOD_SEL_ITEMS) % NUM_CH_MOD_SEL_ITEMS;
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_BOOST_TIME) {
		EpD[SET_BOOST_TIME][1].V1=(uint32_t)EpD[SET_BOOST_TIME][1].V1+1;
		if (EpD[SET_BOOST_TIME][1].V1 >= BOOST_CHARGE_TIME_MAX)
			EpD[SET_BOOST_TIME][1].V1=BOOST_CHARGE_TIME_MAX;
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==VBAT_FLOAT) {
		if (EpD[VBAT_FLOAT][1].V1 <= Vdc_float_max-0.2) {
			EpD[VBAT_FLOAT][1].V1=EpD[VBAT_FLOAT][1].V1+0.1;
		}
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==VBAT_BOOST) {
		if (EpD[VBAT_BOOST][1].V1 <= Vdc_boost_max-0.2) {
			EpD[VBAT_BOOST][1].V1=EpD[VBAT_BOOST][1].V1+0.1;
		}
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IBAT_FLOAT) {
		EpD[SET_IBAT_FLOAT][1].V1=EpD[SET_IBAT_FLOAT][1].V1+0.1;
		if (EpD[SET_IBAT_FLOAT][1].V1 >= Ibat_max)
			EpD[SET_IBAT_FLOAT][1].V1=Ibat_max;
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IBAT_BOOST) {
		EpD[SET_IBAT_BOOST][1].V1=EpD[SET_IBAT_BOOST][1].V1+0.1;
		if (EpD[SET_IBAT_BOOST][1].V1 >= Ibat_max)
			EpD[SET_IBAT_BOOST][1].V1=Ibat_max;
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==I_LIM_TO_FLOAT) {
		if (EpD[I_LIM_TO_FLOAT][1].V1 <= EpD[DEV_NOM_IOUT][1].V1-0.3) { // 0.2 altında tut. boost bunun 0.1 üstü olacak.
			EpD[I_LIM_TO_FLOAT][1].V1=EpD[I_LIM_TO_FLOAT][1].V1+0.1;
		}
		if (EpD[I_LIM_TO_FLOAT][1].V1 >= EpD[I_LIM_TO_BOOST][1].V1-0.1) { // boostu geçmemesi için boost u otomatik artır
			EpD[I_LIM_TO_BOOST][1].V1=EpD[I_LIM_TO_FLOAT][1].V1+0.1;
		}
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==I_LIM_TO_BOOST) {
		EpD[I_LIM_TO_BOOST][1].V1=EpD[I_LIM_TO_BOOST][1].V1+0.1;
		if (EpD[I_LIM_TO_BOOST][1].V1 >= EpD[DEV_NOM_IOUT][1].V1) {
			EpD[I_LIM_TO_BOOST][1].V1=EpD[DEV_NOM_IOUT][1].V1;
		}
	}
}
    }
    else if (currentPage == DROPPER_pg) { // UP
    	if (dropper_edit_mode == 0) {
    		selected_DROPPER_PG_line=(selected_DROPPER_PG_line-1+5) % 5;
		} else if (dropper_edit_mode == 1) {
			if (selected_DROPPER_PG_line == 0) {
			    EpD[SET_DROPPER_MANOTO][1].V1 = (float)(((int)(EpD[SET_DROPPER_MANOTO][1].V1)) ^ 1);
			} else if (selected_DROPPER_PG_line == 1) {
			    EpD[SET_DROPPER_K1][1].V1 = (float)(((int)(EpD[SET_DROPPER_K1][1].V1)) ^ 1);
			} else if (selected_DROPPER_PG_line == 2) {
			    EpD[SET_DROPPER_K2][1].V1 = (float)(((int)(EpD[SET_DROPPER_K2][1].V1)) ^ 1);
			} else if (selected_DROPPER_PG_line == 3) {
				static const float dropper_step_values[6] = {10.0f, 1.0f, 0.1f, 10.0f, 1.0f, 0.1f};
				if (drop_set_dig < 3) {
				    set_dropper_l_hg_V_h += dropper_step_values[drop_set_dig];
				    EpD[SET_DROPP_L_HG_PERC][dropper_edit_mode].V1=(set_dropper_l_hg_V_h/EpD[DEV_NOM_VOUT][0].V1-1)*100;
				} else {
				    EpD[SET_DROPP_L_HG_PERC][dropper_edit_mode].V1 += dropper_step_values[drop_set_dig];
				    set_dropper_l_hg_V_h=EpD[DEV_NOM_VOUT][0].V1 * (1 + (EpD[SET_DROPP_L_HG_PERC][dropper_edit_mode].V1 / 100));
				}
			} else if (selected_DROPPER_PG_line == 4) {
				static const float dropper_step_values[6] = {10.0f, 1.0f, 0.1f, 10.0f, 1.0f, 0.1f};

				if (drop_set_dig < 3) {
				    set_dropper_l_lw_V_h += dropper_step_values[drop_set_dig];
				    EpD[SET_DROPP_L_LW_PERC][dropper_edit_mode].V1=-(set_dropper_l_lw_V_h/EpD[DEV_NOM_VOUT][0].V1-1)*100;
				} else {
				    EpD[SET_DROPP_L_LW_PERC][dropper_edit_mode].V1 += dropper_step_values[drop_set_dig];
				    set_dropper_l_lw_V_h=EpD[DEV_NOM_VOUT][0].V1 * (1 - (EpD[SET_DROPP_L_LW_PERC][dropper_edit_mode].V1 / 100));
				}

			}
		}





    }
    else if (currentPage == FANS_TEMP_pg) { // UP
    	static const float fan_temp_step_values[3] = {100.0f, 10.0f, 1.0f};
		if (!fan_temp_edit_mode) {
    		selected_FAN_TEMP_PG_line=(selected_FAN_TEMP_PG_line-1+NUM_FANS_TEMP_ITEMS) % NUM_FANS_TEMP_ITEMS;
		} else if (fan_temp_edit_mode) {
			if (selected_FAN_TEMP_PG_line == 0) {
				EpD[SET_COOL_FAN_TEMP][1].V1 += fan_temp_step_values[fan_temp_dig];
			}
		}

    }

    else if (currentPage == RELAY_ORDER_pg) {

        if (rel_disp_mode == 1) {
			if (rel_ord_arrow_loc > 1) {
				rel_ord_arrow_loc--;
			} else if (rel_ord_arrow_loc == 1) {
				rel_ord_disp_index = (rel_ord_disp_index - 1 + rel_ord_tb_size) % rel_ord_tb_size;
			}
			rel_ord_tb_sel = (rel_ord_arrow_loc + rel_ord_disp_index) % rel_ord_tb_size;

        } else if (rel_edit_mode == 1) {

			if (rel_dat_arrow_loc > 1) {
				rel_dat_arrow_loc--;
			} else if (rel_dat_arrow_loc == 1) {
				rel_dat_disp_index = (rel_dat_disp_index - 1 + rel_dat_tb_size) % rel_dat_tb_size;
			}
			rel_dat_tb_sel = (rel_dat_arrow_loc + rel_dat_disp_index) % rel_dat_tb_size;
//			if (rel_dat_tb_sel == rel_dat_tb_size-1) rel_dat_tb_sel=rel_dat_tb_size-2;
        }
    }

    else if (currentPage == MANAGEMENT_pg) {
    	selected_MANAGEMENT=(selected_MANAGEMENT-1+NUM_MANAGEMENT_ITEMS) % NUM_MANAGEMENT_ITEMS;
    }
    else if (currentPage == CALIBRATION_pg) {
    	if (cal_sel_edit_mode == cal_none) {
			if (cal_sel_col==0) {
				cal_sel_item_left=((cal_sel_item_left-1+5)%5);
			}
			else if (cal_sel_col==1) {
				cal_sel_item_right=((cal_sel_item_right-1+3)%3);
			}
    	}
    	if (cal_sel_col==0) {
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==0 && cal_sel_digit==0) {
				EpD[SET_VRECT_CAL][0].V1=EpD[SET_VRECT_CAL][0].V1*1.0002;
				EpD[SET_VRECT_CAL][1].V1=EpD[SET_VRECT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==0 && cal_sel_digit==1) {
				EpD[SET_VRECT_CAL][0].V1=EpD[SET_VRECT_CAL][0].V1*1.005;
				EpD[SET_VRECT_CAL][1].V1=EpD[SET_VRECT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==0 && cal_sel_digit==0) {
				EpD[SET_VRECT_OFFS_CAL][0].V1=EpD[SET_VRECT_OFFS_CAL][0].V1+1;
				EpD[SET_VRECT_OFFS_CAL][1].V1=EpD[SET_VRECT_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==0 && cal_sel_digit==1) {
				EpD[SET_VRECT_OFFS_CAL][0].V1=EpD[SET_VRECT_OFFS_CAL][0].V1+10;
				EpD[SET_VRECT_OFFS_CAL][1].V1=EpD[SET_VRECT_OFFS_CAL][0].V1;
			}
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==1 && cal_sel_digit==0) {
				EpD[SET_VLOAD_CAL][0].V1=EpD[SET_VLOAD_CAL][0].V1*1.0002;
				EpD[SET_VLOAD_CAL][1].V1=EpD[SET_VLOAD_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==1 && cal_sel_digit==1) {
				EpD[SET_VLOAD_CAL][0].V1=EpD[SET_VLOAD_CAL][0].V1*1.005;
				EpD[SET_VLOAD_CAL][1].V1=EpD[SET_VLOAD_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==1 && cal_sel_digit==0) {
				EpD[SET_VLOAD_OFFS_CAL][0].V1=EpD[SET_VLOAD_OFFS_CAL][0].V1+1;
				EpD[SET_VLOAD_OFFS_CAL][1].V1=EpD[SET_VLOAD_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==1 && cal_sel_digit==1) {
				EpD[SET_VLOAD_OFFS_CAL][0].V1=EpD[SET_VLOAD_OFFS_CAL][0].V1+10;
				EpD[SET_VLOAD_OFFS_CAL][1].V1=EpD[SET_VLOAD_OFFS_CAL][0].V1;
			}
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==2 && cal_sel_digit==0) {
				EpD[SET_VBAT_CAL][0].V1=EpD[SET_VBAT_CAL][0].V1*1.0002;
				EpD[SET_VBAT_CAL][1].V1=EpD[SET_VBAT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==2 && cal_sel_digit==1) {
				EpD[SET_VBAT_CAL][0].V1=EpD[SET_VBAT_CAL][0].V1*1.005;
				EpD[SET_VBAT_CAL][1].V1=EpD[SET_VBAT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==2 && cal_sel_digit==0) {
				EpD[SET_VBAT_OFFS_CAL][0].V1=EpD[SET_VBAT_OFFS_CAL][0].V1+1;
				EpD[SET_VBAT_OFFS_CAL][1].V1=EpD[SET_VBAT_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==2 && cal_sel_digit==1) {
				EpD[SET_VBAT_OFFS_CAL][0].V1=EpD[SET_VBAT_OFFS_CAL][0].V1+10;
				EpD[SET_VBAT_OFFS_CAL][1].V1=EpD[SET_VBAT_OFFS_CAL][0].V1;
			}
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==3 && cal_sel_digit==0) {
				EpD[SET_IRECT_CAL][0].V1=EpD[SET_IRECT_CAL][0].V1*1.0002;
				EpD[SET_IRECT_CAL][1].V1=EpD[SET_IRECT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==3 && cal_sel_digit==1) {
				EpD[SET_IRECT_CAL][0].V1=EpD[SET_IRECT_CAL][0].V1*1.005;
				EpD[SET_IRECT_CAL][1].V1=EpD[SET_IRECT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==3 && cal_sel_digit==0) {
				EpD[SET_IRECT_OFFS_CAL][0].V1=EpD[SET_IRECT_OFFS_CAL][0].V1+1;
				EpD[SET_IRECT_OFFS_CAL][1].V1=EpD[SET_IRECT_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==3 && cal_sel_digit==1) {
				EpD[SET_IRECT_OFFS_CAL][0].V1=EpD[SET_IRECT_OFFS_CAL][0].V1+10;
				EpD[SET_IRECT_OFFS_CAL][1].V1=EpD[SET_IRECT_OFFS_CAL][0].V1;
			}
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==4 && cal_sel_digit==0) {
				EpD[SET_IBAT_CAL][0].V1=EpD[SET_IBAT_CAL][0].V1*1.0002;
				EpD[SET_IBAT_CAL][1].V1=EpD[SET_IBAT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==4 && cal_sel_digit==1) {
				EpD[SET_IBAT_CAL][0].V1=EpD[SET_IBAT_CAL][0].V1*1.005;
				EpD[SET_IBAT_CAL][1].V1=EpD[SET_IBAT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==4 && cal_sel_digit==0) {
				EpD[SET_IBAT_OFFS_CAL][0].V1=EpD[SET_IBAT_OFFS_CAL][0].V1+1;
				EpD[SET_IBAT_OFFS_CAL][1].V1=EpD[SET_IBAT_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==4 && cal_sel_digit==1) {
				EpD[SET_IBAT_OFFS_CAL][0].V1=EpD[SET_IBAT_OFFS_CAL][0].V1+10;
				EpD[SET_IBAT_OFFS_CAL][1].V1=EpD[SET_IBAT_OFFS_CAL][0].V1;
			}
    	}
    	else if (cal_sel_col==1) {
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_right==0 && cal_sel_digit==0) {
				EpD[SET_ACR_CAL][0].V1=EpD[SET_ACR_CAL][0].V1*1.0002;
				EpD[SET_ACR_CAL][1].V1=EpD[SET_ACR_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_right==0 && cal_sel_digit==1) {
				EpD[SET_ACR_CAL][0].V1=EpD[SET_ACR_CAL][0].V1*1.005;
				EpD[SET_ACR_CAL][1].V1=EpD[SET_ACR_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_right==0 && cal_sel_digit==0) {
				EpD[SET_ACR_OFFS_CAL][0].V1=EpD[SET_ACR_OFFS_CAL][0].V1+1;
				EpD[SET_ACR_OFFS_CAL][1].V1=EpD[SET_ACR_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_right==0 && cal_sel_digit==1) {
				EpD[SET_ACR_OFFS_CAL][0].V1=EpD[SET_ACR_OFFS_CAL][0].V1+10;
				EpD[SET_ACR_OFFS_CAL][1].V1=EpD[SET_ACR_OFFS_CAL][0].V1;
			}
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_right==1 && cal_sel_digit==0) {
				EpD[SET_ACS_CAL][0].V1=EpD[SET_ACS_CAL][0].V1*1.0002;
				EpD[SET_ACS_CAL][1].V1=EpD[SET_ACS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_right==1 && cal_sel_digit==1) {
				EpD[SET_ACS_CAL][0].V1=EpD[SET_ACS_CAL][0].V1*1.005;
				EpD[SET_ACS_CAL][1].V1=EpD[SET_ACS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_right==1 && cal_sel_digit==0) {
				EpD[SET_ACS_OFFS_CAL][0].V1=EpD[SET_ACS_OFFS_CAL][0].V1+1;
				EpD[SET_ACS_OFFS_CAL][1].V1=EpD[SET_ACS_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_right==1 && cal_sel_digit==1) {
				EpD[SET_ACS_OFFS_CAL][0].V1=EpD[SET_ACS_OFFS_CAL][0].V1+10;
				EpD[SET_ACS_OFFS_CAL][1].V1=EpD[SET_ACS_OFFS_CAL][0].V1;
			}
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_right==2 && cal_sel_digit==0) {
				EpD[SET_ACT_CAL][0].V1=EpD[SET_ACT_CAL][0].V1*1.0002;
				EpD[SET_ACT_CAL][1].V1=EpD[SET_ACT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_right==2 && cal_sel_digit==1) {
				EpD[SET_ACT_CAL][0].V1=EpD[SET_ACT_CAL][0].V1*1.005;
				EpD[SET_ACT_CAL][1].V1=EpD[SET_ACT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_right==2 && cal_sel_digit==0) {
				EpD[SET_ACT_OFFS_CAL][0].V1=EpD[SET_ACT_OFFS_CAL][0].V1+1;
				EpD[SET_ACT_OFFS_CAL][1].V1=EpD[SET_ACT_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_right==2 && cal_sel_digit==1) {
				EpD[SET_ACT_OFFS_CAL][0].V1=EpD[SET_ACT_OFFS_CAL][0].V1+10;
				EpD[SET_ACT_OFFS_CAL][1].V1=EpD[SET_ACT_OFFS_CAL][0].V1;
			}
		}


    }
    else if (currentPage == FAULT_CODES_REPORT_pg) {
    	flt_disp_index=(flt_disp_index-1+NUM_FAULT_RECORD) % NUM_FAULT_RECORD;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 11) {
    	rtc_sec_edit = (rtc_sec_edit+1+60) % 60;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 10) {
    	rtc_sec_edit = (rtc_sec_edit+10+60) % 60;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 9) {
    	rtc_min_edit = (rtc_min_edit+1+60) % 60;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 8) {
    	rtc_min_edit = (rtc_min_edit+10+60) % 60;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 7) {
    	rtc_hour_edit = (rtc_hour_edit+1+24) % 24;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 6) {
    	rtc_hour_edit = (rtc_hour_edit+10+24) % 24;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 5) {
    	rtc_year_edit = (rtc_year_edit+1+60) % 60;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 4 ) {
    	rtc_year_edit = (rtc_year_edit+10+60) % 60;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 3) {
    	rtc_month_edit = (rtc_month_edit+1+12) % 12;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 2 ) {
    	rtc_month_edit = (rtc_month_edit+10+12) % 12;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 1) {
    	rtc_day_edit = (rtc_day_edit+1+31) % 31;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 0 ) {
    	rtc_day_edit = (rtc_day_edit+10+31) % 31;
    }
    SHOW_MENU_NOW
}

void bdown_fnc(void) {
    if (currentPage == HOME_PAGE_pg && HOME_PAGE_pg_sel == 1) {
    	HOME_PAGE_pg_sel = 2;
    }
    else if (currentPage == HOME_PAGE_pg && HOME_PAGE_pg_sel == 2) {
    	HOME_PAGE_pg_sel = 1;
    }
    else if (currentPage == MAIN_MENU_pg) {
		main_menu_disp_index=(main_menu_disp_index+1+NUM_MAIN_MENU_ITEMS) % NUM_MAIN_MENU_ITEMS;
    	selected_MAIN_MENU=(selected_MAIN_MENU+1+NUM_MAIN_MENU_ITEMS) % NUM_MAIN_MENU_ITEMS;
    }
    else if (currentPage == DEVICE_SETT_pg) {


    	if (!dev_setting_edit_mode) {
    		if (dev_set_arrow_loc < 4) {
    			dev_set_arrow_loc++;
    		} else if (dev_set_arrow_loc == 4) {
    			dev_set_disp_index=(dev_set_disp_index+1+NUM_DEVICE_SETT_ITEMS) % NUM_DEVICE_SETT_ITEMS;
    		}
    		selected_DEVICE_SETT=(dev_set_disp_index+dev_set_arrow_loc) % NUM_DEVICE_SETT_ITEMS;
    	} else {
    		if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_REV_DET) {
    			if (EpD[SET_BATT_REV_DET][1].V1==1) {
    				EpD[SET_BATT_REV_DET][1].V1=0;
    			} else { EpD[SET_BATT_REV_DET][1].V1=1;}
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_DISC_DET) {
				if (EpD[SET_BATT_DISC_DET][1].V1==1) {
					EpD[SET_BATT_DISC_DET][1].V1=0;
				} else { EpD[SET_BATT_DISC_DET][1].V1=1;}
    		} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==DEV_NOM_VOUT) {
    			EpD[DEV_NOM_VOUT][1].V1=EpD[DEV_NOM_VOUT][1].V1-0.1;
    			if (EpD[DEV_NOM_VOUT][1].V1 <= VDC_NOM_MIN) {
    				EpD[DEV_NOM_VOUT][1].V1=VDC_NOM_MIN;}
    		} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==IRECT_LIM_RT_) {
    			EpD[IRECT_LIM_RT_][1].V1=EpD[IRECT_LIM_RT_][1].V1-0.1;
    			if (EpD[IRECT_LIM_RT_][1].V1 <= Irect_min)
    				EpD[IRECT_LIM_RT_][1].V1=Irect_min;
    		} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == DC_KAC_POS) {
			    EpD[DC_KAC_POS][1].V1=EpD[DC_KAC_POS][1].V1-0.1;
				if (EpD[DC_KAC_POS][1].V1 <= DC_KAC_POS_MIN)
					EpD[DC_KAC_POS][1].V1=DC_KAC_POS_MIN;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == DC_KAC_NEG) {
			    EpD[DC_KAC_NEG][1].V1=EpD[DC_KAC_NEG][1].V1-0.1;
				if (EpD[DC_KAC_NEG][1].V1 <= DC_KAC_NEG_MIN)
					EpD[DC_KAC_NEG][1].V1=DC_KAC_NEG_MIN;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == RECT_SHORT) {
			    EpD[RECT_SHORT][1].V1=EpD[RECT_SHORT][1].V1-0.1;
				if (EpD[RECT_SHORT][1].V1 <= 0)
					EpD[RECT_SHORT][1].V1=0;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == BATT_SHORT) {
			    EpD[BATT_SHORT][1].V1=EpD[BATT_SHORT][1].V1-0.1;
				if (EpD[BATT_SHORT][1].V1 <= 0)
					EpD[BATT_SHORT][1].V1=0;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == VRECT_DC_HIGH_LIM_add) {
			    EpD[VRECT_DC_HIGH_LIM_add][1].V1=EpD[VRECT_DC_HIGH_LIM_add][1].V1-0.1;
				if (EpD[VRECT_DC_HIGH_LIM_add][1].V1 <= 0)
					EpD[VRECT_DC_HIGH_LIM_add][1].V1=0;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == VRECT_DC_LOW_LIM_add) {
			    EpD[VRECT_DC_LOW_LIM_add][1].V1=EpD[VRECT_DC_LOW_LIM_add][1].V1-0.1;
				if (EpD[VRECT_DC_LOW_LIM_add][1].V1 <= 0)
					EpD[VRECT_DC_LOW_LIM_add][1].V1=0;
			}
    	}
    }
    else if (currentPage == CHARGE_SETT_pg) {

if (!chg_setting_edit_mode) {
	if (chg_set_arrow_loc < 4) {
		chg_set_arrow_loc++;
	} else if (chg_set_arrow_loc == 4) {
		chg_set_disp_index=(chg_set_disp_index+1+NUM_CHARGE_SETT_ITEMS) % NUM_CHARGE_SETT_ITEMS;
	}
	selected_CHARGE_SETT=(chg_set_disp_index+chg_set_arrow_loc) % NUM_CHARGE_SETT_ITEMS;
} else {
	if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_CHARGE_MODE) {
		EpD[SET_CHARGE_MODE][1].V1=((uint32_t)EpD[SET_CHARGE_MODE][1].V1+1+NUM_CH_MOD_SEL_ITEMS) % NUM_CH_MOD_SEL_ITEMS;
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_BOOST_TIME) {
		EpD[SET_BOOST_TIME][1].V1=(uint32_t)EpD[SET_BOOST_TIME][1].V1-1;
		if (EpD[SET_BOOST_TIME][1].V1 <= BOOST_CHARGE_TIME_MIN)
			EpD[SET_BOOST_TIME][1].V1=BOOST_CHARGE_TIME_MIN;
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==VBAT_FLOAT) {
		if (EpD[VBAT_FLOAT][1].V1 >= Vdc_float_min+0.1) {
			EpD[VBAT_FLOAT][1].V1=EpD[VBAT_FLOAT][1].V1-0.1;
		}
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==VBAT_BOOST) {
		if (EpD[VBAT_BOOST][1].V1 >= Vdc_boost_min+0.1) {
			EpD[VBAT_BOOST][1].V1=EpD[VBAT_BOOST][1].V1-0.1;
		}
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IBAT_FLOAT) {
		EpD[SET_IBAT_FLOAT][1].V1=EpD[SET_IBAT_FLOAT][1].V1-0.1;
		if (EpD[SET_IBAT_FLOAT][1].V1 <= Ibat_min)
			EpD[SET_IBAT_FLOAT][1].V1=Ibat_min;
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IBAT_BOOST) {
		EpD[SET_IBAT_BOOST][1].V1=EpD[SET_IBAT_BOOST][1].V1-0.1;
		if (EpD[SET_IBAT_BOOST][1].V1 <= Ibat_min)
			EpD[SET_IBAT_BOOST][1].V1=Ibat_min;
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==I_LIM_TO_FLOAT) {
		EpD[I_LIM_TO_FLOAT][1].V1=EpD[I_LIM_TO_FLOAT][1].V1-0.1;
		if (EpD[I_LIM_TO_FLOAT][1].V1 <= 0)
			EpD[I_LIM_TO_FLOAT][1].V1=0;
	} else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==I_LIM_TO_BOOST) {
		if (EpD[I_LIM_TO_BOOST][1].V1 >= 0.3) {  // 0.2 ye eşit ve büyük olmalı ki float bunun 0.1 altında tutulurken 0 ın altına inmesin
			EpD[I_LIM_TO_BOOST][1].V1=EpD[I_LIM_TO_BOOST][1].V1-0.1;
		}
		if (EpD[I_LIM_TO_BOOST][1].V1 <= EpD[I_LIM_TO_FLOAT][1].V1+0.1) { // float ı boost un 0.1 altında tut
			EpD[I_LIM_TO_FLOAT][1].V1=EpD[I_LIM_TO_BOOST][1].V1-0.1;
		}
	}
}
    }
    else if (currentPage == DROPPER_pg) { // DOWN
		if (dropper_edit_mode == 0) {
			selected_DROPPER_PG_line=(selected_DROPPER_PG_line+1+5) % 5;
		} else if (dropper_edit_mode == 1) {
			if (selected_DROPPER_PG_line == 0) {
			    EpD[SET_DROPPER_MANOTO][1].V1 = (float)(((int)(EpD[SET_DROPPER_MANOTO][1].V1)) ^ 1);
			} else if (selected_DROPPER_PG_line == 1) {
			    EpD[SET_DROPPER_K1][1].V1 = (float)(((int)(EpD[SET_DROPPER_K1][1].V1)) ^ 1);
			} else if (selected_DROPPER_PG_line == 2) {
			    EpD[SET_DROPPER_K2][1].V1 = (float)(((int)(EpD[SET_DROPPER_K2][1].V1)) ^ 1);
			} else if (selected_DROPPER_PG_line == 3) {static const float dropper_step_values[6] = {10.0f, 1.0f, 0.1f, 10.0f, 1.0f, 0.1f};

			if (drop_set_dig < 3) {
			    set_dropper_l_hg_V_h -= dropper_step_values[drop_set_dig];
				EpD[SET_DROPP_L_HG_PERC][dropper_edit_mode].V1=(set_dropper_l_hg_V_h/EpD[DEV_NOM_VOUT][0].V1-1)*100;
			} else {
			    EpD[SET_DROPP_L_HG_PERC][dropper_edit_mode].V1 -= dropper_step_values[drop_set_dig];
			    set_dropper_l_hg_V_h=EpD[DEV_NOM_VOUT][0].V1 * (1 + (EpD[SET_DROPP_L_HG_PERC][dropper_edit_mode].V1 / 100));
			}
			} else if (selected_DROPPER_PG_line == 4) {static const float dropper_step_values[6] = {10.0f, 1.0f, 0.1f, 10.0f, 1.0f, 0.1f};

			if (drop_set_dig < 3) {
			    set_dropper_l_lw_V_h -= dropper_step_values[drop_set_dig];
				EpD[SET_DROPP_L_LW_PERC][dropper_edit_mode].V1=-(set_dropper_l_lw_V_h/EpD[DEV_NOM_VOUT][0].V1-1)*100;
			} else {
			    EpD[SET_DROPP_L_LW_PERC][dropper_edit_mode].V1 -= dropper_step_values[drop_set_dig];
			    set_dropper_l_lw_V_h=EpD[DEV_NOM_VOUT][0].V1 * (1 - (EpD[SET_DROPP_L_LW_PERC][dropper_edit_mode].V1 / 100));
			}
			}
		}
	}
    else if (currentPage == FANS_TEMP_pg) { // DOWN
    	static const float fan_temp_step_values[3] = {100.0f, 10.0f, 1.0f};
		if (!fan_temp_edit_mode) {
    		selected_FAN_TEMP_PG_line=(selected_FAN_TEMP_PG_line+1+NUM_FANS_TEMP_ITEMS) % NUM_FANS_TEMP_ITEMS;
		} else if (fan_temp_edit_mode) {
			if (selected_FAN_TEMP_PG_line == 0) {
				EpD[SET_COOL_FAN_TEMP][1].V1 -= fan_temp_step_values[fan_temp_dig];
			}
		}

    }

    else if (currentPage == RELAY_ORDER_pg) {

        if (rel_disp_mode == 1) {
			if (rel_ord_arrow_loc < 4) {
				rel_ord_arrow_loc++;
			} else if (rel_ord_arrow_loc == 4) {
				rel_ord_disp_index = (rel_ord_disp_index + 1 + rel_ord_tb_size) % rel_ord_tb_size;
			}
			rel_ord_tb_sel = (rel_ord_arrow_loc + rel_ord_disp_index) % rel_ord_tb_size;

        } else if (rel_edit_mode == 1) {

			if (rel_dat_arrow_loc < 4) {
				rel_dat_arrow_loc++;
			} else if (rel_dat_arrow_loc == 4) {
				rel_dat_disp_index = (rel_dat_disp_index + 1 + rel_dat_tb_size) % rel_dat_tb_size;
			}
			rel_dat_tb_sel = (rel_dat_arrow_loc + rel_dat_disp_index) % rel_dat_tb_size;
//			if (rel_dat_tb_sel == rel_dat_tb_size-1) rel_dat_tb_sel=0;

        }
    }

    else if (currentPage == MANAGEMENT_pg) {
    	selected_MANAGEMENT=(selected_MANAGEMENT+1) % NUM_MANAGEMENT_ITEMS;
    }
    else if (currentPage == CALIBRATION_pg) {
    	if (cal_sel_edit_mode == cal_none) {
			if (cal_sel_col==0) {
				cal_sel_item_left=(cal_sel_item_left+1)%5;
			}
			else if (cal_sel_col==1) {
				cal_sel_item_right=(cal_sel_item_right+1)%3;
			}
    	}

    	if (cal_sel_col==0) {
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==0 && cal_sel_digit==0) {
				EpD[SET_VRECT_CAL][0].V1=EpD[SET_VRECT_CAL][0].V1/1.0002;
				EpD[SET_VRECT_CAL][1].V1=EpD[SET_VRECT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==0 && cal_sel_digit==1) {
				EpD[SET_VRECT_CAL][0].V1=EpD[SET_VRECT_CAL][0].V1/1.005;
				EpD[SET_VRECT_CAL][1].V1=EpD[SET_VRECT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==0 && cal_sel_digit==0) {
				EpD[SET_VRECT_OFFS_CAL][0].V1=EpD[SET_VRECT_OFFS_CAL][0].V1-1;
				EpD[SET_VRECT_OFFS_CAL][1].V1=EpD[SET_VRECT_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==0 && cal_sel_digit==1) {
				EpD[SET_VRECT_OFFS_CAL][0].V1=EpD[SET_VRECT_OFFS_CAL][0].V1-10;
				EpD[SET_VRECT_OFFS_CAL][1].V1=EpD[SET_VRECT_OFFS_CAL][0].V1;
			}
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==1 && cal_sel_digit==0) {
				EpD[SET_VLOAD_CAL][0].V1=EpD[SET_VLOAD_CAL][0].V1/1.0002;
				EpD[SET_VLOAD_CAL][1].V1=EpD[SET_VLOAD_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==1 && cal_sel_digit==1) {
				EpD[SET_VLOAD_CAL][0].V1=EpD[SET_VLOAD_CAL][0].V1/1.005;
				EpD[SET_VLOAD_CAL][1].V1=EpD[SET_VLOAD_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==1 && cal_sel_digit==0) {
				EpD[SET_VLOAD_OFFS_CAL][0].V1=EpD[SET_VLOAD_OFFS_CAL][0].V1-1;
				EpD[SET_VLOAD_OFFS_CAL][1].V1=EpD[SET_VLOAD_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==1 && cal_sel_digit==1) {
				EpD[SET_VLOAD_OFFS_CAL][0].V1=EpD[SET_VLOAD_OFFS_CAL][0].V1-10;
				EpD[SET_VLOAD_OFFS_CAL][1].V1=EpD[SET_VLOAD_OFFS_CAL][0].V1;
			}
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==2 && cal_sel_digit==0) {
				EpD[SET_VBAT_CAL][0].V1=EpD[SET_VBAT_CAL][0].V1/1.0002;
				EpD[SET_VBAT_CAL][1].V1=EpD[SET_VBAT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==2 && cal_sel_digit==1) {
				EpD[SET_VBAT_CAL][0].V1=EpD[SET_VBAT_CAL][0].V1/1.005;
				EpD[SET_VBAT_CAL][1].V1=EpD[SET_VBAT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==2 && cal_sel_digit==0) {
				EpD[SET_VBAT_OFFS_CAL][0].V1=EpD[SET_VBAT_OFFS_CAL][0].V1-1;
				EpD[SET_VBAT_OFFS_CAL][1].V1=EpD[SET_VBAT_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==2 && cal_sel_digit==1) {
				EpD[SET_VBAT_OFFS_CAL][0].V1=EpD[SET_VBAT_OFFS_CAL][0].V1-10;
				EpD[SET_VBAT_OFFS_CAL][1].V1=EpD[SET_VBAT_OFFS_CAL][0].V1;
			}
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==3 && cal_sel_digit==0) {
				EpD[SET_IRECT_CAL][0].V1=EpD[SET_IRECT_CAL][0].V1/1.0002;
				EpD[SET_IRECT_CAL][1].V1=EpD[SET_IRECT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==3 && cal_sel_digit==1) {
				EpD[SET_IRECT_CAL][0].V1=EpD[SET_IRECT_CAL][0].V1/1.005;
				EpD[SET_IRECT_CAL][1].V1=EpD[SET_IRECT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==3 && cal_sel_digit==0) {
				EpD[SET_IRECT_OFFS_CAL][0].V1=EpD[SET_IRECT_OFFS_CAL][0].V1-1;
				EpD[SET_IRECT_OFFS_CAL][1].V1=EpD[SET_IRECT_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==3 && cal_sel_digit==1) {
				EpD[SET_IRECT_OFFS_CAL][0].V1=EpD[SET_IRECT_OFFS_CAL][0].V1-10;
				EpD[SET_IRECT_OFFS_CAL][1].V1=EpD[SET_IRECT_OFFS_CAL][0].V1;
			}
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==4 && cal_sel_digit==0) {
				EpD[SET_IBAT_CAL][0].V1=EpD[SET_IBAT_CAL][0].V1/1.0002;
				EpD[SET_IBAT_CAL][1].V1=EpD[SET_IBAT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_left==4 && cal_sel_digit==1) {
				EpD[SET_IBAT_CAL][0].V1=EpD[SET_IBAT_CAL][0].V1/1.005;
				EpD[SET_IBAT_CAL][1].V1=EpD[SET_IBAT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==4 && cal_sel_digit==0) {
				EpD[SET_IBAT_OFFS_CAL][0].V1=EpD[SET_IBAT_OFFS_CAL][0].V1-1;
				EpD[SET_IBAT_OFFS_CAL][1].V1=EpD[SET_IBAT_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_left==4 && cal_sel_digit==1) {
				EpD[SET_IBAT_OFFS_CAL][0].V1=EpD[SET_IBAT_OFFS_CAL][0].V1-10;
				EpD[SET_IBAT_OFFS_CAL][1].V1=EpD[SET_IBAT_OFFS_CAL][0].V1;
			}
    	}
    	else if (cal_sel_col==1) {
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_right==0 && cal_sel_digit==0) {
				EpD[SET_ACR_CAL][0].V1=EpD[SET_ACR_CAL][0].V1/1.0002;
				EpD[SET_ACR_CAL][1].V1=EpD[SET_ACR_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_right==0 && cal_sel_digit==1) {
				EpD[SET_ACR_CAL][0].V1=EpD[SET_ACR_CAL][0].V1/1.005;
				EpD[SET_ACR_CAL][1].V1=EpD[SET_ACR_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_right==0 && cal_sel_digit==0) {
				EpD[SET_ACR_OFFS_CAL][0].V1=EpD[SET_ACR_OFFS_CAL][0].V1-1;
				EpD[SET_ACR_OFFS_CAL][1].V1=EpD[SET_ACR_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_right==0 && cal_sel_digit==1) {
				EpD[SET_ACR_OFFS_CAL][0].V1=EpD[SET_ACR_OFFS_CAL][0].V1-10;
				EpD[SET_ACR_OFFS_CAL][1].V1=EpD[SET_ACR_OFFS_CAL][0].V1;
			}
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_right==1 && cal_sel_digit==0) {
				EpD[SET_ACS_CAL][0].V1=EpD[SET_ACS_CAL][0].V1/1.0002;
				EpD[SET_ACS_CAL][1].V1=EpD[SET_ACS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_right==1 && cal_sel_digit==1) {
				EpD[SET_ACS_CAL][0].V1=EpD[SET_ACS_CAL][0].V1/1.005;
				EpD[SET_ACS_CAL][1].V1=EpD[SET_ACS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_right==1 && cal_sel_digit==0) {
				EpD[SET_ACS_OFFS_CAL][0].V1=EpD[SET_ACS_OFFS_CAL][0].V1-1;
				EpD[SET_ACS_OFFS_CAL][1].V1=EpD[SET_ACS_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_right==1 && cal_sel_digit==1) {
				EpD[SET_ACS_OFFS_CAL][0].V1=EpD[SET_ACS_OFFS_CAL][0].V1-10;
				EpD[SET_ACS_OFFS_CAL][1].V1=EpD[SET_ACS_OFFS_CAL][0].V1;
			}
			if (cal_sel_edit_mode == cal_gain && cal_sel_item_right==2 && cal_sel_digit==0) {
				EpD[SET_ACT_CAL][0].V1=EpD[SET_ACT_CAL][0].V1/1.0002;
				EpD[SET_ACT_CAL][1].V1=EpD[SET_ACT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_gain && cal_sel_item_right==2 && cal_sel_digit==1) {
				EpD[SET_ACT_CAL][0].V1=EpD[SET_ACT_CAL][0].V1/1.005;
				EpD[SET_ACT_CAL][1].V1=EpD[SET_ACT_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_right==2 && cal_sel_digit==0) {
				EpD[SET_ACT_OFFS_CAL][0].V1=EpD[SET_ACT_OFFS_CAL][0].V1-1;
				EpD[SET_ACT_OFFS_CAL][1].V1=EpD[SET_ACT_OFFS_CAL][0].V1;
			}
			else if (cal_sel_edit_mode == cal_offset && cal_sel_item_right==2 && cal_sel_digit==1) {
				EpD[SET_ACT_OFFS_CAL][0].V1=EpD[SET_ACT_OFFS_CAL][0].V1-10;
				EpD[SET_ACT_OFFS_CAL][1].V1=EpD[SET_ACT_OFFS_CAL][0].V1;
			}
		}


    }
    else if (currentPage == FAULT_CODES_REPORT_pg) {
    	flt_disp_index=(flt_disp_index+1+NUM_FAULT_RECORD) % NUM_FAULT_RECORD;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 11) {
        	rtc_sec_edit = (rtc_sec_edit-1+60) % 60;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 10) {
    	rtc_sec_edit = (rtc_sec_edit-10+60) % 60;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 9) {
    	rtc_min_edit = (rtc_min_edit-1+60) % 60;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 8) {
    	rtc_min_edit = (rtc_min_edit-10+60) % 60;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 7) {
    	rtc_hour_edit = (rtc_hour_edit-1+24) % 24;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 6) {
    	rtc_hour_edit = (rtc_hour_edit-10+24) % 24;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 5) {
    	rtc_year_edit = (rtc_year_edit-1+60) % 60;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 4 ) {
    	rtc_year_edit = (rtc_year_edit-10+60) % 60;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 3) {
    	rtc_month_edit = (rtc_month_edit-1+12) % 12;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 2) {
    	rtc_month_edit = (rtc_month_edit-10+12) % 12;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 1) {
    	rtc_day_edit = (rtc_day_edit-1+31) % 31;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1 && DATE_TIME_edit_digit == 0) {
    	rtc_day_edit = (rtc_day_edit-10+31) % 31;
    }
    SHOW_MENU_NOW
}


void besc_fnc(void) {
    if (currentPage == HOME_PAGE_pg) {
    	asm("NOP");
    }
    else if (currentPage == MAIN_MENU_pg) {
        currentPage = HOME_PAGE_pg;
    }
    else if (currentPage == FANS_TEMP_pg) {
        currentPage = HOME_PAGE_pg;
    }
    else if (currentPage == CHARGE_SETT_pg) {
    	if (chg_setting_edit_mode) {
    		chg_setting_edit_mode = 0;

		if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_CHARGE_MODE) {
			EpD[SET_CHARGE_MODE][1].V1=EpD[SET_CHARGE_MODE][0].V1;
		}
		else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_BOOST_TIME) {
			EpD[SET_BOOST_TIME][1].V1=EpD[SET_BOOST_TIME][0].V1;
		}
		else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==VBAT_FLOAT) {
			EpD[VBAT_FLOAT][1].V1=EpD[VBAT_FLOAT][0].V1;
		}
		else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==VBAT_BOOST) {
			EpD[VBAT_BOOST][1].V1=EpD[VBAT_BOOST][0].V1;
		}
		else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IBAT_FLOAT) {
			EpD[SET_IBAT_FLOAT][1].V1=EpD[SET_IBAT_FLOAT][0].V1;
		}
		else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IBAT_BOOST) {
			EpD[SET_IBAT_BOOST][1].V1=EpD[SET_IBAT_BOOST][0].V1;
		}
		else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_VBAT_CAL) {
			EpD[SET_VBAT_CAL][1].V1=EpD[SET_VBAT_CAL][0].V1;
		}
		else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==IRECT_LIM_RT_) {
			EpD[IRECT_LIM_RT_][1].V1=EpD[IRECT_LIM_RT_][0].V1;
		}
    	}
    	else if (!chg_setting_edit_mode) {
	        currentPage = MAIN_MENU_pg;
    	}
    }
    else if (currentPage == DEVICE_SETT_pg) {
    	if (dev_setting_edit_mode) {
    		dev_setting_edit_mode = 0;
    		if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_REV_DET) {
    			EpD[SET_BATT_REV_DET][1].V1=EpD[SET_BATT_REV_DET][0].V1;
    		} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==DEV_NOM_VOUT) {
    			EpD[DEV_NOM_VOUT][1].V1=EpD[DEV_NOM_VOUT][0].V1;
    		} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_IRECT_CAL) {
				EpD[SET_IRECT_CAL][1].V1=EpD[SET_IRECT_CAL][0].V1;
    		} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == DC_KAC_POS) {
			    EpD[DC_KAC_POS][1].V1=EpD[DC_KAC_POS][0].V1;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == DC_KAC_NEG) {
			    EpD[DC_KAC_NEG][1].V1=EpD[DC_KAC_NEG][0].V1;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == RECT_SHORT) {
			    EpD[RECT_SHORT][1].V1=EpD[RECT_SHORT][0].V1;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == BATT_SHORT) {
			    EpD[BATT_SHORT][1].V1=EpD[BATT_SHORT][0].V1;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == VRECT_DC_HIGH_LIM_add) {
			    EpD[VRECT_DC_HIGH_LIM_add][1].V1=EpD[VRECT_DC_HIGH_LIM_add][0].V1;
			} else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1 == VRECT_DC_LOW_LIM_add) {
			    EpD[VRECT_DC_LOW_LIM_add][1].V1=EpD[VRECT_DC_LOW_LIM_add][0].V1;
			}
    	} else {
    		currentPage = MAIN_MENU_pg;
    	}
    }
    else if (currentPage == DROPPER_pg) { // ESC
        if (dropper_edit_mode) {
        	dropper_edit_mode=0;
    		if (selected_DROPPER_PG_line==0) {
    			EpD[SET_DROPPER_MANOTO][1].V1=EpD[SET_DROPPER_MANOTO][0].V1;
    		} else if (selected_DROPPER_PG_line==1) {
    			EpD[SET_DROPPER_K1][1].V1=EpD[SET_DROPPER_K1][0].V1;
    		} else if (selected_DROPPER_PG_line==2) {
				EpD[SET_DROPPER_K2][1].V1=EpD[SET_DROPPER_K2][0].V1;
			} else if (selected_DROPPER_PG_line == 3) {
				if (drop_set_dig==0) {
					set_dropper_l_hg_V_h=EpD[SET_DROPP_L_LW_PERC][0].V1;
				} else if (drop_set_dig==1) {
					set_dropper_l_hg_V_h=EpD[SET_DROPP_L_LW_PERC][0].V1;
				} else if (drop_set_dig==2) {
					set_dropper_l_hg_V_h=EpD[SET_DROPP_L_LW_PERC][0].V1;
				} else if (drop_set_dig==3) {
					EpD[SET_DROPP_L_HG_PERC][dropper_edit_mode].V1=EpD[SET_DROPP_L_HG_PERC][0].V1;
				} else if (drop_set_dig==4) {
					EpD[SET_DROPP_L_HG_PERC][dropper_edit_mode].V1=EpD[SET_DROPP_L_HG_PERC][0].V1;
				} else if (drop_set_dig==5) {
					EpD[SET_DROPP_L_HG_PERC][dropper_edit_mode].V1=EpD[SET_DROPP_L_HG_PERC][0].V1;
				}
			}
        } else {
        	currentPage = MAIN_MENU_pg;
        }
    }
    else if (currentPage == RELAY_ORDER_pg) {
        if (rel_disp_mode) {
        	rel_disp_mode = 1;
        	rel_edit_mode = 0;
        	currentPage = MAIN_MENU_pg;
        } else if (rel_edit_mode) {
        	rel_disp_mode = 1;
        	rel_edit_mode = 0;
        }
    }
    else if (currentPage == MANAGEMENT_pg) {
        currentPage = MAIN_MENU_pg;
    }
    else if (currentPage == CALIBRATION_pg) {
        currentPage = MANAGEMENT_pg;
		bat_inspection_allowed=1;
    }
    else if (currentPage == FAULT_CODES_REPORT_pg) {
        currentPage = MAIN_MENU_pg;
        FAULT_CODES_REPORT_disp_mode=0;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 0) {
		currentPage = DEVICE_SETT_pg;
		while_LCD_delay_per=while_LCD_delay_normal_per;
		while_RTC_delay_per=while_RTC_delay_normal_per;
	}
    else if (currentPage == FAULT_CODES_RESET_pg) {
        if (fault_codes_reset_completed) {
        	fault_codes_reset_completed = 0;
        	fault_codes_reset_req = 0;
        	fault_codes_reset_req_right = 0;
            currentPage = MANAGEMENT_pg;
        }
        else if (fault_codes_reset_req) {
        	fault_codes_reset_req_right = 0;
        	fault_codes_reset_req = 0;
        }
        else if (!fault_codes_reset_req) {
        	currentPage = MANAGEMENT_pg;
        }
    }
    else if (currentPage == RECTF_ACTIVE_AT_STARTUP_pg) {
        currentPage = MANAGEMENT_pg;
    }
    else if (currentPage == DEVICE_RESET_pg) {
        currentPage = MANAGEMENT_pg;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1) {
    	DATE_TIME_edit_mode = 0;
    }
    SHOW_MENU_NOW
}












