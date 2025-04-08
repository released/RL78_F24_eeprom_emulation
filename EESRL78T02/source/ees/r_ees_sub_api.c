/**********************************************************************************************************************
    Program Name    : EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : r_ees_sub_api.c
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

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_ees_sub_api.h"
#include "r_ees_exrfd_api.h"
#include "r_ees_descriptor.h"

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_VAR
#include "r_ees_memmap.h"
    /* EES control flags */
    __near st_ees_control_flags_t g_st_ees_control_flags;
#define  R_EES__END_SECTION_EES_VAR
#include "r_ees_memmap.h"

#define  R_EES_START_SECTION_EES_VAR
#include "r_ees_memmap.h"
    /* EES control variables */
    __near st_ees_control_variables_t g_st_ees_control_variables;
#define  R_EES_END_SECTION_EES_VAR
#include "r_ees_memmap.h"


/**********************************************************************************************************************
 Static functions
 *********************************************************************************************************************/

static R_EES_FAR_FUNC uint8_t r_ees_calculate_next_a_flag(uint8_t i_u08_a_flag_value);
static R_EES_FAR_FUNC void    r_ees_fsm_finish_command(void);
static R_EES_FAR_FUNC void    r_ees_fsm_swap_acvive_block_info(void);
static R_EES_FAR_FUNC bool    r_ees_fsm_exrfd_cmd_detect_fatal_error(e_ees_exrfd_ret_status_t i_e_ees_exrfd_ret_value);
static R_EES_FAR_FUNC e_ees_block_status_t r_ees_fsm_get_ees_block_status(void);


