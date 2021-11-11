#include "bsp_sw_delay.h"


// 66 NOP
void bsp_sw_delay_us(uint32_t nTimer)
{
    uint32_t i=0;

    for(i = 0; i < nTimer; i++)
    {
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();

        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();

        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();

        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();

        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();

    }
}


void bsp_sw_delay_ms(uint32_t nTimer)
{
    uint32_t i = 1000 * nTimer;
    bsp_sw_delay_us(i);
}


