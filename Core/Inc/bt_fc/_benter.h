void benter_fnc(void) {
    if (currentPage == HOME_PAGE_pg) {
        currentPage = MAIN_MENU_pg;
    }
    else if (currentPage == MAIN_MENU_pg) {
		if (selected_MAIN_MENU == 0) {
			currentPage = CHARGE_SETT_pg;
		} else if (selected_MAIN_MENU == 1) {
			currentPage = DEVICE_SETT_pg;
		} else if (selected_MAIN_MENU == 2) {
			currentPage = FAULT_CODES_REPORT_pg;
			flt_disp_index=(flt_array_index_last-5+NUM_FAULT_RECORD)%NUM_FAULT_RECORD;
			EpD[SET_UNSEEN_FLT][0].V1=0;
			Rec_Dat_to_EEp_f(SET_UNSEEN_FLT);
			there_is_past_unseen_fault=0;
		} else if (selected_MAIN_MENU == 3) {
			currentPage = MANAGEMENT_pg;
        }
    }
    else if (currentPage == CHARGE_SETT_pg) {

			chg_setting_edit_mode ^= 1;

			if (chg_setting_edit_mode) {
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
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==I_LIM_TO_FLOAT) {
	EpD[I_LIM_TO_FLOAT][1].V1=EpD[I_LIM_TO_FLOAT][0].V1;
	EpD[I_LIM_TO_BOOST][1].V1=EpD[I_LIM_TO_BOOST][0].V1;
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==I_LIM_TO_BOOST) {
	EpD[I_LIM_TO_FLOAT][1].V1=EpD[I_LIM_TO_FLOAT][0].V1;
	EpD[I_LIM_TO_BOOST][1].V1=EpD[I_LIM_TO_BOOST][0].V1;
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IRECT_CAL) {
	EpD[SET_IRECT_CAL][1].V1=EpD[SET_IRECT_CAL][0].V1;
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_VBAT_CAL) {
	EpD[SET_VBAT_CAL][1].V1=EpD[SET_VBAT_CAL][0].V1;
}
			} else if (!chg_setting_edit_mode) {
if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_CHARGE_MODE) {
	EpD[SET_CHARGE_MODE][0].V1=EpD[SET_CHARGE_MODE][1].V1;
	Rec_Dat_to_EEp_f(SET_CHARGE_MODE);
    actions_after_charge_mode_change(0);
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_BOOST_TIME) {
	EpD[SET_BOOST_TIME][0].V1=EpD[SET_BOOST_TIME][1].V1;
	Rec_Dat_to_EEp_f(SET_BOOST_TIME);
	charge_mode_timed_time_cnt=(uint32_t) (EpD[SET_BOOST_TIME][0].V1*60*1000/50);
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==VBAT_FLOAT) {
	EpD[VBAT_FLOAT][0].V1=EpD[VBAT_FLOAT][1].V1;
	Rec_Dat_to_EEp_f(VBAT_FLOAT);
	actions_after_charge_mode_change(1);
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==VBAT_BOOST) {
	EpD[VBAT_BOOST][0].V1=EpD[VBAT_BOOST][1].V1;
	Rec_Dat_to_EEp_f(VBAT_BOOST);
	actions_after_charge_mode_change(2);
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IBAT_FLOAT) {
	EpD[SET_IBAT_FLOAT][0].V1=EpD[SET_IBAT_FLOAT][1].V1;
	Rec_Dat_to_EEp_f(SET_IBAT_FLOAT);
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IBAT_BOOST) {
	EpD[SET_IBAT_BOOST][0].V1=EpD[SET_IBAT_BOOST][1].V1;
	Rec_Dat_to_EEp_f(SET_IBAT_BOOST);
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==I_LIM_TO_FLOAT) {
	EpD[I_LIM_TO_FLOAT][0].V1=EpD[I_LIM_TO_FLOAT][1].V1;
	EpD[I_LIM_TO_BOOST][0].V1=EpD[I_LIM_TO_BOOST][1].V1;
	Rec_Dat_to_EEp_f(I_LIM_TO_FLOAT);
	Rec_Dat_to_EEp_f(I_LIM_TO_BOOST);
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==I_LIM_TO_BOOST) {
	EpD[I_LIM_TO_FLOAT][0].V1=EpD[I_LIM_TO_FLOAT][1].V1;
	EpD[I_LIM_TO_BOOST][0].V1=EpD[I_LIM_TO_BOOST][1].V1;
	Rec_Dat_to_EEp_f(I_LIM_TO_FLOAT);
	Rec_Dat_to_EEp_f(I_LIM_TO_BOOST);
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_IRECT_CAL) {
	EpD[SET_IRECT_CAL][0].V1=EpD[SET_IRECT_CAL][1].V1;
	Rec_Dat_to_EEp_f(SET_IRECT_CAL);
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==SET_VBAT_CAL) {
	EpD[SET_VBAT_CAL][0].V1=EpD[SET_VBAT_CAL][1].V1;
	Rec_Dat_to_EEp_f(SET_VBAT_CAL);
}
			}
    }
    else if (currentPage == DEVICE_SETT_pg) {

		dev_setting_edit_mode ^= 1;

		if (dev_setting_edit_mode) {
if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==255) {
	currentPage = DATE_TIME_pg;
	while_LCD_delay_per=while_LCD_delay_clock_page_per;
	while_RTC_delay_per=while_RTC_delay_clock_page_per;
	dev_setting_edit_mode=0;
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_REV_DET) {
	EpD[SET_BATT_REV_DET][1].V1=EpD[SET_BATT_REV_DET][0].V1;
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==DEV_NOM_VOUT) {
	EpD[DEV_NOM_VOUT][1].V1=EpD[DEV_NOM_VOUT][0].V1;
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==IRECT_LIM_RT_) {
	EpD[IRECT_LIM_RT_][1].V1=EpD[IRECT_LIM_RT_][0].V1;
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==DC_KAC_POS) {
	EpD[DC_KAC_POS][1].V1=EpD[DC_KAC_POS][0].V1;
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==DC_KAC_NEG) {
	EpD[DC_KAC_NEG][1].V1=EpD[DC_KAC_NEG][0].V1;
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==RECT_SHORT) {
	EpD[RECT_SHORT][1].V1=EpD[RECT_SHORT][0].V1;
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==BATT_SHORT) {
	EpD[BATT_SHORT][1].V1=EpD[BATT_SHORT][0].V1;
}
		} else if (!dev_setting_edit_mode) {
if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==999) {
	EpD[SET_CHARGE_MODE][0].V1=EpD[SET_CHARGE_MODE][1].V1;
	Rec_Dat_to_EEp_f(SET_CHARGE_MODE);
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_REV_DET) {
	EpD[SET_BATT_REV_DET][0].V1=EpD[SET_BATT_REV_DET][1].V1;
	Rec_Dat_to_EEp_f(SET_BATT_REV_DET);
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==DEV_NOM_VOUT) {
	EpD[DEV_NOM_VOUT][0].V1=EpD[DEV_NOM_VOUT][1].V1;
	get_max_min_lims_from_DEV_NOM_VOUT();
	Rec_Dat_to_EEp_f(DEV_NOM_VOUT);
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==IRECT_LIM_RT_) {
	EpD[IRECT_LIM_RT_][0].V1=EpD[IRECT_LIM_RT_][1].V1;
	Rec_Dat_to_EEp_f(IRECT_LIM_RT_);
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==DC_KAC_POS) {
	EpD[DC_KAC_POS][0].V1=EpD[DC_KAC_POS][1].V1;
	Rec_Dat_to_EEp_f(DC_KAC_POS);
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==DC_KAC_NEG) {
	EpD[DC_KAC_NEG][0].V1=EpD[DC_KAC_NEG][1].V1;
	Rec_Dat_to_EEp_f(DC_KAC_NEG);
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==RECT_SHORT) {
	EpD[RECT_SHORT][0].V1=EpD[RECT_SHORT][1].V1;
	Rec_Dat_to_EEp_f(RECT_SHORT);
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==BATT_SHORT) {
	EpD[BATT_SHORT][0].V1=EpD[BATT_SHORT][1].V1;
	Rec_Dat_to_EEp_f(BATT_SHORT);
}
		}
    }
    else if (currentPage == MANAGEMENT_pg) {
		if (selected_MANAGEMENT == 0) {
			currentPage = FAULT_CODES_RESET_pg;
		}
		if (selected_MANAGEMENT == 1) {
			currentPage = DEVICE_RESET_pg;
		}
    }
    else if (currentPage == FAULT_CODES_RESET_pg) {
    	fault_codes_reset_req=1;
    	if (fault_codes_reset_req_right == 1) {
			for (int i = 0; i < 48; i++) { // 3/4 of memory is for fault codes. 48 of 64 blocks.
				SPI4_BlockErase(FAULT_RECORD_START_ADDRESS+i*64); // clear fault records
			}
				flt_array_index_next=0; // next fault to write index 0
				memset(array_fault_data, 0xffffffff, sizeof(array_fault_data));
				fault_codes_reset_completed=1;
				EpD[SET_UNSEEN_FLT][0].V1=0;
				Rec_Dat_to_EEp_f(SET_UNSEEN_FLT);
				there_is_past_unseen_fault=0;
				flt_array_index_last = 0;
				flt_disp_index=(flt_array_index_last-5+NUM_FAULT_RECORD)%NUM_FAULT_RECORD;
		}
    }
    else if (currentPage == DEVICE_RESET_pg) {
    	device_reset_req=1;
    	if (device_reset_req_right == 1) {
				NVIC_SystemReset();
		}
    }
    else if (currentPage == FAULT_CODES_REPORT_pg && FAULT_CODES_REPORT_disp_mode == 0) {
    	FAULT_CODES_REPORT_disp_mode = 1;
    }
    else if (currentPage == FAULT_CODES_REPORT_pg && FAULT_CODES_REPORT_disp_mode == 1) {
    	FAULT_CODES_REPORT_disp_mode = 0;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 0) {
    	rtc_sec_edit=rtc_sec_recv;
    	rtc_min_edit=rtc_min_recv;
    	rtc_hour_edit=rtc_hour_recv;
    	rtc_day_edit=rtc_day_recv;
    	rtc_month_edit=rtc_month_recv;
    	rtc_year_edit=rtc_year_recv;
    	DATE_TIME_edit_mode = 1;
    }
    else if (currentPage == DATE_TIME_pg && DATE_TIME_edit_mode == 1) {
    	DATE_TIME_edit_mode = 0;
    	Write_RTC_Time_Data_With_Oscillator();
    }
    SHOW_MENU_NOW
}
