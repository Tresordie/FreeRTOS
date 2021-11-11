#include "usb.h"

//Config used GPIO on D12
static void D12_DATAPort_Config(void)
{
    GPIOx_Configuration_Init(D12_DATA_PORT, D12_DATA_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
}

static void D12_DataCommandSelect_Config(void)
{
    GPIOx_Configuration_Init(D12_A0_PORT, D12_A0_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
}

static void D12_DataWriteIO_Config(void)
{
    GPIOx_Configuration_Init(D12_WR_PORT, D12_WR_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
}

static void D12_DataReadIO_Config(void)
{
    GPIOx_Configuration_Init(D12_RD_PORT, D12_RD_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
}

static void D12_InterruptIO_Config(void)
{
    GPIOx_Configuration_Init(D12_INT_PORT, D12_INT_PIN, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
}

static void D12_BoardLedsIO_Config(void)
{
    GPIOx_Configuration_Init(LEDGP1_PORT, LED1_PIN|LED2_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
    GPIOx_Configuration_Init(LEDGP2_PORT, LED3_PIN|LED4_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
}

static void D12_BoardKeyINT_Config(void)
{
    bsp_BothEdge_EXTI_Init(KEY1_PORT, KEY1_PIN, KEY1_GPIO_PinSource, KEY1_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY1_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY2_PORT, KEY2_PIN, KEY2_GPIO_PinSource, KEY2_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY2_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY3_PORT, KEY3_PIN, KEY3_GPIO_PinSource, KEY3_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY3_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY4_PORT, KEY4_PIN, KEY4_GPIO_PinSource, KEY4_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY4_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY5_PORT, KEY5_PIN, KEY5_GPIO_PinSource, KEY5_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY5_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY6_PORT, KEY6_PIN, KEY6_GPIO_PinSource, KEY6_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY6_EXTI_IRQn, 2, 0);
    
    bsp_BothEdge_EXTI_Init(KEY7_PORT, KEY7_PIN, KEY7_GPIO_PinSource, KEY7_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY7_EXTI_IRQn, 2, 0);
    
    /*
    bsp_BothEdge_EXTI_Init(KEY8_PORT, KEY8_PIN, KEY8_GPIO_PinSource, KEY8_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY8_EXTI_IRQn, 2, 0);
    */
    
    GPIOx_Configuration_Init(KEY8_PORT, KEY8_PIN, GPIO_Mode_IPU, GPIO_Speed_50MHz);
}

void D12GPIOConfiguration(void)
{
    // 改变指定管脚的映射 GPIO_Remap_SWJ_JTAGDisable ，JTAG-DP 禁用 + SW-DP 使能
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    D12_DATAPort_Config();
    D12_DataCommandSelect_Config();
    D12_DataWriteIO_Config();
    D12_DataReadIO_Config();
    D12_InterruptIO_Config();
    D12_BoardLedsIO_Config();
    D12_BoardKeyINT_Config();
}


static void LEDs_ON(void)
{
    LED1_ON;
    LED2_ON;
    LED3_ON;
    LED4_ON;
}

static void LEDs_OFF(void)
{
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    LED4_OFF;
}


void LEDsControl(LED_NUM_t LED_NUM, LED_STATE_t LED_STAT)
{
    if(LED_STAT)
    {
        switch(LED_NUM)
        {
            case LED1:
                LED1_ON;
                break;

            case LED2:
                LED2_ON;
                break;

            case LED3:
                LED3_ON;
                break;

            case LED4:
                LED4_ON;
                break;

            case LED_ALL:
                LEDs_ON();
                break;

            default:
                break;
        }
    }
    else
    {
        switch(LED_NUM)
        {
            case LED1:
                LED1_OFF;
                break;

            case LED2:
                LED2_OFF;
                break;

            case LED3:
                LED3_OFF;
                break;

            case LED4:
                LED4_OFF;
                break;

            case LED_ALL:
                LEDs_OFF();
                break;

            default:
                break;
        }
    }
}


//Set Data/Command
void D12SetData(uint8_t data_command)
{
    GPIO_Write(D12_DATA_PORT, data_command);
}


//Data port data dir set up
void D12SetPortOut(void)
{
    GPIOx_Configuration_Init(D12_DATA_PORT, D12_DATA_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
}

void D12SetPortIn(void)
{
    GPIOx_Configuration_Init(D12_DATA_PORT, D12_DATA_PIN, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
}


//Commnad write
void D12WriteCommand(uint8_t Command)
{
    D12SetPortOut();

    D12SetCommandAddr();
    D12ClearWR();

    D12SetData(Command);
    D12SetWR();
}


//Data write
void D12WriteData(uint8_t Data)
{
    D12SetPortOut();

    //这里将是从D12WriteCommand复制过来的，结果将D12SetCommandAddr()赋值过来
    //没有检查，调试两天才找出来
    D12SetDataAddr();
    D12ClearWR();

    D12SetData(Data);
    D12SetWR();
}


//Data read
unsigned char D12ReadData(void)
{
    unsigned char temp_data = 0;

    D12SetPortIn();

    D12SetDataAddr();
    D12ClearRD();

    temp_data = GPIO_ReadInputData(D12_DATA_PORT);
    D12SetRD();

    return (temp_data);
}


//Read ID
unsigned int D12ReadID(void)
{
    uint16_t temp_dat;
    D12WriteCommand(D12_REGISTER_READID);
    temp_dat = D12ReadData();
    temp_dat |= D12ReadData() << 8;

    return temp_dat;
}


//Simulate USB disconnect & connect
void UsbDisconnect(void)
{
    printf("断开USB连接.");

    D12WriteCommand(D12_REGISTER_SETMODE);
    D12WriteData(D12_SETMODE_USBDISCONNECT);
    D12WriteData(D12_SETMODE_BITALLOCATION_SETMODE);
    bsp_sw_delay_ms(1000);
    
}


void UsbConnect(void)
{
    printf("USB连接.");

    D12WriteCommand(D12_REGISTER_SETMODE);
    D12WriteData(D12_SETMODE_USBCONNECT);
    D12WriteData(D12_SETMODE_BITALLOCATION_SETMODE);
    bsp_sw_delay_ms(1000);
}


// Read D12 interrupt register
unsigned char D12_ReadInterrupt_Register(void)
{
    uint8_t temp_data = 0;

    D12WriteCommand(D12_REGISTER_READ_INTERRUPT);
    temp_data = D12ReadData();

    return (temp_data);
}



