/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-05-31     RT-Thread    first version
 */

#include <rtthread.h>
#include <stdio.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "lcd.h"
#include "JFC103.h"
#include "BrainWave.h"
#include "Network.h"
#include "MLX90614.h"

rt_uint8_t temp[10];

int main(void)
{

        lcd_init();
//    MLX90614_Init();
    JFC103_Init();
    JFC103_Write(0x88);
   lcd_gui_init();
   Network_Init();
    JFC103_Write(0x8A);


//    BrainWave_Init();



    return RT_EOK;
}
