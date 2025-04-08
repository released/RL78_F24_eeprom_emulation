/**********************************************************************************************************************
    Program Name    : EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : r_ees_types.h
    Program Version : V1.00
    Device(s)       : RL78/F24 microcontroller
    Description     : Type definitions header file
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

#ifndef R_EES_TYPES_H
#define R_EES_TYPES_H

#include "r_typedefs.h"
#include "r_ees_compiler.h"
#include "r_ees_defines.h"

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**
 * @enum e_ees_command_t
 * EES command definition
 */
typedef enum e_ees_command
{                                                  /* ----------------------------------------------------- */
    R_EES_ENUM_CMD_UNDEFINED    = (0x00u | 0x00u), /* 0x00, Undefined command                               */
    R_EES_ENUM_CMD_STARTUP      = (0x00u | 0x01u), /* 0x01, Plausibility checks of the EES data             */
    R_EES_ENUM_CMD_WRITE        = (0x00u | 0x02u), /* 0x02, Creates a new instance of EES variable          */
    R_EES_ENUM_CMD_READ         = (0x00u | 0x03u), /* 0x03, Reads last instance of the EES variable         */
    R_EES_ENUM_CMD_REFRESH      = (0x00u | 0x04u), /* 0x04, Copy all last instances into a new active block */
                                                   /* 0x05, Reserved (unused)                               */
    R_EES_ENUM_CMD_FORMAT       = (0x00u | 0x06u), /* 0x06, Format the EES pool, all instances are lost     */
    R_EES_ENUM_CMD_SHUTDOWN     = (0x00u | 0x07u)  /* 0x07, Deactivates the EES in secure state             */
} e_ees_command_t;                                 /* ----------------------------------------------------- */

/**
 * @enum e_ees_ret_status_t
 * EES operation status definition
 */
typedef enum e_ees_ret_status
{                                                           /* ---------------------------------------------------- */
    R_EES_ENUM_RET_STS_OK                = (0x00u | 0x00u), /* 0x00, No error occurred                              */
    R_EES_ENUM_RET_STS_BUSY              = (0x00u | 0x01u), /* 0x01, Request is under processing                    */
    R_EES_ENUM_RET_ERR_CONFIGURATION     = (0x80u | 0x02u), /* 0x82, Invalid EES configuration                      */
    R_EES_ENUM_RET_ERR_INITIALIZATION    = (0x80u | 0x03u), /* 0x83, R_EES_Init, R_EES_Open missing                 */
    R_EES_ENUM_RET_ERR_ACCESS_LOCKED     = (0x80u | 0x04u), /* 0x84, STARTUP missing or fatal operation error       */
    R_EES_ENUM_RET_ERR_PARAMETER         = (0x80u | 0x05u), /* 0x85, Wrong parameter (wrong command or identifier)  */
    R_EES_ENUM_RET_ERR_VERIFY            = (0x80u | 0x06u), /* 0x86, Verify error on any byte in the active block   */
    R_EES_ENUM_RET_ERR_REJECTED          = (0x80u | 0x07u), /* 0x87, Another request already under processing       */
    R_EES_ENUM_RET_ERR_NO_INSTANCE       = (0x80u | 0x08u), /* 0x88, No instance found (variable never written)     */
    R_EES_ENUM_RET_ERR_POOL_FULL         = (0x80u | 0x09u), /* 0x89, No space for writing data                      */
    R_EES_ENUM_RET_ERR_POOL_INCONSISTENT = (0x80u | 0x0Au), /* 0x8A, No active block found within EES pool          */
    R_EES_ENUM_RET_ERR_POOL_EXHAUSTED    = (0x80u | 0x0Bu), /* 0x8B, EES pool is too small for correct operation    */
    R_EES_ENUM_RET_ERR_INTERNAL          = (0x80u | 0x0Cu), /* 0x8C, Internal error                                 */
    R_EES_ENUM_RET_ERR_FLASH_SEQ         = (0x80u | 0x0Du)  /* 0x8D, Flash sequencer error                          */
} e_ees_ret_status_t;                                       /* ---------------------------------------------------- */

/**
 * @struct st_ees_request_t
 * EES requester definition
 */
