/**********************************************************************************************************************
    Program Name    : Sample program for EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : main.c
    Program Version : V1.00.00
    Device(s)       : RL78/F24 microcontroller
    Description     : Sample program for EES control
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
#include "sample_control_ees.h"
#include "sample_ees_defines.h"

#include "ior7f124fpj.h"  

/**********************************************************************************************************************
  Prototype declaration
**********************************************************************************************************************/
int main(void);

/**********************************************************************************************************************
 * Function name : main
 *********************************************************************************************************************/
/**
 *  Sample main.
 *  
 *  @param[in]      -
 *  @return         Execution result status
 *                  - R_EES_ENUM_RET_STS_OK                : No error occurred
 *                  - R_EES_ENUM_RET_STS_BUSY              : Request is under processing
 *                  - R_EES_ENUM_RET_ERR_CONFIGURATION     : Invalid EES configuration
 *                  - R_EES_ENUM_RET_ERR_INITIALIZATION    : R_EES_Init, R_EES_Open missing
 *                  - R_EES_ENUM_RET_ERR_ACCESS_LOCKED     : STARTUP missing or fatal operation error
 *                  - R_EES_ENUM_RET_ERR_PARAMETER         : Wrong parameter (wrong command or identifier)
 *                  - R_EES_ENUM_RET_ERR_VERIFY            : Verify error on any byte in the active block
 *                  - R_EES_ENUM_RET_ERR_REJECTED          : Another request already under processing
 *                  - R_EES_ENUM_RET_ERR_NO_INSTANCE       : No instance found (variable never written)
 *                  - R_EES_ENUM_RET_ERR_POOL_FULL         : No space for writing data
 *                  - R_EES_ENUM_RET_ERR_POOL_INCONSISTENT : No active block found within EES pool
 *                  - R_EES_ENUM_RET_ERR_POOL_EXHAUSTED    : EES pool is too small for correct operation
 *                  - R_EES_ENUM_RET_ERR_INTERNAL          : Internal error
 *                  - R_EES_ENUM_RET_ERR_FLASH_SEQ         : Flash sequencer error
 */
/*********************************************************************************************************************/
int main(void)
{
    /* Local variable definition */
    e_ees_ret_status_t l_e_ees_ret_value;
    
    /* Check whether HOCO is already started */
    if (SAMPLE_VALUE_U01_MASK0_1BIT == HIOSTOP)
    {
        l_e_ees_ret_value = Sample_EES_Control();
    }
    else
    {
        /* HOCO is not started */
        l_e_ees_ret_value = R_EES_ENUM_RET_ERR_CONFIGURATION;
    }
    
    return ((int)l_e_ees_ret_value);
}
/**********************************************************************************************************************
 End of function main
 *********************************************************************************************************************/
