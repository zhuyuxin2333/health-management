#ifndef __JFC103_H
#define __JFC103_H


void JFC103_Read(rt_uint8_t *UART_RX_Buffer);
void JFC103_Init(void);
void JFC103_Write(rt_uint8_t data);
extern rt_int8_t buffer_wave[88];

#endif
