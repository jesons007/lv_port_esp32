#include "nano_tft.h"
#include "soc/gpio_struct.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define _io_bkl  27
#define _io_dat  13
#define _io_clk  14
#define _io__cs  26

//---------------180.xxx ms
#define Nano_TFT_clk_1   (&GPIO)->out_w1ts = 0x00004000
#define Nano_TFT_clk_0   (&GPIO)->out_w1tc = 0x00004000

#define Nano_TFT_dat_1   (&GPIO)->out_w1ts = 0x00002000
#define Nano_TFT_dat_0   (&GPIO)->out_w1tc = 0x00002000

#define Nano_TFT_cs_1   (&GPIO)->out_w1ts = 0x04000000
#define Nano_TFT_cs_0   (&GPIO)->out_w1tc = 0x04000000

#define Nano_TFT_bk_1   (&GPIO)->out_w1ts = (1 << _io_bkl)
#define Nano_TFT_bk_0   (&GPIO)->out_w1tc = (1 << _io_bkl)

void Nano_tft_write_cmd(uint8_t cmd)
{
    Nano_TFT_cs_0;
    Nano_TFT_dat_0;
    Nano_TFT_clk_0;
    Nano_TFT_clk_1;
    for(uint8_t i=0x80; i!=0; i>>=1)
    {
        if(i&cmd)
            Nano_TFT_dat_1;
        else 
            Nano_TFT_dat_0;
        Nano_TFT_clk_0;
        Nano_TFT_clk_1;
    }
    Nano_TFT_cs_1;
}


void Nano_tft_write_dat(uint8_t dat)
{
    Nano_TFT_cs_0;
    Nano_TFT_dat_1;
    Nano_TFT_clk_0;
    Nano_TFT_clk_1;
    for(uint8_t i=0x80; i!=0; i>>=1)
    {
        if(i&dat)
            Nano_TFT_dat_1;
        else 
            Nano_TFT_dat_0;
        Nano_TFT_clk_0;
        Nano_TFT_clk_1;
    }
    Nano_TFT_cs_1;
}

void Nano_tft_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	Nano_tft_write_cmd(0x2a); //--Set MX1_ADDR
	Nano_tft_write_dat(x1 >> 8 & 0xff);   //--Set MX1_ADDR
	Nano_tft_write_dat(x1);
	Nano_tft_write_dat(x2 >> 8 & 0xff);    //Set MX2_ADDR
	Nano_tft_write_dat(x2);
	Nano_tft_write_cmd(0x2b);  //--Set MY1_ADDR
	Nano_tft_write_dat(y1 >> 8 & 0xff);    //--Set MY1_ADDR
	Nano_tft_write_dat(y1);
	Nano_tft_write_dat(y2 >> 8 & 0xff);    //Set MY2_ADDR
	Nano_tft_write_dat(y2);
	Nano_tft_write_cmd(0x2C); //写DDRAM
}

void Nano_tft_set_bg_color(uint16_t color)
{
    int i;
    uint8_t b1 = color>>8;
    uint8_t b2 = (uint8_t)color;
    Nano_tft_set_window(0, 0, 239, 239);
    for (i = 0; i < 57600; i++)
	{
		Nano_tft_write_dat(b1);
		Nano_tft_write_dat(b2);
	}
}


