/**********************************************************************************************************************
    Program Name    : Sample program for EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : sample_control_ees.c
    Program Version : V1.00.00
    Device(s)       : RL78/F24 microcontroller
    Description     : Sample program for EES
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

#include "sample_control_ees.h"
#include "sample_ees_defines.h"
#include "sample_config.h"
#include "r_ees_user_types.h"

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Static global variables
 *********************************************************************************************************************/
#define  SAMPLE_START_SECTION_SMP_VAR
#include "sample_ees_memmap.h"
/* Write buffer for EES variables */
/* The number of array elements depends on the maximum size of EES variables */
static __near uint8_t sg_ar_u08_write_data[255u];
#define  SAMPLE_END_SECTION_SMP_VAR
#include "sample_ees_memmap.h"

#define  SAMPLE_START_SECTION_SMP_VAR
#include "sample_ees_memmap.h"
/* Read buffer for EES variables */
/* The number of array elements depends on the maximum size of EES variables */
static __near uint8_t sg_ar_u08_read_data[255u];
#define  SAMPLE_END_SECTION_SMP_VAR
#include "sample_ees_memmap.h"

#define  SAMPLE_START_SECTION_SMP_VAR
#include "sample_ees_memmap.h"
static __near st_ees_request_t sg_st_ees_request;
#define  SAMPLE_END_SECTION_SMP_VAR
#include "sample_ees_memmap.h"


/**********************************************************************************************************************
 * Function name : Sample_EES_Control
 *********************************************************************************************************************/
