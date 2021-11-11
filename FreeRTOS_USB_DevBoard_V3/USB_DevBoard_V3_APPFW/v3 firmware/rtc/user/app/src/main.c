#include "bsp.h"
#include "main.h"

static void PrintfLogo(void);
rtc_ds1307_datetime_t rtc_datetime;

int main(void)
{
    bsp_Init();
    PrintfLogo();  
    ds1307_init_rtc(1);
    
    while (1)
    {
        /*
        ds1307_get_rtc_datetime (&rtc_datetime);
        DEBUG("Real Time Clock:\r\n");
        DEBUG("%04d年-%02d月-%02d日 周%d %02d:%02d:%02d\r\n", (rtc_datetime.year+2000), rtc_datetime.month, rtc_datetime.date, \
                                                     rtc_datetime.day, rtc_datetime.hour, rtc_datetime.minutes, rtc_datetime.seconds);       
        */            
        LED1_TOGGLE;
        bsp_sw_delay_ms(250);
        LED2_TOGGLE;
        bsp_sw_delay_ms(250);
        LED3_TOGGLE;
        bsp_sw_delay_ms(250);
        LED4_TOGGLE;
        bsp_sw_delay_ms(250);
    }
}


/*
*********************************************************************************************************
*   函 数 名: PrintfLogo
*   功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
    printf("*************************************************************\r\n");
    printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN, \
            __STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
    printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}


