/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>
#include <string.h>

#include "r_smc_entry.h"
#include "platform.h"

#include "misc_config.h"
#include "custom_func.h"

#include "r_ees_api.h"

#include "sample_control_ees.h"
#include "sample_ees_defines.h"
#include "r_ees_user_types.h"


#define  SAMPLE_START_SECTION_SMP_VAR
#include "sample_ees_memmap.h"
/* Write buffer for EES variables */
/* The number of array elements depends on the maximum size of EES variables */
static __near uint8_t sg_ar_u08_write_data[255u];
#define  SAMPLE_END_SECTION_SMP_VAR
#include "sample_ees_memmap.h"

#define  SAMPLE_START_SECTION_SMP_VAR
#include "sample_ees_memmap.h"
/* Read buffer for EES variables */
/* The number of array elements depends on the maximum size of EES variables */
static __near uint8_t sg_ar_u08_read_data[255u];
#define  SAMPLE_END_SECTION_SMP_VAR
#include "sample_ees_memmap.h"

#define  SAMPLE_START_SECTION_SMP_VAR
#include "sample_ees_memmap.h"
static __near st_ees_request_t sg_st_ees_request;
#define  SAMPLE_END_SECTION_SMP_VAR
#include "sample_ees_memmap.h"


/*_____ D E C L A R A T I O N S ____________________________________________*/

volatile struct flag_32bit flag_PROJ_CTL;
#define FLAG_PROJ_TIMER_PERIOD_1000MS                 	(flag_PROJ_CTL.bit0)
#define FLAG_PROJ_TRIG_BTN1                       	    (flag_PROJ_CTL.bit1)
#define FLAG_PROJ_TRIG_BTN2                 	        (flag_PROJ_CTL.bit2)
#define FLAG_PROJ_REVERSE3                    		    (flag_PROJ_CTL.bit3)
#define FLAG_PROJ_REVERSE4                              (flag_PROJ_CTL.bit4)
#define FLAG_PROJ_REVERSE5                              (flag_PROJ_CTL.bit5)
#define FLAG_PROJ_REVERSE6                              (flag_PROJ_CTL.bit6)
#define FLAG_PROJ_REVERSE7                              (flag_PROJ_CTL.bit7)


#define FLAG_PROJ_TRIG_1                                (flag_PROJ_CTL.bit8)
#define FLAG_PROJ_TRIG_2                                (flag_PROJ_CTL.bit9)
#define FLAG_PROJ_TRIG_3                                (flag_PROJ_CTL.bit10)
#define FLAG_PROJ_TRIG_4                                (flag_PROJ_CTL.bit11)
#define FLAG_PROJ_TRIG_5                                (flag_PROJ_CTL.bit12)
#define FLAG_PROJ_REVERSE13                             (flag_PROJ_CTL.bit13)
#define FLAG_PROJ_REVERSE14                             (flag_PROJ_CTL.bit14)
#define FLAG_PROJ_REVERSE15                             (flag_PROJ_CTL.bit15)

/*_____ D E F I N I T I O N S ______________________________________________*/

volatile unsigned long counter_tick = 0;
volatile unsigned long btn_counter_tick = 0;

#define BTN_PRESSED_LONG                                (2500)

/* Defines */
#if 0   // enable debug msg
#define EES_DBG_MSG_OK(s,status)   \
{   \
    printf("**EES %s OK(e_status:0x%02X)\r\n",s,status);   \
    \
}
#else
#define EES_DBG_MSG_OK(s,status)    {(void)(s);(void)(status);}
#endif

#if 1   // enable debug msg
#define EES_DBG_MSG_FAIL(s,status)   \
{   \
    printf("--EES %s FAIL(e_status:0x%02X)\r\n",s,status);   \
    \
}
#else
#define EES_DBG_MSG_FAIL(s,status)    {(void)(s);(void)(status);}
#endif 

/**** CPU frequency (MHz) ****/
/* It must be rounded up digits after the decimal point to form an integer (MHz). */
#define SAMPLE_VALUE_U08_CPU_FREQUENCY                  (40u)

e_ees_ret_status_t l_e_ees_ret_value = R_EES_ENUM_RET_STS_BUSY;
bool l_b_sam_error_flag = false;
bool ees_process_flag = false;

enum
{
    EES_ID_DEFAULT,
    EES_ID001,
    EES_ID002,
    EES_ID003,
    EES_ID004,
    EES_ID005,
    EES_ID006,
    EES_ID007,
    EES_ID008,
    EES_ID009,
    EES_ID010,
    EES_ID011,
    EES_ID012,
    EES_ID013,
    EES_ID014,
    EES_ID015,

    EES_ID016,
    EES_ID017,
    EES_ID018,
    EES_ID019,
    EES_ID020,
    EES_ID021,
    EES_ID022,
    EES_ID023,
    EES_ID024,
    EES_ID025,
    EES_ID026,
    EES_ID027,
    EES_ID028,
    EES_ID029,
    EES_ID030,

    EES_ID031,
    EES_ID032,
    EES_ID033,
    EES_ID034,
    EES_ID035,
    EES_ID036,
    EES_ID037,
    EES_ID038,
    EES_ID039,
    EES_ID040,
    EES_ID041,
    EES_ID042,
    EES_ID043,
    EES_ID044,
    EES_ID045,

    EES_ID046,
    EES_ID047,
    EES_ID048,
    EES_ID049,
    EES_ID050,
    EES_ID051,
    EES_ID052,
    EES_ID053,
    EES_ID054,
    EES_ID055,
    EES_ID056,
    EES_ID057,
    EES_ID058,
    EES_ID059,
    EES_ID060,

