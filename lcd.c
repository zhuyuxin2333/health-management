#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <drv_common.h>
#include "drv_spi.h"
#include "lcd_font.h"
#include "JFC103.h"



#define LCD_DC_PIN            GET_PIN(A, 4)
#define LCD_RES_PIN           GET_PIN(A, 6)

struct rt_spi_device* Lcd;
rt_thread_t  wave = RT_NULL;

#define LCD_SPI_BUS "spi1"
#define LCD_SPI_DEV_NAME    "spi10"
struct rt_spi_configuration spi1_connfig ={
        .data_width = 8,
        .max_hz = 42*1000*1000,
        .mode =  RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB | RT_SPI_NO_CS,

};

void lcd_Fill(rt_uint16_t xsta,rt_uint16_t ysta,rt_uint16_t xend,rt_uint16_t yend,rt_uint16_t color);
void lcd_DrawLine(rt_int16_t x0, rt_int16_t y0, rt_int16_t x1, rt_int16_t y1, rt_uint16_t color);
void LCD_ShowIntNum(rt_uint16_t x,rt_uint16_t y,rt_uint16_t num,rt_uint8_t len,rt_uint16_t fc,rt_uint16_t bc,rt_uint8_t sizey);
void lcd_DrawPoint(rt_uint16_t x,rt_uint16_t y,rt_uint16_t color);

void wave_entry(void *parameter)
{
    rt_int16_t pointx, pointy;
    rt_int16_t pointxlast; // 用于存储上一个点的x坐标
    rt_int16_t pointylast; // 用于存储上一个点的y坐标
    rt_uint8_t range = 80;
    float scale = (float)range / 127;

    while(1)
        {

            for(uint8_t i = 1; i < 64; i++)
            {
            if(i == 1)
            {

                pointx = 5;
                pointy = (buffer_wave[1] ==-60)? 0:buffer_wave[1]*scale;
            }
            pointxlast = pointx;
            pointylast = pointy;
            pointx = i*5;
            if(buffer_wave[i] == -60)
            {
                pointy = 0;
            }else
            {
                pointy = buffer_wave[i]*scale;
            }

            lcd_Fill(pointx,100,pointx+5,305,0xffff);
            lcd_DrawLine(pointx,100,pointx+1,100,0xf800);
            lcd_DrawLine(pointx,140,pointx+1,140,0xf800);
            lcd_DrawLine(pointx,180,pointx+1,180,0xf800);
            lcd_DrawLine(pointx,220,pointx+1,220,0xf800);
            lcd_DrawLine(pointx,260,pointx+1,260,0xf800);
            lcd_DrawLine(pointx,300,pointx+1,300,0xf800);
            lcd_DrawLine(pointxlast,200-pointylast,pointx,200-pointy,0x0000);
            rt_thread_mdelay(3);
            }
            LCD_ShowIntNum(85,310,buffer_wave[65],3,0xf800,0xffff,32);
            LCD_ShowIntNum(85,345,buffer_wave[66],3,0xf800,0xffff,32);

        }

}



void Lcd_spi_Init(void)
{
    rt_hw_spi_device_attach(LCD_SPI_BUS, LCD_SPI_DEV_NAME, RT_NULL, RT_NULL);
    Lcd = (struct rt_spi_device *)rt_device_find(LCD_SPI_DEV_NAME);
    rt_spi_configure(Lcd, &spi1_connfig);
    wave = rt_thread_create("wave",wave_entry,RT_NULL,2048,15,10);
}

