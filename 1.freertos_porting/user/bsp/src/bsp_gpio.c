#include "bsp_gpio.h"


/**
  * @brief  Configures the different system clocks.
  * @param  GPIOA - GPIOG
  * @retval None
  */
void bsp_InitRCC(GPIO_TypeDef* GPIOx)
{
    if(GPIOx == GPIOA)
    {   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   }

    else if(GPIOx == GPIOB)
    {   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   }

    else if(GPIOx == GPIOC)
    {   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   }

    else if(GPIOx == GPIOD)
    {   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);   }

    else if(GPIOx == GPIOE)
    {   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);   }

    else if(GPIOx == GPIOF)
    {   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);   }

    else if(GPIOx == GPIOG)
    {   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);   }
}


/**
  * @brief  Config GPIOx Pin Mode
  * @param  GPIOA - GPIOG, GPIO_Pin_x decided by MCU
  * @retval None
  */
void bsp_InitGPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    bsp_InitRCC(GPIOx);

    if(GPIOx == GPIOA)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    else if(GPIOx == GPIOB)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    else if(GPIOx == GPIOC)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    else if(GPIOx == GPIOD)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
    else if(GPIOx == GPIOE)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
        GPIO_Init(GPIOE, &GPIO_InitStructure);
    }
    else if(GPIOx == GPIOF)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
        GPIO_Init(GPIOF, &GPIO_InitStructure);
    }
    else if(GPIOx == GPIOG)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
        GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
}



/**
  * @brief  Set GPIO Pin
  * @param  None
  * @retval None
  */

void bsp_setIO_OutputHigh(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{
    GPIO_SetBits(GPIOx, GPIO_Pin_x);
}


/**
  * @brief  Reset GPIO Pin
  * @param  None
  * @retval None
  */

void bsp_setIO_OutputLow(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{
    GPIO_ResetBits(GPIOx, GPIO_Pin_x);
}


/**
  * @brief  Disable JTAG and Enable SWD
  * @param  None
  * @retval None
  */
void JTAG_GPIO_DISABLE(void)
{
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}