    EES_ID061,
    EES_ID062,
    EES_ID063,
    EES_ID064,
    EES_ID065,
    EES_ID066,
    EES_ID067,
    EES_ID068,
    EES_ID069,
    EES_ID070,
    EES_ID071,
    EES_ID072,
    EES_ID073,
    EES_ID074,
    EES_ID075,

    EES_ID076,
    EES_ID077,
    EES_ID078,
    EES_ID079,
    EES_ID080,
    EES_ID081,
    EES_ID082,
    EES_ID083,
    EES_ID084,
    EES_ID085,
    EES_ID086,
    EES_ID087,
    EES_ID088,
    EES_ID089,
    EES_ID090,

    EES_ID091,
    EES_ID092,
    EES_ID093,
    EES_ID094,
    EES_ID095,
    EES_ID096,
    EES_ID097,
    EES_ID098,
    EES_ID099,
    EES_ID100,
    EES_ID101,
    EES_ID102,
    EES_ID103,
    EES_ID104,
    EES_ID105,

    EES_ID106,
    EES_ID107,
    EES_ID108,
    EES_ID109,
    EES_ID110,
    EES_ID111,
    EES_ID112,
    EES_ID113,
    EES_ID114,
    EES_ID115,
    EES_ID116,
    EES_ID117,
    EES_ID118,
    EES_ID119,
    EES_ID120,

    EES_ID121,
    EES_ID122,
    EES_ID123,
    EES_ID124,
    EES_ID125,
    EES_ID126,
    EES_ID127,
    EES_ID128,
    EES_ID129,
    EES_ID130,
    EES_ID131,
    EES_ID132,
    EES_ID133,
    EES_ID134,
    // EES_ID135,

    // EES_ID136,
    // EES_ID137,
    // EES_ID138,
    // EES_ID139,
    // EES_ID140,
    // EES_ID141,
    // EES_ID142,
    // EES_ID143,
    // EES_ID144,
    // EES_ID145,
    // EES_ID146,
    // EES_ID147,
    // EES_ID148,
    // EES_ID149,
    // EES_ID150,

    // EES_ID151,
    // EES_ID152,
    // EES_ID153,
    // EES_ID154,
    // EES_ID155,
    // EES_ID156,
    // EES_ID157,
    // EES_ID158,
    // EES_ID159,
    // EES_ID160,
    // EES_ID161,
    // EES_ID162,
    // EES_ID163,
    // EES_ID164,
    // EES_ID165,

    // EES_ID166,
    // EES_ID167,
    // EES_ID168,
    // EES_ID169,
    // EES_ID170,
    // EES_ID171,
    // EES_ID172,
    // EES_ID173,
    // EES_ID174,
    // EES_ID175,
    // EES_ID176,
    // EES_ID177,
    // EES_ID178,
    // EES_ID179,
    // EES_ID180,

    // EES_ID181,
    // EES_ID182,
    // EES_ID183,
    // EES_ID184,
    // EES_ID185,
    // EES_ID186,
    // EES_ID187,
    // EES_ID188,
    // EES_ID189,
    // EES_ID190,
    // EES_ID191,
    // EES_ID192,
    // EES_ID193,
    // EES_ID194,
    // EES_ID195,

    // EES_ID196,
    // EES_ID197,
    // EES_ID198,
    // EES_ID199,
    // EES_ID200,
    // EES_ID201,
    // EES_ID202,
    // EES_ID203,
    // EES_ID204,
    // EES_ID205,
    // EES_ID206,
    // EES_ID207,
    // EES_ID208,
    // EES_ID209,
    // EES_ID210,

    // EES_ID211,
    // EES_ID212,
    // EES_ID213,
    // EES_ID214,
    // EES_ID215,
    // EES_ID216,
    // EES_ID217,
    // EES_ID218,
    // EES_ID219,
    // EES_ID220,
    // EES_ID221,
    // EES_ID222,
    // EES_ID223,
    // EES_ID224,
    // EES_ID225,

    // EES_ID226,
    // EES_ID227,
    // EES_ID228,
    // EES_ID229,
    // EES_ID230,
    // EES_ID231,
    // EES_ID232,
    // EES_ID233,
    // EES_ID234,
    // EES_ID235,
    // EES_ID236,
    // EES_ID237,
    // EES_ID238,
    // EES_ID239,
    // EES_ID240,

    // EES_ID241,
    // EES_ID242,
    // EES_ID243,
    // EES_ID244,
    // EES_ID245,
    // EES_ID246,
    // EES_ID247,
    // EES_ID248,
    // EES_ID249,
    // EES_ID250,
    // EES_ID251,
    // EES_ID252,
    // EES_ID253,
    // EES_ID254,

};

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

unsigned long btn_get_tick(void)
{
	return (btn_counter_tick);
}

void btn_set_tick(unsigned long t)
{
	btn_counter_tick = t;
}

void btn_tick_counter(void)
{
	btn_counter_tick++;
    if (btn_get_tick() >= 60000)
    {
        btn_set_tick(0);
    }
}

unsigned long get_tick(void)
{
	return (counter_tick);
}

void set_tick(unsigned long t)
{
	counter_tick = t;
}

void tick_counter(void)
{
	counter_tick++;
    if (get_tick() >= 60000)
    {
        set_tick(0);
    }
}

void delay_ms(unsigned long ms)
{
	#if 1
    unsigned long tickstart = get_tick();
    unsigned long wait = ms;
	unsigned long tmp = 0;
	
    while (1)
    {
		if (get_tick() > tickstart)	// tickstart = 59000 , tick_counter = 60000
		{
			tmp = get_tick() - tickstart;
		}
		else // tickstart = 59000 , tick_counter = 2048
		{
			tmp = 60000 -  tickstart + get_tick();
		}		
		
		if (tmp > wait)
			break;
    }
	
	#else
	TIMER_Delay(TIMER0, 1000*ms);
	#endif
}

