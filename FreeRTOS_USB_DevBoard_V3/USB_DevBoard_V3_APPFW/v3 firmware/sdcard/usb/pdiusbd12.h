#ifndef __PDIUSBD12_H__
#define __PDIUSBD12_H__

#include "stm32f10x.h"
#include "bsp.h"

/**
 * D12 Register list 
 */

// D12 ID register
#define D12_REGISTER_READID                         0xFD

// D12 set mode register(USB_DP pull up resistor soft connect control)
#define D12_REGISTER_SETMODE                        0xF3
#define D12_SETMODE_USBDISCONNECT                   0x06                 //Usb disconnect
#define D12_SETMODE_USBCONNECT                      0x16                 //Usb connect
#define D12_SETMODE_BITALLOCATION_SETMODE           0x47

// D12 read interrupt register
#define D12_REGISTER_READ_INTERRUPT                 0xF4

// D12 read interrupt register
#define D12_REGISTER_Ep0_OUT_Select                 0x00
#define D12_REGISTER_Ep0_IN_Select                  0x01
#define D12_REGISTER_Ep1_OUT_Select                 0x02
#define D12_REGISTER_Ep1_IN_Select                  0x03
#define D12_REGISTER_Ep2_OUT_Select                 0x04
#define D12_REGISTER_Ep2_IN_Select                  0x05

// D12 read buffer register
#define D12_REGISTER_ReadBuffer                     0xF0
#define D12_REGISTER_WriteBuffer                    0xF0

// D12 last transaction status register
#define D12_REGISTER_Ep0_OUT_LastTransactionStatus  0x40
#define D12_REGISTER_Ep0_IN_LastTransactionStatus   0x41
#define D12_REGISTER_Ep1_OUT_LastTransactionStatus  0x42
#define D12_REGISTER_Ep1_IN_LastTransactionStatus   0x43
#define D12_REGISTER_Ep2_OUT_LastTransactionStatus  0x44
#define D12_REGISTER_Ep2_IN_LastTransactionStatus   0x45

// D12 Endpoints
#define Ep0_OUT                                     0x00
#define Ep0_IN                                      0x01
#define Ep1_OUT                                     0x02
#define Ep1_IN                                      0x03
#define Ep2_OUT                                     0x04
#define Ep2_IN                                      0x05

// D12 Acknowledge setup register
#define D12_REGISTER_AcknowledgeSetup               0xF1

// D12 Clear Buffer register and Validate Buffer register
#define D12_REGISTER_ClearBuffer                    0xF2
#define D12_REGISTER_ValidateBuffer                 0xFA

// D12 set address register
#define D12_REGISTER_SetAddress                     0xD0
#define D12_REGISTER_SetEndpointEnable              0xD8

// Command and Data address
#define D12_COMMAND_ADD        1
#define D12_DATA_ADD           0

#define D12_DATA_PORT          GPIOE
#define D12_DATA_PIN           0xFF

#define D12_A0_PORT            GPIOB
#define D12_A0_PIN             GPIO_Pin_1

#define D12_WR_PORT            GPIOB
#define D12_WR_PIN             GPIO_Pin_9

#define D12_RD_PORT            GPIOB
#define D12_RD_PIN             GPIO_Pin_8

#define D12_INT_PORT           GPIOB
#define D12_INT_PIN            GPIO_Pin_0


//LED port
#define LEDGP1_PORT            GPIOB
#define LED1_PIN               GPIO_Pin_11
#define LED2_PIN               GPIO_Pin_10

#define LEDGP2_PORT            GPIOC
#define LED3_PIN               GPIO_Pin_5
#define LED4_PIN               GPIO_Pin_4

// LED class
typedef enum
{
    LED1 = 1,
    LED2,
    LED3,
    LED4,
    LED_ALL
}LED_NUM_t;

typedef enum
{
    LED_OFF,
    LED_ON
}LED_STATE_t;

typedef struct
{
    LED_NUM_t LED_Num;
    LED_STATE_t LED_STAT;
}LED_PROP_t;


//Key board
#define KEY1_PORT              GPIOD
#define KEY1_PIN               GPIO_Pin_8
#define KEY1_GPIO_PinSource    GPIO_PinSource8
#define KEY1_EXTI_Line         EXTI_Line8
#define KEY1_EXTI_IRQn         EXTI9_5_IRQn

#define KEY2_PORT              GPIOD
#define KEY2_PIN               GPIO_Pin_10
#define KEY2_GPIO_PinSource    GPIO_PinSource10
#define KEY2_EXTI_Line         EXTI_Line10
#define KEY2_EXTI_IRQn         EXTI15_10_IRQn

