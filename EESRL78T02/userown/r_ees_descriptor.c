/**********************************************************************************************************************
    Program Name    : EEPROM Emulation Software (EES RL78 Type02)
    
    File Name       : r_ees_descriptor.c
    Program Version : V1.00
    Device(s)       : RL78/F24 microcontroller
    Description     : Descriptor for EES
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
#include "r_ees_types.h"
#include "r_ees_descriptor.h"
#include "r_ees_user_types.h"


/**********************************************************************************************************************
 Const variables
 *********************************************************************************************************************/

/* EES descriptor */
#define  R_EES_START_SECTION_EES_CNST
#include "r_ees_memmap.h"
__far const uint8_t g_ar_u08_ees_descriptor[R_EES_VALUE_U08_VAR_NO + 2u] =
{
	(uint8_t)(R_EES_VALUE_U08_VAR_NO),      /* variable count   */
	(uint8_t)(sizeof(type_ID001)),          /* id=1             */
	(uint8_t)(sizeof(type_ID002)),          /* id=2             */
	(uint8_t)(sizeof(type_ID003)),          /* id=3             */
	(uint8_t)(sizeof(type_ID004)),          /* id=4             */
	(uint8_t)(sizeof(type_ID005)),          /* id=5             */
	(uint8_t)(sizeof(type_ID006)),          /* id=6             */
	(uint8_t)(sizeof(type_ID007)),          /* id=7             */
	(uint8_t)(sizeof(type_ID008)),          /* id=8             */	  
	(uint8_t)(sizeof(type_ID009)),
	(uint8_t)(sizeof(type_ID010)),
	(uint8_t)(sizeof(type_ID011)),
	(uint8_t)(sizeof(type_ID012)),
	(uint8_t)(sizeof(type_ID013)),
	(uint8_t)(sizeof(type_ID014)),
	(uint8_t)(sizeof(type_ID015)),
	(uint8_t)(sizeof(type_ID016)),
	(uint8_t)(sizeof(type_ID017)),
	(uint8_t)(sizeof(type_ID018)),
	(uint8_t)(sizeof(type_ID019)),
	(uint8_t)(sizeof(type_ID020)),
	(uint8_t)(sizeof(type_ID021)),
	(uint8_t)(sizeof(type_ID022)),
	(uint8_t)(sizeof(type_ID023)),
	(uint8_t)(sizeof(type_ID024)),
	(uint8_t)(sizeof(type_ID025)),
	(uint8_t)(sizeof(type_ID026)),
	(uint8_t)(sizeof(type_ID027)),
	(uint8_t)(sizeof(type_ID028)),
	(uint8_t)(sizeof(type_ID029)),
	(uint8_t)(sizeof(type_ID030)),
	(uint8_t)(sizeof(type_ID031)),
	(uint8_t)(sizeof(type_ID032)),
	(uint8_t)(sizeof(type_ID033)),
	(uint8_t)(sizeof(type_ID034)),
	(uint8_t)(sizeof(type_ID035)),
	(uint8_t)(sizeof(type_ID036)),
	(uint8_t)(sizeof(type_ID037)),
	(uint8_t)(sizeof(type_ID038)),
	(uint8_t)(sizeof(type_ID039)),
	(uint8_t)(sizeof(type_ID040)),
	(uint8_t)(sizeof(type_ID041)),
	(uint8_t)(sizeof(type_ID042)),
	(uint8_t)(sizeof(type_ID043)),
	(uint8_t)(sizeof(type_ID044)),
	(uint8_t)(sizeof(type_ID045)),
	(uint8_t)(sizeof(type_ID046)),
	(uint8_t)(sizeof(type_ID047)),
	(uint8_t)(sizeof(type_ID048)),
	(uint8_t)(sizeof(type_ID049)),
	(uint8_t)(sizeof(type_ID050)),
	(uint8_t)(sizeof(type_ID051)),
	(uint8_t)(sizeof(type_ID052)),
	(uint8_t)(sizeof(type_ID053)),
	(uint8_t)(sizeof(type_ID054)),
	(uint8_t)(sizeof(type_ID055)),
	(uint8_t)(sizeof(type_ID056)),
	(uint8_t)(sizeof(type_ID057)),
	(uint8_t)(sizeof(type_ID058)),
	(uint8_t)(sizeof(type_ID059)),
	(uint8_t)(sizeof(type_ID060)),
	(uint8_t)(sizeof(type_ID061)),
	(uint8_t)(sizeof(type_ID062)),
	(uint8_t)(sizeof(type_ID063)),
	(uint8_t)(sizeof(type_ID064)),
	(uint8_t)(sizeof(type_ID065)),
	(uint8_t)(sizeof(type_ID066)),
	(uint8_t)(sizeof(type_ID067)),
	(uint8_t)(sizeof(type_ID068)),
	(uint8_t)(sizeof(type_ID069)),
	(uint8_t)(sizeof(type_ID070)),
	(uint8_t)(sizeof(type_ID071)),
	(uint8_t)(sizeof(type_ID072)),
	(uint8_t)(sizeof(type_ID073)),
	(uint8_t)(sizeof(type_ID074)),
	(uint8_t)(sizeof(type_ID075)),
	(uint8_t)(sizeof(type_ID076)),
	(uint8_t)(sizeof(type_ID077)),
	(uint8_t)(sizeof(type_ID078)),
	(uint8_t)(sizeof(type_ID079)),
	(uint8_t)(sizeof(type_ID080)),
	(uint8_t)(sizeof(type_ID081)),
	(uint8_t)(sizeof(type_ID082)),
	(uint8_t)(sizeof(type_ID083)),
	(uint8_t)(sizeof(type_ID084)),
	(uint8_t)(sizeof(type_ID085)),
	(uint8_t)(sizeof(type_ID086)),
	(uint8_t)(sizeof(type_ID087)),
	(uint8_t)(sizeof(type_ID088)),
	(uint8_t)(sizeof(type_ID089)),
	(uint8_t)(sizeof(type_ID090)),
	(uint8_t)(sizeof(type_ID091)),
	(uint8_t)(sizeof(type_ID092)),
	(uint8_t)(sizeof(type_ID093)),
	(uint8_t)(sizeof(type_ID094)),
	(uint8_t)(sizeof(type_ID095)),
	(uint8_t)(sizeof(type_ID096)),
	(uint8_t)(sizeof(type_ID097)),
	(uint8_t)(sizeof(type_ID098)),
	(uint8_t)(sizeof(type_ID099)),
	(uint8_t)(sizeof(type_ID100)),
	(uint8_t)(sizeof(type_ID101)),
	(uint8_t)(sizeof(type_ID102)),
	(uint8_t)(sizeof(type_ID103)),
	(uint8_t)(sizeof(type_ID104)),
	(uint8_t)(sizeof(type_ID105)),
	(uint8_t)(sizeof(type_ID106)),
	(uint8_t)(sizeof(type_ID107)),
	(uint8_t)(sizeof(type_ID108)),
	(uint8_t)(sizeof(type_ID109)),
	(uint8_t)(sizeof(type_ID110)),
	(uint8_t)(sizeof(type_ID111)),
	(uint8_t)(sizeof(type_ID112)),
	(uint8_t)(sizeof(type_ID113)),
	(uint8_t)(sizeof(type_ID114)),
	(uint8_t)(sizeof(type_ID115)),
	(uint8_t)(sizeof(type_ID116)),
	(uint8_t)(sizeof(type_ID117)),
	(uint8_t)(sizeof(type_ID118)),
	(uint8_t)(sizeof(type_ID119)),
	(uint8_t)(sizeof(type_ID120)),
	(uint8_t)(sizeof(type_ID121)),
	(uint8_t)(sizeof(type_ID122)),
	(uint8_t)(sizeof(type_ID123)),
	(uint8_t)(sizeof(type_ID124)),
	(uint8_t)(sizeof(type_ID125)),
	(uint8_t)(sizeof(type_ID126)),
	(uint8_t)(sizeof(type_ID127)),
	(uint8_t)(sizeof(type_ID128)),
	(uint8_t)(sizeof(type_ID129)),
	(uint8_t)(sizeof(type_ID130)),
	(uint8_t)(sizeof(type_ID131)),
	(uint8_t)(sizeof(type_ID132)),
	(uint8_t)(sizeof(type_ID133)),
	(uint8_t)(sizeof(type_ID134)),
	
	// (uint8_t)(sizeof(type_ID135)),
	// (uint8_t)(sizeof(type_ID136)),
	// (uint8_t)(sizeof(type_ID137)),
	// (uint8_t)(sizeof(type_ID138)),
	// (uint8_t)(sizeof(type_ID139)),
	// (uint8_t)(sizeof(type_ID140)),
	// (uint8_t)(sizeof(type_ID141)),
	// (uint8_t)(sizeof(type_ID142)),
	// (uint8_t)(sizeof(type_ID143)),
	// (uint8_t)(sizeof(type_ID144)),
	// (uint8_t)(sizeof(type_ID145)),
	// (uint8_t)(sizeof(type_ID146)),
	// (uint8_t)(sizeof(type_ID147)),
	// (uint8_t)(sizeof(type_ID148)),
	// (uint8_t)(sizeof(type_ID149)),
	// (uint8_t)(sizeof(type_ID150)),
	// (uint8_t)(sizeof(type_ID151)),
	// (uint8_t)(sizeof(type_ID152)),
	// (uint8_t)(sizeof(type_ID153)),
	// (uint8_t)(sizeof(type_ID154)),
	// (uint8_t)(sizeof(type_ID155)),
	// (uint8_t)(sizeof(type_ID156)),
	// (uint8_t)(sizeof(type_ID157)),
	// (uint8_t)(sizeof(type_ID158)),
	// (uint8_t)(sizeof(type_ID159)),
	// (uint8_t)(sizeof(type_ID160)),
	// (uint8_t)(sizeof(type_ID161)),
	// (uint8_t)(sizeof(type_ID162)),
	// (uint8_t)(sizeof(type_ID163)),
	// (uint8_t)(sizeof(type_ID164)),
	// (uint8_t)(sizeof(type_ID165)),
	// (uint8_t)(sizeof(type_ID166)),
	// (uint8_t)(sizeof(type_ID167)),
	// (uint8_t)(sizeof(type_ID168)),
	// (uint8_t)(sizeof(type_ID169)),
	// (uint8_t)(sizeof(type_ID170)),
	// (uint8_t)(sizeof(type_ID171)),
	// (uint8_t)(sizeof(type_ID172)),
	// (uint8_t)(sizeof(type_ID173)),
	// (uint8_t)(sizeof(type_ID174)),
	// (uint8_t)(sizeof(type_ID175)),
	// (uint8_t)(sizeof(type_ID176)),
	// (uint8_t)(sizeof(type_ID177)),
	// (uint8_t)(sizeof(type_ID178)),
	// (uint8_t)(sizeof(type_ID179)),
	// (uint8_t)(sizeof(type_ID180)),
	// (uint8_t)(sizeof(type_ID181)),
	// (uint8_t)(sizeof(type_ID182)),
	// (uint8_t)(sizeof(type_ID183)),
	// (uint8_t)(sizeof(type_ID184)),
	// (uint8_t)(sizeof(type_ID185)),
	// (uint8_t)(sizeof(type_ID186)),
	// (uint8_t)(sizeof(type_ID187)),
	// (uint8_t)(sizeof(type_ID188)),
	// (uint8_t)(sizeof(type_ID189)),
	// (uint8_t)(sizeof(type_ID190)),
	// (uint8_t)(sizeof(type_ID191)),
	// (uint8_t)(sizeof(type_ID192)),
	// (uint8_t)(sizeof(type_ID193)),
	// (uint8_t)(sizeof(type_ID194)),
	// (uint8_t)(sizeof(type_ID195)),
	// (uint8_t)(sizeof(type_ID196)),
	// (uint8_t)(sizeof(type_ID197)),
	// (uint8_t)(sizeof(type_ID198)),
	// (uint8_t)(sizeof(type_ID199)),
	// (uint8_t)(sizeof(type_ID200)),
	// (uint8_t)(sizeof(type_ID201)),
	// (uint8_t)(sizeof(type_ID202)),
	// (uint8_t)(sizeof(type_ID203)),
	// (uint8_t)(sizeof(type_ID204)),
	// (uint8_t)(sizeof(type_ID205)),
	// (uint8_t)(sizeof(type_ID206)),
	// (uint8_t)(sizeof(type_ID207)),
	// (uint8_t)(sizeof(type_ID208)),
	// (uint8_t)(sizeof(type_ID209)),
	// (uint8_t)(sizeof(type_ID210)),
	// (uint8_t)(sizeof(type_ID211)),
	// (uint8_t)(sizeof(type_ID212)),
	// (uint8_t)(sizeof(type_ID213)),
	// (uint8_t)(sizeof(type_ID214)),
	// (uint8_t)(sizeof(type_ID215)),
	// (uint8_t)(sizeof(type_ID216)),
	// (uint8_t)(sizeof(type_ID217)),
	// (uint8_t)(sizeof(type_ID218)),
	// (uint8_t)(sizeof(type_ID219)),
	// (uint8_t)(sizeof(type_ID220)),
	// (uint8_t)(sizeof(type_ID221)),
	// (uint8_t)(sizeof(type_ID222)),
	// (uint8_t)(sizeof(type_ID223)),
	// (uint8_t)(sizeof(type_ID224)),
	// (uint8_t)(sizeof(type_ID225)),
	// (uint8_t)(sizeof(type_ID226)),
	// (uint8_t)(sizeof(type_ID227)),
	// (uint8_t)(sizeof(type_ID228)),
	// (uint8_t)(sizeof(type_ID229)),
	// (uint8_t)(sizeof(type_ID230)),
	// (uint8_t)(sizeof(type_ID231)),
	// (uint8_t)(sizeof(type_ID232)),
	// (uint8_t)(sizeof(type_ID233)),
	// (uint8_t)(sizeof(type_ID234)),
	// (uint8_t)(sizeof(type_ID235)),
	// (uint8_t)(sizeof(type_ID236)),
	// (uint8_t)(sizeof(type_ID237)),
	// (uint8_t)(sizeof(type_ID238)),
	// (uint8_t)(sizeof(type_ID239)),
	// (uint8_t)(sizeof(type_ID240)),
	// (uint8_t)(sizeof(type_ID241)),
	// (uint8_t)(sizeof(type_ID242)),
	// (uint8_t)(sizeof(type_ID243)),
	// (uint8_t)(sizeof(type_ID244)),
	// (uint8_t)(sizeof(type_ID245)),
	// (uint8_t)(sizeof(type_ID246)),
	// (uint8_t)(sizeof(type_ID247)),
	// (uint8_t)(sizeof(type_ID248)),
	// (uint8_t)(sizeof(type_ID249)),
	// (uint8_t)(sizeof(type_ID250)),
	// (uint8_t)(sizeof(type_ID251)),
	// (uint8_t)(sizeof(type_ID252)),
	// (uint8_t)(sizeof(type_ID253)),
	// (uint8_t)(sizeof(type_ID254)),

  
	(uint8_t)(0x00u)                   /* zero terminator  */
};
#define  R_EES_END_SECTION_EES_CNST
#include "r_ees_memmap.h"