#define  SAMPLE_START_SECTION_SMP_EES
#include "sample_ees_memmap.h"

/*
    It is not allowed to call R_EES_Handler() in an interrupt service routine
    
    The EES does not support multitask execution. Do not execute the EES functions during interrupt 
    processing
    
    It is not allowed to call R_EES_Execute function in an interrupt service routine.

    All members of the request structure (st_ees_request_t) must be initialized once before a EES 
    command is executed. If any unused member exists in the request structure (st_ees_request_t), set a 
    desired value for the member. If any member is not initialized, the RL78/F23 and RL78/F24 
    microcontrollers may be reset due to a RAM parity error. For details, refer to “User’s Manual: Hardware” 
    of a target device. 

*/

void __EES_close(void)
{   

    /******************************************************************************************************************
     * SHUTDOWN
     *****************************************************************************************************************/
    if (true != l_b_sam_error_flag)
    {
        /* Set parameters for SHUTDOWN command */
        sg_st_ees_request.e_command = R_EES_ENUM_CMD_SHUTDOWN;
        
        /* Execute SHUTDOWN command */
        R_EES_Execute(&sg_st_ees_request);
        
        /* When another command is running, call R_EES_Handler and retry SHUTDOWN command */
        while (R_EES_ENUM_RET_ERR_REJECTED == sg_st_ees_request.e_status)
        {
            /* It is possible to write the program for detecting timeout here as necessity requires */
            R_EES_Handler();
            R_EES_Execute(&sg_st_ees_request);
        }
        
        l_e_ees_ret_value = sg_st_ees_request.e_status;
        
        /* Check returned status of the command */
        if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
        {
            /* SHUTDOWN command was finished successfully */
            EES_DBG_MSG_OK("SHUTDOWN",l_e_ees_ret_value);
            /* No operation */
        }
        else
        {
            /* SHUTDOWN error handling */
            l_b_sam_error_flag = true;
            EES_DBG_MSG_FAIL("SHUTDOWN",l_e_ees_ret_value);
        }
    }
    
    /******************************************************************************************************************
     * Close EES
     *****************************************************************************************************************/
    if (true != l_b_sam_error_flag)
    {
        l_e_ees_ret_value = R_EES_Close();
        if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
        {
            /* EES was closed successfully */
            /* No operation */
            EES_DBG_MSG_OK("R_EES_Close",l_e_ees_ret_value);
        }
        else
        {
            /* Error handling, but never occurs */
            l_b_sam_error_flag = true;
            EES_DBG_MSG_FAIL("R_EES_Close",l_e_ees_ret_value);
        }
    }
    else
    {
        /* No operation */
    }
    

}

void __EES_startup(void)
{
    /******************************************************************************************************************
     * STARTUP
     *****************************************************************************************************************/
    if (true != l_b_sam_error_flag)
    {
        do
        {
            /* Set parameters for STARTUP command */
            sg_st_ees_request.e_command = R_EES_ENUM_CMD_STARTUP;
            
            /* Execute STARTUP command */
            R_EES_Execute(&sg_st_ees_request);
            
            /* Wait for the end of running command */
            while (R_EES_ENUM_RET_STS_BUSY == sg_st_ees_request.e_status)
            {
                /* It is possible to write the program for detecting timeout here as necessity requires */
                R_EES_Handler();
            }
            
            l_e_ees_ret_value = sg_st_ees_request.e_status;
            
            /* Check returned status of the command */
            if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
            {
                /* STARTUP command was finished successfully */
                EES_DBG_MSG_OK("STARTUP",l_e_ees_ret_value);
                /* No operation */
            }
            else if (R_EES_ENUM_RET_ERR_POOL_INCONSISTENT == l_e_ees_ret_value)
            {
                /******************************************************************************************************
                 * FORMAT
                 *****************************************************************************************************/
                /* Set parameters for FORMAT command */
                sg_st_ees_request.e_command = R_EES_ENUM_CMD_FORMAT;
                
                /* Execute FORMAT command */
                R_EES_Execute(&sg_st_ees_request);
                
                /* Wait for the end of running command */
                while (R_EES_ENUM_RET_STS_BUSY == sg_st_ees_request.e_status)
                {
                    /* It is possible to write the program for detecting timeout here as necessity requires */
                    R_EES_Handler();
                }
                
                l_e_ees_ret_value = sg_st_ees_request.e_status;
                
                /* Check returned status of the command */
                if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
                {
                    /* FORMAT command was finished successfully */
                    EES_DBG_MSG_OK("FORMAT",l_e_ees_ret_value);
                    /* No operation */
                }
                else
                {
                    /* FORMAT error handling */
                    l_b_sam_error_flag = true;
                    EES_DBG_MSG_FAIL("FORMAT",l_e_ees_ret_value);
                }
            }
            else
            {
                /* STARTUP error handling */
                /* If R_EES_ENUM_RET_ERR_VERIFY is returned, then REFRESH command should be executed */
                l_b_sam_error_flag = true;
                EES_DBG_MSG_FAIL("STARTUP",l_e_ees_ret_value);
            }
        } while ((R_EES_ENUM_CMD_FORMAT == sg_st_ees_request.e_command)
                 && (true != l_b_sam_error_flag));
    }
    else
    {
        /* No operation */
    }
    
}