typedef struct st_ees_request
{                                       /* ---------------------------------- */
    uint8_t __near *   np_u08_address;  /* Source/destination RAM-address     */
    uint8_t            u08_identifier;  /* Variable identifier                */
    e_ees_command_t    e_command;       /* Command has to be processed        */
    e_ees_ret_status_t e_status;        /* Error code after command execution */
} st_ees_request_t;                     /* ---------------------------------- */

/**
 * @struct st_ees_exrfd_descriptor
 * EES(EXRFD) descriptor definition
 */
typedef struct st_ees_exrfd_descriptor
{                                                       /* --------------------------------------------- */
    uint16_t u16_ees_physical_block_size;               /* Size of physical block                        */
    uint8_t  u08_ees_physical_blocks_per_virtual_block; /* Number of physical blocks per a virtual block */
    uint8_t  u08_ees_pool_virtual_blocks;               /* Total number of virtual blocks for EES pool   */
} st_ees_exrfd_descriptor_t;                            /* --------------------------------------------- */


/**********************************************************************************************************************
 Internal typedef definitions
 *********************************************************************************************************************/

/**
 * @enum e_ees_command_t
 * EES block status
 */
typedef enum e_ees_block_status
{                                                   /* -------------------- */
    R_EES_ENUM_BLOCK_STATUS_UNDEFINED   = 0x00u,    /* 0x00, Undefined      */
    R_EES_ENUM_BLOCK_STATUS_ACTIVE      = 0x01u,    /* 0x01, Active block   */
    R_EES_ENUM_BLOCK_STATUS_INVALID     = 0x02u,    /* 0x02, Invalid block  */
    R_EES_ENUM_BLOCK_STATUS_EXCLUDED    = 0x03u     /* 0x03, Excluded block */
} e_ees_block_status_t;                             /* -------------------- */

/**
 * @enum e_ees_exrfd_ret_status_t
 * EES(EXRFD) operation status definition
 */
typedef enum e_ees_exrfd_ret_status
{                                                       /* ---------------------------------------------------- */
    R_EES_EXRFD_ENUM_RET_STS_OK              = 0x00u,   /* 0x00, No error occurred                              */
    R_EES_EXRFD_ENUM_RET_STS_BUSY            = 0x01u,   /* 0x01, EXRFD Command is under processing              */
    R_EES_EXRFD_ENUM_RET_ERR_CONFIGURATION   = 0x10u,   /* 0x10, Invalid EES pool configuration                 */
    R_EES_EXRFD_ENUM_RET_ERR_INITIALIZATION  = 0x11u,   /* 0x11, R_EES_EXRFD_Init, R_EES_EXRFD_Open missing     */
    R_EES_EXRFD_ENUM_RET_ERR_REJECTED        = 0x12u,   /* 0x12, Another EXRFD command already under processing */
    R_EES_EXRFD_ENUM_RET_ERR_PARAMETER       = 0x13u,   /* 0x13, Wrong parameter                                */
    R_EES_EXRFD_ENUM_RET_ERR_INTERNAL        = 0x14u,   /* 0x14, Internal error                                 */
    R_EES_EXRFD_ENUM_RET_ERR_MODE_MISMATCHED = 0x20u,   /* 0x20, Flash memory mode error                        */
    R_EES_EXRFD_ENUM_RET_ERR_CFDF_SEQUENCER  = 0x21u,   /* 0x21, Flash memory sequencer error                   */
    R_EES_EXRFD_ENUM_RET_ERR_ERASE           = 0x22u,   /* 0x22, Erase error                                    */
    R_EES_EXRFD_ENUM_RET_ERR_BLANKCHECK      = 0x23u,   /* 0x23, Blankcheck error                               */
    R_EES_EXRFD_ENUM_RET_ERR_WRITE           = 0x24u,   /* 0x24, Write error                                    */
    R_EES_EXRFD_ENUM_RET_ERR_IVERIFY         = 0x25u    /* 0x25, IVerify error                                  */
} e_ees_exrfd_ret_status_t;                             /* ---------------------------------------------------- */

/**
 * @enum e_ees_exrfd_exec_state_t
 * EES(EXRFD) exec state definition
 */
