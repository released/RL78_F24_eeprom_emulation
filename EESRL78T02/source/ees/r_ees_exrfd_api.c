/**********************************************************************************************************************
    Program Name    : EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : r_ees_exrfd_api.c
    Program Version : V1.00
    Device(s)       : RL78/F24 microcontroller
    Description     : EES(EXRFD) control program
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
#include "r_ees_exrfd_api.h"
#include "r_ees_descriptor.h"

#include "r_rfd_common_api.h"
#include "r_rfd_common_control_api.h"
#include "r_rfd_data_flash_api.h"

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_VAR
#include "r_ees_memmap.h"
    /* Size of a physical block */
    __near uint16_t g_u16_ees_physical_block_size;
#define  R_EES_END_SECTION_EES_VAR
#include "r_ees_memmap.h"
    
#define  R_EES_START_SECTION_EES_VAR
#include "r_ees_memmap.h"
    /* Number of physical blocks per a virtual block */
    __near uint8_t g_u08_ees_physical_blocks_per_virtual_block;
#define  R_EES_END_SECTION_EES_VAR
#include "r_ees_memmap.h"

#define  R_EES_START_SECTION_EES_VAR
#include "r_ees_memmap.h"
    /* Number of virtual blocks */
    __near uint8_t g_u08_ees_pool_virtual_blocks;
#define  R_EES_END_SECTION_EES_VAR
#include "r_ees_memmap.h"

#define  R_EES_START_SECTION_EES_VAR
#include "r_ees_memmap.h"
    /* EXRFD control flags */
    __near st_ees_exrfd_control_flags_t g_st_ees_exrfd_control_flags;
#define  R_EES_END_SECTION_EES_VAR
#include "r_ees_memmap.h"

#define  R_EES_START_SECTION_EES_VAR
#include "r_ees_memmap.h"
    /* EXRFD execution state */
    __near e_ees_exrfd_exec_state_t g_e_ees_exrfd_exec_state;
#define  R_EES_END_SECTION_EES_VAR
#include "r_ees_memmap.h"

#define  R_EES_START_SECTION_EES_VAR
#include "r_ees_memmap.h"
    /* Physical block number to erase */
    __near uint8_t g_u08_ees_exrfd_cmd_start_block;
#define  R_EES_END_SECTION_EES_VAR
#include "r_ees_memmap.h"

#define  R_EES_START_SECTION_EES_VAR
#include "r_ees_memmap.h"
    /* Address to write, iverify or blankcheck */
    __near uint32_t g_u32_ees_exrfd_cmd_start_addr;
#define  R_EES_END_SECTION_EES_VAR
#include "r_ees_memmap.h"

#define  R_EES_START_SECTION_EES_VAR
#include "r_ees_memmap.h"
    /* Counter to erase, write, iverify or blankcheck */
    __near uint16_t g_u16_ees_exrfd_cmd_count;
#define  R_EES_END_SECTION_EES_VAR
#include "r_ees_memmap.h"

#define  R_EES_START_SECTION_EES_VAR
#include "r_ees_memmap.h"
    /* Pointer to write data */
    uint8_t __near * __near g_np_u08_ees_exrfd_cmd_write_data;
#define  R_EES_END_SECTION_EES_VAR
#include "r_ees_memmap.h"


/**********************************************************************************************************************
 Static functions
 *********************************************************************************************************************/
static R_EES_FAR_FUNC e_ees_exrfd_ret_status_t r_ees_exrfd_get_seq_error_status(void);
static R_EES_FAR_FUNC e_ees_exrfd_ret_status_t r_ees_exrfd_finish_state(void);
static R_EES_FAR_FUNC e_ees_exrfd_ret_status_t r_ees_exrfd_check_cmd_executable(void);
static R_EES_FAR_FUNC bool r_ees_exrfd_is_valid_byte_parameter(uint16_t i_u16_offset_addr, uint16_t i_u16_size);
static R_EES_FAR_FUNC void r_ees_exrfd_clear_cmd_workarea(void);


