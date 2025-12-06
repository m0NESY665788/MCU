#ifndef DHT11_H
#define DHT11_H

#include "main.h"

// *** PG9 专用配置 ***
// 寄存器解释：PG9 在 CRH 寄存器的第 4-7 位
// 清零掩码：0xFFFFFF0F (清除第2个格子的配置)
// 输入模式(8)：0x00000080 (8左移1位)
// 输出模式(3)：0x00000030 (3左移1位)

// 设置 PG9 为“上拉输入”
#define DHT11_IO_IN()  {GPIOG->CRH&=0xFFFFFF0F;GPIOG->CRH|=0x00000080; GPIOG->ODR|=GPIO_PIN_9;}

// 设置 PG9 为“推挽输出”
#define DHT11_IO_OUT() {GPIOG->CRH&=0xFFFFFF0F;GPIOG->CRH|=0x00000030;} 

// IO操作函数 (改成了 GPIO_PIN_9)
#define	DHT11_DQ_OUT(X)  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, (GPIO_PinState)X)
#define	DHT11_DQ_IN      HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_9)

uint8_t DHT11_Init(void);
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Check(void);
void DHT11_Rst(void);

#endif