typedef enum e_ees_exrfd_exec_state
{                                               /* -------------------- */
    R_EES_EXRFD_ENUM_READY          = 0x00u,    /* Ready                */
    R_EES_EXRFD_ENUM_ERASE_START    = 0x10u,    /* Start erasing        */
    R_EES_EXRFD_ENUM_ERASE_BUSY1    = 0x11u,    /* Continue erasing     */
    R_EES_EXRFD_ENUM_ERASE_BUSY2    = 0x12u,    /* End erasing          */
    R_EES_EXRFD_ENUM_BLANKCHK_START = 0x20u,    /* Start blankcheck     */
    R_EES_EXRFD_ENUM_BLANKCHK_BUSY1 = 0x21u,    /* Continue blankcheck  */
    R_EES_EXRFD_ENUM_BLANKCHK_BUSY2 = 0x22u,    /* End blankcheck       */
    R_EES_EXRFD_ENUM_WRITE_START    = 0x30u,    /* Start writing        */
    R_EES_EXRFD_ENUM_WRITE_BUSY1    = 0x31u,    /* Continue writing     */
    R_EES_EXRFD_ENUM_WRITE_BUSY2    = 0x32u,    /* End writing          */
    R_EES_EXRFD_ENUM_IVERIFY_START  = 0x40u,    /* Start iverify        */
    R_EES_EXRFD_ENUM_IVERIFY_BUSY1  = 0x41u,    /* Continue iverify     */
    R_EES_EXRFD_ENUM_IVERIFY_BUSY2  = 0x42u     /* End iverify          */
} e_ees_exrfd_exec_state_t;                     /* -------------------- */

/**
 * @struct st_ees_control_flags_t
 * EES control flags definition
 */
typedef struct st_ees_control_flags
{
    bool b_ees_initialized;
    bool b_ees_opened;
    bool b_ees_started_up;
    bool b_ees_busy;
    bool b_ees_read_enabled;
    bool b_ees_write_enabled;
    bool b_ees_format_busy;
} st_ees_control_flags_t;

/**
 * @struct st_ees_control_variables
 * EES control variables definition
 */
typedef struct st_ees_control_variables
{
    void (R_EES_FAR_FUNC * fp_fnc_fsm_state)(void);
    void (R_EES_FAR_FUNC * fp_fnc_fsm_ret_state)(void);
    void (R_EES_FAR_FUNC * fp_fnc_fsm_ret_err_state)(void);
    st_ees_request_t __near * np_st_ees_request;
    e_ees_ret_status_t       e_ees_temp_status;
    e_ees_exrfd_ret_status_t e_ees_fsm_exrfd_ret_status;
    uint8_t __near * np_u08_exrfd_cmd_data;
    uint16_t u16_virtual_block_size;
    uint16_t u16_ees_rwp;
    uint16_t u16_ees_dwp;
    uint16_t u16_ees_startup_last_valid_rwp;
    uint16_t u16_ees_refresh_copy_offset;
    uint16_t u16_exrfd_cmd_offset_addr;
    uint16_t u16_exrfd_cmd_size;
    uint8_t  ar_u08_exrfd_cmd_data_buffer[R_EES_VALUE_U08_EXRFD_DATA_BUFFER_SIZE];
    uint8_t  u08_exrfd_cmd_virtual_block_number;
    uint8_t  u08_ees_var_count;
    uint8_t  u08_ees_block_iterator;
    uint8_t  u08_ees_active0_block_index;
    uint8_t  u08_ees_active1_block_index;
    uint8_t  u08_ees_active0_block_a_flag;
    uint8_t  u08_ees_active1_block_a_flag;
    uint8_t  u08_ees_active_block_count;
    uint8_t  u08_ees_non_excluded_block_count;
    uint8_t  u08_ees_id_size_storage;
    uint8_t  u08_ees_refresh_retry;
    uint8_t  u08_ees_refresh_block;
    uint8_t  u08_ees_refresh_a_flag;
    uint8_t  u08_ees_refresh_ref_iterator;
    uint8_t  u08_ees_refresh_copy_size;
    uint8_t  u08_ees_remain_exrfd_cmd_count;
    bool     b_ees_invalid_range0_found_flag;
    bool     b_ees_invalid_range1_found_flag;
    bool     b_ees_invalid_range2_found_flag;
} st_ees_control_variables_t;

/**
 * @struct st_ees_exrfd_control_flags_t
 * EES(EXRFD) control flags definition
 */
typedef struct st_ees_exrfd_control_flags
{
    bool b_ees_exrfd_initialized;
    bool b_ees_exrfd_opened;
    bool b_ees_exrfd_busy;
} st_ees_exrfd_control_flags_t;

#endif /* end of R_EES_TYPES_H */
