/**********************************************************************************************************************
    Program Name    : EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : r_ees_sub_api.h
    Program Version : V1.00
    Device(s)       : RL78/F24 microcontroller
    Description     : Sub functions for EES
**********************************************************************************************************************/

/**********************************************************************************************************************
    DISCLAIMER
    This software is supplied by Renesas Electronics Corporation and is only intended for use with
    Renesas products. No other uses are authorized. This software is owned by Renesas Electronics
    Corporation and is protected under all applicable laws, including copyright laws.
    THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
    TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR
    ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
    BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
    Renesas reserves the right, without notice, to make changes to this software and to discontinue the
    availability of this software. By using this software, you agree to the additional terms and conditions
    found by accessing the following link:
    http://www.renesas.com/disclaimer
    
    Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/

#ifndef R_EES_SUB_API_H
#define R_EES_SUB_API_H

#include "r_ees.h"

/**********************************************************************************************************************
 Prototype declaration
 *********************************************************************************************************************/

extern __near st_ees_control_flags_t     g_st_ees_control_flags;
extern __near st_ees_control_variables_t g_st_ees_control_variables;

extern R_EES_FAR_FUNC bool r_ees_is_valid_configuration(void);
extern R_EES_FAR_FUNC bool r_ees_is_valid_requester(st_ees_request_t __near * ionp_st_ees_request);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_01(void);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_02(void);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_03(void);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_04(void);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_05a(void);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_05b(void);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_06(void);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_07(void);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_08(void);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_09(void);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_10(void);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_11(void);
extern R_EES_FAR_FUNC void r_ees_fsm_startup_state_12(void);
extern R_EES_FAR_FUNC void r_ees_fsm_write_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_write_state_01(void);
extern R_EES_FAR_FUNC void r_ees_fsm_write_state_02(void);
extern R_EES_FAR_FUNC void r_ees_fsm_write_state_03(void);
extern R_EES_FAR_FUNC void r_ees_fsm_write_state_04(void);
extern R_EES_FAR_FUNC void r_ees_fsm_read_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_read_state_01(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_01(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_02(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_03(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_04(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_05(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_06(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_07(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_08(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_09(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_10(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_11(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_12(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_13(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_14(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_15(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_16(void);
extern R_EES_FAR_FUNC void r_ees_fsm_refresh_state_17(void);
extern R_EES_FAR_FUNC void r_ees_fsm_format_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_format_state_01(void);
extern R_EES_FAR_FUNC void r_ees_fsm_format_state_02(void);
extern R_EES_FAR_FUNC void r_ees_fsm_format_state_03(void);
extern R_EES_FAR_FUNC void r_ees_fsm_format_state_04(void);
extern R_EES_FAR_FUNC void r_ees_fsm_format_state_05(void);
extern R_EES_FAR_FUNC void r_ees_fsm_format_state_06(void);
extern R_EES_FAR_FUNC void r_ees_fsm_format_state_07(void);
extern R_EES_FAR_FUNC void r_ees_fsm_format_state_08(void);
extern R_EES_FAR_FUNC void r_ees_fsm_format_state_09(void);
extern R_EES_FAR_FUNC void r_ees_fsm_format_state_10(void);
extern R_EES_FAR_FUNC void r_ees_fsm_format_state_11(void);
extern R_EES_FAR_FUNC void r_ees_fsm_shutdown_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_erase_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_bwv_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_inner_blankcheck_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_write_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_inner_write_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_iverify_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_inner_iverify_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_read_state_00(void);
extern R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_state_01(void);
extern R_EES_FAR_FUNC void r_ees_fsm_exit_state(void);

#endif /* end of R_EES_SUB_API_H */
