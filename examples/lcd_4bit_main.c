// MIT License

// Copyright (c) 2020 phonght32

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "stm_err.h"
#include "stm_log.h"

#include "hd44780.h"

#define TASK_SIZE   1024
#define TASK_PRIOR  5

static const char *TAG = "APP_MAIN";

hd44780_handle_t handle;

static void example_task(void* arg)
{
    hd44780_hw_info_t hw_info = {
        .gpio_port_rs = GPIO_PORT_A,
        .gpio_num_rs = GPIO_NUM_0,
        .gpio_port_rw = GPIO_PORT_A,
        .gpio_num_rw = GPIO_NUM_2,
        .gpio_port_en = GPIO_PORT_A,
        .gpio_num_en = GPIO_NUM_4,
        .gpio_port_d4 = GPIO_PORT_A,
        .gpio_num_d4 = GPIO_NUM_1,
        .gpio_port_d5 = GPIO_PORT_A,
        .gpio_num_d5 = GPIO_NUM_3,
        .gpio_port_d6 = GPIO_PORT_A,
        .gpio_num_d6 = GPIO_NUM_5,
        .gpio_port_d7 = GPIO_PORT_A,
        .gpio_num_d7 = GPIO_NUM_7,
    };

    hd44780_cfg_t config = {
        .size = HD44780_SIZE_16_2,
        .comm_mode = HD44780_COMM_MODE_4BIT,
        .hw_info = hw_info,
    };

    handle = hd44780_init(&config);
    hd44780_clear(handle); 

    while (1)
    {    
        hd44780_home(handle);
        hd44780_write_string(handle, "LCD with STM-IDF");

        hd44780_gotoxy(handle, 0, 1);
        hd44780_write_string(handle, "LCD size: 16x2");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

int main(void)
{
    /* Set log output level */
    stm_log_level_set("*", STM_LOG_NONE);
    stm_log_level_set("APP_MAIN", STM_LOG_INFO);

    /* Create task */
    xTaskCreate(example_task, "example_task", TASK_SIZE, NULL, TASK_PRIOR, NULL);

    /* Start RTOS scheduler */
    vTaskStartScheduler();
}