void __EES_open(void)
{
    DI(); 

    /* Local variable definitions */
    
    /* Set local variables */
    l_e_ees_ret_value  = R_EES_ENUM_RET_STS_OK;
    l_b_sam_error_flag = false;
    
    /******************************************************************************************************************
     * Open EES
     *****************************************************************************************************************/
    if (true != l_b_sam_error_flag)
    {
        l_e_ees_ret_value = R_EES_Open();
        if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
        {
            /* EES was opened successfully */
            /* No operation */
            EES_DBG_MSG_OK("R_EES_Open",l_e_ees_ret_value);
        }
        else
        {
            /* Error handling */
            l_b_sam_error_flag = true;
            EES_DBG_MSG_FAIL("R_EES_Open",l_e_ees_ret_value);
        }
    }
    else
    {
        /* No operation */
    }
    
    /* Initialize EES requester */
    sg_st_ees_request.np_u08_address = NULL;//&sg_ar_u08_write_data[0];
    sg_st_ees_request.u08_identifier = SAMPLE_VALUE_U08_INIT_VARIABLE;
    sg_st_ees_request.e_command      = R_EES_ENUM_CMD_UNDEFINED;
    sg_st_ees_request.e_status       = R_EES_ENUM_RET_ERR_PARAMETER;

    EI();

}

void __EES_init(void)
{    
    DI(); 

    /* Local variable definitions */
    
    /* Set local variables */
    l_e_ees_ret_value  = R_EES_ENUM_RET_STS_OK;
    l_b_sam_error_flag = false;
    
    /******************************************************************************************************************
     * Initialize EES
     *****************************************************************************************************************/
    l_e_ees_ret_value = R_EES_Init(SAMPLE_VALUE_U08_CPU_FREQUENCY);
    if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
    {
        /* EES was initialized successfully */
        /* No operation */
        EES_DBG_MSG_OK("R_EES_Init",l_e_ees_ret_value);
    }
    else
    {
        /* Error handling */
        l_b_sam_error_flag = true;
        EES_DBG_MSG_FAIL("R_EES_Init",l_e_ees_ret_value);
    }
    
    EI();
}


// use sg_ar_u08_read_data as ram buffer
bool __EES_read_data(uint8_t data_ID)
{    
    bool exec_flag = false;

    DI(); 

    l_e_ees_ret_value  = R_EES_ENUM_RET_STS_OK;
    l_b_sam_error_flag = false;    

    __EES_open();
    __EES_startup();

    /******************************************************************************************************************
     * READ
     *****************************************************************************************************************/
    if (true != l_b_sam_error_flag)
    {
        /* Set parameters for READ command */
        sg_st_ees_request.np_u08_address = & sg_ar_u08_read_data[0];
        sg_st_ees_request.u08_identifier = data_ID;
        sg_st_ees_request.e_command      = R_EES_ENUM_CMD_READ;
        
        /* Execute READ command */
        R_EES_Execute(&sg_st_ees_request);
        
        /* Wait for the end of running command */
        while (R_EES_ENUM_RET_STS_BUSY == sg_st_ees_request.e_status)
        {
            /* It is possible to write the program for detecting timeout here as necessity requires */
            R_EES_Handler();
        }
        
        l_e_ees_ret_value = sg_st_ees_request.e_status;
        
        if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
        {
            /* READ command was finished successfully */
            exec_flag = true;
            EES_DBG_MSG_OK("READ",l_e_ees_ret_value);

            /* Compare read data with source data */
            // for (l_u16_count = 0u; l_u16_count < sizeof(type_A); l_u16_count++)
            // {
            //     if (sg_ar_u08_write_data[l_u16_count] !=  sg_ar_u08_read_data[l_u16_count])
            //     {
            //         /* Mismatch data error handling */
            //         l_b_sam_error_flag = true;
            //         break;
            //     }
            // }
        }
        else
        {
            /* READ error handling */
            l_b_sam_error_flag = true;
            EES_DBG_MSG_FAIL("READ",l_e_ees_ret_value);
        }
    }
    else
    {
        /* No operation */
    }

    __EES_close();

    EI();

    return exec_flag;
}

// use sg_ar_u08_write_data as ram buffer
bool __EES_write_data(uint8_t data_ID)
{    
    bool exec_flag = false;

    DI(); 

    l_e_ees_ret_value  = R_EES_ENUM_RET_STS_OK;
    l_b_sam_error_flag = false;

    __EES_open();
    __EES_startup();

    /******************************************************************************************************************
     * WRITE
     *****************************************************************************************************************/
    if (true != l_b_sam_error_flag)
    {
        do
        {
            /* Set data to write */
            // sg_ar_u08_write_data[0] = 0xAAu;
            // sg_ar_u08_write_data[1] = 0x55u;
            
            /* Set parameters for WRITE command */
            sg_st_ees_request.np_u08_address = &sg_ar_u08_write_data[0u];
            sg_st_ees_request.u08_identifier = data_ID;
            sg_st_ees_request.e_command      = R_EES_ENUM_CMD_WRITE;
            
            /* Execute WRITE command */
            R_EES_Execute(&sg_st_ees_request);
            
            /* Wait for the end of running command */
            while (R_EES_ENUM_RET_STS_BUSY == sg_st_ees_request.e_status)
            {
                /* It is possible to write the program for detecting timeout here as necessity requires */
                R_EES_Handler();
            }
            
            l_e_ees_ret_value = sg_st_ees_request.e_status;
            
            /* Check returned status of the command */
            if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
            {
                /* WRITE command was finished successfully */
                /* No operation */
                exec_flag = true;                
                EES_DBG_MSG_OK("WRITE",l_e_ees_ret_value);
            }
            else if (R_EES_ENUM_RET_ERR_POOL_FULL == l_e_ees_ret_value)
            {
                /******************************************************************************************************
                 * REFRESH
                 *****************************************************************************************************/
                /* Set parameters for REFRESH command */
                sg_st_ees_request.e_command = R_EES_ENUM_CMD_REFRESH;
                
                /* Execute REFRESH command */
                R_EES_Execute(&sg_st_ees_request);
                
                /* Wait for the end of running command */
                while (R_EES_ENUM_RET_STS_BUSY == sg_st_ees_request.e_status)
                {
                    /* It is possible to write the program for detecting timeout here as necessity requires */
                    R_EES_Handler();
                }
                
                l_e_ees_ret_value = sg_st_ees_request.e_status;
                
                /* Check returned status of the command */
                if (R_EES_ENUM_RET_STS_OK == l_e_ees_ret_value)
                {
                    /* REFRESH command was finished successfully */
                    EES_DBG_MSG_OK("REFRESH",l_e_ees_ret_value);
                    /* No operation */
                }
                else
                {
                    /* REFRESH error handling */
                    l_b_sam_error_flag = true;
                    EES_DBG_MSG_FAIL("REFRESH",l_e_ees_ret_value);
                }
            }
            else
            {
                /* WRITE error handling */
                l_b_sam_error_flag = true;
                EES_DBG_MSG_FAIL("WRITE",l_e_ees_ret_value);
            }
        } while ((R_EES_ENUM_CMD_REFRESH == sg_st_ees_request.e_command)
                 && (true != l_b_sam_error_flag));
    }
    else
    {
        /* No operation */
    }

    __EES_close();
    
    EI();

    return exec_flag;
}