#define KEY3_PORT              GPIOD
#define KEY3_PIN               GPIO_Pin_12
#define KEY3_GPIO_PinSource    GPIO_PinSource12
#define KEY3_EXTI_Line         EXTI_Line12
#define KEY3_EXTI_IRQn         EXTI15_10_IRQn

#define KEY4_PORT              GPIOC
#define KEY4_PIN               GPIO_Pin_6
#define KEY4_GPIO_PinSource    GPIO_PinSource6
#define KEY4_EXTI_Line         EXTI_Line6
#define KEY4_EXTI_IRQn         EXTI9_5_IRQn


#define KEY5_PORT              GPIOD
#define KEY5_PIN               GPIO_Pin_9
#define KEY5_GPIO_PinSource    GPIO_PinSource9
#define KEY5_EXTI_Line         EXTI_Line9
#define KEY5_EXTI_IRQn         EXTI9_5_IRQn

#define KEY6_PORT              GPIOD
#define KEY6_PIN               GPIO_Pin_11
#define KEY6_GPIO_PinSource    GPIO_PinSource11
#define KEY6_EXTI_Line         EXTI_Line11
#define KEY6_EXTI_IRQn         EXTI15_10_IRQn

#define KEY7_PORT              GPIOD
#define KEY7_PIN               GPIO_Pin_15
#define KEY7_GPIO_PinSource    GPIO_PinSource15
#define KEY7_EXTI_Line         EXTI_Line15
#define KEY7_EXTI_IRQn         EXTI15_10_IRQn

#define KEY8_PORT              GPIOC
#define KEY8_PIN               GPIO_Pin_7
#define KEY8_GPIO_PinSource    GPIO_PinSource7
#define KEY8_EXTI_Line         EXTI_Line7
#define KEY8_EXTI_IRQn         EXTI9_5_IRQn


// Key Macro
#define KEY1                   0x01                 // cursor left
#define KEY2                   0x02                 // cursor right
#define KEY3                   0x04                 // cursor up
#define KEY4                   0x08                 // cursor down
#define KEY5                   0x10                 // button left
#define KEY6                   0x20                 // button right
#define KEY7                   0x40                 // 
#define KEY8                   0x80                 // 


// macro to operate GPIO
#define D12SetCommandAddr()    GPIO_SetBits(D12_A0_PORT, D12_A0_PIN)
#define D12SetDataAddr()       GPIO_ResetBits(D12_A0_PORT, D12_A0_PIN)

// WR control
#define D12SetWR()             GPIO_SetBits(D12_WR_PORT, D12_WR_PIN)
#define D12ClearWR()           GPIO_ResetBits(D12_WR_PORT, D12_WR_PIN)

// RD control
#define D12SetRD()             GPIO_SetBits(D12_RD_PORT, D12_RD_PIN)
#define D12ClearRD()           GPIO_ResetBits(D12_RD_PORT, D12_RD_PIN)

// Get Interrupt status
#define D12GetINTPin()         GPIO_ReadInputDataBit(D12_INT_PORT, D12_INT_PIN)

// macro to operate LEDs GPIO
#define digitalHigh(p, i)      {p->BSRR = i;}       // set GPIO level high
#define digitalLow(p, i)       {p->BRR = i;}        // set GPIO level low
#define digitalToggle(p, i)    {p->ODR ^= i;}       // toggle GPIO level 

#define LED1_TOGGLE            digitalToggle(LEDGP1_PORT, LED1_PIN)
#define LED2_TOGGLE            digitalToggle(LEDGP1_PORT, LED2_PIN)
#define LED3_TOGGLE            digitalToggle(LEDGP2_PORT, LED3_PIN)
#define LED4_TOGGLE            digitalToggle(LEDGP2_PORT, LED4_PIN)


/************************************************************************
 ******************************* function declare************************
 ***********************************************************************/

// LED control
void LEDsControl(LED_NUM_t LED_NUM, LED_STATE_t LED_STAT);

// Config used GPIO on D12
void D12GPIOConfiguration(void);

// Set Data/Command
void D12SetData(uint8_t data_command);

// Data port data dir set up
void D12SetPortOut(void);
void D12SetPortIn(void);

// Commnad write
void D12WriteCommand(uint8_t Command);


// Data write
void D12WriteData(uint8_t Data);

// Data read
unsigned char D12ReadData(void);

// Read ID
unsigned int D12ReadID(void);


// Simulate USB disconnect & connect
void UsbDisconnect(void);
void UsbConnect(void);

// Read D12 interrupt register
unsigned char D12_ReadInterrupt_Register(void);

#endif

