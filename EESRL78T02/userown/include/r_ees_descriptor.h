/**********************************************************************************************************************
    Program Name    : EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : r_ees_descriptor.h
    Program Version : V1.00
    Device(s)       : RL78/F24 microcontroller
    Description     : Descriptor header file of EES
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

#ifndef R_EES_DESCRIPTOR_H
#define R_EES_DESCRIPTOR_H

#include "r_ees_types.h"

/* Defines */

/* Specifies the size of one physical data flash block */
#define R_EES_EXRFD_VALUE_U16_PHYSICAL_BLOCK_SIZE               (1024u)

/* Specifies the number of physical data flash blocks per one virtual block */
#define R_EES_EXRFD_VALUE_U08_PHYSICAL_BLOCKS_PER_VIRTUAL_BLOCK (2u)

/* Specifies the number of virtual blocks */
#define R_EES_EXRFD_VALUE_U08_POOL_VIRTUAL_BLOCKS               (8u)

/* Specifies the number of EES variables (1-254) */
#define R_EES_VALUE_U08_VAR_NO                                  (134u)  //(134u)


/* ----------------------------------------------------------------------------------------------------------------- */
/* ------------                                                                                         ------------ */
/* ------------      B E G I N    O F    U N T O U C H A B L E    D E C L A R A T I O N    A R E A      ------------ */
/* ------------                                                                                         ------------ */
/* ----------------------------------------------------------------------------------------------------------------- */

/* External references */
extern __far const uint8_t g_ar_u08_ees_descriptor[R_EES_VALUE_U08_VAR_NO + 2u];
extern __far const st_ees_exrfd_descriptor_t g_st_ees_exrfd_descriptor;
extern __near uint16_t g_ar_u16_ram_ref_table[R_EES_VALUE_U08_VAR_NO];

/* ----------------------------------------------------------------------------------------------------------------- */
/* ------------                                                                                         ------------ */
/* ------------        E N D    O F    U N T O U C H A B L E    D E C L A R A T I O N    A R E A        ------------ */
/* ------------                                                                                         ------------ */
/* ----------------------------------------------------------------------------------------------------------------- */

#endif /* end of R_EES_DESCRIPTOR_H */