/* ----------------------------------------------------------------------------------------------------------------- */
/* ------------                                                                                         ------------ */
/* ------------      B E G I N    O F    U N T O U C H A B L E    D E C L A R A T I O N    A R E A      ------------ */
/* ------------                                                                                         ------------ */
/* ----------------------------------------------------------------------------------------------------------------- */
/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/* for EES(EXRFD) */
#define  R_EES_START_SECTION_EES_CNST
#include "r_ees_memmap.h"
__far const st_ees_exrfd_descriptor_t g_st_ees_exrfd_descriptor =
{
    (uint16_t) R_EES_EXRFD_VALUE_U16_PHYSICAL_BLOCK_SIZE,
    (uint8_t)  R_EES_EXRFD_VALUE_U08_PHYSICAL_BLOCKS_PER_VIRTUAL_BLOCK,
    (uint8_t)  R_EES_EXRFD_VALUE_U08_POOL_VIRTUAL_BLOCKS
};
#define  R_EES_END_SECTION_EES_CNST
#include "r_ees_memmap.h"

#define  R_EES_START_SECTION_EES_VAR
#include "r_ees_memmap.h"
/* EES RAM reference table */
__near uint16_t g_ar_u16_ram_ref_table[R_EES_VALUE_U08_VAR_NO];
#define  R_EES__END_SECTION_EES_VAR
#include "r_ees_memmap.h"

/* ----------------------------------------------------------------------------------------------------------------- */
/* ------------                                                                                         ------------ */
/* ------------        E N D    O F    U N T O U C H A B L E    D E C L A R A T I O N    A R E A        ------------ */
/* ------------                                                                                         ------------ */
/* ----------------------------------------------------------------------------------------------------------------- */