#define  SAMPLE_END_SECTION_SMP_EES
#include "sample_ees_memmap.h"

bool EES_TimeoutHandler(uint8_t ees_id)
{
    uint32_t timeout = 0xFFFF;
    while(timeout--);
    printf("%s:TIMEOUT error(ID:%d)\r\n",__func__ ,ees_id);

    return true;
}

void EES_ProcessHandler(uint8_t flag,uint8_t ees_id)
{
    if (flag)
    {   
        // printf("%s finish(ID:%d)\r\n",__func__,ees_id);
    }
    else
    {
        printf("%s err(ID:%d)\r\n",__func__,ees_id);
    }
}

void EES_ReadWriteTest(void)
{
    uint8_t ees_id = 0;
    uint8_t i = 0;
    uint16_t l_u16_count = 0;
    uint16_t l_u16_len = 0;

    /*----------------------------------------*/
    if (FLAG_PROJ_TRIG_1)   // set initial value to each ID
    {
        FLAG_PROJ_TRIG_1 = 0;

        printf("ESS set initial value start\r\n");

        /*----------------------------------------*/
        // fill 0 ~ 255
        ees_process_flag = false;
        ees_id = EES_ID001;    
        l_u16_len = sizeof(type_ID001);
        for (l_u16_count = 0u; l_u16_count < l_u16_len; l_u16_count++)
        {
            sg_ar_u08_write_data[l_u16_count] = l_u16_count;
        }
        ees_process_flag = __EES_write_data(ees_id);
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        /*----------------------------------------*/
        // fill 0 ~ 255
        ees_process_flag = false;
        ees_id = EES_ID002;    
        l_u16_len = sizeof(type_ID002);
        for (l_u16_count = 0u; l_u16_count < l_u16_len; l_u16_count++)
        {
            sg_ar_u08_write_data[l_u16_count] = l_u16_count;
        }
        ees_process_flag = __EES_write_data(ees_id);
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        /*----------------------------------------*/
        // fill 0 ~ 255
        ees_process_flag = false;
        ees_id = EES_ID003;    
        l_u16_len = sizeof(type_ID003);
        for (l_u16_count = 0u; l_u16_count < l_u16_len; l_u16_count++)
        {
            sg_ar_u08_write_data[l_u16_count] = l_u16_count;
        }
        ees_process_flag = __EES_write_data(ees_id);
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        /*----------------------------------------*/
        // fill 4 ~ 4+64-1
        ees_process_flag = false;
        ees_id = EES_ID004;    
        l_u16_len = sizeof(type_ID004);
        for (l_u16_count = 0u; l_u16_count < l_u16_len; l_u16_count++)
        {
            sg_ar_u08_write_data[l_u16_count] = l_u16_count + ees_id;
        }
        ees_process_flag = __EES_write_data(ees_id);
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        /*----------------------------------------*/
        // fill 5 ~ 5+20-1
        ees_process_flag = false;
        ees_id = EES_ID005;    
        l_u16_len = sizeof(type_ID005);
        for (l_u16_count = 0u; l_u16_count < l_u16_len; l_u16_count++)
        {
            sg_ar_u08_write_data[l_u16_count] = l_u16_count + ees_id;
        }
        ees_process_flag = __EES_write_data(ees_id);
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        /*----------------------------------------*/
        // fill 6 ~ 6+8-1
        ees_process_flag = false;
        ees_id = EES_ID006;    
        l_u16_len = sizeof(type_ID006);
        for (l_u16_count = 0u; l_u16_count < l_u16_len; l_u16_count++)
        {
            sg_ar_u08_write_data[l_u16_count] = l_u16_count + ees_id;
        }
        ees_process_flag = __EES_write_data(ees_id);
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        /*----------------------------------------*/
        // fill 7 ~ 7+2-1
        ees_process_flag = false;
        ees_id = EES_ID007;    
        l_u16_len = sizeof(type_ID007);
        for (l_u16_count = 0u; l_u16_count < l_u16_len; l_u16_count++)
        {
            sg_ar_u08_write_data[l_u16_count] = l_u16_count + ees_id;
        }
        ees_process_flag = __EES_write_data(ees_id);
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        /*----------------------------------------*/
        for (i = EES_ID008; i < EES_ID134 + 1; i++)
        {
            ees_process_flag = false;
            ees_id = i;
            sg_ar_u08_write_data[0] = i;
            ees_process_flag = __EES_write_data(ees_id);
            while(!ees_process_flag)
            {        
                ees_process_flag = EES_TimeoutHandler(ees_id);
            }
            EES_ProcessHandler(ees_process_flag,ees_id);
        }

        printf("ESS set initial value end\r\n");

    }

    if (FLAG_PROJ_TRIG_2)   // read data from each ID
    {
        FLAG_PROJ_TRIG_2 = 0;
        
        printf("ESS read value start\r\n");

        /*----------------------------------------*/
        ees_process_flag = false;
        ees_id = EES_ID001;    
        l_u16_len = sizeof(type_ID001);
        ees_process_flag = __EES_read_data(ees_id);    
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        if (ees_process_flag)
        {   
            printf("%s(ID:%d,len:%d)\r\n",__func__ ,ees_id,l_u16_len);
            dump_buffer((unsigned char *)sg_ar_u08_read_data,l_u16_len);
        }

        /*----------------------------------------*/
        ees_process_flag = false;
        ees_id = EES_ID002;    
        l_u16_len = sizeof(type_ID002);
        ees_process_flag = __EES_read_data(ees_id);    
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        if (ees_process_flag)
        {   
            printf("%s(ID:%d,len:%d)\r\n",__func__ ,ees_id,l_u16_len);
            dump_buffer((unsigned char *)sg_ar_u08_read_data,l_u16_len);
        }

        /*----------------------------------------*/
        ees_process_flag = false;
        ees_id = EES_ID003;    
        l_u16_len = sizeof(type_ID003);
        ees_process_flag = __EES_read_data(ees_id);    
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        if (ees_process_flag)
        {   
            printf("%s(ID:%d,len:%d)\r\n",__func__ ,ees_id,l_u16_len);
            dump_buffer((unsigned char *)sg_ar_u08_read_data,l_u16_len);
        }

        /*----------------------------------------*/
        ees_process_flag = false;
        ees_id = EES_ID004;    
        l_u16_len = sizeof(type_ID004);
        ees_process_flag = __EES_read_data(ees_id);    
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        if (ees_process_flag)
        {   
            printf("%s(ID:%d,len:%d)\r\n",__func__ ,ees_id,l_u16_len);
            dump_buffer((unsigned char *)sg_ar_u08_read_data,l_u16_len);
        }

        /*----------------------------------------*/
        ees_process_flag = false;
        ees_id = EES_ID005;    
        l_u16_len = sizeof(type_ID005);
        ees_process_flag = __EES_read_data(ees_id);    
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        if (ees_process_flag)
        {   
            printf("%s(ID:%d,len:%d)\r\n",__func__ ,ees_id,l_u16_len);
            dump_buffer((unsigned char *)sg_ar_u08_read_data,l_u16_len);
        }

        /*----------------------------------------*/
        ees_process_flag = false;
        ees_id = EES_ID006;    
        l_u16_len = sizeof(type_ID006);
        ees_process_flag = __EES_read_data(ees_id);    
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        if (ees_process_flag)
        {   
            printf("%s(ID:%d,len:%d)\r\n",__func__ ,ees_id,l_u16_len);
            dump_buffer((unsigned char *)sg_ar_u08_read_data,l_u16_len);
        }

        /*----------------------------------------*/
        ees_process_flag = false;
        ees_id = EES_ID007;    
        l_u16_len = sizeof(type_ID007);
        ees_process_flag = __EES_read_data(ees_id);    
        while(!ees_process_flag)
        {        
            ees_process_flag = EES_TimeoutHandler(ees_id);
        }
        EES_ProcessHandler(ees_process_flag,ees_id);

        if (ees_process_flag)
        {   
            printf("%s(ID:%d,len:%d)\r\n",__func__ ,ees_id,l_u16_len);
            dump_buffer((unsigned char *)sg_ar_u08_read_data,l_u16_len);
        }

        /*----------------------------------------*/
            
        for (i = EES_ID008; i < EES_ID134 + 1; i++)
        {
            ees_process_flag = false;
            ees_id = i;    
            l_u16_len = 1;
            ees_process_flag = __EES_read_data(ees_id);    
            while(!ees_process_flag)
            {        
                ees_process_flag = EES_TimeoutHandler(ees_id);
            }
            EES_ProcessHandler(ees_process_flag,ees_id);

            if (ees_process_flag)
            {   
                printf("%s(ID:%d,len:%d)\r\n",__func__ ,ees_id,l_u16_len);
                dump_buffer((unsigned char *)sg_ar_u08_read_data,l_u16_len);
            }
        }

        printf("ESS read value end\r\n");
    }

}


