#include "dht11.h"
#include "delay.h"

// 复位DHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	// 设置为输出
	DHT11_DQ_OUT(0); 	// 拉低
	delay_ms(30);    	// 拉低 30ms (给足复位时间)
	DHT11_DQ_OUT(1); 	// 拉高 
	delay_us(30);     	// 拉高 30us
}

// 等待DHT11的回应
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
	DHT11_IO_IN();      // 设置为输入	 
    
    // 等待拉低
    while (DHT11_DQ_IN && retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    
    // 等待拉高
    while (!DHT11_DQ_IN && retry<200)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=200)return 1;	    
	return 0;
}

// 读取一位
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN && retry<200) // 等待变低
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN && retry<200) // 等待变高
	{
		retry++;
		delay_us(1);
	}
	delay_us(40); // 延时判断
	if(DHT11_DQ_IN) return 1;
	else return 0;		   
}

// 读取一个字节
uint8_t DHT11_Read_Byte(void)    
{        
	uint8_t i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; 
		dat|=DHT11_Read_Bit();
	}						    
	return dat;
}

// 读取数据
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}
	else return 1;
	return 0;	    
}

// 初始化 PG9
uint8_t DHT11_Init(void)
{	 
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 开启 GPIOG 时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();

    // 默认输出高电平
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_SET);

    // 配置 PG9
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    DHT11_Rst();
	return DHT11_Check();
}
