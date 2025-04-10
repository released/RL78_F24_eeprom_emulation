/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>

#include "r_smc_entry.h"
#include "platform.h"

#include "r_ees_api.h"
#include "sample_control_ees.h"
#include "sample_ees_defines.h"
#include "r_ees_user_types.h"

#include "app_ees.h"


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

/*_____ D E F I N I T I O N S ______________________________________________*/

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



/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/



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

void EES_dump_buffer(unsigned char *pucBuff, int nBytes)
{
    unsigned short  i = 0;
    
    printf("dump_buffer : %2d\r\n" , nBytes);    
    for (i = 0 ; i < nBytes ; i++)
    {
        printf("0x%02X," , pucBuff[i]);
        if ((i+1)%8 ==0)
        {
            printf("\r\n");
        }            
    }
    printf("\r\n\r\n");
}

void EES_clear_buffer(volatile uint8_t *buffer, uint16_t size) 
{
    while (size--) 
	{
        *buffer++ = 0;
    }
}

void EES_copy_buffer(void *dest, void *src, unsigned long size)
{
    unsigned char *pu8Src, *pu8Dest;
    unsigned long i;
    
    pu8Dest = (unsigned char *)dest;
    pu8Src  = (unsigned char *)src;


	#if 0
	  while (size--)
	    *pu8Dest++ = *pu8Src++;
	#else
    for (i = 0; i < size; i++)
        pu8Dest[i] = pu8Src[i];
	#endif
}


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


void EES_read(uint8_t data_ID , uint8_t* buffer_rd , uint8_t len)
{
    uint8_t ees_id = 0;
    uint16_t l_u16_len = 0;	
	
    EES_clear_buffer(sg_ar_u08_read_data,sizeof(sg_ar_u08_read_data));
    ees_process_flag = false;
    ees_id = data_ID;    
    l_u16_len = len;
    ees_process_flag = __EES_read_data(ees_id);    
    while(!ees_process_flag)
    {        
        ees_process_flag = EES_TimeoutHandler(ees_id);
    }
    EES_ProcessHandler(ees_process_flag,ees_id);

    if (ees_process_flag)
    {   		
		EES_copy_buffer(buffer_rd,sg_ar_u08_read_data,l_u16_len);

        printf("%s(ID:%d,len:%d)\r\n",__func__ ,ees_id,l_u16_len);
        EES_dump_buffer((unsigned char *)sg_ar_u08_read_data,l_u16_len);
    }	
}

void EES_write(uint8_t data_ID , uint8_t* buffer_wr , uint8_t len)
{
    uint8_t ees_id = 0;

    EES_clear_buffer(sg_ar_u08_write_data,sizeof(sg_ar_u08_write_data));	
    ees_process_flag = false;
    ees_id = data_ID;    
	EES_copy_buffer(sg_ar_u08_write_data,buffer_wr,len);	
    ees_process_flag = __EES_write_data(ees_id);
    while(!ees_process_flag)
    {        
        ees_process_flag = EES_TimeoutHandler(ees_id);
    }
    EES_ProcessHandler(ees_process_flag,ees_id);
	
}



