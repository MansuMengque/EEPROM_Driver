#include	"spi_software.h"
#include	"stm32f10x.h"
#include	"stm32f10x_gpio.h"
#include	"stdint.h"
#include	"timeout_delay.h"

/* ���Ͳ���������
   �������ݳ��ȿ��Զ���
   ���ܳ��Ƚ�8λ������״̬�Ĵ���������ݳ��� */
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