void EES_InitialValueTest(void)
{
    uint8_t ees_id = 0;

    /*----------------------------------------*/
    ees_process_flag = false;
    ees_id = EES_ID134;
    sg_ar_u08_write_data[0] = 0x5A;
    ees_process_flag = __EES_write_data(ees_id);
    while(!ees_process_flag)
    {        
        ees_process_flag = EES_TimeoutHandler(ees_id);
    }
    EES_ProcessHandler(ees_process_flag,ees_id);

    /*----------------------------------------*/
    ees_process_flag = false;
    ees_id = EES_ID007;
    sg_ar_u08_write_data[0] = 0x11;
    sg_ar_u08_write_data[1] = 0x22;
    ees_process_flag = __EES_write_data(ees_id);
    while(!ees_process_flag)
    {        
        ees_process_flag = EES_TimeoutHandler(ees_id);
    }
    EES_ProcessHandler(ees_process_flag,ees_id);

}


void EES_ReloadTest(void)
{
    uint8_t ees_id = 0;
    uint8_t tmp = 0;
    uint16_t l_u16_count = 0;
    uint16_t l_u16_len = 0;

    /*----------------------------------------*/
    ees_process_flag = false;
    ees_id = EES_ID007;
    l_u16_len = sizeof(type_ID007);
    ees_process_flag = __EES_read_data(ees_id);    
    while(!ees_process_flag)
    {        
        ees_process_flag = EES_TimeoutHandler(ees_id);
    }
    EES_ProcessHandler(ees_process_flag,ees_id);

    if (ees_process_flag)
    {   
        printf("%s(ID:%d,len:%d)\r\n",__func__ ,ees_id,l_u16_len);
        dump_buffer((unsigned char *)sg_ar_u08_read_data,l_u16_len);
    }


    /*----------------------------------------*/
    ees_process_flag = false;
    ees_id = EES_ID134;
    l_u16_len = sizeof(type_ID134);
    ees_process_flag = __EES_read_data(ees_id);    
    while(!ees_process_flag)
    {        
        ees_process_flag = EES_TimeoutHandler(ees_id);
    }
    EES_ProcessHandler(ees_process_flag,ees_id);

    if (ees_process_flag)
    {   
        tmp = sg_ar_u08_read_data[0];
        if (tmp != 0x5A)     
        {
            sg_ar_u08_write_data[0] = 0x5A;
            __EES_write_data(ees_id);
        }
        else
        {
            printf("%s(ID:%d,len:%d)\r\n",__func__ ,ees_id,l_u16_len);
            dump_buffer((unsigned char *)sg_ar_u08_read_data,l_u16_len);
        }
    }    


    /*----------------------------------------*/
    ees_process_flag = false;
    ees_id = EES_ID005;    
    l_u16_len = sizeof(type_ID005);
    for (l_u16_count = 0u; l_u16_count < l_u16_len; l_u16_count++)
    {
        sg_ar_u08_write_data[l_u16_count] = l_u16_count;
    }
    sg_ar_u08_write_data[0] = 0x5A;
    sg_ar_u08_write_data[1] = 0x5A;
    sg_ar_u08_write_data[l_u16_len-2] = 0xA5;
    sg_ar_u08_write_data[l_u16_len-1] = 0xA5;
    ees_process_flag = __EES_write_data(ees_id);    
    while(!ees_process_flag)
    {        
        ees_process_flag = EES_TimeoutHandler(ees_id);
    }
    EES_ProcessHandler(ees_process_flag,ees_id);

     /*----------------------------------------*/
    ees_process_flag = false;
    ees_id = EES_ID005;    
    l_u16_len = sizeof(type_ID005);
    ees_process_flag = __EES_read_data(ees_id);    
    while(!ees_process_flag)
    {        
        ees_process_flag = EES_TimeoutHandler(ees_id);
    }
    EES_ProcessHandler(ees_process_flag,ees_id);

    if (ees_process_flag)
    {   
        printf("%s(ID:%d,len:%d)\r\n",__func__ ,ees_id,l_u16_len);
        dump_buffer((unsigned char *)sg_ar_u08_read_data,l_u16_len);
    }

    /*----------------------------------------*/
    ees_process_flag = false;
    ees_id = EES_ID005;    
    l_u16_len = sizeof(type_ID005);
    for (l_u16_count = 0u; l_u16_count < l_u16_len; l_u16_count++)
    {
        sg_ar_u08_write_data[l_u16_count] = l_u16_count + 0x10;
    }
    sg_ar_u08_write_data[0] = 0x5A;
    sg_ar_u08_write_data[1] = 0x5A;
    sg_ar_u08_write_data[l_u16_len-2] = 0xA5;
    sg_ar_u08_write_data[l_u16_len-1] = 0xA5;
    ees_process_flag = __EES_write_data(ees_id);    
    while(!ees_process_flag)
    {        
        ees_process_flag = EES_TimeoutHandler(ees_id);
    }
    EES_ProcessHandler(ees_process_flag,ees_id);

     /*----------------------------------------*/
    ees_process_flag = false;
    ees_id = EES_ID005;    
    l_u16_len = sizeof(type_ID005);
    ees_process_flag = __EES_read_data(ees_id);    
    while(!ees_process_flag)
    {        
        ees_process_flag = EES_TimeoutHandler(ees_id);
    }
    EES_ProcessHandler(ees_process_flag,ees_id);

    if (ees_process_flag)
    {   
        printf("%s(ID:%d,len:%d)\r\n",__func__ ,ees_id,l_u16_len);
        dump_buffer((unsigned char *)sg_ar_u08_read_data,l_u16_len);
    }


}


