/**********************************************************************************************************************
    Program Name    : Sample program for EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : sample_ees_memmap.h
    Program Version : V1.00.00
    Device(s)       : RL78/F24 microcontroller
    Description     : Sample memory map header file
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

#include "r_ees_compiler.h"

#if (EES_COMPILER_CC == EES_COMPILER)
    /* Section definition for sample control EES code */
    #if defined (SAMPLE_START_SECTION_SMP_EES)
        #undef  SAMPLE_START_SECTION_SMP_EES
        #pragma section text SMP_EES
    #elif defined (SAMPLE_END_SECTION_SMP_EES)
        #undef  SAMPLE_END_SECTION_SMP_EES
        #pragma section
    /* Section definition for sample variables to control EES */
    #elif defined (SAMPLE_START_SECTION_SMP_VAR)
        #undef  SAMPLE_START_SECTION_SMP_VAR
        #pragma section bss SMP_VAR
    #elif defined (SAMPLE_END_SECTION_SMP_VAR)
        #undef  SAMPLE_END_SECTION_SMP_VAR
        #pragma section
    #endif  /* Section definition for CCRL */
#elif (EES_COMPILER_IAR == EES_COMPILER)
    /* Section definition for sample control EES code */
    #if defined (SAMPLE_START_SECTION_SMP_EES)
        #undef  SAMPLE_START_SECTION_SMP_EES
        #pragma location = "SMP_EES"
    #elif defined (SAMPLE_END_SECTION_SMP_EES)
        #undef  SAMPLE_END_SECTION_SMP_EES
    /* Section definition for sample variables to control EES */
    #elif defined (SAMPLE_START_SECTION_SMP_VAR)
        #undef  SAMPLE_START_SECTION_SMP_VAR
        #pragma location = "SMP_VAR"
    #elif defined (SAMPLE_END_SECTION_SMP_VAR)
        #undef  SAMPLE_END_SECTION_SMP_VAR
    #endif  /* Section definition for IAR */
#endif /* end of section definitions for each compiler */