/**********************************************************************************************************************
 * Function name : R_EES_EXRFD_Init
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Initializes the EXRFD.
 *  
 *  @param[in]      i_u08_cpu_frequency : 
 *                    CPU operating frequency (MHz)
 *  @return         Execution result status
 *                  - R_EES_EXRFD_ENUM_RET_STS_OK              : Successful completion
 *                  - R_EES_EXRFD_ENUM_RET_ERR_CONFIGURATION   : Configuration error
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Init(uint8_t i_u08_cpu_frequency)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    e_rfd_ret_t              l_e_rfd_ret_status;
    
    /* Set local variables */
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_STS_OK;
    
    /* Clear all EXRFD control flags */
    g_st_ees_exrfd_control_flags.b_ees_exrfd_initialized = false;
    g_st_ees_exrfd_control_flags.b_ees_exrfd_opened      = false;
    g_st_ees_exrfd_control_flags.b_ees_exrfd_busy        = false;
    
    /* Initialize execution state */
    g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_READY;
    
    /* Clear physical block number to erase */
    g_u08_ees_exrfd_cmd_start_block = R_EES_VALUE_U08_INIT_VARIABLE;
    
    /* Clear address to write, iverify or blankcheck */
    g_u32_ees_exrfd_cmd_start_addr = R_EES_VALUE_U32_INIT_VARIABLE;
    
    /* Clear counter to erase, write, iverify or blankcheck */
    g_u16_ees_exrfd_cmd_count = R_EES_VALUE_U16_INIT_VARIABLE;
    
    /* Clear pointer to write data */    
    g_np_u08_ees_exrfd_cmd_write_data = ((uint8_t __near *)0x0000u);
    
    /* Check whether the number of physical blocks and virtual blocks is lower than the minimum limit or not */
    if ((0x0000u == g_st_ees_exrfd_descriptor.u16_ees_physical_block_size)
        || (R_EES_EXRFD_VALUE_U08_PHYSICAL_BLOCKS_PER_VIRTUAL_BLOCK_MIN 
            > g_st_ees_exrfd_descriptor.u08_ees_physical_blocks_per_virtual_block) 
        || (R_EES_EXRFD_VALUE_U08_POOL_VIRTUAL_BLOCKS_MIN 
            > g_st_ees_exrfd_descriptor.u08_ees_pool_virtual_blocks))
    {
        /* Configuration error */
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_CONFIGURATION;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Check whether size of EES pool exceeds data flash memory area or not */
    if ((uint32_t)((uint32_t)g_st_ees_exrfd_descriptor.u16_ees_physical_block_size 
                   * (uint32_t)g_st_ees_exrfd_descriptor.u08_ees_physical_blocks_per_virtual_block 
                   * (uint32_t)g_st_ees_exrfd_descriptor.u08_ees_pool_virtual_blocks) 
        > R_EES_EXRFD_VALUE_U32_DF_MEMORY_LIMIT)
    {
        /* Configuration error */
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_CONFIGURATION;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Set global variables */
    g_u16_ees_physical_block_size               = g_st_ees_exrfd_descriptor.u16_ees_physical_block_size;
    g_u08_ees_physical_blocks_per_virtual_block = 
        g_st_ees_exrfd_descriptor.u08_ees_physical_blocks_per_virtual_block;
    g_u08_ees_pool_virtual_blocks               = g_st_ees_exrfd_descriptor.u08_ees_pool_virtual_blocks;
    
    /* Initialize RFD */
    l_e_rfd_ret_status = R_RFD_Init(i_u08_cpu_frequency);
    if (R_RFD_ENUM_RET_STS_OK != l_e_rfd_ret_status)
    {
        /* Configuration error */
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_CONFIGURATION;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Set EXRFD control flags */
    g_st_ees_exrfd_control_flags.b_ees_exrfd_initialized = true;
    
    return (l_e_ees_exrfd_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_EXRFD_Init
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_EXRFD_Open
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Opens the EXRFD.
 *  
 *  @param[in]      -
 *  @return         Execution result status
 *                  - R_EES_EXRFD_ENUM_RET_STS_OK              : Successful completion
 *                  - R_EES_EXRFD_ENUM_RET_ERR_REJECTED        : Not ready to execute commands
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Open(void)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    
    /* Set local variables */
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_STS_OK;
    
    /* Check EXRFD control flags */
    if (true == g_st_ees_exrfd_control_flags.b_ees_exrfd_opened)
    {
        /* EXRFD is already opened */
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    if (true != g_st_ees_exrfd_control_flags.b_ees_exrfd_initialized)
    {
        /* Rejected error (not initialized) */
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_REJECTED;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Enable to access data flash */
    R_RFD_SetDataFlashAccessMode(R_RFD_ENUM_DF_ACCESS_ENABLE);
    
    /* Set EXRFD control flags */
    g_st_ees_exrfd_control_flags.b_ees_exrfd_opened = true;
    
    return (l_e_ees_exrfd_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_EXRFD_Open
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_EXRFD_Close
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Closes the EXRFD.
 *  
 *  @param[in]      -
 *  @return         Execution result status
 *                  - R_EES_EXRFD_ENUM_RET_STS_OK              : Successful completion
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Close(void)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    
    /* Set local variables */
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_STS_OK;
    
    /* Check EXRFD control flags */
    if (true != g_st_ees_exrfd_control_flags.b_ees_exrfd_opened)
    {
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Disable to access data flash */
    R_RFD_SetDataFlashAccessMode(R_RFD_ENUM_DF_ACCESS_DISABLE);
    
    /* Set EXRFD control flags */
    g_st_ees_exrfd_control_flags.b_ees_exrfd_busy   = false;
    g_st_ees_exrfd_control_flags.b_ees_exrfd_opened = false;
    
    return (l_e_ees_exrfd_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_EXRFD_Close
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_EXRFD_Erase
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  This command erases physical blocks included in the target virtual block in the data flash memory.
 *  
 *  @param[in]      i_u08_virtual_block_number : 
 *                    virtual block number to erase
 *  @return         Execution result status
 *                  - R_EES_EXRFD_ENUM_RET_STS_BUSY            : Command is still being processed
 *                  - R_EES_EXRFD_ENUM_RET_ERR_INITIALIZATION  : Failed to initialization
 *                  - R_EES_EXRFD_ENUM_RET_ERR_REJECTED        : Not ready to execute commands
 *                  - R_EES_EXRFD_ENUM_RET_ERR_PARAMETER       : Out of parameter range
 *                  - R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED : Failed to change flash memory mode
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Erase(uint8_t i_u08_virtual_block_number)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    e_rfd_ret_t              l_e_rfd_ret_status;
    
    /* Check EXRFD control flags and EXRFD execution state */
    l_e_ees_exrfd_ret_value = r_ees_exrfd_check_cmd_executable();
    if (R_EES_EXRFD_ENUM_RET_STS_OK != l_e_ees_exrfd_ret_value)
    {
        /* Error occurs */
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_STS_BUSY;
    }
    
    /* Check input parameter */
    if (i_u08_virtual_block_number >= g_u08_ees_pool_virtual_blocks)
    {
        /* Parameter error */
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_PARAMETER;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Change data flash memory programming mode */
    l_e_rfd_ret_status = R_RFD_SetFlashMemoryMode(R_RFD_ENUM_FLASH_MODE_DATA_PROGRAMMING);
    
    /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
    R_EES_EXRFD_INTERNAL_CODE_FLASH_MODE_ERASE;
    
    if (R_RFD_ENUM_RET_STS_OK != l_e_rfd_ret_status)
    {
        /* Failed to change flash memory mode */
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Clear command work area */
    r_ees_exrfd_clear_cmd_workarea();
    
    /* Set global variables to erase */
    g_u08_ees_exrfd_cmd_start_block = i_u08_virtual_block_number * g_u08_ees_physical_blocks_per_virtual_block;
    g_u16_ees_exrfd_cmd_count       = (uint16_t)g_u08_ees_physical_blocks_per_virtual_block;
    
    /* Set EXRFD execution state */
    g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_ERASE_START;
    
    /* Set EXRFD control flags */
    g_st_ees_exrfd_control_flags.b_ees_exrfd_busy = true;
    
    /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
    R_EES_EXRFD_INTERNAL_CODE_CMD_ERASE;
    
    return (l_e_ees_exrfd_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_EXRFD_Erase
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_EXRFD_Write
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  This command writes the data to the specified address in the data flash memory.
 *  
 *  @param[in]      i_u16_offset_addr : 
 *                    Offset address to write
 *  @param[in]      *inp_u08_write_data : 
 *                    Pointer to write data
 *  @param[in]      i_u16_size : 
 *                    Write data size
 *  @return         Execution result status
 *                  - R_EES_EXRFD_ENUM_RET_STS_BUSY            : Command is still being processed
 *                  - R_EES_EXRFD_ENUM_RET_ERR_INITIALIZATION  : Failed to initialization
 *                  - R_EES_EXRFD_ENUM_RET_ERR_REJECTED        : Not ready to execute commands
 *                  - R_EES_EXRFD_ENUM_RET_ERR_PARAMETER       : Out of parameter range
 *                  - R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED : Failed to change flash memory mode
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Write(uint16_t i_u16_offset_addr, 
                                                          uint8_t __near * inp_u08_write_data, 
                                                          uint16_t i_u16_size)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    e_rfd_ret_t              l_e_rfd_ret_status;
    bool                     l_b_valid_parameter_flag;
    
    /* Check EXRFD control flags and EXRFD execution state */
    l_e_ees_exrfd_ret_value = r_ees_exrfd_check_cmd_executable();
    if (R_EES_EXRFD_ENUM_RET_STS_OK != l_e_ees_exrfd_ret_value)
    {
        /* Error occurs */
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_STS_BUSY;
    }
    
    /* Check input parameter */
    l_b_valid_parameter_flag = r_ees_exrfd_is_valid_byte_parameter(i_u16_offset_addr, i_u16_size);
    if (true != l_b_valid_parameter_flag)
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_PARAMETER;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Change data flash memory programming mode */
    l_e_rfd_ret_status = R_RFD_SetFlashMemoryMode(R_RFD_ENUM_FLASH_MODE_DATA_PROGRAMMING);
    
    /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
    R_EES_EXRFD_INTERNAL_CODE_FLASH_MODE_WRITE;
    
    if (R_RFD_ENUM_RET_STS_OK != l_e_rfd_ret_status)
    {
        /* Failed to change flash memory mode */
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Clear command work area */
    r_ees_exrfd_clear_cmd_workarea();
    
    /* Set global variables to write */
    g_u32_ees_exrfd_cmd_start_addr    = R_EES_EXRFD_VALUE_U32_DF_BASE_ADDR + (uint32_t)i_u16_offset_addr;
    g_np_u08_ees_exrfd_cmd_write_data = inp_u08_write_data;
    g_u16_ees_exrfd_cmd_count         = i_u16_size;
    
    /* Set EXRFD execution state */
    g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_WRITE_START;
    
    /* Set EXRFD control flags */
    g_st_ees_exrfd_control_flags.b_ees_exrfd_busy = true;
    
    /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
    R_EES_EXRFD_INTERNAL_CODE_CMD_WRITE;
    
    return (l_e_ees_exrfd_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_EXRFD_Write
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_EXRFD_BlankCheck
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  This command performs blankcheck at the specified address in the data flash memory.
 *  
 *  @param[in]      i_u16_offset_addr : 
 *                    Offset address to perform blankcheck
 *  @param[in]      i_u16_size : 
 *                    Blankcheck data size
 *  @return         Execution result status
 *                  - R_EES_EXRFD_ENUM_RET_STS_BUSY            : Command is still being processed
 *                  - R_EES_EXRFD_ENUM_RET_ERR_INITIALIZATION  : Failed to initialization
 *                  - R_EES_EXRFD_ENUM_RET_ERR_REJECTED        : Not ready to execute commands
 *                  - R_EES_EXRFD_ENUM_RET_ERR_PARAMETER       : Out of parameter range
 *                  - R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED : Failed to change flash memory mode
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_BlankCheck(uint16_t i_u16_offset_addr, uint16_t i_u16_size)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    e_rfd_ret_t              l_e_rfd_ret_status;
    bool                     l_b_valid_parameter_flag;
    
    /* Check EXRFD control flags and EXRFD execution state */
    l_e_ees_exrfd_ret_value = r_ees_exrfd_check_cmd_executable();
    if (R_EES_EXRFD_ENUM_RET_STS_OK != l_e_ees_exrfd_ret_value)
    {
        /* Error occurs */
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_STS_BUSY;
    }
    
    /* Check input parameter */
    l_b_valid_parameter_flag = r_ees_exrfd_is_valid_byte_parameter(i_u16_offset_addr, i_u16_size);
    if (true != l_b_valid_parameter_flag)
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_PARAMETER;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Change data flash memory programming mode */
    l_e_rfd_ret_status = R_RFD_SetFlashMemoryMode(R_RFD_ENUM_FLASH_MODE_DATA_PROGRAMMING);
    
    /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
    R_EES_EXRFD_INTERNAL_CODE_FLASH_MODE_BLANKCHECK;
    
    if (R_RFD_ENUM_RET_STS_OK != l_e_rfd_ret_status)
    {
        /* Failed to change flash memory mode */
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Clear command work area */
    r_ees_exrfd_clear_cmd_workarea();
    
    /* Set global variables to perform blankcheck */
    g_u32_ees_exrfd_cmd_start_addr = R_EES_EXRFD_VALUE_U32_DF_BASE_ADDR + (uint32_t)i_u16_offset_addr;
    g_u16_ees_exrfd_cmd_count      = i_u16_size;
    
    /* Set EXRFD execution state */
    g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_BLANKCHK_START;
    
    /* Set EXRFD control flags */
    g_st_ees_exrfd_control_flags.b_ees_exrfd_busy = true;
    
    /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
    R_EES_EXRFD_INTERNAL_CODE_CMD_BLANKCHECK;
    
    return (l_e_ees_exrfd_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_EXRFD_BlankCheck
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_EXRFD_IVerify
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  This command performs iverify at the specified address in the data flash memory.
 *  
 *  @param[in]      i_u16_offset_addr : 
 *                    Offset address to perform iverify
 *  @param[in]      i_u16_size : 
 *                    iverify data size
 *  @return         Execution result status
 *                  - R_EES_EXRFD_ENUM_RET_STS_BUSY            : Command is still being processed
 *                  - R_EES_EXRFD_ENUM_RET_ERR_INITIALIZATION  : Failed to initialization
 *                  - R_EES_EXRFD_ENUM_RET_ERR_REJECTED        : Not ready to execute commands
 *                  - R_EES_EXRFD_ENUM_RET_ERR_PARAMETER       : Out of parameter range
 *                  - R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED : Failed to change flash memory mode
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_IVerify(uint16_t i_u16_offset_addr, uint16_t i_u16_size)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    e_rfd_ret_t              l_e_rfd_ret_status;
    bool                     l_b_valid_parameter_flag;
    
    /* Check EXRFD control flags and EXRFD execution state */
    l_e_ees_exrfd_ret_value = r_ees_exrfd_check_cmd_executable();
    if (R_EES_EXRFD_ENUM_RET_STS_OK != l_e_ees_exrfd_ret_value)
    {
        /* Error occurs */
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_STS_BUSY;
    }
    
    /* Check input parameter */
    l_b_valid_parameter_flag = r_ees_exrfd_is_valid_byte_parameter(i_u16_offset_addr, i_u16_size);
    if (true != l_b_valid_parameter_flag)
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_PARAMETER;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Change data flash memory programming mode */
    l_e_rfd_ret_status = R_RFD_SetFlashMemoryMode(R_RFD_ENUM_FLASH_MODE_DATA_PROGRAMMING);
    
    /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
    R_EES_EXRFD_INTERNAL_CODE_FLASH_MODE_IVERIFY;
    
    if (R_RFD_ENUM_RET_STS_OK != l_e_rfd_ret_status)
    {
        /* Failed to change flash memory mode */
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Clear command work area */
    r_ees_exrfd_clear_cmd_workarea();
    
    /* Set global variables to perform iverify */
    g_u32_ees_exrfd_cmd_start_addr = R_EES_EXRFD_VALUE_U32_DF_BASE_ADDR + (uint32_t)i_u16_offset_addr;
    g_u16_ees_exrfd_cmd_count      = i_u16_size;
    
    /* Set EXRFD execution state */
    g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_IVERIFY_START;
    
    /* Set EXRFD control flags */
    g_st_ees_exrfd_control_flags.b_ees_exrfd_busy = true;
    
    /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
    R_EES_EXRFD_INTERNAL_CODE_CMD_IVERIFY;
    
    return (l_e_ees_exrfd_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_EXRFD_IVerify
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_EXRFD_Read
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  This command reads the specified address in the data flash memory.
 *  
 *  @param[in]      i_u16_offset_addr : 
 *                    Offset address to read
 *  @param[out]     *onp_u08_read_data : 
 *                    Pointer to read data
 *  @param[in]      i_u16_size : 
 *                    Read data size
 *  @return         Execution result status
 *                  - R_EES_EXRFD_ENUM_RET_STS_OK              : Successful completion
 *                  - R_EES_EXRFD_ENUM_RET_ERR_INITIALIZATION  : Failed to initialization
 *                  - R_EES_EXRFD_ENUM_RET_ERR_REJECTED        : Not ready to execute commands
 *                  - R_EES_EXRFD_ENUM_RET_ERR_PARAMETER       : Out of parameter range
 *                  - R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED : Failed to change flash memory mode
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Read(uint16_t i_u16_offset_addr, 
                                                         uint8_t __near * onp_u08_read_data, 
                                                         uint16_t i_u16_size)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    e_rfd_ret_t              l_e_rfd_ret_status;
    uint16_t                 l_u16_count;
    uint32_t                 l_u32_physical_addr;
    uint8_t __near *         l_np_u08_read_data_work;
    bool                     l_b_valid_parameter_flag;
    
    /* Check EXRFD control flags and EXRFD execution state */
    l_e_ees_exrfd_ret_value = r_ees_exrfd_check_cmd_executable();
    if (R_EES_EXRFD_ENUM_RET_STS_OK != l_e_ees_exrfd_ret_value)
    {
        /* Error occurs */
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Check input parameter */
    l_b_valid_parameter_flag = r_ees_exrfd_is_valid_byte_parameter(i_u16_offset_addr, i_u16_size);
    if (true != l_b_valid_parameter_flag)
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_PARAMETER;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    /* Change non programming mode */
    l_e_rfd_ret_status = R_RFD_SetFlashMemoryMode(R_RFD_ENUM_FLASH_MODE_DATA_TO_NONPROGRAMMABLE);
    
    /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
    R_EES_EXRFD_INTERNAL_CODE_FLASH_MODE_READ;
    
    if (R_RFD_ENUM_RET_STS_OK != l_e_rfd_ret_status)
    {
        /* Failed to change flash memory mode */
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        /* No operation */
    }
    
    l_u32_physical_addr = R_EES_EXRFD_VALUE_U32_DF_BASE_ADDR + (uint32_t)i_u16_offset_addr;
    l_np_u08_read_data_work = onp_u08_read_data;
    for (l_u16_count = 0u; l_u16_count < i_u16_size; l_u16_count++)
    {
        *l_np_u08_read_data_work = (*(uint8_t __far *)(l_u32_physical_addr + (uint32_t)l_u16_count));
        l_np_u08_read_data_work++;
    }
    
    /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
    R_EES_EXRFD_INTERNAL_CODE_CMD_READ;
    
    return (l_e_ees_exrfd_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_EXRFD_Read
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : R_EES_EXRFD_Handler
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  This function is used to proceed the execution of the running EXRFD command.
 *  
 *  @param[in]      -
 *  @return         Execution result status
 *                  - R_EES_EXRFD_ENUM_RET_STS_OK              : Successful completion or idle
 *                  - R_EES_EXRFD_ENUM_RET_STS_BUSY            : Command is still being processed
 *                  - R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED : Failed to change flash memory mode
 *                  - R_EES_EXRFD_ENUM_RET_ERR_CFDF_SEQUENCER  : CFDF sequencer error
 *                  - R_EES_EXRFD_ENUM_RET_ERR_ERASE           : Erase action error
 *                  - R_EES_EXRFD_ENUM_RET_ERR_WRITE           : Write action error
 *                  - R_EES_EXRFD_ENUM_RET_ERR_BLANKCHK        : Blankcheck action error
 *                  - R_EES_EXRFD_ENUM_RET_ERR_IVERIFY         : IVerify action error
 *                  - R_EES_EXRFD_ENUM_RET_ERR_INTERNAL        : Internal error
 */
/*********************************************************************************************************************/
R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Handler(void)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_rfd_seq_status;
    e_rfd_ret_t              l_e_rfd_ret_status;
    
    
    if (true != g_st_ees_exrfd_control_flags.b_ees_exrfd_busy)
    {
        /* No running command (idle) */
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_STS_OK;
        return (l_e_ees_exrfd_ret_value);
    }
    else
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_STS_BUSY;
    }
    
    switch (g_e_ees_exrfd_exec_state)
    {
        case R_EES_EXRFD_ENUM_READY:
            l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_STS_OK;
            break;
        case R_EES_EXRFD_ENUM_ERASE_START:
            R_RFD_EraseDataFlashReq(g_u08_ees_exrfd_cmd_start_block);
            g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_ERASE_BUSY1;
            break;
        case R_EES_EXRFD_ENUM_WRITE_START:
            R_RFD_WriteDataFlashReq(g_u32_ees_exrfd_cmd_start_addr, g_np_u08_ees_exrfd_cmd_write_data);
            g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_WRITE_BUSY1;
            break;
        case R_EES_EXRFD_ENUM_BLANKCHK_START:
            R_RFD_BlankCheckDataFlashReq(g_u32_ees_exrfd_cmd_start_addr, g_u16_ees_exrfd_cmd_count);
            g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_BLANKCHK_BUSY1;
            break;
        case R_EES_EXRFD_ENUM_IVERIFY_START:
            R_RFD_IVerifyDataFlashReq(g_u32_ees_exrfd_cmd_start_addr, g_u16_ees_exrfd_cmd_count);
            g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_IVERIFY_BUSY1;
            break;
        case R_EES_EXRFD_ENUM_ERASE_BUSY1:
            l_e_rfd_ret_status = R_RFD_CheckCFDFSeqEndStep1();
            if (R_RFD_ENUM_RET_STS_OK == l_e_rfd_ret_status)
            {
                g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_ERASE_BUSY2;
            }
            else
            {
                /* R_RFD_ENUM_RET_STS_BUSY */
            }
            break;
        case R_EES_EXRFD_ENUM_WRITE_BUSY1:
            l_e_rfd_ret_status = R_RFD_CheckCFDFSeqEndStep1();
            if (R_RFD_ENUM_RET_STS_OK == l_e_rfd_ret_status)
            {
                g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_WRITE_BUSY2;
            }
            else
            {
                /* R_RFD_ENUM_RET_STS_BUSY */
            }
            break;
        case R_EES_EXRFD_ENUM_BLANKCHK_BUSY1:
            l_e_rfd_ret_status = R_RFD_CheckCFDFSeqEndStep1();
            if (R_RFD_ENUM_RET_STS_OK == l_e_rfd_ret_status)
            {
                g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_BLANKCHK_BUSY2;
            }
            else
            {
                /* R_RFD_ENUM_RET_STS_BUSY */
            }
            break;
        case R_EES_EXRFD_ENUM_IVERIFY_BUSY1:
            l_e_rfd_ret_status = R_RFD_CheckCFDFSeqEndStep1();
            if (R_RFD_ENUM_RET_STS_OK == l_e_rfd_ret_status)
            {
                g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_IVERIFY_BUSY2;
            }
            else
            {
                /* R_RFD_ENUM_RET_STS_BUSY */
            }
            break;
        case R_EES_EXRFD_ENUM_ERASE_BUSY2:
        case R_EES_EXRFD_ENUM_WRITE_BUSY2:
        case R_EES_EXRFD_ENUM_BLANKCHK_BUSY2:
        case R_EES_EXRFD_ENUM_IVERIFY_BUSY2:
            l_e_rfd_ret_status = R_RFD_CheckCFDFSeqEndStep2();
            
            /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
            R_EES_EXRFD_INTERNAL_CODE_FSQ_STEP2_STATUS;
            
            if (R_RFD_ENUM_RET_STS_OK == l_e_rfd_ret_status)
            {
                l_e_ees_exrfd_ret_rfd_seq_status = r_ees_exrfd_get_seq_error_status();
                if (R_EES_EXRFD_ENUM_RET_STS_OK == l_e_ees_exrfd_ret_rfd_seq_status)
                {
                    /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
                    R_EES_EXRFD_INTERNAL_CODE_EXEC_STATE;
                    
                    if (R_EES_EXRFD_ENUM_ERASE_BUSY2 == g_e_ees_exrfd_exec_state)
                    {
                        /* Check remain erase block */
                        if (1u < g_u16_ees_exrfd_cmd_count)
                        {
                            g_u16_ees_exrfd_cmd_count--;
                            g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_ERASE_START;
                            g_u08_ees_exrfd_cmd_start_block++;
                        }
                        else
                        {
                            g_u16_ees_exrfd_cmd_count = R_EES_VALUE_U16_INIT_VARIABLE;
                            l_e_ees_exrfd_ret_value = r_ees_exrfd_finish_state();
                        }
                    }
                    else if (R_EES_EXRFD_ENUM_WRITE_BUSY2 == g_e_ees_exrfd_exec_state)
                    {
                        /* Check remain write size */
                        if (1u < g_u16_ees_exrfd_cmd_count)
                        {
                            g_u16_ees_exrfd_cmd_count--;
                            g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_WRITE_START;
                            g_u32_ees_exrfd_cmd_start_addr++;
                            g_np_u08_ees_exrfd_cmd_write_data++;
                        }
                        else
                        {
                            g_u16_ees_exrfd_cmd_count = R_EES_VALUE_U16_INIT_VARIABLE;
                            l_e_ees_exrfd_ret_value = r_ees_exrfd_finish_state();
                        }
                    }
                    else if ((R_EES_EXRFD_ENUM_BLANKCHK_BUSY2 == g_e_ees_exrfd_exec_state) || 
                             (R_EES_EXRFD_ENUM_IVERIFY_BUSY2 == g_e_ees_exrfd_exec_state))
                    {
                        g_u16_ees_exrfd_cmd_count = R_EES_VALUE_U16_INIT_VARIABLE;
                        l_e_ees_exrfd_ret_value = r_ees_exrfd_finish_state();
                    }
                    else
                    {
                        /* Internal error (never occurs) */
                        l_e_ees_exrfd_ret_value = r_ees_exrfd_finish_state();
                        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_INTERNAL;
                        
                        /* Clear all EXRFD control flags for safety (it is impossible to continue EXRFD command) */
                        g_st_ees_exrfd_control_flags.b_ees_exrfd_initialized = false;
                        g_st_ees_exrfd_control_flags.b_ees_exrfd_opened      = false;
                        g_st_ees_exrfd_control_flags.b_ees_exrfd_busy        = false;
                    }
                }
                else
                {
                    l_e_ees_exrfd_ret_value = r_ees_exrfd_finish_state();
                    if (R_EES_EXRFD_ENUM_RET_STS_OK == l_e_ees_exrfd_ret_value)
                    {
                        /* R_EES_EXRFD_ENUM_RET_ERR_CFDF_SEQUENCER  */
                        /* R_EES_EXRFD_ENUM_RET_ERR_ERASE           */
                        /* R_EES_EXRFD_ENUM_RET_ERR_WRITE           */
                        /* R_EES_EXRFD_ENUM_RET_ERR_BLANKCHK        */
                        /* R_EES_EXRFD_ENUM_RET_ERR_IVERIFY         */
                        l_e_ees_exrfd_ret_value = l_e_ees_exrfd_ret_rfd_seq_status;
                    }
                    else
                    {
                        /* R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED */
                    }
                }
            }
            else
            {
                /* R_RFD_ENUM_RET_STS_BUSY */
            }
            break;
        default:
            /* Unknown EXRFD execution state error */
            l_e_ees_exrfd_ret_value = r_ees_exrfd_finish_state();
            l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_INTERNAL;
            
            /* Clear all EXRFD control flags for safety (it is impossible to continue EXRFD command) */
            g_st_ees_exrfd_control_flags.b_ees_exrfd_initialized = false;
            g_st_ees_exrfd_control_flags.b_ees_exrfd_opened      = false;
            g_st_ees_exrfd_control_flags.b_ees_exrfd_busy        = false;
            break;
    }
    
    return (l_e_ees_exrfd_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function R_EES_EXRFD_Handler
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Internal functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function name : r_ees_exrfd_get_seq_error_status
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Gets the state of the data flash memory sequencer and result.
 *  
 *  @param[in]      -
 *  @return         Execution result status
 *                  - R_EES_EXRFD_ENUM_RET_STS_OK              : Successful completion
 *                  - R_EES_EXRFD_ENUM_RET_ERR_CFDF_SEQUENCER  : CF/DF sequencer error
 *                  - R_EES_EXRFD_ENUM_RET_ERR_ERASE           : Erase action error
 *                  - R_EES_EXRFD_ENUM_RET_ERR_WRITE           : Write action error
 *                  - R_EES_EXRFD_ENUM_RET_ERR_BLANKCHK        : Blankcheck action error
 *                  - R_EES_EXRFD_ENUM_RET_ERR_IVERIFY         : IVerify action error
 */
/*********************************************************************************************************************/
static R_EES_FAR_FUNC e_ees_exrfd_ret_status_t r_ees_exrfd_get_seq_error_status(void)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    uint8_t                  l_u08_rfd_cmd_seq_error_status; 
    
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_STS_OK;
    l_u08_rfd_cmd_seq_error_status = R_EES_VALUE_U08_INIT_VARIABLE;
    
    R_RFD_GetSeqErrorStatus(&l_u08_rfd_cmd_seq_error_status);
    
    /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
    R_EES_EXRFD_INTERNAL_CODE_FSQ_STATUS;
    
    /* check each flash error */
    if (R_EES_VALUE_U08_MASK0_8BIT != (l_u08_rfd_cmd_seq_error_status 
                                            & R_EES_EXRFD_VALUE_U08_MASK1_FSQ_STATUS_ERR_CFDF_SEQUENCER))
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_CFDF_SEQUENCER;
    }
    else if (R_EES_VALUE_U08_MASK0_8BIT != (l_u08_rfd_cmd_seq_error_status 
                                            & R_EES_EXRFD_VALUE_U08_MASK1_FSQ_STATUS_ERR_ERASE))
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_ERASE;
    }
    else if (R_EES_VALUE_U08_MASK0_8BIT != (l_u08_rfd_cmd_seq_error_status 
                                            & R_EES_EXRFD_VALUE_U08_MASK1_FSQ_STATUS_ERR_WRITE))
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_WRITE;
    }
    else if (R_EES_VALUE_U08_MASK0_8BIT != (l_u08_rfd_cmd_seq_error_status 
                                            & R_EES_EXRFD_VALUE_U08_MASK1_FSQ_STATUS_ERR_BLANKCHECK))
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_BLANKCHECK;
    }
    else if (R_EES_VALUE_U08_MASK0_8BIT != (l_u08_rfd_cmd_seq_error_status 
                                            & R_EES_EXRFD_VALUE_U08_MASK1_FSQ_STATUS_ERR_IVERIFY))
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_IVERIFY;
    }
    else
    {
        /* no error */
    }
    
    return (l_e_ees_exrfd_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_exrfd_get_seq_error_status
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_exrfd_finish_state
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Finishes the state for the EXRFD command.
 *  
 *  @param[in]      -
 *  @return         Execution result status
 *                  - R_EES_EXRFD_ENUM_RET_STS_OK              : Successful completion
 *                  - R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED : Failed to change flash memory mode
 */
/*********************************************************************************************************************/
static R_EES_FAR_FUNC e_ees_exrfd_ret_status_t r_ees_exrfd_finish_state(void)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    e_rfd_ret_t              l_e_rfd_ret_status;
    
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_STS_OK;
    
    R_RFD_ClearSeqRegister();
    
    /* Set EXRFD execution state */
    g_e_ees_exrfd_exec_state = R_EES_EXRFD_ENUM_READY;
    
    /* Change non programming mode */
    l_e_rfd_ret_status = R_RFD_SetFlashMemoryMode(R_RFD_ENUM_FLASH_MODE_DATA_TO_NONPROGRAMMABLE);
    
    /* Please ignore R_EES_EXRFD_INTERNAL_CODE_*, because it is an internal code for testing */
    R_EES_EXRFD_INTERNAL_CODE_FLASH_MODE_FINISH;
    
    if (R_RFD_ENUM_RET_STS_OK != l_e_rfd_ret_status)
    {
        /* Failed to change flash memory mode */
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED;
    }
    else
    {
        /* No operation */
    }
    
    /* Set EXRFD control flags */
    g_st_ees_exrfd_control_flags.b_ees_exrfd_busy = false;
    
    return (l_e_ees_exrfd_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_exrfd_finish_state
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_exrfd_check_cmd_executable
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Checks the EES EXRFD command status.
 *  
 *  @param[in]      -
 *  @return         Execution result status
 *                  - R_EES_EXRFD_ENUM_RET_STS_OK              : Successful completion
 *                  - R_EES_EXRFD_ENUM_RET_ERR_INITIALIZATION  : Initialization missing
 *                  - R_EES_EXRFD_ENUM_RET_ERR_REJECTED        : Not ready to execute commands
 */
/*********************************************************************************************************************/
static R_EES_FAR_FUNC e_ees_exrfd_ret_status_t r_ees_exrfd_check_cmd_executable(void)
{
    /* Local variable definitions */
    e_ees_exrfd_ret_status_t l_e_ees_exrfd_ret_value;
    
    l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_STS_OK;
    
    /* Check EXRFD control flags */
    if ((true != g_st_ees_exrfd_control_flags.b_ees_exrfd_initialized) 
        || (true != g_st_ees_exrfd_control_flags.b_ees_exrfd_opened))
    {
        l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_INITIALIZATION;
    }
    else
    {
        /* Check EXRFD execution state */
        if ((R_EES_EXRFD_ENUM_READY != g_e_ees_exrfd_exec_state) 
            || (true == g_st_ees_exrfd_control_flags.b_ees_exrfd_busy))
        {
            l_e_ees_exrfd_ret_value = R_EES_EXRFD_ENUM_RET_ERR_REJECTED;
        }
        else
        {
            /* No operation */
        }
    }
    
    return (l_e_ees_exrfd_ret_value);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_exrfd_check_cmd_executable
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_exrfd_is_valid_byte_parameter
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Checks whether the parameters for EXRFD command oriented byte is valid or not.
 *  
 *  @param[in]      i_u16_offset_addr : 
 *                    Offset address
 *  @param[in]      i_u16_size : 
 *                    data size
 *  @return         
 *                  - true  : Valid parameters
 *                  - false : Invalid parameters
 */
/*********************************************************************************************************************/
static R_EES_FAR_FUNC bool r_ees_exrfd_is_valid_byte_parameter(uint16_t i_u16_offset_addr, uint16_t i_u16_size)
{
    /* Local variable definitions */
    
    /* Check input parameter*/
    /* Check whether specified size is 0 or not */
    if (0u == i_u16_size)
    {
        /* Invalid parameter */
        return (false);
    }
    else
    {
        /* No operation */
    }
    
    /* Check whether specified address exceeds data flash memory area or not */
    if (R_EES_EXRFD_VALUE_U32_DEVICE_MEMORY_LIMIT < (R_EES_EXRFD_VALUE_U32_DF_BASE_ADDR + (uint32_t)i_u16_offset_addr))
    {
        /* Invalid parameter */
        return (false);
    }
    else
    {
        /* No operation */
    }
    
    /* Check whether last address exceeds EES pool or not */
    if ((i_u16_offset_addr + i_u16_size) > (g_u16_ees_physical_block_size 
                                            * (uint16_t)g_u08_ees_physical_blocks_per_virtual_block 
                                            * (uint16_t)g_u08_ees_pool_virtual_blocks))
    {
        /* Invalid parameter */
        return (false);
    }
    else
    {
        /* No operation */
    }
    
    return (true);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_exrfd_is_valid_byte_parameter
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function name : r_ees_exrfd_clear_cmd_workarea
 *********************************************************************************************************************/
#define  R_EES_START_SECTION_EES_CODE
#include "r_ees_memmap.h"
/*********************************************************************************************************************/
/**
 *  Initializes the command work area.
 *  
 *  @param[in]      -
 *  @return         ---
 */
/*********************************************************************************************************************/
static R_EES_FAR_FUNC void r_ees_exrfd_clear_cmd_workarea(void)
{
    g_u08_ees_exrfd_cmd_start_block   = R_EES_VALUE_U08_INIT_VARIABLE;
    g_u32_ees_exrfd_cmd_start_addr    = R_EES_VALUE_U32_INIT_VARIABLE;
    g_u16_ees_exrfd_cmd_count         = R_EES_VALUE_U16_INIT_VARIABLE;
    g_np_u08_ees_exrfd_cmd_write_data = ((uint8_t __near *)0x0000u);
}

#define  R_EES_END_SECTION_EES_CODE
#include "r_ees_memmap.h"
/**********************************************************************************************************************
 End of function r_ees_exrfd_clear_cmd_workarea
 *********************************************************************************************************************/

/**********************************************************************************************************************
 End of internal functions
 *********************************************************************************************************************/