void Timer_1ms_IRQ(void)
{
    tick_counter();

    if ((get_tick() % 1000) == 0)
    {
        FLAG_PROJ_TIMER_PERIOD_1000MS = 1;
    }

    if ((get_tick() % 50) == 0)
    {

    }	

    Button_Process_long_counter();
}


/*
    F24 target board
    LED1 connected to P66, LED2 connected to P67
*/
void LED_Toggle(void)
{
    // PIN_WRITE(6,6) = ~PIN_READ(6,6);
    // PIN_WRITE(6,7) = ~PIN_READ(6,7);
    P6_bit.no6 = ~P6_bit.no6;
    P6_bit.no7 = ~P6_bit.no7;
}

void loop(void)
{
	// static unsigned long LOG1 = 0;

    if (FLAG_PROJ_TIMER_PERIOD_1000MS)
    {
        FLAG_PROJ_TIMER_PERIOD_1000MS = 0;

        // printf("log(timer):%4d\r\n",LOG1++);
        LED_Toggle();             
    }

    Button_Process_in_polling();

    EES_ReadWriteTest();
}


// F24 EVB , P137/INTP0 , set both edge 
void Button_Process_long_counter(void)
{
    if (FLAG_PROJ_TRIG_BTN2)
    {
        btn_tick_counter();
    }
    else
    {
        btn_set_tick(0);
    }
}

