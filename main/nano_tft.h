#pragma once
#include "driver/gpio.h"


void Nano_tft_init(void);
void Nano_tft_write_cmd(uint8_t cmd);
void Nano_tft_write_dat(uint8_t dat);

void Nano_tft_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void Nano_tft_set_bg_color(uint16_t color);

void Nano_tft_Fill_Color(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2, uint16_t *color_p);

/*
//------------726.xxx ms
// #define Nano_TFT_clk_1   gpio_set_level(_io_clk, 1)
// #define Nano_TFT_clk_0   gpio_set_level(_io_clk, 0)

// #define Nano_TFT_dat_1   gpio_set_level(_io_dat, 1)
// #define Nano_TFT_dat_0   gpio_set_level(_io_dat, 0)

// #define Nano_TFT_cs_1   gpio_set_level(_io__cs, 1)
// #define Nano_TFT_cs_0   gpio_set_level(_io__cs, 0)

// #define Nano_TFT_bk_1   gpio_set_level(_io_bkl, 1)
// #define Nano_TFT_bk_0   gpio_set_level(_io_bkl, 0)


gpio_set_level(gpio_num, 1)  =>  gpio_hal_set_level(gpio_context.gpio_hal, gpio_num, level)  =>  

#define     gpio_hal_set_level(hal, gpio_num, level)      gpio_ll_set_level((hal)->dev, gpio_num, level)

gpio_context.gpio_hal   ===    &_gpio_hal  
=>
static gpio_hal_context_t _gpio_hal = {
    .dev = GPIO_HAL_GET_HW(GPIO_PORT_0)
};

hw = GPIO_HAL_GET_HW(GPIO_PORT_0)   =  (&GPIO)

static inline void gpio_ll_set_level(gpio_dev_t *hw, gpio_num_t gpio_num, uint32_t level)
{
    if (level) {
        if (gpio_num < 32) {
            hw->out_w1ts = (1 << gpio_num);
        } else {
            hw->out1_w1ts.data = (1 << (gpio_num - 32));
        }
    } else {
        if (gpio_num < 32) {
            hw->out_w1tc = (1 << gpio_num);
        } else {
            hw->out1_w1tc.data = (1 << (gpio_num - 32));
        }
    }
}



*/





// void Nano_tft_write_cmd(uint8_t cmd)
// {
//     Nano_TFT_cs_0;
//     Nano_TFT_dat_0;
//     Nano_TFT_clk_0;
//     Nano_TFT_clk_1;
//     for(uint8_t i=0x80; i!=0; i>>=1)
//     {
//         if(i&cmd)
//             Nano_TFT_dat_1;
//         else 
//             Nano_TFT_dat_0;
//         Nano_TFT_clk_0;
//         Nano_TFT_clk_1;
//     }
//     Nano_TFT_cs_1;
// }


// void Nano_tft_write_dat(uint8_t dat)
// {
//     Nano_TFT_cs_0;
//     Nano_TFT_dat_1;
//     Nano_TFT_clk_0;
//     Nano_TFT_clk_1;
//     for(uint8_t i=0x80; i!=0; i>>=1)
//     {
//         if(i&dat)
//             Nano_TFT_dat_1;
//         else 
//             Nano_TFT_dat_0;
//         Nano_TFT_clk_0;
//         Nano_TFT_clk_1;
//     }
//     Nano_TFT_cs_1;
// }
