/**********************************************************************************************************************
    Program Name    : EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : r_ees_api.c
    Program Version : V1.00
    Device(s)       : RL78/F24 microcontroller
    Description     : EES control program
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
#include "r_ees_api.h"
#include "r_ees_sub_api.h"
#include "r_ees_exrfd_api.h"

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_Init
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Initialization of EES internal data, variables as well plausibility check of EES configuration.
 *  
 *  @param[in]      i_u08_cpu_frequency : 
                      CPU operating frequency (MHz)
 *  @return         Execution result status
 *                  - R_EES_ENUM_RET_STS_OK              : Successful completion
 *                  - R_EES_ENUM_RET_ERR_CONFIGURATION   : Configuration error
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_ret_status_t R_EES_Init(uint8_t i_u08_cpu_frequency)
{
    /* Local variable definitions */
    e_ees_ret_status_t       l_e_ees_ret_value;
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    bool                     l_b_valid_config_flag;
    
    /* Initialize EES control flags */
    g_st_ees_control_flags.b_ees_initialized   = false;
    g_st_ees_control_flags.b_ees_opened        = false;
    g_st_ees_control_flags.b_ees_started_up    = false;
    g_st_ees_control_flags.b_ees_busy          = false;
    g_st_ees_control_flags.b_ees_read_enabled  = false;
    g_st_ees_control_flags.b_ees_write_enabled = false;
    g_st_ees_control_flags.b_ees_format_busy   = false;
    
    /* Clear internal variables */
    g_st_ees_control_variables.u16_ees_rwp = R_EES_VALUE_U16_INIT_VARIABLE;
    g_st_ees_control_variables.u16_ees_dwp = R_EES_VALUE_U16_INIT_VARIABLE;
    g_st_ees_control_variables.fp_fnc_fsm_state         = &r_ees_fsm_exit_state;
    g_st_ees_control_variables.fp_fnc_fsm_ret_state     = &r_ees_fsm_exit_state;
    g_st_ees_control_variables.fp_fnc_fsm_ret_err_state = &r_ees_fsm_exit_state;
    
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_Init(i_u08_cpu_frequency);
    if (R_EES_EXRFD_ENUM_RET_STS_OK == l_e_ees_exrfd_ret_value)
    {
        /* Check EES configuration */
        l_b_valid_config_flag = r_ees_is_valid_configuration();
        if (true == l_b_valid_config_flag)
        {
            /* Set EES control flags (initialized) */
            g_st_ees_control_flags.b_ees_initialized = true;
            
            l_e_ees_ret_value = R_EES_ENUM_RET_STS_OK;
        }
        else
        {
            /* EES configuration is invalid */
            l_e_ees_ret_value = R_EES_ENUM_RET_ERR_CONFIGURATION;
        }
    }
    else
    {
        /* Failed to initialize EXRFD */
        l_e_ees_ret_value = R_EES_ENUM_RET_ERR_CONFIGURATION;
    }
    
    return (l_e_ees_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_Init
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_Open
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Opens the EES logically and activates the data flash electrically.
 *  
 *  @param[in]      -
 *  @return         Execution result status
 *                  - R_EES_ENUM_RET_STS_OK       : Successful completion
 *                  - R_EES_ENUM_RET_ERR_REJECTED : EES is not initialized
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_ret_status_t R_EES_Open(void)
{
    /* Local variable definitions */
    e_ees_ret_status_t       l_e_ees_ret_value;
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    
    if (true == g_st_ees_control_flags.b_ees_initialized)
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_Open();
        if (R_EES_EXRFD_ENUM_RET_STS_OK == l_e_ees_exrfd_ret_value)
        {
            g_st_ees_control_flags.b_ees_opened = true;
            l_e_ees_ret_value = R_EES_ENUM_RET_STS_OK;
        }
        else
        {
            l_e_ees_ret_value = R_EES_ENUM_RET_ERR_REJECTED;
        }
    }
    else
    {
        l_e_ees_ret_value = R_EES_ENUM_RET_ERR_REJECTED;
    }
    
    return (l_e_ees_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_Open
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_Close
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Closes the EES logically and deactivates the data flash electrically.
 *  
 *  @param[in]      -
 *  @return         Execution result status
 *                  - R_EES_ENUM_RET_STS_OK       : Completion
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_ret_status_t R_EES_Close(void)
{
    /* Local variable definitions */
    e_ees_ret_status_t       l_e_ees_ret_value;
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_Close();
    
    /* R_EES_EXRFD_Close() always returns R_EES_EXRFD_ENUM_RET_STS_OK */
    /* No need to check return value, but it is checked. */
    if (R_EES_EXRFD_ENUM_RET_STS_OK == l_e_ees_exrfd_ret_value)
    {
        l_e_ees_ret_value = R_EES_ENUM_RET_STS_OK;
    }
    else
    {
        /* Ignore any error (never occurs) */
        l_e_ees_ret_value = R_EES_ENUM_RET_STS_OK;
    }
    
    g_st_ees_control_flags.b_ees_opened        = false;
    g_st_ees_control_flags.b_ees_started_up    = false;
    g_st_ees_control_flags.b_ees_busy          = false;
    g_st_ees_control_flags.b_ees_read_enabled  = false;
    g_st_ees_control_flags.b_ees_write_enabled = false;
    g_st_ees_control_flags.b_ees_format_busy   = false;
    
    return (l_e_ees_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_Close
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_Execute
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Initiates the EES command.
 *  
 *  @param[in]      ionp_st_ees_request : 
 *                    Pointer to the EES requester
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void R_EES_Execute(st_ees_request_t __near * ionp_st_ees_request)
{
    /* Local variable definitions */
    bool l_b_valid_requester_flag;
    
    /* Check requester */
    l_b_valid_requester_flag = r_ees_is_valid_requester(ionp_st_ees_request);
    if (true == l_b_valid_requester_flag)
    {
        /* Call R_EES_Handler in case request accepted */
        R_EES_Handler();
    }
    else
    {
        /* No operation */
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_Execute
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_Handler
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Continues the running command.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC void R_EES_Handler(void)
{
    /* Local variable definitions */
    
    if ((true == g_st_ees_control_flags.b_ees_initialized) && (true == g_st_ees_control_flags.b_ees_busy))
    {
        g_st_ees_control_variables.fp_fnc_fsm_state();
    }
    else
    {
        /* No operation */
    }
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_Handler
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_GetSpace
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Gets the momentary space in the entire EES pool expressed in bytes.
 *  
 *  @param[in]      onp_u16_space : 
 *                    Pointer to the variable that stores space in the EES pool
 *  @return         Execution result status
 *                  - R_EES_ENUM_RET_STS_OK             : Successful completion
 *                  - R_EES_ENUM_RET_ERR_INITIALIZATION : R_EES_Init() or R_EES_Open() missing
 *                  - R_EES_ENUM_RET_ERR_ACCESS_LOCKED  : STARTUP command missing
 *                  - R_EES_ENUM_RET_ERR_REJECTED       : Any command is running
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_ret_status_t R_EES_GetSpace(uint16_t __near * onp_u16_space)
{
    /* Local variable definitions */
    e_ees_ret_status_t l_e_ees_ret_value;
    
    if ((true == g_st_ees_control_flags.b_ees_initialized) && (true == g_st_ees_control_flags.b_ees_opened))
    {
        if (true != g_st_ees_control_flags.b_ees_busy)
        {
            if (true == g_st_ees_control_flags.b_ees_started_up)
            {
                /* Calculate space */
                if ((true == g_st_ees_control_flags.b_ees_write_enabled) 
                    && (((g_st_ees_control_variables.u16_ees_rwp + R_EES_VALUE_U08_SEPARATOR_SIZE) - 1u) 
                        < g_st_ees_control_variables.u16_ees_dwp))
                {
                    /* Free space = DWP - RWP - SEPARATOR_SIZE + 1 */
                    * onp_u16_space = ((g_st_ees_control_variables.u16_ees_dwp 
                                        - g_st_ees_control_variables.u16_ees_rwp) 
                                       - R_EES_VALUE_U08_SEPARATOR_SIZE) + 1u;
                }
                else
                {
                    * onp_u16_space = 0u;
                }
                l_e_ees_ret_value = R_EES_ENUM_RET_STS_OK;
            }
            else
            {
                /* Startup command is not executed */
                l_e_ees_ret_value = R_EES_ENUM_RET_ERR_ACCESS_LOCKED;
            }
        }
        else
        {
            /* EES is busy */
            l_e_ees_ret_value = R_EES_ENUM_RET_ERR_REJECTED;
        }
    }
    else
    {
        /* EES is not initialized */
        l_e_ees_ret_value = R_EES_ENUM_RET_ERR_INITIALIZATION;
    }
    
    return (l_e_ees_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_GetSpace
 *********************************************************************************************************************/
