/**********************************************************************************************************************
    Program Name    : EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : r_ees_compiler.h
    Program Version : V1.00
    Device(s)       : RL78/F24 microcontroller
    Description     : Compiler dependent header file
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

#ifndef R_EES_COMPILER_H
#define R_EES_COMPILER_H

/* Compiler definition */
#define EES_COMPILER_CC  (1)
#define EES_COMPILER_IAR (2)

#if defined (__CCRL__)
    #define EES_COMPILER EES_COMPILER_CC
#elif defined (__IAR_SYSTEMS_ICC__)
    #define EES_COMPILER EES_COMPILER_IAR
#else
    /* Unknown compiler error */
    #error  "Non-supported compiler."
#endif

/* Compiler dependent definition */
#if (EES_COMPILER_CC == EES_COMPILER)
    #define R_EES_FAR_FUNC                           __far
#elif (EES_COMPILER_IAR == EES_COMPILER)
    #define R_EES_FAR_FUNC                           __far_func
#else
    /* Unknown compiler error */
    #error  "Non-supported compiler."
#endif

#endif /* end of R_EES_COMPILER_H */
