/**********************************************************************************************************************
    Program Name    : EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : r_ees_defines.h
    Program Version : V1.00
    Device(s)       : RL78/F24 microcontroller
    Description     : EES defines header file
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

#ifndef R_EES_DEFINES_H
#define R_EES_DEFINES_H

/* Defines */
#define R_EES_VALUE_U08_INIT_VARIABLE                               (0x00u)
#define R_EES_VALUE_U16_INIT_VARIABLE                               (0x0000u)
#define R_EES_VALUE_U32_INIT_VARIABLE                               (0x00000000uL)

#define R_EES_VALUE_U01_MASK0_1BIT                                  (0u)
#define R_EES_VALUE_U01_MASK1_1BIT                                  (1u)
#define R_EES_VALUE_U08_MASK0_8BIT                                  (0x00u)
#define R_EES_VALUE_U08_MASK1_8BIT                                  (0xFFu)

#define R_EES_EXRFD_VALUE_U08_PHYSICAL_BLOCKS_PER_VIRTUAL_BLOCK_MIN (0x01u)
#define R_EES_EXRFD_VALUE_U08_POOL_VIRTUAL_BLOCKS_MIN               (0x02u)
#define R_EES_VALUE_U08_EES_VARS_MIN                                (0x01u)
#define R_EES_VALUE_U08_EES_VARS_MAX                                (0xFEu)
#define R_EES_VALUE_U08_HEADER_SIZE                                 (0x08u)
#define R_EES_VALUE_U08_SEPARATOR_SIZE                              (0x02u)
#define R_EES_VALUE_U08_REFERENCE_ENTRY_SIZE                        (0x02u)
#define R_EES_VALUE_U08_A_FLAG_INDEX                                (0x00u)
#define R_EES_VALUE_U08_B_FLAG_INDEX                                (0x01u)
#define R_EES_VALUE_U08_I_FLAG_INDEX                                (0x02u)
#define R_EES_VALUE_U08_X_FLAG_INDEX                                (0x03u)
#define R_EES_VALUE_U08_HEADER_VALID_FLAG_SIZE                      (0x04u)
#define R_EES_VALUE_U08_A_FLAG_ORDER_MIN                            (0x01u)
#define R_EES_VALUE_U08_A_FLAG_ORDER_MAX                            (0x03u)
#define R_EES_VALUE_U08_SOR_INDEX                                   (0x00u)
#define R_EES_VALUE_U08_EOR_INDEX                                   (0x01u)
#define R_EES_VALUE_U08_REFRESH_RETRY                               (0x01u)
#define R_EES_VALUE_U08_EXRFD_DATA_BUFFER_SIZE                      (0x08u)

/* Internal test macro */
#define R_EES_EXRFD_INTERNAL_CODE_FLASH_MODE_ERASE
#define R_EES_EXRFD_INTERNAL_CODE_FLASH_MODE_WRITE
#define R_EES_EXRFD_INTERNAL_CODE_FLASH_MODE_IVERIFY
#define R_EES_EXRFD_INTERNAL_CODE_FLASH_MODE_BLANKCHECK
#define R_EES_EXRFD_INTERNAL_CODE_FLASH_MODE_READ
#define R_EES_EXRFD_INTERNAL_CODE_FLASH_MODE_FINISH
#define R_EES_EXRFD_INTERNAL_CODE_CMD_ERASE
#define R_EES_EXRFD_INTERNAL_CODE_CMD_WRITE
#define R_EES_EXRFD_INTERNAL_CODE_CMD_IVERIFY
#define R_EES_EXRFD_INTERNAL_CODE_CMD_BLANKCHECK
#define R_EES_EXRFD_INTERNAL_CODE_CMD_READ
#define R_EES_EXRFD_INTERNAL_CODE_FSQ_STATUS
#define R_EES_EXRFD_INTERNAL_CODE_FSQ_STEP2_STATUS
#define R_EES_EXRFD_INTERNAL_CODE_EXEC_STATE

#endif /* end of R_EES_DEFINES_H */