void Nano_tft_init(void)
{
    gpio_pad_select_gpio(_io_bkl);
    gpio_pad_select_gpio(_io_dat);
    gpio_pad_select_gpio(_io_clk);
    gpio_pad_select_gpio(_io__cs);
    
    gpio_set_direction(_io_bkl, GPIO_MODE_OUTPUT);
    gpio_set_direction(_io_dat, GPIO_MODE_OUTPUT);
    gpio_set_direction(_io_clk, GPIO_MODE_OUTPUT);
    gpio_set_direction(_io__cs, GPIO_MODE_OUTPUT);

    gpio_set_level(_io_bkl, 1);
    gpio_set_level(_io_dat, 1);
    gpio_set_level(_io_clk, 1);
    gpio_set_level(_io__cs, 1);

    vTaskDelay(120 / portTICK_PERIOD_MS);
	Nano_tft_write_cmd(0x11);
	vTaskDelay(120 / portTICK_PERIOD_MS);

    //--------------------------------Display and color format setting-------------------

	Nano_tft_write_cmd(0x36);
	Nano_tft_write_dat(0x00);
	Nano_tft_write_cmd(0x3a);
	Nano_tft_write_dat(0x05);
//--------------------------------ST7789S Frame rate setting-------------------------

	Nano_tft_write_cmd(0xb2);
	Nano_tft_write_dat(0x0c);
	Nano_tft_write_dat(0x0c);
	Nano_tft_write_dat(0x00);
	Nano_tft_write_dat(0x33);
	Nano_tft_write_dat(0x33);
	Nano_tft_write_cmd(0xb7);
	Nano_tft_write_dat(0x35);
//---------------------------------ST7789S Power setting-----------------------------

	Nano_tft_write_cmd(0xbb);
	Nano_tft_write_dat(0x35);
	Nano_tft_write_cmd(0xc0);
	Nano_tft_write_dat(0x2c);
	Nano_tft_write_cmd(0xc2);
	Nano_tft_write_dat(0x01);
	Nano_tft_write_cmd(0xc3);
	Nano_tft_write_dat(0x13);
	Nano_tft_write_cmd(0xc4);
	Nano_tft_write_dat(0x20);
	Nano_tft_write_cmd(0xc6);
	Nano_tft_write_dat(0x0f);
	Nano_tft_write_cmd(0xca);
	Nano_tft_write_dat(0x0f);
	Nano_tft_write_cmd(0xc8);
	Nano_tft_write_dat(0x08);
	Nano_tft_write_cmd(0x55);
	Nano_tft_write_dat(0x90);
	Nano_tft_write_cmd(0xd0);
	Nano_tft_write_dat(0xa4);
	Nano_tft_write_dat(0xa1);
//--------------------------------ST7789S gamma setting------------------------------
	Nano_tft_write_cmd(0xe0);
	Nano_tft_write_dat(0xd0);
	Nano_tft_write_dat(0x00);
	Nano_tft_write_dat(0x06);
	Nano_tft_write_dat(0x09);
	Nano_tft_write_dat(0x0b);
	Nano_tft_write_dat(0x2a);
	Nano_tft_write_dat(0x3c);
	Nano_tft_write_dat(0x55);
	Nano_tft_write_dat(0x4b);
	Nano_tft_write_dat(0x08);
	Nano_tft_write_dat(0x16);
	Nano_tft_write_dat(0x14);
	Nano_tft_write_dat(0x19);
	Nano_tft_write_dat(0x20);
	Nano_tft_write_cmd(0xe1);
	Nano_tft_write_dat(0xd0);
	Nano_tft_write_dat(0x00);
	Nano_tft_write_dat(0x06);
	Nano_tft_write_dat(0x09);
	Nano_tft_write_dat(0x0b);
	Nano_tft_write_dat(0x29);
	Nano_tft_write_dat(0x36);
	Nano_tft_write_dat(0x54);
	Nano_tft_write_dat(0x4b);
	Nano_tft_write_dat(0x0d);
	Nano_tft_write_dat(0x16);
	Nano_tft_write_dat(0x14);
	Nano_tft_write_dat(0x21);
	Nano_tft_write_dat(0x20);
	Nano_tft_write_cmd(0x29);
	Nano_tft_write_cmd(0x21);

    // Nano_tft_clear_16bit_pix(0xffff);  //清屏全白
}

void Nano_tft_Fill_Color(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2, uint16_t *color_p)
{
	uint16_t i,j;
	j=(x2-x1+1)*(y2-y1+1);
	Nano_tft_set_window(x1,y1,x2,y2);

	for(i=0;i<j;i++)
	{
		Nano_tft_write_dat((*color_p)>>8);
		Nano_tft_write_dat((uint8_t)(*color_p));
		color_p++;
	}
}
