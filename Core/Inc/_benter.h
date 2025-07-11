void benter_fnc(void) {
	management_menu_en_cnt=0;
	management_menu_dis_cnt=0;
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
			currentPage = DROPPER_pg;
		} else if (selected_MAIN_MENU == 4) {
			currentPage = FANS_TEMP_pg;
		} else if (selected_MAIN_MENU == 5) {
			currentPage = RELAY_ORDER_pg;
        } else if (selected_MAIN_MENU == 6 && management_menu_en) {
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
	actions_after_charge_voltage_change();
		blm_op_phase = B_SKIP_DELAY_RESTART;					// cancel op. bring_vtarg_back_skip_delay
		blm_enable_collect_samples = 0;
		blm_corr_buf_index = 0;
}
else if (CHARGE_SETT_Items[selected_CHARGE_SETT].V1==VBAT_BOOST) {
	EpD[VBAT_BOOST][0].V1=EpD[VBAT_BOOST][1].V1;
	Rec_Dat_to_EEp_f(VBAT_BOOST);
	actions_after_charge_voltage_change();
		blm_op_phase = B_SKIP_DELAY_RESTART;					// cancel op. bring_vtarg_back_skip_delay
		blm_enable_collect_samples = 0;
		blm_corr_buf_index = 0;
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
	dev_setting_edit_mode=0;
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_REV_DET) {
	EpD[SET_BATT_REV_DET][1].V1=EpD[SET_BATT_REV_DET][0].V1;
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_DISC_CHK_EN) {
	EpD[SET_BATT_DISC_CHK_EN][1].V1=EpD[SET_BATT_DISC_CHK_EN][0].V1;
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
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==VRECT_DC_HIGH_LIM_add) {
	EpD[VRECT_DC_HIGH_LIM_add][1].V1=EpD[VRECT_DC_HIGH_LIM_add][0].V1;
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==VRECT_DC_LOW_LIM_add) {
	EpD[VRECT_DC_LOW_LIM_add][1].V1=EpD[VRECT_DC_LOW_LIM_add][0].V1;
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
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==SET_BATT_DISC_CHK_EN) {
	EpD[SET_BATT_DISC_CHK_EN][0].V1=EpD[SET_BATT_DISC_CHK_EN][1].V1;
	Rec_Dat_to_EEp_f(SET_BATT_DISC_CHK_EN);
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==DEV_NOM_VOUT) {
	EpD[DEV_NOM_VOUT][0].V1=EpD[DEV_NOM_VOUT][1].V1;
	DEV_NOM_VOUT_changed_fc();
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
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==VRECT_DC_HIGH_LIM_add) {
	EpD[VRECT_DC_HIGH_LIM_add][0].V1=EpD[VRECT_DC_HIGH_LIM_add][1].V1;
	vrect_dc_high_lim=targ_DC_voltage*(1+(EpD[VRECT_DC_HIGH_LIM_add][0].V1/100));
	vrect_dc_high_lim_ret=targ_DC_voltage*(1+(EpD[VRECT_DC_HIGH_LIM_add][0].V1/100)-0.01);
	Rec_Dat_to_EEp_f(VRECT_DC_HIGH_LIM_add);
}
else if (DEVICE_SETT_Items[selected_DEVICE_SETT].V1==VRECT_DC_LOW_LIM_add) {
	EpD[VRECT_DC_LOW_LIM_add][0].V1=EpD[VRECT_DC_LOW_LIM_add][1].V1;
	vrect_dc_low_lim=targ_DC_voltage/(1+(EpD[VRECT_DC_LOW_LIM_add][0].V1/100));
	vrect_dc_low_lim_ret=targ_DC_voltage/(1+(EpD[VRECT_DC_LOW_LIM_add][0].V1/100)-0.01);
	Rec_Dat_to_EEp_f(VRECT_DC_LOW_LIM_add);
}
		}
    }
    else if (currentPage == DROPPER_pg) {
    	dropper_edit_mode ^= 1;
		if (dropper_edit_mode) {
			if (selected_DROPPER_PG_line == 1) {
				EpD[SET_DROPPER_MANOTO][1].V1=EpD[SET_DROPPER_MANOTO][0].V1;
			}
			if (selected_DROPPER_PG_line == 1) {
				EpD[SET_DROPPER_K1][1].V1=EpD[SET_DROPPER_K1][0].V1;
			} else if (selected_DROPPER_PG_line == 2) {
				EpD[SET_DROPPER_K2][1].V1=EpD[SET_DROPPER_K2][0].V1;
			} else if (selected_DROPPER_PG_line == 3) {
				dropper_edit_blink=1; // edit cursor blink oluyor
					set_dropper_l_hg_V_h=set_dropper_l_hg_V;
					EpD[SET_DROPP_L_HG_PERC][1].V1=EpD[SET_DROPP_L_HG_PERC][0].V1;
			} else if (selected_DROPPER_PG_line == 4) {
				dropper_edit_blink=1;
					set_dropper_l_lw_V_h=set_dropper_l_lw_V;
					EpD[SET_DROPP_L_LW_PERC][1].V1=EpD[SET_DROPP_L_LW_PERC][0].V1;
			}
		} else if (!dropper_edit_mode) {
			if (selected_DROPPER_PG_line == 0) {
				EpD[SET_DROPPER_MANOTO][0].V1 = EpD[SET_DROPPER_MANOTO][1].V1;
				dropper_control_man_oto=EpD[SET_DROPPER_MANOTO][0].V1;
				Rec_Dat_to_EEp_f(SET_DROPPER_MANOTO);
			} else if (selected_DROPPER_PG_line == 1) {
				EpD[SET_DROPPER_K1][0].V1 = EpD[SET_DROPPER_K1][1].V1;
				DROPP_BATT_CTRL(EpD[SET_DROPPER_K1][0].V1);
				change_state_f(ST_DROPPER_K2, EpD[SET_DROPPER_K1][0].V1);
				Rec_Dat_to_EEp_f(SET_DROPPER_K1);
			} else if (selected_DROPPER_PG_line == 2) {
				EpD[SET_DROPPER_K2][0].V1 = EpD[SET_DROPPER_K2][1].V1;
				DROPP_LOAD_CTRL(EpD[SET_DROPPER_K2][0].V1);
				change_state_f(ST_DROPPER_K1, EpD[SET_DROPPER_K2][0].V1);
				Rec_Dat_to_EEp_f(SET_DROPPER_K2);
			} else if (selected_DROPPER_PG_line == 3) {
				set_dropper_l_hg_V=set_dropper_l_hg_V_h;
				EpD[SET_DROPP_L_HG_PERC][0].V1=EpD[SET_DROPP_L_HG_PERC][1].V1;
				Rec_Dat_to_EEp_f(SET_DROPP_L_HG_PERC);
			} else if (selected_DROPPER_PG_line == 4) {
				set_dropper_l_lw_V=set_dropper_l_lw_V_h;
				EpD[SET_DROPP_L_LW_PERC][0].V1=EpD[SET_DROPP_L_LW_PERC][1].V1;
				Rec_Dat_to_EEp_f(SET_DROPP_L_LW_PERC);
			}
		}
	}
    else if (currentPage == FANS_TEMP_pg) {
    	fan_temp_edit_mode ^= 1;
		if (fan_temp_edit_mode) {
			if (selected_FAN_TEMP_PG_line == 0) {
				EpD[SET_COOL_FAN_TEMP][1].V1 = EpD[SET_COOL_FAN_TEMP][0].V1;
			} else if (selected_FAN_TEMP_PG_line == 1) {
				EpD[SET_TRANSF_FAN_TEMP][1].V1 = EpD[SET_TRANSF_FAN_TEMP][0].V1;
			} else if (selected_FAN_TEMP_PG_line == 2) {
				EpD[SET_OVERTEMP_ALARM][1].V1 = EpD[SET_OVERTEMP_ALARM][0].V1;
			} else if (selected_FAN_TEMP_PG_line == 3) {
				EpD[SET_OVERTEMP_OPEN][1].V1 = EpD[SET_OVERTEMP_OPEN][0].V1;
			} else if (selected_FAN_TEMP_PG_line == 4) {
				EpD[SET_OVT_OPEN_DELAY][1].V1 = EpD[SET_OVT_OPEN_DELAY][0].V1;
				ovtmp_open_per=(uint32_t) (EpD[SET_OVT_OPEN_DELAY][0].V1*1000/50); // calculate alarm to open duration in 50ms
			}
			fan_temp_edit_blink=1;
		} else if (!fan_temp_edit_mode) {
			if (selected_FAN_TEMP_PG_line == 0) {
				EpD[SET_COOL_FAN_TEMP][0].V1 = EpD[SET_COOL_FAN_TEMP][1].V1;
				Rec_Dat_to_EEp_f(SET_COOL_FAN_TEMP);
			} else if (selected_FAN_TEMP_PG_line == 1) {
				EpD[SET_TRANSF_FAN_TEMP][0].V1 = EpD[SET_TRANSF_FAN_TEMP][1].V1;
				Rec_Dat_to_EEp_f(SET_TRANSF_FAN_TEMP);
			} else if (selected_FAN_TEMP_PG_line == 2) {
				EpD[SET_OVERTEMP_ALARM][0].V1 = EpD[SET_OVERTEMP_ALARM][1].V1;
				Rec_Dat_to_EEp_f(SET_OVERTEMP_ALARM);
			} else if (selected_FAN_TEMP_PG_line == 3) {
				EpD[SET_OVERTEMP_OPEN][0].V1 = EpD[SET_OVERTEMP_OPEN][1].V1;
				Rec_Dat_to_EEp_f(SET_OVERTEMP_OPEN);
			} else if (selected_FAN_TEMP_PG_line == 4) {
				EpD[SET_OVT_OPEN_DELAY][0].V1 = EpD[SET_OVT_OPEN_DELAY][1].V1;
				Rec_Dat_to_EEp_f(SET_OVT_OPEN_DELAY);
			}
		}
    }
    else if (currentPage == RELAY_ORDER_pg) {
    	rel_disp_mode ^=1;
    	rel_edit_mode ^=1;

        if (rel_disp_mode == 1) { // ekranda order değiştirildi. tabloda değiş
        	SIRALI_TABLO_RELOUT[sirali_tablo_sel].tbl_code=TAM_TABLO_RELOUT[tam_tablo_sel].tbl_code;
        	SIRALI_TABLO_RELOUT_process(); // bu state in değerini gir ve rel_out_16Bit_Data'yı güncelle
        	save_SIRALI_TABLO_RELOUT_to_eep(); //sıkıştırılmış orderı 20 bitlik parçalara ayır ve eeproma kaydet
        } else if (rel_edit_mode == 1) {
        }
    }

    else if (currentPage == MANAGEMENT_pg) {
		if (selected_MANAGEMENT == 0) {
			currentPage = FAULT_CODES_RESET_pg;
		}
		if (selected_MANAGEMENT == 1) {
			currentPage = DEVICE_RESET_pg;
		}
		if (selected_MANAGEMENT == 2) {
			currentPage = RECTF_ACTIVE_AT_STARTUP_pg;
		}
		if (selected_MANAGEMENT == 3) {
			currentPage = CALIBRATION_1_pg;
//			EpD[SET_BATT_DISC_CHK_EN][0].V1=0;
			blm_pause=1;
		}
		if (selected_MANAGEMENT == 4) {
			currentPage = TEST_pg;
		}
    }
    else if (currentPage == CALIBRATION_2_pg) {
		cal_pg2_sel_edit_mode=(cal_pg2_sel_edit_mode+1)%2;
		if (cal_pg2_sel_edit_mode == cal_offset) { // yani enter a basılmış ve gain den offset edit moduna geçilmiş. yani gain değerinin kaydedilmesi lazım.
			if (cal_pg2_sel_col==0) {
				if (cal_pg2_sel_item_left==0) {
					Rec_Dat_to_EEp_f(SET_FRQ_CAL);
				}
			}
		}
		if (cal_pg2_sel_edit_mode == cal_none) { // yani enter a basılmış ve offset den no edit moduna geçilmiş. yani offset değerinin kaydedilmesi lazım.
			if (cal_pg2_sel_col==0) {
				if (cal_pg2_sel_item_left==0) {
					Rec_Dat_to_EEp_f(SET_FRQ_CAL);
				}
			}
		}
    }
    else if (currentPage == CALIBRATION_1_pg) {
    	cal_pg1_sel_edit_mode=(cal_pg1_sel_edit_mode+1)%3;

		if (cal_pg1_sel_edit_mode == cal_offset) { // yani enter a basılmış ve gain den offset edit moduna geçilmiş. yani gain değerinin kaydedilmesi lazım.
			if (cal_pg1_sel_col==0) {
				if (cal_pg1_sel_item_left==0) {
					Rec_Dat_to_EEp_f(SET_VRECT_CAL);
				}
				else if (cal_pg1_sel_item_left==1) {
					Rec_Dat_to_EEp_f(SET_VLOAD_CAL);
				}
				else if (cal_pg1_sel_item_left==2) {
					Rec_Dat_to_EEp_f(SET_VBAT_CAL);
				}
				else if (cal_pg1_sel_item_left==3) {
					Rec_Dat_to_EEp_f(SET_IRECT_CAL);
				}
				else if (cal_pg1_sel_item_left==4) {
					Rec_Dat_to_EEp_f(SET_IBAT_CAL);
				}
			}
			if (cal_pg1_sel_col==1) {
				if (cal_pg1_sel_item_right==0) {
					Rec_Dat_to_EEp_f(SET_ACR_CAL);
				}
				else if (cal_pg1_sel_item_right==1) {
					Rec_Dat_to_EEp_f(SET_ACS_CAL);
				}
				else if (cal_pg1_sel_item_right==2) {
					Rec_Dat_to_EEp_f(SET_ACT_CAL);
				}
			}
		}
		if (cal_pg1_sel_edit_mode == cal_none) { // yani enter a basılmış ve offset den no edit moduna geçilmiş. yani offset değerinin kaydedilmesi lazım.
			if (cal_pg1_sel_col==0) {
				if (cal_pg1_sel_item_left==0) {
					Rec_Dat_to_EEp_f(SET_VRECT_OFFS_CAL);
				}
				else if (cal_pg1_sel_item_left==1) {
					Rec_Dat_to_EEp_f(SET_VLOAD_OFFS_CAL);
				}
				else if (cal_pg1_sel_item_left==2) {
					Rec_Dat_to_EEp_f(SET_VBAT_OFFS_CAL);
				}
				else if (cal_pg1_sel_item_left==3) {
					Rec_Dat_to_EEp_f(SET_IRECT_OFFS_CAL);
				}
				else if (cal_pg1_sel_item_left==4) {
					Rec_Dat_to_EEp_f(SET_IBAT_OFFS_CAL);
				}
			}
			if (cal_pg1_sel_col==1) {
				if (cal_pg1_sel_item_right==0) {
					Rec_Dat_to_EEp_f(SET_ACR_OFFS_CAL);
				}
				else if (cal_pg1_sel_item_right==1) {
					Rec_Dat_to_EEp_f(SET_ACS_OFFS_CAL);
				}
				else if (cal_pg1_sel_item_right==2) {
					Rec_Dat_to_EEp_f(SET_ACT_OFFS_CAL);
				}
			}
		}
    }
    else if (currentPage == TEST_pg) {
    	test_edit_mode ^=1;
        if (test_edit_mode) {
			if (selected_TEST_PG_line==0) {
				temp_test_thy_1=0;
			}
			if (selected_TEST_PG_line==1) {
				temp_test_trf_2=0;
			}
        }
        if (!test_edit_mode) {
        	__NOP();
        }
		if (selected_TEST_PG_line==2) {
			buzzer_override^=1;
		}
		if (selected_TEST_PG_line==3) {
			leds_rels_override^=1;
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
    else if (currentPage == RECTF_ACTIVE_AT_STARTUP_pg) {
//    	rectf_active_at_startup_req=1;
    	if (rectf_active_at_startup_req_right == 0) {
        	if (EpD[RECT_ACTV_AT_STARTUP][0].V1 == 1) {
				EpD[RECT_ACTV_AT_STARTUP][0].V1=0;
				EpD[RECT_ACTV_AT_STARTUP][1].V1=0;
				Rec_Dat_to_EEp_f(RECT_ACTV_AT_STARTUP);
        	}
	        currentPage = MANAGEMENT_pg;
    	} else if (rectf_active_at_startup_req_right == 1) {
        	if (EpD[RECT_ACTV_AT_STARTUP][0].V1 == 0) {
        		EpD[RECT_ACTV_AT_STARTUP][0].V1=1;
    			EpD[RECT_ACTV_AT_STARTUP][1].V1=1;
        		Rec_Dat_to_EEp_f(RECT_ACTV_AT_STARTUP);
        	}
	        currentPage = MANAGEMENT_pg;
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
    	PRF_GEN("Send datetime to RTC IC");
    	Write_RTC_Time_Data_With_Oscillator();
    	PRF_GEN("Completed");
    }
    SHOW_MENU_NOW
}
