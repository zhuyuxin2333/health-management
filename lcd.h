#ifndef __LCD_H
#define __LCD_H
void Lcd_spi_Init(void);
void lcd_init(void);
void lcd_gui_init(void);
void lcd_write_data_16(rt_uint16_t Data);
void lcd_Fill(rt_uint16_t xsta,rt_uint16_t ysta,rt_uint16_t xend,rt_uint16_t yend,rt_uint16_t color);
void lcd_DrawLine(rt_uint16_t x0,rt_uint16_t y0,rt_uint16_t x1,rt_uint16_t y1,rt_uint16_t color);
void LCD_ShowChinese(rt_uint16_t x,rt_uint16_t y,rt_uint8_t num,rt_uint8_t size);
void LCD_ShowIntNum(rt_uint16_t x,rt_uint16_t y,rt_uint16_t num,rt_uint16_t len,rt_uint16_t fc,rt_uint16_t bc,rt_uint8_t sizey);
void LCD_ShowChar(rt_uint16_t x,rt_uint16_t y,rt_uint8_t num,rt_uint16_t fc,rt_uint16_t bc,rt_uint8_t sizey,rt_uint8_t mode);
void lcd_wave(rt_uint8_t *data);
#endif
