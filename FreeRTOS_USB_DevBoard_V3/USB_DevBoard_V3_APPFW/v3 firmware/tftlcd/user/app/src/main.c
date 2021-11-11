#include "bsp.h"
#include "main.h"
#include "usb.h"

/***
 * Function declare
 */
static void PrintfLogo(void);


int main(void)
{
    uint16_t D12_ID = 0;
    uint8_t InterruptSource = 0;

    bsp_Init();
    PrintfLogo();
  
    LCD_Init();
    Display_Company_LOGO(0, 0, gImage_1);
    
    while (1)
    {    
        LED1_TOGGLE;
        bsp_sw_delay_ms(200);
        LED2_TOGGLE;
        bsp_sw_delay_ms(200);
        LED3_TOGGLE;
        bsp_sw_delay_ms(200);
        LED4_TOGGLE;
        bsp_sw_delay_ms(200);  
    }
}


/*
*********************************************************************************************************
*   �� �� ��: PrintfLogo
*   ����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
    printf("*************************�ڽ����ӿƼ�************************\r\n\r\n");
    printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN, \
            __STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
    printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}