/**********************************************************************************************************************
 * Function name : r_ees_is_valid_configuration
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Checks EES configuration.
 *  
 *  R_EES_EXRFD_Init must be called before this function.
 *  
 *  @param[in]      -
 *  @return
 *                  - true  : Valid configuration
 *                  - false : Invalid configuration
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC bool r_ees_is_valid_configuration(void)
{
    /* Local variable definitions */
    uint8_t  l_u08_ees_var_max_data_size;
    uint8_t  l_u08_ees_var_current_data_size;
    uint16_t l_u16_ees_required_data_size_sum;
    uint16_t l_u16_ees_virtual_block_free_space;
    uint8_t  l_u08_count;
    
    /* Initialize EES control variable (EES variable count) */
    g_st_ees_control_variables.u08_ees_var_count = g_ar_u08_ees_descriptor[0u];
    
    /* Check EES variables */
    if ((R_EES_VALUE_U08_EES_VARS_MIN > g_st_ees_control_variables.u08_ees_var_count) 
        || (R_EES_VALUE_U08_EES_VARS_MAX < g_st_ees_control_variables.u08_ees_var_count))
    {
        /* The numbers of EES variables is invalid */
        return (false);
    }
    else
    {
        /* No operation */
    }
    
    /* Initialize EES control variable (Virtual block size) */
    g_st_ees_control_variables.u16_virtual_block_size = 
        g_u16_ees_physical_block_size * (uint16_t)g_u08_ees_physical_blocks_per_virtual_block;
    
    /* Check whether the defined variables fit into the active block */
    l_u08_ees_var_max_data_size = R_EES_VALUE_U08_INIT_VARIABLE;
    l_u16_ees_required_data_size_sum = R_EES_VALUE_U16_INIT_VARIABLE;
    for (l_u08_count = 1u; l_u08_count <= g_st_ees_control_variables.u08_ees_var_count; l_u08_count++)
    {
        l_u08_ees_var_current_data_size = g_ar_u08_ees_descriptor[l_u08_count];
        if (0u == l_u08_ees_var_current_data_size)
        {
            /* Size of EES variable is invalid */
            return (false);
        }
        else
        {
            /* No operation */
        }
        
        if (l_u08_ees_var_current_data_size > l_u08_ees_var_max_data_size)
        {
            l_u08_ees_var_max_data_size = l_u08_ees_var_current_data_size;
        }
        else
        {
            /* No operation */
        }
        
        l_u16_ees_required_data_size_sum = l_u16_ees_required_data_size_sum 
                                           + (uint16_t)l_u08_ees_var_current_data_size 
                                           + R_EES_VALUE_U08_REFERENCE_ENTRY_SIZE;
    }
    
    if (0u != g_ar_u08_ees_descriptor[g_st_ees_control_variables.u08_ees_var_count + 1u])
    {
        /* Delimiter is not found */
        return (false);
    }
    else
    {
        /* No operation */
    }
    
    l_u16_ees_required_data_size_sum = l_u16_ees_required_data_size_sum + (uint16_t)l_u08_ees_var_max_data_size 
                                       + R_EES_VALUE_U08_REFERENCE_ENTRY_SIZE;
    l_u16_ees_virtual_block_free_space = g_st_ees_control_variables.u16_virtual_block_size 
                                         - R_EES_VALUE_U08_HEADER_SIZE - R_EES_VALUE_U08_SEPARATOR_SIZE;
    if (l_u16_ees_required_data_size_sum > l_u16_ees_virtual_block_free_space)
    {
        /* The defined variables could not fit into the active block */
        return (false);
    }
    else
    {
        /* No operation */
    }
    
    /* EES configuration is valid */
    return (true);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_is_valid_configuration
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_is_valid_requester
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Checks EES requester.
 *  
 *  @param[in]      ionp_st_ees_request : 
 *                    Pointer to the EES requester
 *  @return
 *                  - true  : Valid requester
 *                  - false : Invalid requester
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC bool r_ees_is_valid_requester(st_ees_request_t __near * ionp_st_ees_request)
{
    /* Local variable definitions */
    bool l_b_need_startup_flag;
    bool l_b_need_write_flag;
    bool l_b_need_id_flag;
    e_ees_command_t e_request_command;
    
    if ((true != g_st_ees_control_flags.b_ees_initialized) || (true != g_st_ees_control_flags.b_ees_opened))
    {
        /* EES is not initialized */
        ionp_st_ees_request->e_status = R_EES_ENUM_RET_ERR_INITIALIZATION;
        return (false);
    }
    else
    {
        /* no operation */
    }
    
    if (true == g_st_ees_control_flags.b_ees_busy) 
    {
        /* EES is busy with another request */
        ionp_st_ees_request->e_status = R_EES_ENUM_RET_ERR_REJECTED;
        return (false);
    }
    else
    {
        /* no operation */
    }
    
    l_b_need_startup_flag = true;
    l_b_need_write_flag   = true;
    l_b_need_id_flag      = true;
    g_st_ees_control_variables.np_st_ees_request = ionp_st_ees_request;
    e_request_command = ionp_st_ees_request->e_command;
    if (R_EES_ENUM_CMD_WRITE == e_request_command)
    {
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_write_state_00;
    }
    else if (R_EES_ENUM_CMD_READ == e_request_command)
    {
        l_b_need_write_flag = false;
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_read_state_00;
    }
    else if (R_EES_ENUM_CMD_REFRESH == e_request_command)
    {
        l_b_need_id_flag = false;
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_refresh_state_00;
    }
    else if (R_EES_ENUM_CMD_STARTUP == e_request_command) 
    {
        l_b_need_startup_flag = false;
        l_b_need_write_flag   = false;
        l_b_need_id_flag      = false;
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_startup_state_00;
    }
    else if (R_EES_ENUM_CMD_SHUTDOWN == e_request_command)
    {
        l_b_need_write_flag = false;
        l_b_need_id_flag    = false;
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_shutdown_state_00;
    }
    else if (R_EES_ENUM_CMD_FORMAT == e_request_command)
    {
        l_b_need_startup_flag = false;
        l_b_need_write_flag   = false;
        l_b_need_id_flag      = false;
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_format_state_00;
    }
    else
    {
        /* Illegal command request */
        ionp_st_ees_request->e_status = R_EES_ENUM_RET_ERR_PARAMETER;
        return (false);
    }
    
    if ((true == l_b_need_startup_flag) && (true != g_st_ees_control_flags.b_ees_started_up))
    {
        /* Startup command is not executed */
        ionp_st_ees_request->e_status = R_EES_ENUM_RET_ERR_ACCESS_LOCKED;
        return (false);
    }
    else
    {
        /* no operation */
    }
    
    if ((true == l_b_need_write_flag) && (true != g_st_ees_control_flags.b_ees_write_enabled))
    {
        /* EES pool is exhausted */
        ionp_st_ees_request->e_status = R_EES_ENUM_RET_ERR_POOL_EXHAUSTED;
        return (false);
    }
    else
    {
        /* no operation */
    }
    
    if ((true == l_b_need_id_flag) 
        && ((0u == ionp_st_ees_request->u08_identifier) 
            || (g_st_ees_control_variables.u08_ees_var_count < ionp_st_ees_request->u08_identifier)))
    {
        /* Illegal ID */
        ionp_st_ees_request->e_status = R_EES_ENUM_RET_ERR_PARAMETER;
        return (false);
    }
    else
    {
        /* no operation */
    }
    
    /* Set busy status */
    g_st_ees_control_flags.b_ees_busy = true;
    ionp_st_ees_request->e_status = R_EES_ENUM_RET_STS_BUSY;
    
    /* Initialize temporary status */
    g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_STS_OK;
    
    /* EES requester is valid */
    return (true);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_is_valid_requester
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * functions for finite state machine (FSM)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 00:
 *  Initiates the STARTUP command.
 *  Control flags and variables will be cleared.
 *
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_00(void)
{
    /* Local variable definitions */
    
    /* Initialize EES control flags */
    g_st_ees_control_flags.b_ees_started_up = false;
    g_st_ees_control_flags.b_ees_write_enabled = false;
    g_st_ees_control_flags.b_ees_read_enabled = false;
    
    /* Clear internal variables */
    g_st_ees_control_variables.u08_ees_block_iterator           = R_EES_VALUE_U08_INIT_VARIABLE;
    g_st_ees_control_variables.u08_ees_active0_block_index      = R_EES_VALUE_U08_INIT_VARIABLE;
    g_st_ees_control_variables.u08_ees_active1_block_index      = R_EES_VALUE_U08_INIT_VARIABLE;
    g_st_ees_control_variables.u08_ees_active0_block_a_flag     = R_EES_VALUE_U08_INIT_VARIABLE;
    g_st_ees_control_variables.u08_ees_active1_block_a_flag     = R_EES_VALUE_U08_INIT_VARIABLE;
    g_st_ees_control_variables.u08_ees_active_block_count       = R_EES_VALUE_U08_INIT_VARIABLE;
    g_st_ees_control_variables.u08_ees_non_excluded_block_count = R_EES_VALUE_U08_INIT_VARIABLE;
    g_st_ees_control_variables.b_ees_invalid_range0_found_flag  = false;
    g_st_ees_control_variables.b_ees_invalid_range1_found_flag  = false;
    g_st_ees_control_variables.b_ees_invalid_range2_found_flag  = false;
    
    /* Set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_startup_state_01;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_01
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 01:
 *  Prepares to read first/next block header.
 *
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_01(void)
{
    /* Local variable definitions */
    
    /* Prepare to read block header */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr = (uint16_t)g_st_ees_control_variables.u08_ees_block_iterator 
                                                           * g_st_ees_control_variables.u16_virtual_block_size;
    g_st_ees_control_variables.u16_exrfd_cmd_size        = R_EES_VALUE_U08_HEADER_SIZE;
    g_st_ees_control_variables.np_u08_exrfd_cmd_data     = 
        &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
    
    /* Set next state (read error never occurs by EXRFD read command) */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_read_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_startup_state_02;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_exit_state;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_01
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_02
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 02:
 *  Analyzes all block statuses.
 *
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_02(void)
{
    /* Local variable definitions */
    e_ees_block_status_t l_e_ees_block_status;
    
    /* Analyze block */
    l_e_ees_block_status = r_ees_fsm_get_ees_block_status();
    if (R_EES_ENUM_BLOCK_STATUS_EXCLUDED == l_e_ees_block_status)
    {
        /* No operation */
    }
    else
    {
        g_st_ees_control_variables.u08_ees_non_excluded_block_count++;
        
        if (R_EES_ENUM_BLOCK_STATUS_INVALID == l_e_ees_block_status)
        {
            if (0u == g_st_ees_control_variables.u08_ees_active_block_count)
            {
                /* Invalid block is found before first active block */
                g_st_ees_control_variables.b_ees_invalid_range0_found_flag = true;
            }
            else if (1u == g_st_ees_control_variables.u08_ees_active_block_count)
            {
                 /* Invalid block is found after first active block */
                 g_st_ees_control_variables.b_ees_invalid_range1_found_flag = true;
            }
            else if (2u == g_st_ees_control_variables.u08_ees_active_block_count)
            {
                 /* Invalid block is found after second active block */
                 g_st_ees_control_variables.b_ees_invalid_range2_found_flag = true;
            }
            else
            {
                /* No operation */
            }
        }
        else
        {
            g_st_ees_control_variables.u08_ees_active_block_count++;
            
            if (1u == g_st_ees_control_variables.u08_ees_active_block_count)
            {
                /* First active block is found */
                g_st_ees_control_variables.u08_ees_active0_block_index = 
                    g_st_ees_control_variables.u08_ees_block_iterator;
                g_st_ees_control_variables.u08_ees_active0_block_a_flag = 
                    g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[R_EES_VALUE_U08_A_FLAG_INDEX];
            }
            else if (2u == g_st_ees_control_variables.u08_ees_active_block_count)
            {
                /* Second active block is found */
                g_st_ees_control_variables.u08_ees_active1_block_index = 
                    g_st_ees_control_variables.u08_ees_block_iterator;
                g_st_ees_control_variables.u08_ees_active1_block_a_flag = 
                    g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[R_EES_VALUE_U08_A_FLAG_INDEX];
            }
            else
            {
                /* No operation */
            }
        }
    }
    
    g_st_ees_control_variables.u08_ees_block_iterator++;
    if (g_st_ees_control_variables.u08_ees_block_iterator < g_u08_ees_pool_virtual_blocks)
    {
        /* Set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_startup_state_01;
    }
    else
    {
        /* All blocks are analyzed, set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_startup_state_03;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_02
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_03
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 03 :
 *  Checks the pool consistency and identifies the current active block.
 *  In case of the error "INCONSISTENT", the STARTUP command will be finished immediately.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_03(void)
{
    /* Local variable definitions */
    uint8_t l_u08_next_active0_a_flag;
    uint8_t l_u08_next_active1_a_flag;
    
    /* Analyze EES pool */
    if ((0u == g_st_ees_control_variables.u08_ees_active_block_count) 
        || (2u < g_st_ees_control_variables.u08_ees_active_block_count))
    {
        /* No active block or more than 2 active blocks */
        g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_POOL_INCONSISTENT;
    }
    else if (1u == g_st_ees_control_variables.u08_ees_active_block_count)
    {
        /* 1 active block */
        if ((true != g_st_ees_control_variables.b_ees_invalid_range0_found_flag) 
            && (true != g_st_ees_control_variables.b_ees_invalid_range1_found_flag))
        {
            /* No invalid block, then EES pool is exhausted */
            g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_POOL_EXHAUSTED;
        }
        else
        {
            /* No operation */
        }
    }
    else
    {
        /* 2 active blocks */
        if ((true == g_st_ees_control_variables.b_ees_invalid_range1_found_flag) 
            && ((true == g_st_ees_control_variables.b_ees_invalid_range0_found_flag) 
                || (true == g_st_ees_control_variables.b_ees_invalid_range2_found_flag)))
        {
            /* Non-contiguous active blocks */
            g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_POOL_INCONSISTENT;
        }
        else
        {
            /* Calculate next A-flag */
            l_u08_next_active0_a_flag = 
                r_ees_calculate_next_a_flag(g_st_ees_control_variables.u08_ees_active0_block_a_flag);
            l_u08_next_active1_a_flag = 
                r_ees_calculate_next_a_flag(g_st_ees_control_variables.u08_ees_active1_block_a_flag);
            
            /* Contiguous active blocks */
            if ((true == g_st_ees_control_variables.b_ees_invalid_range1_found_flag) 
                || (true == g_st_ees_control_variables.b_ees_invalid_range0_found_flag) 
                || (true == g_st_ees_control_variables.b_ees_invalid_range2_found_flag))
            {
                /* Invalid blocks found */
                if (true == g_st_ees_control_variables.b_ees_invalid_range1_found_flag)
                {
                    /* Invalid blocks exist between active0 and active1 */
                    if (l_u08_next_active1_a_flag != g_st_ees_control_variables.u08_ees_active0_block_a_flag)
                    {
                        /* Wrong order of active blocks */
                        g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_POOL_INCONSISTENT;
                    }
                    else
                    {
                        /* EES pool is correct, then no operation */
                    }
                }
                else
                {
                    /* Invalid blocks exist between active1 and active0 */
                    if (l_u08_next_active0_a_flag != g_st_ees_control_variables.u08_ees_active1_block_a_flag)
                    {
                        /* Wrong order of active blocks */
                        g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_POOL_INCONSISTENT;
                    }
                    else
                    {
                        /* EES pool is correct, but swaps active block information to change active0 new one */
                        r_ees_fsm_swap_acvive_block_info();
                    }
                }
            }
            else
            {
                /* Only 2 active blocks (no invalid block) */
                if (l_u08_next_active0_a_flag == g_st_ees_control_variables.u08_ees_active1_block_a_flag)
                {
                    /* EES pool is correct, but swap active block information to change active0 block new one */
                    r_ees_fsm_swap_acvive_block_info();
                }
                else if (l_u08_next_active1_a_flag != g_st_ees_control_variables.u08_ees_active0_block_a_flag)
                {
                    /* Wrong order of active blocks (never occurs) */
                    g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_POOL_INCONSISTENT;
                }
                else
                {
                    /* EES pool is correct and active0 is new one, then no operation */
                }
            }
        }
    }
    
    if (R_EES_ENUM_RET_ERR_POOL_INCONSISTENT == g_st_ees_control_variables.e_ees_temp_status)
    {
        /* EES pools is inconsistent, then finish command */
        r_ees_fsm_finish_command();
    }
    else
    {
        if (1u == g_st_ees_control_variables.u08_ees_active_block_count)
        {
            /* Set next state */
            g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_startup_state_06;
        }
        else
        {
            /* Set next state */
            g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_startup_state_04;
        }
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_03
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_04
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 04:
 *  Prepares to perform verify on the B-flag of the newest active block.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_04(void)
{
    /* Local variable definitions */
    
    /* Prepare verify on the B-flag of new active block */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr = 
         ((uint16_t)g_st_ees_control_variables.u08_ees_active0_block_index
         * g_st_ees_control_variables.u16_virtual_block_size) + R_EES_VALUE_U08_B_FLAG_INDEX;
    g_st_ees_control_variables.u16_exrfd_cmd_size        = 1u;
    
    /* Set next state  */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_iverify_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_startup_state_05b;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_startup_state_05a;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_04
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_05a
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 05a:
 *  Swap the target block information for an invalidation of one active block.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_05a(void)
{
    /* Local variable definitions */
    
    /* Swap active block information because a verify error occurs on the B-flag of the new active block */
    r_ees_fsm_swap_acvive_block_info();
    
    /* Set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_startup_state_05b;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_05a
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_05b
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 05b:
 *  Performs an invalidation of one active block.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_05b(void)
{
    /* Local variable definitions */
    
    /* Prepare to write I-flag in active1 block header */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr        = 
        ((uint16_t)g_st_ees_control_variables.u08_ees_active1_block_index
         * g_st_ees_control_variables.u16_virtual_block_size) + R_EES_VALUE_U08_I_FLAG_INDEX;
    g_st_ees_control_variables.u16_exrfd_cmd_size               = 1u;       /* The size of I-flag is 1 */
    g_st_ees_control_variables.np_u08_exrfd_cmd_data            = 
        &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
    g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u] = 0x00u;    /* Data to write I-flag is 0x00 */
    
    /* Set next state (ignore write error) */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_write_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_startup_state_06;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_startup_state_06;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_05b
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_06
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 06 :
 *  Initializes the reference search procedure for RAM reference table update.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_06(void)
{
    /* Local variable definitions */
    uint8_t  l_u08_count;
    uint16_t l_u16_block_offset_address;
    
    /* Clear RAM reference table */
    for (l_u08_count = 0u; l_u08_count < g_st_ees_control_variables.u08_ees_var_count; l_u08_count++)
    {
        g_ar_u16_ram_ref_table[l_u08_count] = R_EES_VALUE_U16_INIT_VARIABLE;
    }
    
    l_u16_block_offset_address = (uint16_t)g_st_ees_control_variables.u08_ees_active0_block_index 
                                 * g_st_ees_control_variables.u16_virtual_block_size;
    g_st_ees_control_variables.u16_ees_rwp = l_u16_block_offset_address + R_EES_VALUE_U08_HEADER_SIZE;
    g_st_ees_control_variables.u16_ees_dwp = (l_u16_block_offset_address 
                                              + g_st_ees_control_variables.u16_virtual_block_size) - 1u;
    g_st_ees_control_variables.u16_ees_startup_last_valid_rwp = R_EES_VALUE_U16_INIT_VARIABLE;
    
    /* Set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_startup_state_07;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_06
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_07
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 07 :
 *  Initializes the read process of the first/next reference.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_07(void)
{
    /* Local variable definitions */
    
    /* Prepare to read reference entry */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr = g_st_ees_control_variables.u16_ees_rwp;
    g_st_ees_control_variables.u16_exrfd_cmd_size        = R_EES_VALUE_U08_REFERENCE_ENTRY_SIZE;
    g_st_ees_control_variables.np_u08_exrfd_cmd_data     = 
        &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
    
    /* Set next state (read error never occurs by EXRFD read command) */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_read_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_startup_state_08;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_exit_state;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_07
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_08
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 08 :
 *  Analyzes the read reference entry and fill the RAM reference table.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_08(void)
{
    /* Local variable definitions */
    uint8_t l_u08_sor;
    uint8_t l_u08_eor;
    uint8_t l_u08_current_data_size;
    bool    l_b_finish_reference_entry_flag;
    
    /* Set local variables */
    l_u08_sor  = g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[R_EES_VALUE_U08_SOR_INDEX];
    l_u08_eor  = g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[R_EES_VALUE_U08_EOR_INDEX];
    l_b_finish_reference_entry_flag = false;
    
    /* Check reference entry */
    if ((0xFFu == l_u08_sor) && (0xFFu == l_u08_eor))
    {
        /* Separator is found, then finish filling the RAM reference table */
        l_b_finish_reference_entry_flag = true;
    }
    else
    {
        /* Reference entry is found */
        if ((0x00u != l_u08_sor) && (l_u08_sor <= g_st_ees_control_variables.u08_ees_var_count)
            && (0xFFu == (l_u08_sor ^ l_u08_eor)))
        {
            /* Valid reference entry is found */
            /* Get the size of the current data (index is already verified within certain bounds) */
            l_u08_current_data_size = g_ar_u08_ees_descriptor[l_u08_sor];
            
            /* Update RWP */
            g_st_ees_control_variables.u16_ees_rwp = g_st_ees_control_variables.u16_ees_rwp 
                                                     + R_EES_VALUE_U08_REFERENCE_ENTRY_SIZE;
            
            if (g_st_ees_control_variables.u16_ees_dwp 
                > (g_st_ees_control_variables.u16_ees_rwp + (uint16_t)l_u08_current_data_size))
            {
                /* Update DWP */
                g_st_ees_control_variables.u16_ees_dwp = g_st_ees_control_variables.u16_ees_dwp 
                                                         - l_u08_current_data_size;
                
                /* Update RAM reference table for current data (index is already verified within certain bounds) */
                g_ar_u16_ram_ref_table[l_u08_sor - 1u] = g_st_ees_control_variables.u16_ees_dwp + 1u;
                
                g_st_ees_control_variables.u16_ees_startup_last_valid_rwp = g_st_ees_control_variables.u16_ees_rwp
                                                                            - R_EES_VALUE_U08_REFERENCE_ENTRY_SIZE;
            }
            else
            {
                /* Separator may be lost, then make EES pool full */
                g_st_ees_control_variables.u16_ees_dwp = g_st_ees_control_variables.u16_ees_rwp + 1u;
                
                /* Finish filling the RAM reference table */
                l_b_finish_reference_entry_flag = true;
            }
        }
        else
        {
            /* Invalid reference entry is found, then make EES pool full */
            g_st_ees_control_variables.u16_ees_dwp = g_st_ees_control_variables.u16_ees_rwp + 1u;
            
            /* Finish filling the RAM reference table */
            l_b_finish_reference_entry_flag = true;
        }
    }
    
    if (true == l_b_finish_reference_entry_flag)
    {
        /* Set EES control flags */
        g_st_ees_control_flags.b_ees_read_enabled = true;
        
        /* Finish filling the RAM reference table */
        if (R_EES_ENUM_RET_ERR_POOL_EXHAUSTED == g_st_ees_control_variables.e_ees_temp_status)
        {
            /* Set EES control flags */
            g_st_ees_control_flags.b_ees_started_up = true;
            
            /* Finish command */
            r_ees_fsm_finish_command();
        }
        else
        {
            /* Set next state */
            g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_startup_state_09;
        }
    }
    else
    {
        /* Set next state to read the next reference entry */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_startup_state_07;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_08
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_09
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 09:
 *  Prepares to perform verify on the block header of the active block.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_09(void)
{
    /* Local variable definitions */
    
    /* Prepare verify on the block header of the active block */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr = 
         ((uint16_t)g_st_ees_control_variables.u08_ees_active0_block_index
         * g_st_ees_control_variables.u16_virtual_block_size);
    g_st_ees_control_variables.u16_exrfd_cmd_size        = R_EES_VALUE_U08_HEADER_VALID_FLAG_SIZE;
    
    /* Set next state  */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_iverify_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_startup_state_10;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_startup_state_11;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_09
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_10
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 10:
 *  Prepares to perform verify on the last reference entry in the active block.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_10(void)
{
    /* Local variable definitions */
    
    if (R_EES_VALUE_U16_INIT_VARIABLE != g_st_ees_control_variables.u16_ees_startup_last_valid_rwp)
    {
        /* The active block has at least one more valid reference entry */
        /* Prepare verify on the block header of the active block */
        g_st_ees_control_variables.u16_exrfd_cmd_offset_addr = 
            g_st_ees_control_variables.u16_ees_startup_last_valid_rwp;
        g_st_ees_control_variables.u16_exrfd_cmd_size        = R_EES_VALUE_U08_REFERENCE_ENTRY_SIZE;
        
        /* Set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_iverify_state_00;
        g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_startup_state_12;
        g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_startup_state_11;
    }
    else
    {
        /* The active block has no valid reference entry, then set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_startup_state_12;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_10
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_11
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 11:
 *  Sets the verify error.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_11(void)
{
    /* Local variable definitions */
    
    /* The verify error occurs in the block header or the last reference entry in the active block */
    g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_VERIFY;
    
    /* Make EES pool full */
    g_st_ees_control_variables.u16_ees_dwp = g_st_ees_control_variables.u16_ees_rwp + 1u;
    
    /* Set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_startup_state_12;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_11
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_startup_state_12
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  STARTUP state 12:
 *  Finishes STARTUP command.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_startup_state_12(void)
{
    /* Local variable definitions */
    
    /* Set EES control flags */
    g_st_ees_control_flags.b_ees_write_enabled = true;
    g_st_ees_control_flags.b_ees_started_up = true;
    
    /* Finish command */
    r_ees_fsm_finish_command();
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_startup_state_12
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_write_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  WRITE state 00:
 *  Initiates the WRITE command and prepare to write SoR of the new instance.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_write_state_00(void)
{
    /* Local variable definitions */
    uint8_t  l_u08_current_id;
    uint16_t l_u16_free_space;
    
    /* Get information of the current ID which is verified within certain bounds */
    l_u08_current_id = g_st_ees_control_variables.np_st_ees_request->u08_identifier;
    g_st_ees_control_variables.u08_ees_id_size_storage = g_ar_u08_ees_descriptor[l_u08_current_id];
    
    /* Calculate free space */
    if (((g_st_ees_control_variables.u16_ees_rwp + R_EES_VALUE_U08_SEPARATOR_SIZE) - 1u) 
        < g_st_ees_control_variables.u16_ees_dwp)
    {
        /* Free space = DWP - RWP - SEPARATOR_SIZE + 1 */
        l_u16_free_space = ((g_st_ees_control_variables.u16_ees_dwp - g_st_ees_control_variables.u16_ees_rwp) 
                            - R_EES_VALUE_U08_SEPARATOR_SIZE) + 1u;
    }
    else
    {
        l_u16_free_space = 0u;
    }
    
    if (l_u16_free_space 
        >= ((uint16_t)g_st_ees_control_variables.u08_ees_id_size_storage + R_EES_VALUE_U08_REFERENCE_ENTRY_SIZE))
    {
        /* EES pool has enough space to write the new instance */
        /* Prepare to write SoR of the new instance */
        g_st_ees_control_variables.u16_exrfd_cmd_offset_addr        = g_st_ees_control_variables.u16_ees_rwp 
                                                                      + R_EES_VALUE_U08_SOR_INDEX;
        g_st_ees_control_variables.u16_exrfd_cmd_size               = 1u;       /* The size of SoR is 1 */
        g_st_ees_control_variables.np_u08_exrfd_cmd_data            = 
            &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
        g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u] = l_u08_current_id;
        
        /* Set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_bwv_state_00;
        g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_write_state_01;
        g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_write_state_04;
    }
    else
    {
        /* No space to write the new instance, then finish command */
        g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_POOL_FULL;
        r_ees_fsm_finish_command();
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_write_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_write_state_01
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  WRITE state 01:
 *  Prepares to write data of the new instance.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_write_state_01(void)
{
    /* Local variable definitions */
    
    /* Prepare to write data of the new instance (EES pool has enough space to write the new instance) */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr = (g_st_ees_control_variables.u16_ees_dwp 
                                                            - g_st_ees_control_variables.u08_ees_id_size_storage) + 1u;
    g_st_ees_control_variables.u16_exrfd_cmd_size        = g_st_ees_control_variables.u08_ees_id_size_storage;
    g_st_ees_control_variables.np_u08_exrfd_cmd_data     = 
        g_st_ees_control_variables.np_st_ees_request->np_u08_address;
    
    /* Set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_bwv_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_write_state_02;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_write_state_04;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_write_state_01
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_write_state_02
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  WRITE state 02:
 *  Prepares to write EoR of the new instance.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_write_state_02(void)
{
    /* Local variable definitions */
    uint8_t  l_u08_current_id;
    
    /* Get information of the current ID which is verified within certain bounds */
    l_u08_current_id = g_st_ees_control_variables.np_st_ees_request->u08_identifier;
    
    /* Prepare to write EoR of the new instance */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr        = g_st_ees_control_variables.u16_ees_rwp 
                                                                  + R_EES_VALUE_U08_EOR_INDEX;
    g_st_ees_control_variables.u16_exrfd_cmd_size               = 1u;   /* The size of EoR is 1 */
    g_st_ees_control_variables.np_u08_exrfd_cmd_data            = 
        &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
    g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u] = l_u08_current_id ^ (uint8_t)0xFFu;
    
    /* Set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_bwv_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_write_state_03;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_write_state_04;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_write_state_02
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_write_state_03
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  WRITE state 03:
 *  Calculates the new RWP and DWP.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_write_state_03(void)
{
    /* Local variable definitions */
    uint8_t  l_u08_current_id;
    
    /* Get information of the current ID which is verified within certain bounds */
    l_u08_current_id = g_st_ees_control_variables.np_st_ees_request->u08_identifier;
    
    /* Update RWP */
    g_st_ees_control_variables.u16_ees_rwp =  g_st_ees_control_variables.u16_ees_rwp 
                                              + R_EES_VALUE_U08_REFERENCE_ENTRY_SIZE;
    
    /* Update DWP (EES pool has enough space to write the new instance) */
    g_st_ees_control_variables.u16_ees_dwp = g_st_ees_control_variables.u16_ees_dwp 
                                             - g_st_ees_control_variables.u08_ees_id_size_storage;
    
    /* Update RAM reference table for current data (ID is already verified within certain bounds)*/
    g_ar_u16_ram_ref_table[l_u08_current_id - 1u] = g_st_ees_control_variables.u16_ees_dwp + 1u;
    
    /* Finish command */
    g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_STS_OK;
    r_ees_fsm_finish_command();
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_write_state_03
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_write_state_04
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  WRITE state 04:
 *  The EES pool will be consumed because of any flash error, and finish WRITE command.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_write_state_04(void)
{
    /* Local variable definitions */
    
    /* Any flash error except fatal error occurs, then make EES pool full */
    g_st_ees_control_variables.u16_ees_dwp = g_st_ees_control_variables.u16_ees_rwp + 1u;
    
    /* EES pool becomes full, then finish command */
    g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_POOL_FULL;
    r_ees_fsm_finish_command();
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_write_state_04
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_read_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  READ state 00:
 *  initiates the READ command of the instance in case the RAM reference is not invalid.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_read_state_00(void)
{
    /* Local variable definitions */
    uint8_t  l_u08_current_id;
    uint8_t  l_u08_current_id_size;
    uint16_t l_u16_current_id_ram_reference;
    
    /* Get RAM reference of the current ID which is verified within certain bounds */
    l_u08_current_id = g_st_ees_control_variables.np_st_ees_request->u08_identifier;
    l_u16_current_id_ram_reference = g_ar_u16_ram_ref_table[l_u08_current_id - 1u];
    if (R_EES_VALUE_U16_INIT_VARIABLE == l_u16_current_id_ram_reference)
    {
        /* No instance, then finish command */
        g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_NO_INSTANCE;
        r_ees_fsm_finish_command();
    }
    else
    {
        /* Instance is found in the RAM reference table */
        l_u08_current_id_size = g_ar_u08_ees_descriptor[l_u08_current_id];
        
        /* Prepare to read data of current instance */
        g_st_ees_control_variables.u16_exrfd_cmd_offset_addr = l_u16_current_id_ram_reference;
        g_st_ees_control_variables.u16_exrfd_cmd_size        = l_u08_current_id_size;
        g_st_ees_control_variables.np_u08_exrfd_cmd_data     = 
            g_st_ees_control_variables.np_st_ees_request->np_u08_address;
        
        /* Set next state (read error never occurs by EXRFD read command) */
        g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_read_state_00;
        g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_read_state_01;
        g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_exit_state;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_read_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_read_state_01
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  READ state 01:
 *  Finishes READ command.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_read_state_01(void)
{
    /* Local variable definitions */
    
    /* Finish command */
    g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_STS_OK;
    r_ees_fsm_finish_command();
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_read_state_01
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 00:
 *  Initiates the REFRESH command.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_00(void)
{
    /* Local variable definitions */
    
    /* Clear refresh retry-counter */
    g_st_ees_control_variables.u08_ees_refresh_retry = R_EES_VALUE_U08_INIT_VARIABLE;
    
    /* Set refresh block the next block of the current active block */
    g_st_ees_control_variables.u08_ees_refresh_block = g_st_ees_control_variables.u08_ees_active0_block_index + 1u;
    
    /* Set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_refresh_state_01;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_01
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 01:
 *  Checks whether the target refresh block reaches the active block or not.
 *  If yes, the refresh cannot be performed due to missing non excluded block and EES pool is exhausted.
 *  If not, then prepare to read block header.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_01(void)
{
    /* Local variable definitions */
    
    if (g_st_ees_control_variables.u08_ees_refresh_block >= g_u08_ees_pool_virtual_blocks)
    {
        /* Target refresh block exceeds EES pool, then set target refresh block 0 */
        g_st_ees_control_variables.u08_ees_refresh_block = R_EES_VALUE_U08_INIT_VARIABLE;
    }
    else
    {
        /* No operation */
    }
    
    if (g_st_ees_control_variables.u08_ees_refresh_block == g_st_ees_control_variables.u08_ees_active0_block_index)
    {
        /* Target refresh block reaches the active block, then EES pool is exhausted */
        g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_POOL_EXHAUSTED;
        g_st_ees_control_flags.b_ees_write_enabled = false;
        r_ees_fsm_finish_command();
    }
    else
    {
        /* Prepare to read block header */
        g_st_ees_control_variables.u16_exrfd_cmd_offset_addr = 
            (uint16_t)g_st_ees_control_variables.u08_ees_refresh_block 
            * g_st_ees_control_variables.u16_virtual_block_size;
        g_st_ees_control_variables.u16_exrfd_cmd_size        = R_EES_VALUE_U08_HEADER_SIZE;
        g_st_ees_control_variables.np_u08_exrfd_cmd_data     = 
            &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
        
        /* Set next state (read error never occurs by EXRFD read command) */
        g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_read_state_00;
        g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_refresh_state_02;
        g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_exit_state;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_01
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_02
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 02:
 *  Checks the status of the potential refresh block.
 *  If the block status is not excluded, then prepares to erase that block.
 *  If the block status is excluded, then takes next block.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_02(void)
{
    /* Local variable definitions */
    e_ees_block_status_t l_e_ees_block_status;
    
    /* Analyze block */
    l_e_ees_block_status = r_ees_fsm_get_ees_block_status();
    if (R_EES_ENUM_BLOCK_STATUS_EXCLUDED == l_e_ees_block_status)
    {
        /* Excluded block is found, then set next state (search another non excluded block) */
        g_st_ees_control_variables.u08_ees_refresh_block++;
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_refresh_state_01;
    }
    else
    {
        /* Non excluded block is found, then prepare to erase target refresh block */
        g_st_ees_control_variables.u08_exrfd_cmd_virtual_block_number = 
            g_st_ees_control_variables.u08_ees_refresh_block;
        
        /* Set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_erase_state_00;
        g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_refresh_state_03;
        g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_refresh_state_14;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_02
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_03
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 03:
 *  Prepares to write A-flag in the target refresh block.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_03(void)
{
    /* Local variable definitions */
    uint8_t l_u08_next_active0_a_flag;
    
    /* Calculate next A-flag */
    l_u08_next_active0_a_flag = 
        r_ees_calculate_next_a_flag(g_st_ees_control_variables.u08_ees_active0_block_a_flag);
    
    /* Set A-flag value of the target refresh block */
    g_st_ees_control_variables.u08_ees_refresh_a_flag = l_u08_next_active0_a_flag;
    
    /* Prepare to write A-flag in refresh block header */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr        =  
        ((uint16_t)g_st_ees_control_variables.u08_ees_refresh_block 
        * g_st_ees_control_variables.u16_virtual_block_size) + R_EES_VALUE_U08_A_FLAG_INDEX;
    g_st_ees_control_variables.u16_exrfd_cmd_size               = 1u;       /* The size of A-flag is 1 */
    g_st_ees_control_variables.np_u08_exrfd_cmd_data            = 
        &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
    g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u] = l_u08_next_active0_a_flag;
    
    /* Set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_bwv_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_refresh_state_04;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_refresh_state_13;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_03
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_04
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 04:
 *  Initiates the copy process.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_04(void)
{
    /* Local variable definitions */
    uint16_t l_u16_block_offset_address;
    
    /* Set new RWP and DWP */
    l_u16_block_offset_address = (uint16_t)g_st_ees_control_variables.u08_ees_refresh_block 
                                 * g_st_ees_control_variables.u16_virtual_block_size;
    g_st_ees_control_variables.u16_ees_rwp = l_u16_block_offset_address + R_EES_VALUE_U08_HEADER_SIZE;
    g_st_ees_control_variables.u16_ees_dwp = (l_u16_block_offset_address 
                                              + g_st_ees_control_variables.u16_virtual_block_size) - 1u;
    
    /* Clear referenced instance iterator */
    g_st_ees_control_variables.u08_ees_refresh_ref_iterator = R_EES_VALUE_U08_INIT_VARIABLE;
    
    /* Set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_refresh_state_05;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_04
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_05
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 05:
 *  Initializes copy process for next variables and prepare to write SoR and EoR of that variable.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_05(void)
{
    /* Local variable definitions */
    uint8_t  l_u08_current_id;
    uint8_t  l_u08_current_data_size;
    uint16_t l_u16_current_id_ram_reference;
    
    /* Check whether instance which be copied exists or not */
    if (g_st_ees_control_variables.u08_ees_refresh_ref_iterator < g_st_ees_control_variables.u08_ees_var_count)
    {
        /* Instance which be copied may remain */
        /* Get RAM reference of the current reference iterator which is verified within certain bounds */
        l_u16_current_id_ram_reference = 
            g_ar_u16_ram_ref_table[g_st_ees_control_variables.u08_ees_refresh_ref_iterator];
        if (R_EES_VALUE_U16_INIT_VARIABLE == l_u16_current_id_ram_reference)
        {
            /* No written instance */
            g_st_ees_control_variables.u08_ees_refresh_ref_iterator++;
            
            /* Set next state (omit to set because next state is same state) */
        }
        else
        {
            /* Instance is found */
            /* Get the size of the current data (index is already verified within certain bounds) */
            l_u08_current_id        = g_st_ees_control_variables.u08_ees_refresh_ref_iterator + 1u;
            l_u08_current_data_size = g_ar_u08_ees_descriptor[l_u08_current_id];
            
            /* Initialize control variables to copy instances */
            g_st_ees_control_variables.u16_ees_refresh_copy_offset = l_u16_current_id_ram_reference 
                                                                     + (uint16_t)l_u08_current_data_size;
            g_st_ees_control_variables.u08_ees_refresh_copy_size   = l_u08_current_data_size;
            
            /* Prepare to write SoR and EoR for the new instance */
            g_st_ees_control_variables.u16_exrfd_cmd_offset_addr        = g_st_ees_control_variables.u16_ees_rwp 
                                                                          + R_EES_VALUE_U08_SOR_INDEX;
            g_st_ees_control_variables.u16_exrfd_cmd_size               = R_EES_VALUE_U08_REFERENCE_ENTRY_SIZE;
            g_st_ees_control_variables.np_u08_exrfd_cmd_data            = 
                &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
            g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u] = l_u08_current_id;
            g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[1u] = l_u08_current_id ^ (uint8_t)0xFFu;
            
            /* Update RWP */
            g_st_ees_control_variables.u16_ees_rwp =  g_st_ees_control_variables.u16_ees_rwp 
                                                      + R_EES_VALUE_U08_REFERENCE_ENTRY_SIZE;
            
            /* Set next state */
            g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_bwv_state_00;
            g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_refresh_state_06;
            g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_refresh_state_13;
        }
    }
    else
    {
        /* No more instances which are copied, then set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_refresh_state_08;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_05
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_06
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 06:
 *  Prepares to read the first/remaining data.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_06(void)
{
    /* Local variable definitions */
    uint8_t  l_u08_current_copy_size;
    
    /* Check whether remaining copy data exists or not */
    if (0u == g_st_ees_control_variables.u08_ees_refresh_copy_size)
    {
        /* No remaining copy data, then check next reference */
        g_st_ees_control_variables.u08_ees_refresh_ref_iterator++;
        
        /* Set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_refresh_state_05;
    }
    else
    {
        /* Remaining copy data exists, then set copy size */
        if (R_EES_VALUE_U08_EXRFD_DATA_BUFFER_SIZE > g_st_ees_control_variables.u08_ees_refresh_copy_size)
        {
            l_u08_current_copy_size = g_st_ees_control_variables.u08_ees_refresh_copy_size;
        }
        else
        {
            l_u08_current_copy_size = R_EES_VALUE_U08_EXRFD_DATA_BUFFER_SIZE;
        }
        
        /* Update control variables to copy current instance */
        g_st_ees_control_variables.u16_ees_refresh_copy_offset = 
            g_st_ees_control_variables.u16_ees_refresh_copy_offset - (uint16_t)l_u08_current_copy_size;
        g_st_ees_control_variables.u08_ees_refresh_copy_size   = 
            g_st_ees_control_variables.u08_ees_refresh_copy_size - l_u08_current_copy_size;
        
        /* Prepare to read a part of current instance */
        g_st_ees_control_variables.u16_exrfd_cmd_offset_addr = g_st_ees_control_variables.u16_ees_refresh_copy_offset;
        g_st_ees_control_variables.u16_exrfd_cmd_size        = l_u08_current_copy_size;
        g_st_ees_control_variables.np_u08_exrfd_cmd_data     = 
            &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
        
        /* Set next state (read error never occurs by EXRFD read command) */
        g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_read_state_00;
        g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_refresh_state_07;
        g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_exit_state;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_06
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_07
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 07:
 *  Prepares to write the first/remaining data.
 *  REFRESH state 06 must be executed just before this state, because same parameters for EXRFD command are used.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_07(void)
{
    /* Local variable definitions */
    
    /* Update DWP */
    g_st_ees_control_variables.u16_ees_dwp = g_st_ees_control_variables.u16_ees_dwp 
                                             - g_st_ees_control_variables.u16_exrfd_cmd_size;
    
    /* Prepare to write a part of current instance (size and data buffer have already been set by previous state) */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr = g_st_ees_control_variables.u16_ees_dwp + 1u;
    
    /* Set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_bwv_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_refresh_state_06;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_refresh_state_13;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_07
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_08
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 08:
 *  Prepares to write B-flag in the target refresh block.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_08(void)
{
    /* Local variable definitions */
    
    /* Prepare to write B-flag in active block header */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr        =  
        ((uint16_t)g_st_ees_control_variables.u08_ees_refresh_block 
        * g_st_ees_control_variables.u16_virtual_block_size) + R_EES_VALUE_U08_B_FLAG_INDEX;
    g_st_ees_control_variables.u16_exrfd_cmd_size               = 1u;       /* The size of B-flag is 1 */
    g_st_ees_control_variables.np_u08_exrfd_cmd_data            = 
        &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
    g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u] = 
        g_st_ees_control_variables.u08_ees_refresh_a_flag ^ (uint8_t)0xFFu; /* Data to write B-flag */
    
    /* Start REFRESH command again, then set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_bwv_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_refresh_state_09;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_refresh_state_13;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_08
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_09
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 09:
 *  Initializes RWP and DWP for the new RAM reference table.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_09(void)
{
    /* Local variable definitions */
    uint16_t l_u16_block_offset_address;
    
    /* Initialize RWP and DWP */
    l_u16_block_offset_address = (uint16_t)g_st_ees_control_variables.u08_ees_refresh_block 
                                 * g_st_ees_control_variables.u16_virtual_block_size;
    g_st_ees_control_variables.u16_ees_rwp = l_u16_block_offset_address + R_EES_VALUE_U08_HEADER_SIZE;
    g_st_ees_control_variables.u16_ees_dwp = (l_u16_block_offset_address 
                                              + g_st_ees_control_variables.u16_virtual_block_size) - 1u;
    
    /* Set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_refresh_state_10;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_09
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_10
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 10:
 *  Prepares to read first/next reference entry.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_10(void)
{
    /* Local variable definitions */
    
    /* Prepare to read reference entry */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr = g_st_ees_control_variables.u16_ees_rwp;
    g_st_ees_control_variables.u16_exrfd_cmd_size        = R_EES_VALUE_U08_REFERENCE_ENTRY_SIZE;
    g_st_ees_control_variables.np_u08_exrfd_cmd_data     = 
        &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
    
    /* Set next state (read error never occurs by EXRFD read command) */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_read_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_refresh_state_11;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_exit_state;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_10
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_11
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 11:
 *  Updates RAM reference table.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_11(void)
{
    /* Local variable definitions */
    uint8_t l_u08_sor;
    uint8_t l_u08_eor;
    uint8_t l_u08_current_data_size;
    
    /* Set local variables */
    l_u08_sor  = g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[R_EES_VALUE_U08_SOR_INDEX];
    l_u08_eor  = g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[R_EES_VALUE_U08_EOR_INDEX];
    
    /* Check reference entry */
    if ((0xFFu == l_u08_sor) && (0xFFu == l_u08_eor))
    {
        /* Separator is found, then finish updating the RAM reference table */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_refresh_state_12;
    }
    else
    {
        /* Reference entry is found */
        /* Get the size of the current data (assume that SoR is correct in refresh process) */
        l_u08_current_data_size = g_ar_u08_ees_descriptor[l_u08_sor];
        
        /* Update RWP */
        g_st_ees_control_variables.u16_ees_rwp = g_st_ees_control_variables.u16_ees_rwp 
                                                 + R_EES_VALUE_U08_REFERENCE_ENTRY_SIZE;
        
        /* Update DWP (assume DWP is always greater than RWP in refresh process) */
        g_st_ees_control_variables.u16_ees_dwp = g_st_ees_control_variables.u16_ees_dwp 
                                                 - l_u08_current_data_size;
        
        /* Update RAM reference table for current data (assume that SoR is correct in refresh process) */
        g_ar_u16_ram_ref_table[l_u08_sor - 1u] = g_st_ees_control_variables.u16_ees_dwp + 1u;
        
        /* Set next state to read the next reference entry */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_refresh_state_10;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_11
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_12
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 12:
 *  Prepares to write I-flag in old active block and changes refreshed block into new active block.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_12(void)
{
    /* Local variable definitions */

    /* Prepare to write I-flag in old active block header */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr        = 
        ((uint16_t)g_st_ees_control_variables.u08_ees_active0_block_index
         * g_st_ees_control_variables.u16_virtual_block_size) + R_EES_VALUE_U08_I_FLAG_INDEX;
    g_st_ees_control_variables.u16_exrfd_cmd_size               = 1u;       /* The size of I-flag is 1 */
    g_st_ees_control_variables.np_u08_exrfd_cmd_data            = 
        &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
    g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u] = 0x00u;    /* Data to write I-flag is 0x00 */
    
    /* Change refreshed block into new active block */
    g_st_ees_control_variables.u08_ees_active0_block_index   = g_st_ees_control_variables.u08_ees_refresh_block;
    g_st_ees_control_variables.u08_ees_active0_block_a_flag  = g_st_ees_control_variables.u08_ees_refresh_a_flag;
    
    /* Set next state (ignore write error) */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_write_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_refresh_state_17;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_refresh_state_17;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_12
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_13
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 13:
 *  Blankcheck or write error occurs.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_13(void)
{
    /* Local variable definitions */
    
    /* Check refresh retry-counter */
    if (R_EES_VALUE_U08_REFRESH_RETRY > g_st_ees_control_variables.u08_ees_refresh_retry)
    {
        /* Update refresh retry-counter */
        g_st_ees_control_variables.u08_ees_refresh_retry++;
        
        /* Set next state (retry refresh) */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_refresh_state_01;
    }
    else
    {
        /* Retry-counter reaches the limit, then set next state (exclude the target block) */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_refresh_state_14;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_13
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_14
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 14:
 *  Prepares to write X-flag and start REFRESH command again.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_14(void)
{
    /* Local variable definitions */
    
    /* Prepare to write X-flag in refresh block header */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr        =  
        ((uint16_t)g_st_ees_control_variables.u08_ees_refresh_block 
        * g_st_ees_control_variables.u16_virtual_block_size) + R_EES_VALUE_U08_X_FLAG_INDEX;
    g_st_ees_control_variables.u16_exrfd_cmd_size               = 1u;       /* The size of X-flag is 1 */
    g_st_ees_control_variables.np_u08_exrfd_cmd_data            = 
        &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
    g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u] = 0x00u;    /* Data to write X-flag is 0x00 */
    
    /* Set next state. When X-flag is written successfully, then start REFRESH command again */
    /* In other cases, the value of X-flag must be checked. */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_write_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_refresh_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_refresh_state_15;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_14
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_15
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 15:
 *  Prepares to read X-flag after X-flag is written with write error.
 *  REFRESH state 14 must be executed just before this state, because same parameters for EXRFD command are used.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_15(void)
{
    /* Local variable definitions */
    
    /* Omit to set parameters to read X-flag, because parameters have already been set by previous state */
    
    /* Set next state (read error never occurs by EXRFD read command) */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_read_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_refresh_state_16;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_exit_state;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_15
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_16
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 16:
 *  Checks read X-flag.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_16(void)
{
    /* Local variable definitions */
    
    /* Check read X-flag */
    if (0xFFu == g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u])
    {
        /* X-flag is not written at all, then it is fatal error for REFRESH command */
        /* Finish command with flash sequencer error, because REFRESH command cannot be started again */
        g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_FLASH_SEQ;
        
        /* Initialize EES control flags for safety (it is impossible to continue the control of the EES) */
        g_st_ees_control_flags.b_ees_initialized   = false;
        g_st_ees_control_flags.b_ees_opened        = false;
        g_st_ees_control_flags.b_ees_started_up    = false;
        g_st_ees_control_flags.b_ees_busy          = false;
        g_st_ees_control_flags.b_ees_read_enabled  = false;
        g_st_ees_control_flags.b_ees_write_enabled = false;
        g_st_ees_control_flags.b_ees_format_busy   = false;
        
        r_ees_fsm_finish_command();
    }
    else
    {
        /* X-flag is changed from 0xFF, then restart REFRESH command */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_refresh_state_00;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_16
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_refresh_state_17
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  REFRESH state 17:
 *  Finishes REFRESH command.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_refresh_state_17(void)
{
    /* Local variable definitions */
    
    /* Finish command */
    r_ees_fsm_finish_command();
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_refresh_state_17
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_format_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  FORMAT state 00:
 *  Initiates the FORMAT command.
 *  Control flags and variables will be cleared.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_format_state_00(void)
{
    /* Local variable definitions */
    
    /* Set EES control flags */
    g_st_ees_control_flags.b_ees_started_up    = false;
    g_st_ees_control_flags.b_ees_read_enabled  = false;
    g_st_ees_control_flags.b_ees_write_enabled = false;
    g_st_ees_control_flags.b_ees_format_busy   = true;
    
    /* Clear internal variables */
    g_st_ees_control_variables.u08_ees_block_iterator           = R_EES_VALUE_U08_INIT_VARIABLE;
    g_st_ees_control_variables.u08_ees_active0_block_index      = R_EES_VALUE_U08_INIT_VARIABLE;
    g_st_ees_control_variables.u08_ees_non_excluded_block_count = g_u08_ees_pool_virtual_blocks;
    
    /* Set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_format_state_01;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_format_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_format_state_01
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  FORMAT state 01:
 *  Prepares to read first/next block header.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_format_state_01(void)
{
    /* Local variable definitions */
    
    /* Prepare to read block header */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr = (uint16_t)g_st_ees_control_variables.u08_ees_block_iterator 
                                                           * g_st_ees_control_variables.u16_virtual_block_size;
    g_st_ees_control_variables.u16_exrfd_cmd_size        = R_EES_VALUE_U08_HEADER_SIZE;
    g_st_ees_control_variables.np_u08_exrfd_cmd_data     = 
        &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
    
    /* Set next state (read error never occurs by EXRFD read command) */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_read_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_format_state_02;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_exit_state;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_format_state_01
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_format_state_02
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  FORMAT state 02:
 *  Analyzes first/next block header. In case that the analyzed block is active, prepares to write I-flag.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_format_state_02(void)
{
    /* Local variable definitions */
    e_ees_block_status_t l_e_ees_block_status;
    
    /* Analyze block */
    l_e_ees_block_status = r_ees_fsm_get_ees_block_status();
    if (R_EES_ENUM_BLOCK_STATUS_ACTIVE == l_e_ees_block_status)
    {
        /* An active block is found */
        /* Prepare to write I-flag in active block header */
        g_st_ees_control_variables.u16_exrfd_cmd_offset_addr        =  
            ((uint16_t)g_st_ees_control_variables.u08_ees_block_iterator 
            * g_st_ees_control_variables.u16_virtual_block_size) + R_EES_VALUE_U08_I_FLAG_INDEX;
        g_st_ees_control_variables.u16_exrfd_cmd_size               = 1u;       /* The size of I-flag is 1 */
        g_st_ees_control_variables.np_u08_exrfd_cmd_data            = 
            &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
        g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u] = 0x00u;    /* Data to write I-flag is 0x00 */
        
        /* Set next state (ignore write error) */
        g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_write_state_00;
        g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_format_state_03;
        g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_format_state_03;
    }
    else
    {
        /* Set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_format_state_03;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_format_state_02
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_format_state_03
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  FORMAT state 03:
 *  Checks whether all blocks were analyzed or not.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_format_state_03(void)
{
    /* Local variable definitions */
    
    g_st_ees_control_variables.u08_ees_block_iterator++;
    if (g_st_ees_control_variables.u08_ees_block_iterator < g_u08_ees_pool_virtual_blocks)
    {
        /* Set next state (search another active block) */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_format_state_01;
    }
    else
    {
        /* All active blocks are invalidated, then clear iterator for the next state */
        g_st_ees_control_variables.u08_ees_block_iterator = R_EES_VALUE_U08_INIT_VARIABLE;
        
        /* Set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_format_state_04;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_format_state_03
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_format_state_04
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  FORMAT state 04:
 *  Prepares to erase a virtual block.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_format_state_04(void)
{
    /* Local variable definitions */
    
    /* Prepare to erase a virtual block */
    g_st_ees_control_variables.u08_exrfd_cmd_virtual_block_number = g_st_ees_control_variables.u08_ees_block_iterator;
    
    /* Set next state */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_erase_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_format_state_05;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_format_state_06;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_format_state_04
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_format_state_05
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  FORMAT state 05:
 *  Checks whether all blocks are erased or not.
 *  If all blocks are erased, at least 2 non excluded blocks must be available 
 *  otherwise EES pool exhausted will be returned.
 *  If everything is ok, the activation process will be started.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_format_state_05(void)
{
    /* Local variable definitions */
    
    g_st_ees_control_variables.u08_ees_block_iterator++;
    if (g_st_ees_control_variables.u08_ees_block_iterator < g_u08_ees_pool_virtual_blocks)
    {
        /* EES pool still has unerased block, then set next state (erase another block) */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_format_state_04;
    }
    else if (R_EES_EXRFD_VALUE_U08_POOL_VIRTUAL_BLOCKS_MIN 
             > g_st_ees_control_variables.u08_ees_non_excluded_block_count)
    {
        /* All blocks are erased, but EES pool is exhausted */
        g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_POOL_EXHAUSTED;
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_format_state_11;
    }
    else
    {
        /* All blocks are erased, then clear iterator for the next state */
        g_st_ees_control_variables.u08_ees_block_iterator = R_EES_VALUE_U08_INIT_VARIABLE;
        
        /* Set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_format_state_07;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_format_state_05
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_format_state_06
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  FORMAT state 06:
 *  Prepares to write X-flag.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_format_state_06(void)
{
    /* Local variable definitions */
    
    /* Decrement non-excluded blocks (never fall below zero) */
    g_st_ees_control_variables.u08_ees_non_excluded_block_count--;
    
    /* Prepare to write X-flag in active block header */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr        =  
        ((uint16_t)g_st_ees_control_variables.u08_ees_block_iterator 
        * g_st_ees_control_variables.u16_virtual_block_size) + R_EES_VALUE_U08_X_FLAG_INDEX;
    g_st_ees_control_variables.u16_exrfd_cmd_size               = 1u;       /* The size of X-flag is 1 */
    g_st_ees_control_variables.np_u08_exrfd_cmd_data            = 
        &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
    g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u] = 0x00u;    /* Data to write X-flag is 0x00 */
    
    /* Set next state (ignore write error) */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_write_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_format_state_05;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_format_state_05;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_format_state_06
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_format_state_07
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  FORMAT state 07:
 *  Prepares to read first/next erased block header.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_format_state_07(void)
{
    /* Local variable definitions */
    
    /* Prepare to read block header */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr = (uint16_t)g_st_ees_control_variables.u08_ees_block_iterator 
                                                           * g_st_ees_control_variables.u16_virtual_block_size;
    g_st_ees_control_variables.u16_exrfd_cmd_size        = R_EES_VALUE_U08_HEADER_SIZE;
    g_st_ees_control_variables.np_u08_exrfd_cmd_data     = 
        &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
    
    /* Set next state (read error never occurs by EXRFD read command) */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_read_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_format_state_08;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_exit_state;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_format_state_07
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_format_state_08
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  FORMAT state 08:
 *  Analyzes first/next erased block header.
 *  If the block status is not excluded, then prepare to write A and B-flag.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_format_state_08(void)
{
    /* Local variable definitions */
    e_ees_block_status_t l_e_ees_block_status;
    
    /* Analyze block */
    l_e_ees_block_status = r_ees_fsm_get_ees_block_status();
    if (R_EES_ENUM_BLOCK_STATUS_EXCLUDED == l_e_ees_block_status)
    {
        /* Excluded block is found, then set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_format_state_09;
    }
    else
    {
        /* Activate block */
        /* Prepare to write A and B-flag in active block header */
        g_st_ees_control_variables.u16_exrfd_cmd_offset_addr        =  
            ((uint16_t)g_st_ees_control_variables.u08_ees_block_iterator 
            * g_st_ees_control_variables.u16_virtual_block_size) + R_EES_VALUE_U08_A_FLAG_INDEX;
        g_st_ees_control_variables.u16_exrfd_cmd_size               = 2u;   /* Total size of A and B-flag is 2 */
        g_st_ees_control_variables.np_u08_exrfd_cmd_data            = 
            &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
        g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u] = 0x01u;    /* Data to write A-flag is 0x01 */
        g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[1u] = 0xFEu;    /* Data to write B-flag is 0xFE */
        
        /* Set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_bwv_state_00;
        g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_format_state_11;
        g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_format_state_10;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_format_state_08
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_format_state_09
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  FORMAT state 09:
 *  Checks non excluded block counter after excluding a block.
 *  If non excluded block counter is lower than 2 blocks, EES pool exhausted status will be returned.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_format_state_09(void)
{
    /* Local variable definitions */
    
    if (R_EES_EXRFD_VALUE_U08_POOL_VIRTUAL_BLOCKS_MIN 
        > g_st_ees_control_variables.u08_ees_non_excluded_block_count)
    {
        /* EES pool is exhausted */
        g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_POOL_EXHAUSTED;
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_format_state_11;
    }
    else
    {
        /* Try to activate next block */
        g_st_ees_control_variables.u08_ees_block_iterator++;
        
        /* Set next state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_format_state_07;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_format_state_09
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_format_state_10
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  FORMAT state 10:
 *  Prepares to write X-flag.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_format_state_10(void)
{
    /* Local variable definitions */
    
    /* Decrement non-excluded blocks (never fall below zero) */
    g_st_ees_control_variables.u08_ees_non_excluded_block_count--;
    
    /* Prepare to write X-flag in active block header */
    g_st_ees_control_variables.u16_exrfd_cmd_offset_addr        =  
        ((uint16_t)g_st_ees_control_variables.u08_ees_block_iterator 
        * g_st_ees_control_variables.u16_virtual_block_size) + R_EES_VALUE_U08_X_FLAG_INDEX;
    g_st_ees_control_variables.u16_exrfd_cmd_size               = 1u;       /* The size of X-flag is 1 */
    g_st_ees_control_variables.np_u08_exrfd_cmd_data            = 
        &g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u];
    g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[0u] = 0x00u;    /* Data to write X-flag is 0x00 */
    
    /* Set next state (ignore write error) */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exrfd_cmd_write_state_00;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_format_state_09;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_format_state_09;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_format_state_10
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_format_state_11
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  FORMAT state 11:
 *  Finishes FORMAT command.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_format_state_11(void)
{
    /* Local variable definitions */
    
    /* Set EES control flags */
    g_st_ees_control_flags.b_ees_format_busy = false;
    
    /* Finish command */
    r_ees_fsm_finish_command();
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_format_state_11
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_shutdown_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  SHUTDOWN state 00:
 *  Sets EES control flags and finishes SHUTDOWN command.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_shutdown_state_00(void)
{
    /* Local variable definitions */
    
    /* Set EES control flags */
    g_st_ees_control_flags.b_ees_started_up    = false;
    g_st_ees_control_flags.b_ees_read_enabled  = false;
    g_st_ees_control_flags.b_ees_write_enabled = false;
    g_st_ees_control_flags.b_ees_format_busy   = false;
    
    /* Finish command */
    g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_STS_OK;
    r_ees_fsm_finish_command();
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_shutdown_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_exrfd_cmd_erase_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  EXRFD erase command state 00:
 *  Starts EXRFD erase command.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_erase_state_00(void)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    bool l_b_detect_ees_exrfd_error_flag;
    
    /* Execute EXRFD erase command */
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_Erase(g_st_ees_control_variables.u08_exrfd_cmd_virtual_block_number);
    
    /* Set EXRFD return status */
    g_st_ees_control_variables.e_ees_fsm_exrfd_ret_status = l_e_ees_exrfd_ret_value;
    
    /* Check EXRFD error */
    l_b_detect_ees_exrfd_error_flag = r_ees_fsm_exrfd_cmd_detect_fatal_error(l_e_ees_exrfd_ret_value);
    
    if (true == l_b_detect_ees_exrfd_error_flag)
    {
        /* Fatal error is detected, then finish command */
        r_ees_fsm_finish_command();
    }
    else
    {
        /* Set remaining EXRFD command counter */
        g_st_ees_control_variables.u08_ees_remain_exrfd_cmd_count = R_EES_VALUE_U08_INIT_VARIABLE;
        
        /* Clear next EXRFD command state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_exrfd_cmd_state_01;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_exrfd_cmd_erase_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_exrfd_cmd_bwv_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  EXRFD blankcheck, write and iverify command state 00:
 *  Starts EXRFD blankcheck,write and iverify command.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_bwv_state_00(void)
{
    /* Local variable definitions */
    
    /* Set remaining EXRFD command counter for EXRFD write command */
    g_st_ees_control_variables.u08_ees_remain_exrfd_cmd_count = 2u;
    
    /* Call inner EXRFD blankcheck command */
    r_ees_fsm_exrfd_cmd_inner_blankcheck_state_00();
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_exrfd_cmd_bwv_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_exrfd_cmd_inner_blankcheck_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Inner EXRFD blankcheck command state 00:
 *  Starts EXRFD blankcheck command.
 *  This state does not set remaining EXRFD command counter.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_inner_blankcheck_state_00(void)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    bool l_b_detect_ees_exrfd_error_flag;
    
    /* Execute EXRFD blankcheck command */
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_BlankCheck(g_st_ees_control_variables.u16_exrfd_cmd_offset_addr, 
                                                     g_st_ees_control_variables.u16_exrfd_cmd_size);
    
    /* Set EXRFD return status */
    g_st_ees_control_variables.e_ees_fsm_exrfd_ret_status = l_e_ees_exrfd_ret_value;
    
    /* Check EXRFD error */
    l_b_detect_ees_exrfd_error_flag = r_ees_fsm_exrfd_cmd_detect_fatal_error(l_e_ees_exrfd_ret_value);
    
    if (true == l_b_detect_ees_exrfd_error_flag)
    {
        /* Fatal error is detected, then finish command */
        r_ees_fsm_finish_command();
    }
    else
    {
        /* Set next EXRFD command state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_exrfd_cmd_state_01;
    }
    
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_exrfd_cmd_inner_blankcheck_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_exrfd_cmd_write_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  EXRFD write command state 00:
 *  Starts EXRFD write command.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_write_state_00(void)
{
    /* Local variable definitions */
    
    /* Clear remaining EXRFD command counter */
    g_st_ees_control_variables.u08_ees_remain_exrfd_cmd_count = R_EES_VALUE_U08_INIT_VARIABLE;
    
    /* Call inner EXRFD command */
    r_ees_fsm_exrfd_cmd_inner_write_state_00();
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_exrfd_cmd_write_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_exrfd_cmd_inner_write_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Inner EXRFD write command state 00:
 *  Starts EXRFD write command.
 *  This state does not set remaining EXRFD command counter.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_inner_write_state_00(void)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    bool l_b_detect_ees_exrfd_error_flag;
    
    /* Execute EXRFD write command */
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_Write(g_st_ees_control_variables.u16_exrfd_cmd_offset_addr, 
                                                g_st_ees_control_variables.np_u08_exrfd_cmd_data, 
                                                g_st_ees_control_variables.u16_exrfd_cmd_size);
    
    /* Set EXRFD return status */
    g_st_ees_control_variables.e_ees_fsm_exrfd_ret_status = l_e_ees_exrfd_ret_value;
    
    /* Check EXRFD error */
    l_b_detect_ees_exrfd_error_flag = r_ees_fsm_exrfd_cmd_detect_fatal_error(l_e_ees_exrfd_ret_value);
    
    if (true == l_b_detect_ees_exrfd_error_flag)
    {
        /* Fatal error is detected, then finish command */
        r_ees_fsm_finish_command();
    }
    else
    {
        /* Set next EXRFD command state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_exrfd_cmd_state_01;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_exrfd_cmd_inner_write_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_exrfd_cmd_iverify_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  EXRFD iverify command state 00:
 *  Starts EXRFD iverify command.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_iverify_state_00(void)
{
    /* Local variable definitions */
    
    /* Clear remaining EXRFD command counter */
    g_st_ees_control_variables.u08_ees_remain_exrfd_cmd_count = R_EES_VALUE_U08_INIT_VARIABLE;
    
    /* Call inner EXRFD command */
    r_ees_fsm_exrfd_cmd_inner_iverify_state_00();
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_exrfd_cmd_iverify_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_exrfd_cmd_inner_iverify_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Inner EXRFD iverify command state 00:
 *  Starts EXRFD iverify command.
 *  This state does not set remaining EXRFD command counter.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_inner_iverify_state_00(void)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    bool l_b_detect_ees_exrfd_error_flag;
    
    /* Execute EXRFD iverify command */
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_IVerify(g_st_ees_control_variables.u16_exrfd_cmd_offset_addr, 
                                                  g_st_ees_control_variables.u16_exrfd_cmd_size);
    
    /* Set EXRFD return status */
    g_st_ees_control_variables.e_ees_fsm_exrfd_ret_status = l_e_ees_exrfd_ret_value;
    
    /* Check EXRFD error */
    l_b_detect_ees_exrfd_error_flag = r_ees_fsm_exrfd_cmd_detect_fatal_error(l_e_ees_exrfd_ret_value);
    
    if (true == l_b_detect_ees_exrfd_error_flag)
    {
        /* Fatal error is detected, then finish command */
        r_ees_fsm_finish_command();
    }
    else
    {
        /* Set next EXRFD command state */
        g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_exrfd_cmd_state_01;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_exrfd_cmd_inner_iverify_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_exrfd_cmd_read_state_00
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  EXRFD read command state 00:
 *  Starts EXRFD read command and finishes.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_read_state_00(void)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    bool l_b_detect_ees_exrfd_error_flag;
    
    /* Execute EXRFD read command */
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_Read(g_st_ees_control_variables.u16_exrfd_cmd_offset_addr, 
                                               g_st_ees_control_variables.np_u08_exrfd_cmd_data, 
                                               g_st_ees_control_variables.u16_exrfd_cmd_size);
    
    /* Set EXRFD return status */
    g_st_ees_control_variables.e_ees_fsm_exrfd_ret_status = l_e_ees_exrfd_ret_value;
    
    /* Check EXRFD error */
    l_b_detect_ees_exrfd_error_flag = r_ees_fsm_exrfd_cmd_detect_fatal_error(l_e_ees_exrfd_ret_value);
    
    if (true == l_b_detect_ees_exrfd_error_flag)
    {
        /* Fatal error is detected, then finish command */
        r_ees_fsm_finish_command();
    }
    else
    {
        /* Return to the EES command state */
        g_st_ees_control_variables.fp_fnc_fsm_state = g_st_ees_control_variables.fp_fnc_fsm_ret_state;
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_exrfd_cmd_read_state_00
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_exrfd_cmd_state_01
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  EXRFD command state 01:
 *  Continues performing EXRFD command.
 *  When EXRFD command returns a status except BUSY, finishes EXRFD command.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_exrfd_cmd_state_01(void)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    bool l_b_detect_ees_exrfd_error_flag;
    
    /* Execute EXRFD command */
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_Handler();
    
    /* Set EXRFD return status */
    g_st_ees_control_variables.e_ees_fsm_exrfd_ret_status = l_e_ees_exrfd_ret_value;
    
    if (R_EES_EXRFD_ENUM_RET_STS_BUSY == l_e_ees_exrfd_ret_value)
    {
        /* Execute this state again, then no operation */
    }
    else if (R_EES_EXRFD_ENUM_RET_STS_OK == l_e_ees_exrfd_ret_value)
    {
        if (1u < g_st_ees_control_variables.u08_ees_remain_exrfd_cmd_count)
        {
            /* Set remaining EXRFD command counter for EXRFD iverify command */
            g_st_ees_control_variables.u08_ees_remain_exrfd_cmd_count = 1u;
            
            /* EXRFD blankcheck command is done, but EXRFD write command remains */
            g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_exrfd_cmd_inner_write_state_00;
        }
        else if (1u == g_st_ees_control_variables.u08_ees_remain_exrfd_cmd_count)
        {
            /* Clear remaining EXRFD command counter */
            g_st_ees_control_variables.u08_ees_remain_exrfd_cmd_count = R_EES_VALUE_U08_INIT_VARIABLE;
            
            /* EXRFD write command is done, but EXRFD iverify command remains */
            g_st_ees_control_variables.fp_fnc_fsm_state = &r_ees_fsm_exrfd_cmd_inner_iverify_state_00;
        }
        else
        {
            /* EXRFD command is done, then return to the EES command state */
            g_st_ees_control_variables.fp_fnc_fsm_state = g_st_ees_control_variables.fp_fnc_fsm_ret_state;
        }
        
    }
    else
    {
        /* Check fatal EXRFD error */
        l_b_detect_ees_exrfd_error_flag = r_ees_fsm_exrfd_cmd_detect_fatal_error(l_e_ees_exrfd_ret_value);
        
        if (true == l_b_detect_ees_exrfd_error_flag)
        {
            /* Fatal error is detected, then finish command */
            r_ees_fsm_finish_command();
        }
        else
        {
            /* Return to the EES command state with error */
            g_st_ees_control_variables.fp_fnc_fsm_state = g_st_ees_control_variables.fp_fnc_fsm_ret_err_state;
        }
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_exrfd_cmd_state_01
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_exit_state
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  This function is a dummy state for safety (default state).
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void r_ees_fsm_exit_state(void)
{
    /* No operation */
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_exit_state
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * End of functions for finite state machine (FSM)
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Internal functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name : r_ees_calculate_next_a_flag
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Calculates next A-flag value.
 *  
 *  @param[in]      i_u08_a_flag_value : 
 *                    A-flag value
 *  @return         Next A-flag value
 */
/*********************************************************************************************************************/
static R_EES_FAR_FUNC uint8_t r_ees_calculate_next_a_flag(uint8_t i_u08_a_flag_value)
{
    /* Local variable definitions */
    uint8_t l_next_a_flag_value;
    
    /* Set local variables */
    l_next_a_flag_value = i_u08_a_flag_value + 1u;
    
    if (l_next_a_flag_value > R_EES_VALUE_U08_A_FLAG_ORDER_MAX)
    {
        l_next_a_flag_value = R_EES_VALUE_U08_A_FLAG_ORDER_MIN;
    }
    else
    {
        /* No operation */
    }
    
    return (l_next_a_flag_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_calculate_next_a_flag
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_finish_command
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Finishes FSM of the running command.
 *  This internal function implicitly uses global variables.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
static R_EES_FAR_FUNC void r_ees_fsm_finish_command(void)
{
    /* Local variable definitions */
    
    /* Set status of the finished command */
    g_st_ees_control_variables.np_st_ees_request->e_status = g_st_ees_control_variables.e_ees_temp_status;
    
    /* Clear busy status */
    g_st_ees_control_flags.b_ees_busy = false;
    
    /* Clear fsm state */
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exit_state;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_exit_state;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_exit_state;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_finish_command
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_swap_acvive_block_info
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Swaps information of active blocks.
 *  This internal function implicitly uses global variables.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
static R_EES_FAR_FUNC void r_ees_fsm_swap_acvive_block_info(void)
{
    /* Local variable definitions */
    uint8_t l_u08_temp_block_index;
    uint8_t l_u08_temp_block_a_flag;
    
    /* Swap information of active blocks */
    l_u08_temp_block_index   = g_st_ees_control_variables.u08_ees_active0_block_index;
    l_u08_temp_block_a_flag  = g_st_ees_control_variables.u08_ees_active0_block_a_flag;
    g_st_ees_control_variables.u08_ees_active0_block_index   = g_st_ees_control_variables.u08_ees_active1_block_index;
    g_st_ees_control_variables.u08_ees_active0_block_a_flag  = g_st_ees_control_variables.u08_ees_active1_block_a_flag;
    g_st_ees_control_variables.u08_ees_active1_block_index   = l_u08_temp_block_index;
    g_st_ees_control_variables.u08_ees_active1_block_a_flag  = l_u08_temp_block_a_flag;
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_swap_acvive_block_info
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_exrfd_cmd_detect_fatal_error
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Detects fatal error of EXRFD command.
 *  If any fatal error of EXRFD command is detected, then EES command will be finished forcibly.
 *  This internal function implicitly uses global variables.
 *  
 *  @param[in]      l_e_ees_exrfd_ret_value : 
 *                    EXRFD error
 *  @return
 *                  - true  : Fatal error is detected
 *                  - false : Fatal error is not detected
 */
/*********************************************************************************************************************/
static R_EES_FAR_FUNC bool r_ees_fsm_exrfd_cmd_detect_fatal_error(e_ees_exrfd_ret_status_t i_e_ees_exrfd_ret_value)
{
    /* Local variable definitions */
    bool l_b_detect_fatal_error_flag;
    
    /* Detect fatal error of EXRFD command */
    if (R_EES_EXRFD_ENUM_RET_ERR_INITIALIZATION == i_e_ees_exrfd_ret_value)
    {
        g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_INITIALIZATION;
        l_b_detect_fatal_error_flag = true;
    }
    else if ((R_EES_EXRFD_ENUM_RET_ERR_REJECTED == i_e_ees_exrfd_ret_value) 
             || (R_EES_EXRFD_ENUM_RET_ERR_PARAMETER == i_e_ees_exrfd_ret_value) 
             || (R_EES_EXRFD_ENUM_RET_ERR_INTERNAL == i_e_ees_exrfd_ret_value))
    {
        g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_INTERNAL;
        l_b_detect_fatal_error_flag = true;
    }
    else if ((R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED == i_e_ees_exrfd_ret_value) 
             || (R_EES_EXRFD_ENUM_RET_ERR_CFDF_SEQUENCER == i_e_ees_exrfd_ret_value))
    {
        g_st_ees_control_variables.e_ees_temp_status = R_EES_ENUM_RET_ERR_FLASH_SEQ;
        l_b_detect_fatal_error_flag = true;
    }
    else
    {
        l_b_detect_fatal_error_flag = false;
    }
    
    if (true == l_b_detect_fatal_error_flag)
    {
        /* Fatal error occurs */
        /* Initialize EES control flags for safety (it is impossible to continue the control of the EES) */
        g_st_ees_control_flags.b_ees_initialized   = false;
        g_st_ees_control_flags.b_ees_opened        = false;
        g_st_ees_control_flags.b_ees_started_up    = false;
        g_st_ees_control_flags.b_ees_busy          = false;
        g_st_ees_control_flags.b_ees_read_enabled  = false;
        g_st_ees_control_flags.b_ees_write_enabled = false;
        g_st_ees_control_flags.b_ees_format_busy   = false;
    }
    else
    {
        /* No operation */
    }
    
    return (l_b_detect_fatal_error_flag);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_exrfd_cmd_detect_fatal_error
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_fsm_get_ees_block_status
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Gets EES block status from internal data buffer.
 *  The internal data buffer for EXRFD command must be set before this function.
 *  This internal function implicitly uses global variables.
 *
 *  @param[in]      -
 *  @return         EES block status from the internal data buffer (block header)
 */
/*********************************************************************************************************************/
static R_EES_FAR_FUNC e_ees_block_status_t r_ees_fsm_get_ees_block_status(void)
{
    /* Local variable definitions */
    e_ees_block_status_t l_e_ees_block_status;
    uint8_t l_u08_a_flag;
    uint8_t l_u08_b_flag;
    
    /* Set local variables */
    l_u08_a_flag = g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[R_EES_VALUE_U08_A_FLAG_INDEX];
    l_u08_b_flag = g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[R_EES_VALUE_U08_B_FLAG_INDEX];
    
    /* Check block header */
    if (0xFFu != g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[R_EES_VALUE_U08_X_FLAG_INDEX])
    {
        /* Block status is "Excluded" */
        l_e_ees_block_status = R_EES_ENUM_BLOCK_STATUS_EXCLUDED;
    }
    else if (0xFFu != g_st_ees_control_variables.ar_u08_exrfd_cmd_data_buffer[R_EES_VALUE_U08_I_FLAG_INDEX]) 
    {
        /* Block status is "Invalid" */
        l_e_ees_block_status = R_EES_ENUM_BLOCK_STATUS_INVALID;
    }
    else if (((R_EES_VALUE_U08_A_FLAG_ORDER_MIN <= l_u08_a_flag) 
               && (R_EES_VALUE_U08_A_FLAG_ORDER_MAX >= l_u08_a_flag)) 
             && (0xFFu == (l_u08_a_flag ^ l_u08_b_flag)))
    {
        /* Block status is "Active" */
        l_e_ees_block_status = R_EES_ENUM_BLOCK_STATUS_ACTIVE;
    }
    else
    {
        /* Block status is "Invalid" */
        l_e_ees_block_status = R_EES_ENUM_BLOCK_STATUS_INVALID;
    }
    
    return (l_e_ees_block_status);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_fsm_get_ees_block_status
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * End of internal functions
 *********************************************************************************************************************/
