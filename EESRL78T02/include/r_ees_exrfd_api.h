/**********************************************************************************************************************
    Program Name    : EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : r_ees_exrfd_api.h
    Program Version : V1.00
    Device(s)       : RL78/F24 microcontroller
    Description     : API definition of EES(EXRFD)
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

#ifndef R_EES_EXRFD_API_H
#define R_EES_EXRFD_API_H

#include "r_ees.h"

/**********************************************************************************************************************
 Prototype declaration
 *********************************************************************************************************************/

extern __near uint16_t g_u16_ees_physical_block_size;
extern __near uint8_t  g_u08_ees_physical_blocks_per_virtual_block;
extern __near uint8_t  g_u08_ees_pool_virtual_blocks;
extern __near st_ees_exrfd_control_flags_t g_st_ees_exrfd_control_flags;
extern __near e_ees_exrfd_exec_state_t     g_e_ees_exrfd_exec_state;
extern __near uint8_t  g_u08_ees_exrfd_cmd_start_block;
extern __near uint32_t g_u32_ees_exrfd_cmd_start_addr;
extern __near uint16_t g_u16_ees_exrfd_cmd_count;
extern uint8_t __near * __near g_np_u08_ees_exrfd_cmd_write_data;

extern R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Init(uint8_t i_u08_cpu_frequency);
extern R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Open(void);
extern R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Close(void);
extern R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Erase(uint8_t i_u08_virtual_block_number);
extern R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Write(uint16_t i_u16_offset_addr, 
                                                                 uint8_t __near * inp_u08_write_data, 
                                                                 uint16_t i_u16_size);
extern R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_BlankCheck(uint16_t i_u16_offset_addr, uint16_t i_u16_size);
extern R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_IVerify(uint16_t i_u16_offset_addr, uint16_t i_u16_size);
extern R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Read(uint16_t i_u16_offset_addr, 
                                                                uint8_t __near * onp_u08_read_data, 
                                                                uint16_t i_u16_size);
extern R_EES_FAR_FUNC e_ees_exrfd_ret_status_t R_EES_EXRFD_Handler(void);

#endif /* end of R_EES_EXRFD_API_H */