void lcd_gpio_Init(void)
{
    rt_pin_mode(LCD_DC_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_RES_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(LCD_RES_PIN, PIN_LOW);
    rt_thread_mdelay(100);
    rt_pin_write(LCD_RES_PIN, PIN_HIGH);
}

void lcd_write_cmd(rt_uint8_t Cmd)
{
    rt_pin_write(LCD_DC_PIN, PIN_LOW);
    rt_spi_send(Lcd, &Cmd, 1);
    rt_pin_write(LCD_DC_PIN, PIN_HIGH);
}

void lcd_write_data(rt_uint8_t Data)
{

    rt_spi_send(Lcd, &Data, 1);
}

void lcd_write_data_16(rt_uint16_t Data)
{
    rt_uint16_t Temp = (Data >> 8) |(Data << 8);
//    rt_uint8_t temp_buf[2]={(Data>>8),Data};
    rt_spi_send(Lcd, &Temp, 2);
}
//void byte_16(void)
//{
//    rt_err_t res = 0;
//    spi1_connfig.data_width = 16;
//    res = rt_spi_configure(Lcd, &spi1_connfig);
//    if(res == RT_EOK)
//    {
//        rt_kprintf("chenggou\n");
//    }
//}

void lcd_init(void)
{

    Lcd_spi_Init();
    lcd_gpio_Init();
    lcd_write_cmd(0x11); //Sleep out
    rt_thread_mdelay(120);    //Delay 120ms

    lcd_write_cmd(0Xf0);
    lcd_write_data(0xc3);
    lcd_write_cmd(0Xf0);
    lcd_write_data(0x96);
    lcd_write_cmd(0x36);    // Memory Access Control
    lcd_write_data(0x48);

    lcd_write_cmd(0x3A);
    lcd_write_data(0x05);

    lcd_write_cmd(0Xe8);
    lcd_write_data(0x40);
    lcd_write_data(0x82);
    lcd_write_data(0x07);
    lcd_write_data(0x18);
    lcd_write_data(0x27);
    lcd_write_data(0x0a);
    lcd_write_data(0xb6);
    lcd_write_data(0x33);

    lcd_write_cmd(0Xc5);
    lcd_write_data(0x27);

    lcd_write_cmd(0Xc2);
    lcd_write_data(0xa7);

    lcd_write_cmd(0Xe0);

    lcd_write_data(0xf0);
    lcd_write_data(0x01);
    lcd_write_data(0x06);
    lcd_write_data(0x0f);
    lcd_write_data(0x12);
    lcd_write_data(0x1d);
    lcd_write_data(0x36);
    lcd_write_data(0x54);
    lcd_write_data(0x44);
    lcd_write_data(0x0c);
    lcd_write_data(0x18);
    lcd_write_data(0x16);
    lcd_write_data(0x13);
    lcd_write_data(0x15);

    lcd_write_cmd(0Xe1);
    lcd_write_data(0xf0);
    lcd_write_data(0x01);
    lcd_write_data(0x05);
    lcd_write_data(0x0a);
    lcd_write_data(0x0b);
    lcd_write_data(0x07);
    lcd_write_data(0x32);
    lcd_write_data(0x44);
    lcd_write_data(0x44);
    lcd_write_data(0x0c);
    lcd_write_data(0x18);
    lcd_write_data(0x17);
    lcd_write_data(0x13);
    lcd_write_data(0x16);

    lcd_write_cmd(0Xf0);
    lcd_write_data(0x3c);

    lcd_write_cmd(0Xf0);
    lcd_write_data(0x69);

    lcd_write_cmd(0X29);
    rt_kprintf("lcd Init\r\n");
    lcd_Fill(0,0,320,480,0x0000);
//    lcd_Fill(0,0,320,480,0x5555);
//    lcd_Fill(0,0,320,480,0x8888);
    lcd_Fill(0,0,320,480,0xffff);
//    lcd_Fill(0,0,320,480,0x0000);
//    lcd_Fill(0,0,320,480,0x5555);
//    lcd_Fill(0,0,320,480,0x8888);
//    lcd_Fill(0,0,320,480,0xffff);
    rt_thread_startup(wave);
}




void lcd_Address_Set(rt_uint16_t x1,rt_uint16_t y1,rt_uint16_t x2,rt_uint16_t y2)
{
        lcd_write_cmd(0x2a);//列地址设置
        lcd_write_data_16(x1);
        lcd_write_data_16(x2);
        lcd_write_cmd(0x2b);//行地址设置
        lcd_write_data_16(y1);
        lcd_write_data_16(y2);
        lcd_write_cmd(0x2c);//储存器写
}

void lcd_DrawPoint(rt_uint16_t x,rt_uint16_t y,rt_uint16_t color)
{
    lcd_Address_Set(x,y,x,y);//设置光标位置
    lcd_write_data_16(color);
}

//rt_uint8_t SendBuff[640];
void lcd_Fill(rt_uint16_t xsta, rt_uint16_t ysta, rt_uint16_t xend, rt_uint16_t yend, rt_uint16_t color) {
    rt_uint16_t i, j;
    rt_uint16_t length = xend - xsta;
    rt_uint8_t *SendBuff = (rt_uint8_t *)malloc(length * 2);
    lcd_Address_Set(xsta, ysta, xend - 1, yend - 1); // 设置显示范围
    for (i = ysta; i < yend; i++) {
        for (j = xsta; j < xend; j++) {
            // 计算SendBuff数组中的索引
            rt_uint16_t index = (j - xsta) * 2;
            SendBuff[index] = color >> 8;
            SendBuff[index + 1] = color;
        }
        rt_spi_send(Lcd, SendBuff, length * 2);
    }
    free(SendBuff); // 释放动态分配的内存
}


//void lcd_Fill(rt_uint16_t xsta, rt_uint16_t ysta, rt_uint16_t xend, rt_uint16_t yend, rt_uint16_t color) {
//    rt_uint16_t i, j;
//   // rt_uint16_t length = xend - xsta;
//   // rt_uin16_t *SendBuff = (rt_uint16_t *)malloc(length);
//    lcd_Address_Set(xsta, ysta, xend - 1, yend - 1); // 设置显示范围
//    for (i = ysta; i < yend; i++) {
//        for (j = xsta; j < xend; j++) {
//            // 计算SendBuff数组中的索引
//
//            rt_spi_sendrecv16(Lcd,color);
//
//        }
////        rt_spi_send(Lcd, SendBuff, length);
////        rt_spi_sendrecv16(Lcd, data)
//    }
//    //free(SendBuff); // 释放动态分配的内存
//}


void lcd_DrawLine(rt_int16_t x0, rt_int16_t y0, rt_int16_t x1, rt_int16_t y1, rt_uint16_t color)
{
    rt_int16_t dx = abs(x1 - x0);
    rt_int16_t sx = x0 < x1 ? 1 : -1;
    if (dx == 0) {
        // 处理水平线的特殊情况
        for (rt_int16_t i = 0; i <= abs(y1 - y0); i++) {
            lcd_DrawPoint(x0, y0 + (y0 < y1 ? i : -i), color);
        }
        return;
    }

    rt_int16_t dy = abs(y1 - y0);
    rt_int16_t sy = y0 < y1 ? 1 : -1;
    rt_int16_t err = dx - dy;

    while (1)
    {
        lcd_DrawPoint(x0, y0, color);

        if (x0 == x1 && y0 == y1) break;

        rt_int16_t e2 = err * 2;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}



void LCD_ShowChar(rt_uint16_t x, rt_uint16_t y, rt_uint8_t num, rt_uint16_t fc, rt_uint16_t bc, rt_uint8_t sizey, rt_uint8_t mode)
{
    rt_uint8_t temp, sizex, m = 0;
    rt_uint16_t i, TypefaceNum;
    rt_uint16_t x0 = x;
    sizex = sizey / 2;
    TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
    num = num - ' ';
    lcd_Address_Set(x, y, x + sizex - 1, y + sizey - 1);

    const rt_uint8_t *ascii_array;
    switch (sizey)
    {
    case 12:
        ascii_array = ascii_1206[num];
        break;
    case 16:
        ascii_array = ascii_1608[num];
        break;
    case 24:
        ascii_array = ascii_2412[num];
        break;
    case 32:
        ascii_array = ascii_3216[num];
        break;
    default:
        return;
    }

    for (i = 0; i < TypefaceNum; i++)
    {
        temp = ascii_array[i];
        for (rt_uint8_t t = 0; t < 8; t++)
        {
            if (!mode)
            {
                lcd_write_data_16((temp & (0x01 << t)) ? fc : bc);
                m++;
                if (m % sizex == 0)
                {
                    m = 0;
                    break;
                }
            }
            else
            {
                if (temp & (0x01 << t))
                {
                    lcd_DrawPoint(x, y, fc);
                }
                x++;
                if ((x - x0) == sizex)
                {
                    x = x0;
                    y++;
                    break;
                }
            }
        }
    }
}

void LCD_ShowChinese(rt_uint16_t x, rt_uint16_t y, rt_uint8_t num, rt_uint8_t size) {
    if (size != 32) {
        return;
    }

    rt_uint16_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * size;  // 计算 csize
    rt_uint16_t y0 = y;

    for (rt_uint16_t t = 0; t < csize; t++) {
        rt_uint16_t temp = chinese[num][t];
        for (rt_uint8_t t1 = 0; t1 < 8; t1++) {
            rt_uint16_t color = (temp & 0x80) ? 0xf800 : 0xffff;
            lcd_DrawPoint(x, y, color);
            temp <<= 1;
            y++;
            if ((y - y0) == size) {
                y = y0;
                x++;
                break;
            }
        }
    }
}
rt_uint32_t mypow(rt_uint8_t m,rt_uint8_t n)
{
    rt_uint32_t result=1;
    while(n--)result*=m;
    return result;
}


void LCD_ShowIntNum(rt_uint16_t x,rt_uint16_t y,rt_uint16_t num,rt_uint8_t len,rt_uint16_t fc,rt_uint16_t bc,rt_uint8_t sizey)
{
    rt_uint8_t t,temp;
    rt_uint8_t enshow=0;
    rt_uint8_t sizex=sizey/2;
    for(t=0;t<len;t++)
    {
        temp=(num/mypow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
                continue;
            }else enshow=1;

        }
        LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
    }
}
void lcd_gui_init(void)
{
        LCD_ShowChinese(70,5,12,32);
        LCD_ShowChinese(150,5,13,32);
        LCD_ShowChinese(230,5,14,32);

        lcd_Fill(0,100,320,301,0xffff);
//        lcd_DrawLine(2,100,2,301,0xf800);
//        lcd_DrawLine(40,100,40,301,0xf800);
//        lcd_DrawLine(80,100,80,301,0xf800);
//        lcd_DrawLine(120,100,120,301,0xf800);
//        lcd_DrawLine(160,100,160,301,0xf800);
//        lcd_DrawLine(200,100,200,301,0xf800);
//        lcd_DrawLine(240,100,240,301,0xf800);
//        lcd_DrawLine(280,100,280,301,0xf800);
//        lcd_DrawLine(318,100,318,301,0xf800);
//
//        lcd_DrawLine(2,100,320,100,0xf800);
//        lcd_DrawLine(2,140,320,140,0xf800);
//        lcd_DrawLine(2,180,320,180,0xf800);
//        lcd_DrawLine(2,220,320,220,0xf800);
//        lcd_DrawLine(2,260,320,260,0xf800);
//        lcd_DrawLine(2,300,320,300,0xf800);

        LCD_ShowChinese(10,310,0,32);                   //心率
        LCD_ShowChinese(40,310,1,32);
        LCD_ShowChar(70,310,':',0xf800,0xffff,32,0);
        LCD_ShowChinese(140,310,2,32);
        LCD_ShowChar(172,310,'/',0xf800,0xffff,32,0);
        LCD_ShowChinese(187,310,3,32);
        LCD_ShowChinese(220,310,4,32);

        LCD_ShowChinese(10,345,5,32);                   //血氧
        LCD_ShowChinese(40,345,6,32);
        LCD_ShowChar(70,345,':',0xf800,0xffff,32,0);
        LCD_ShowChar(140,345,'%',0xf800,0xffff,32,0);


}