void Button_Process_in_polling(void)
{
    static unsigned char cnt = 0;

    if (FLAG_PROJ_TRIG_BTN1)
    {
        FLAG_PROJ_TRIG_BTN1 = 0;
        printf("BTN pressed(%d)\r\n",cnt);

        if (cnt == 0)   //set both edge  , BTN pressed
        {
            FLAG_PROJ_TRIG_BTN2 = 1;
        }
        else if (cnt == 1)  //set both edge  , BTN released
        {
            FLAG_PROJ_TRIG_BTN2 = 0;
        }

        cnt = (cnt >= 1) ? (0) : (cnt+1) ;
    }

    if ((FLAG_PROJ_TRIG_BTN2 == 1) && 
        (btn_get_tick() > BTN_PRESSED_LONG))
    {         
        printf("BTN pressed LONG\r\n");
        btn_set_tick(0);
        FLAG_PROJ_TRIG_BTN2 = 0;
    }
}

// F24 EVB , P137/INTP0
void Button_Process_in_IRQ(void)    
{
    FLAG_PROJ_TRIG_BTN1 = 1;
}

void UARTx_Process(unsigned char rxbuf)
{    
    if (rxbuf > 0x7F)
    {
        printf("invalid command\r\n");
    }
    else
    {
        printf("press:%c(0x%02X)\r\n" , rxbuf,rxbuf);   // %c :  C99 libraries.
        switch(rxbuf)
        {
            case '1':
                FLAG_PROJ_TRIG_1 = 1;
                break;
            case '2':
                FLAG_PROJ_TRIG_2 = 1;
                break;
            case '3':
                FLAG_PROJ_TRIG_3 = 1;
                break;
            case '4':
                FLAG_PROJ_TRIG_4 = 1;
                break;
            case '5':
                FLAG_PROJ_TRIG_5 = 1;
                break;

            case 'X':
            case 'x':
                RL78_soft_reset(7);
                break;
            case 'Z':
            case 'z':
                RL78_soft_reset(1);
                break;
        }
    }
}

/*
    Reset Control Flag Register (RESF) 
    BIT7 : TRAP
    BIT6 : 0
    BIT5 : 0
    BIT4 : WDCLRF
    BIT3 : 0
    BIT2 : 0
    BIT1 : IAWRF
    BIT0 : LVIRF
*/
void check_reset_source(void)
{
    /*
        Internal reset request by execution of illegal instruction
        0  Internal reset request is not generated, or the RESF register is cleared. 
        1  Internal reset request is generated. 
    */
    uint8_t src = RESF;
    printf("Reset Source <0x%08X>\r\n", src);

    #if 1   //DEBUG , list reset source
    if (src & BIT0)
    {
        printf("0)voltage detector (LVD)\r\n");       
    }
    if (src & BIT1)
    {
        printf("1)illegal-memory access\r\n");       
    }
    if (src & BIT2)
    {
        printf("2)EMPTY\r\n");       
    }
    if (src & BIT3)
    {
        printf("3)EMPTY\r\n");       
    }
    if (src & BIT4)
    {
        printf("4)watchdog timer (WDT) or clock monitor\r\n");       
    }
    if (src & BIT5)
    {
        printf("5)EMPTY\r\n");       
    }
    if (src & BIT6)
    {
        printf("6)EMPTY\r\n");       
    }
    if (src & BIT7)
    {
        printf("7)execution of illegal instruction\r\n");       
    }
    #endif

}

/*
    7:Internal reset by execution of illegal instruction
    1:Internal reset by illegal-memory access
*/
//perform sofware reset
void _reset_by_illegal_instruction(void)
{
    static const unsigned char illegal_Instruction = 0xFF;
    void (*dummy) (void) = (void (*)(void))&illegal_Instruction;
    dummy();
}
void _reset_by_illegal_memory_access(void)
{
    #if 1
    const unsigned char ILLEGAL_ACCESS_ON = 0x80;
    IAWCTL |= ILLEGAL_ACCESS_ON;            // switch IAWEN on (default off)
    *(__far volatile char *)0x00000 = 0x00; //write illegal address 0x00000(RESET VECTOR)
    #else
    signed char __far* a;                   // Create a far-Pointer
    IAWCTL |= _80_CGC_ILLEGAL_ACCESS_ON;    // switch IAWEN on (default off)
    a = (signed char __far*) 0x0000;        // Point to 0x000000 (FLASH-ROM area)
    *a = 0;
    #endif
}

void RL78_soft_reset(unsigned char flag)
{
    switch(flag)
    {
        case 7: // do not use under debug mode
            _reset_by_illegal_instruction();        
            break;
        case 1:
            _reset_by_illegal_memory_access();
            break;
    }
}

// retarget printf
int __far putchar(int c)
{
    // F24 , UART0
    STMK0 = 1U;    /* disable INTST0 interrupt */
    SDR00L = (unsigned char)c;
    while(STIF0 == 0)
    {

    }
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    return c;
}

void hardware_init(void)
{
    // const unsigned char indicator[] = "hardware_init";
    BSP_EI();
    R_Config_UART0_Start();         // UART , P15 , P16
    R_Config_TAU0_1_Start();        // TIMER
    R_Config_INTC_INTP0_Start();    // BUTTON , P137 
    
    __EES_init();
    EES_InitialValueTest();
    EES_ReloadTest();

    // check_reset_source();
    printf("%s finish\r\n\r\n",__func__);
}
