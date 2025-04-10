# RL78_F24_eeprom_emulation
 RL78_F24_eeprom_emulation

udpate @ 2025/04/08

1. initial F24 EVB , to test eeprom emulate library

- UART : P15,P16 , to printf message

2. refer to document : 

- R20UT5218EJ : EEPROM Emulation Software RL78 Type 02 User’s Manual

https://www.renesas.com/en/document/mat/eeprom-emulation-software-rl78-type-02-users-manual-rl78f23-and-rl78f24?r=488896

download EEPROM Emulation Software RL78 Type 02 for RL78/F23 and RL78/F24

https://www.renesas.com/en/document/scd/eeprom-emulation-software-rl78-type-02-rl78f23-and-rl78f24?r=488896


3. refer to user manual : __7.1.2  Example of Registration of Target Folders and Target Files__ , 

to add EEPROM Emulation (EESRL78T02) library and RFD librarty (RFDRL78T02) to project

![image](https://github.com/released/RL78_F24_eeprom_emulation/blob/main/tree01.jpg)

![image](https://github.com/released/RL78_F24_eeprom_emulation/blob/main/tree02.jpg)


refer to __7.1.3.3  Section Item Settings__  , to modify section

![image](https://github.com/released/RL78_F24_eeprom_emulation/blob/main/Link_Options_Section.jpg)

![image](https://github.com/released/RL78_F24_eeprom_emulation/blob/main/Link_Options_Section_ram_addr.jpg)


4. refer to \EESRL78T02\sample\RL78_F24\EES\CCRL\source\cstart.asm , to add library

```c
;-----------------------------------------------------------------------------
;   RAM section
;-----------------------------------------------------------------------------
.SECTION .dataR, DATA
.SECTION .sdataR, DATA
.SECTION RFD_DATA_nR, DATA
; .SECTION .datafR, DATAF
; .SECTION .textfR, TEXTF

;$IF (__RENESAS_VERSION__ < 0x01010000)  ; for CC-RL V1.00
;-----------------------------------------------------------------------------
;   stack area
;-----------------------------------------------------------------------------
; !!! [CAUTION] !!!
; Set up stack size suitable for a project.
.SECTION .stack_bss, BSS
_stackend:
    .DS     0x500
_stacktop:
;$ENDIF

```


```c

	; copy RFD external variables having initial value (near)
	MOV	ES,#HIGHW(STARTOF(RFD_DATA_n))
	MOVW	BC,#LOWW(SIZEOF(RFD_DATA_n))
	BR	$.L2_RFD_DATA
.L1_RFD_DATA:
	DECW	BC
	MOV	A,ES:LOWW(STARTOF(RFD_DATA_n))[BC]
	MOV	LOWW(STARTOF(RFD_DATA_nR))[BC],A
.L2_RFD_DATA:
	CLRW	AX
	CMPW	AX,BC
	BNZ	$.L1_RFD_DATA

```


![image](https://github.com/released/RL78_F24_eeprom_emulation/blob/main/cstart01.jpg)

![image](https://github.com/released/RL78_F24_eeprom_emulation/blob/main/cstart02.jpg)


5. enable debug msg if necessary

```c

#if 0   // enable debug msg
#define EES_DBG_MSG_OK(s,status)   \
{   \
    printf("**EES %s OK(e_status:0x%02X)\r\n",s,status);   \
    \
}
#else
#define EES_DBG_MSG_OK(s,status)    {(void)(s);(void)(status);}
#endif

```

![image](https://github.com/released/RL78_F24_eeprom_emulation/blob/main/enable_dbg_msg.jpg)

6. below is log message

![image](https://github.com/released/RL78_F24_eeprom_emulation/blob/main/log1.jpg)

7. press digit 1 to set initial value in each ID

![image](https://github.com/released/RL78_F24_eeprom_emulation/blob/main/log2.jpg)


press digit 2 to read value in each ID

![image](https://github.com/released/RL78_F24_eeprom_emulation/blob/main/log3.jpg)
![image](https://github.com/released/RL78_F24_eeprom_emulation/blob/main/log4.jpg)


press digit 3 to add counter , combine with digit 1 (write) and digit 2 (read) , to verify result
![image](https://github.com/released/RL78_F24_eeprom_emulation/blob/main/log5.jpg)

