#include	"spi_software.h"
#include	"stm32f10x.h"
#include	"stm32f10x_gpio.h"
#include	"stdint.h"
#include	"timeout_delay.h"

/* 发送并接收数据
   发送数据长度可自定义
   接受长度仅8位，满足状态寄存器与读数据长度 */
dataType CSpi_Software::dataTransform(addressType data, uint8_t dataLength)
{
	addressType mark = 1<<(dataLength-1);
	dataType read = 0;
	for(addressType i = 0; i < dataLength; i++)
	{
		if(data & mark)
    {
			MOSI_HIGH;
    }
		else
    {
			MOSI_LOW;
    }
		SHORT_DELAY;
		SCL_HIGH;
		DELAY;
		if(MISO)
    {
			read |= mark;
    }
    else
			;
		SCL_LOW;
		mark >>= 1;
	}
	return read;
}

