#include <rtthread.h>
#include <rtdevice.h>
#include "Network.h"
#include <stdio.h>
#define SAMPLE_UART4_NAME       "uart4"

rt_uint8_t bufferaa[] ={0x12,0x12,0x45,0x34,0x20};
rt_device_t Uart4;
struct serial_configure Uart4_config;


void Network_SendData(rt_uint8_t *Data,rt_uint8_t Length)
{
    char CIPSEND[20];
    sprintf(CIPSEND,"AT+CIPSEND=0,%d\r\n",Length);
    rt_device_write(Uart4, 0, CIPSEND,20);
    rt_thread_mdelay(100);
    rt_device_write(Uart4, 0, Data,Length);
}

void Network_Init(void)
{
    char Rx[50];
    Uart4 = rt_device_find(SAMPLE_UART4_NAME);
    Uart4_config.baud_rate = BAUD_RATE_115200;
    Uart4_config.data_bits = DATA_BITS_8;
    Uart4_config.stop_bits = STOP_BITS_1;
    Uart4_config.parity = PARITY_NONE;
    Uart4_config.bufsz     = 512;
    rt_device_control(Uart4, RT_DEVICE_CTRL_CONFIG,(void*)&Uart4_config);
    rt_device_open(Uart4, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    rt_device_write(Uart4, 0, "AT+CIPMUX=1\r\n",sizeof("AT+CIPMUX=1\r\n"));
    rt_thread_mdelay(2000);
    rt_device_read(Uart4, 0, Rx, 20);
    rt_kprintf("%s\n",Rx);
    rt_device_write(Uart4, 0, "AT+CIPSERVER=1,8888\r\n",sizeof("AT+CIPSERVER=1,8888\r\n"));
    rt_thread_mdelay(2000);
    rt_device_read(Uart4, 0, Rx, 50);
    rt_kprintf("%s\n",Rx);

}
