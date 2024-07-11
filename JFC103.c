#include <rtthread.h>
#include <rtdevice.h>
#include "JFC103.h"
#include "lcd.h"
#include "stdio.h"
#include "Network.h"
#define SAMPLE_UART_NAME       "uart2"

rt_device_t Uart2;
struct serial_configure Uart2_config;
struct rt_semaphore sem;
rt_thread_t  thread1 = RT_NULL;

rt_size_t rx_size = 0;
rt_uint8_t buffer[88];
rt_int8_t buffer_wave[88];

void JFC103_Write(rt_uint8_t data)
{
    rt_device_write(Uart2, 0, &data,1);
}


rt_err_t rx_callback(rt_device_t dev,rt_size_t size)
{
    rt_sem_release(&sem);
    rx_size = size;
    return RT_EOK;
}

char str[10];

void thread1_entry(void *parameter)
{
    rt_size_t len = 0;


    while(1)
    {
        rt_sem_take(&sem, RT_WAITING_FOREVER);
        len = rt_device_read(Uart2, 0, buffer, rx_size);
        buffer[len] = '\0';
        if(buffer[0] == 0xff)
        {
//            for (int i = 0; i < sizeof(str)/sizeof(str[0]); ++i)
//            {
//                str[i] = RT_NULL;
//            }
            sprintf(str,"a%d",buffer[65]);
            Network_SendData ((rt_uint8_t*)str,sizeof(str)/sizeof(str[0]));
            rt_thread_mdelay(50);
            sprintf(str,"b%d",buffer[66]);
            Network_SendData ((rt_uint8_t*)str,sizeof(str)/sizeof(str[0]));
            rt_thread_mdelay(50);
            sprintf(str,"c%d",buffer[71]);
            Network_SendData ((rt_uint8_t*)str,sizeof(str)/sizeof(str[0]));
            rt_thread_mdelay(50);
            sprintf(str,"d%d",buffer[72]);
            Network_SendData ((rt_uint8_t*)str,sizeof(str)/sizeof(str[0]));
            rt_thread_mdelay(50);
            for(rt_uint8_t i = 1;i < 67; i++)
            {
                buffer_wave[i] = (rt_int8_t)buffer[i];
            }

        }

    }
}



void JFC103_Init(void)
{
    Uart2 =  rt_device_find(SAMPLE_UART_NAME);

    Uart2_config.baud_rate = BAUD_RATE_38400;
    Uart2_config.data_bits = DATA_BITS_8;
    Uart2_config.stop_bits = STOP_BITS_1;
    Uart2_config.parity = PARITY_NONE;
    Uart2_config.bufsz     = 512;
    rt_device_control(Uart2, RT_DEVICE_CTRL_CONFIG,(void*)&Uart2_config);
    rt_device_open(Uart2, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_DMA_RX);
    rt_device_set_rx_indicate(Uart2, rx_callback);              //中断
    rt_sem_init(&sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

    thread1 = rt_thread_create("uart2",thread1_entry,RT_NULL,2048,19,5);
    rt_thread_startup(thread1);
}