#define  SAMPLE_START_SECTION_SMP_EES
#include "sample_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Sample function for EES control.
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
 *
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_ret_status_t Sample_EES_Control(void)
{
    /* Local variable definitions */
    e_ees_ret_status_t l_e_ees_ret_value;
    bool               l_b_sam_error_flag;
    uint16_t           l_u16_count;
    
    /* Set local variables */
    l_e_ees_ret_value  = R_EES_ENUM_RET_STS_OK;
    l_b_sam_error_flag = false;
    
    /******************************************************************************************************************
     * Initialize EES
     *****************************************************************************************************************/
    l_e_ees_ret_value = R_EES_Init(SAMPLE_VALUE_U08_CPU_FREQUENCY);
    if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
    {
        /* EES was initialized successfully */
        /* No operation */
    }
    else
    {
        /* Error handling */
        l_b_sam_error_flag = true;
    }
    
    /******************************************************************************************************************
     * Open EES
     *****************************************************************************************************************/
    if (true != l_b_sam_error_flag)
    {
        l_e_ees_ret_value = R_EES_Open();
        if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
        {
            /* EES was opened successfully */
            /* No operation */
        }
        else
        {
            /* Error handling */
            l_b_sam_error_flag = true;
        }
    }
    else
    {
        /* No operation */
    }
    
    /* Initialize EES requester */
    sg_st_ees_request.np_u08_address = &sg_ar_u08_write_data[0];
    sg_st_ees_request.u08_identifier = SAMPLE_VALUE_U08_INIT_VARIABLE;
    sg_st_ees_request.e_command      = R_EES_ENUM_CMD_UNDEFINED;
    
    /******************************************************************************************************************
     * STARTUP
     *****************************************************************************************************************/
    if (true != l_b_sam_error_flag)
    {
        do
        {
            /* Set parameters for STARTUP command */
            sg_st_ees_request.e_command = R_EES_ENUM_CMD_STARTUP;
            
            /* Execute STARTUP command */
            R_EES_Execute(&sg_st_ees_request);
            
            /* Wait for the end of running command */
            while (R_EES_ENUM_RET_STS_BUSY == sg_st_ees_request.e_status)
            {
                /* It is possible to write the program for detecting timeout here as necessity requires */
                R_EES_Handler();
            }
            
            l_e_ees_ret_value = sg_st_ees_request.e_status;
            
            /* Check returned status of the command */
            if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
            {
                /* STARTUP command was finished successfully */
                /* No operation */
            }
            else if (R_EES_ENUM_RET_ERR_POOL_INCONSISTENT == l_e_ees_ret_value)
            {
                /******************************************************************************************************
                 * FORMAT
                 *****************************************************************************************************/
                /* Set parameters for FORMAT command */
                sg_st_ees_request.e_command = R_EES_ENUM_CMD_FORMAT;
                
                /* Execute FORMAT command */
                R_EES_Execute(&sg_st_ees_request);
                
                /* Wait for the end of running command */
                while (R_EES_ENUM_RET_STS_BUSY == sg_st_ees_request.e_status)
                {
                    /* It is possible to write the program for detecting timeout here as necessity requires */
                    R_EES_Handler();
                }
                
                l_e_ees_ret_value = sg_st_ees_request.e_status;
                
                /* Check returned status of the command */
                if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
                {
                    /* FORMAT command was finished successfully */
                    /* No operation */
                }
                else
                {
                    /* FORMAT error handling */
                    l_b_sam_error_flag = true;
                }
            }
            else
            {
                /* STARTUP error handling */
                /* If R_EES_ENUM_RET_ERR_VERIFY is returned, then REFRESH command should be executed */
                l_b_sam_error_flag = true;
            }
        } while ((R_EES_ENUM_CMD_FORMAT == sg_st_ees_request.e_command)
                 && (true != l_b_sam_error_flag));
    }
    else
    {
        /* No operation */
    }
    
    /******************************************************************************************************************
     * WRITE
     *****************************************************************************************************************/
    if (true != l_b_sam_error_flag)
    {
        do
        {
            /* Set data to write */
            sg_ar_u08_write_data[0] = 0xAAu;
            sg_ar_u08_write_data[1] = 0x55u;
            
            /* Set parameters for WRITE command */
            sg_st_ees_request.np_u08_address = &sg_ar_u08_write_data[0u];
            sg_st_ees_request.u08_identifier = 1u;
            sg_st_ees_request.e_command      = R_EES_ENUM_CMD_WRITE;
            
            /* Execute WRITE command */
            R_EES_Execute(&sg_st_ees_request);
            
            /* Wait for the end of running command */
            while (R_EES_ENUM_RET_STS_BUSY == sg_st_ees_request.e_status)
            {
                /* It is possible to write the program for detecting timeout here as necessity requires */
                R_EES_Handler();
            }
            
            l_e_ees_ret_value = sg_st_ees_request.e_status;
            
            /* Check returned status of the command */
            if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
            {
                /* WRITE command was finished successfully */
                /* No operation */
            }
            else if (R_EES_ENUM_RET_ERR_POOL_FULL == l_e_ees_ret_value)
            {
                /******************************************************************************************************
                 * REFRESH
                 *****************************************************************************************************/
                /* Set parameters for REFRESH command */
                sg_st_ees_request.e_command = R_EES_ENUM_CMD_REFRESH;
                
                /* Execute REFRESH command */
                R_EES_Execute(&sg_st_ees_request);
                
                /* Wait for the end of running command */
                while (R_EES_ENUM_RET_STS_BUSY == sg_st_ees_request.e_status)
                {
                    /* It is possible to write the program for detecting timeout here as necessity requires */
                    R_EES_Handler();
                }
                
                l_e_ees_ret_value = sg_st_ees_request.e_status;
                
                /* Check returned status of the command */
                if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
                {
                    /* REFRESH command was finished successfully */
                    /* No operation */
                }
                else
                {
                    /* REFRESH error handling */
                    l_b_sam_error_flag = true;
                }
            }
            else
            {
                /* WRITE error handling */
                l_b_sam_error_flag = true;
            }
        } while ((R_EES_ENUM_CMD_REFRESH == sg_st_ees_request.e_command)
                 && (true != l_b_sam_error_flag));
    }
    else
    {
        /* No operation */
    }
    
    /******************************************************************************************************************
     * READ
     *****************************************************************************************************************/
    if (true != l_b_sam_error_flag)
    {
        /* Set parameters for READ command */
        sg_st_ees_request.np_u08_address = & sg_ar_u08_read_data[0];
        sg_st_ees_request.u08_identifier = 1u;
        sg_st_ees_request.e_command      = R_EES_ENUM_CMD_READ;
        
        /* Execute READ command */
        R_EES_Execute(&sg_st_ees_request);
        
        /* Wait for the end of running command */
        while (R_EES_ENUM_RET_STS_BUSY == sg_st_ees_request.e_status)
        {
            /* It is possible to write the program for detecting timeout here as necessity requires */
            R_EES_Handler();
        }
        
        l_e_ees_ret_value = sg_st_ees_request.e_status;
        
        if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
        {
            /* READ command was finished successfully */
            /* Compare read data with source data */
            for (l_u16_count = 0u; l_u16_count < sizeof(type_A); l_u16_count++)
            {
                if (sg_ar_u08_write_data[l_u16_count] !=  sg_ar_u08_read_data[l_u16_count])
                {
                    /* Mismatch data error handling */
                    l_b_sam_error_flag = true;
                    break;
                }
            }
        }
        else
        {
            /* READ error handling */
            l_b_sam_error_flag = true;
        }
    }
    else
    {
        /* No operation */
    }
    
    /******************************************************************************************************************
     * SHUTDOWN
     *****************************************************************************************************************/
    if (true != l_b_sam_error_flag)
    {
        /* Set parameters for SHUTDOWN command */
        sg_st_ees_request.e_command = R_EES_ENUM_CMD_SHUTDOWN;
        
        /* Execute SHUTDOWN command */
        R_EES_Execute(&sg_st_ees_request);
        
        /* When another command is running, call R_EES_Handler and retry SHUTDOWN command */
        while (R_EES_ENUM_RET_ERR_REJECTED == sg_st_ees_request.e_status)
        {
            /* It is possible to write the program for detecting timeout here as necessity requires */
            R_EES_Handler();
            R_EES_Execute(&sg_st_ees_request);
        }
        
        l_e_ees_ret_value = sg_st_ees_request.e_status;
        
        /* Check returned status of the command */
        if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
        {
            /* SHUTDOWN command was finished successfully */
            /* No operation */
        }
        else
        {
            /* SHUTDOWN error handling */
            l_b_sam_error_flag = true;
        }
    }
    
    /******************************************************************************************************************
     * Close EES
     *****************************************************************************************************************/
    if (true != l_b_sam_error_flag)
    {
        l_e_ees_ret_value = R_EES_Close();
        if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
        {
            /* EES was closed successfully */
            /* No operation */
        }
        else
        {
            /* Error handling, but never occurs */
            l_b_sam_error_flag = true;
        }
    }
    else
    {
        /* No operation */
    }
    
    return(l_e_ees_ret_value);
}

#define  SAMPLE_END_SECTION_SMP_EES
#include "sample_ees_memmap.h"
/**********************************************************************************************************************
 End of function Sample_EES_Control
 *********************************************************************************************************************/
