/**********************************************************************************************************************
    Program Name    : EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : r_ees_device.h
    Program Version : V1.00
    Device(s)       : RL78/F24 microcontroller
    Description     : EES device header file
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

#ifndef R_EES_DEVICE_H
#define R_EES_DEVICE_H

/* Defines */
#define R_EES_EXRFD_VALUE_U32_DF_BASE_ADDR                         (0x000F1000uL)

#define R_EES_EXRFD_VALUE_U32_DEVICE_MEMORY_LIMIT                  (0x000FFFFFuL)
#define R_EES_EXRFD_VALUE_U32_DF_MEMORY_LIMIT                      (0x0000FFFFuL)

#define R_EES_EXRFD_VALUE_U08_MASK1_FSQ_STATUS_ERR_CFDF_SEQUENCER  (0x10u)
#define R_EES_EXRFD_VALUE_U08_MASK1_FSQ_STATUS_ERR_ERASE           (0x01u)
#define R_EES_EXRFD_VALUE_U08_MASK1_FSQ_STATUS_ERR_WRITE           (0x02u)
#define R_EES_EXRFD_VALUE_U08_MASK1_FSQ_STATUS_ERR_IVERIFY         (0x04u)
#define R_EES_EXRFD_VALUE_U08_MASK1_FSQ_STATUS_ERR_BLANKCHECK      (0x08u)

#endif /* end of R_EES_DEVICE_H */
