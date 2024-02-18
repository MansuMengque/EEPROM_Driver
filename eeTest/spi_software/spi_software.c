#include	"spi_software.h"
#include	"stm32f10x.h"
#include	"stm32f10x_gpio.h"
#include	"stdint.h"
#include	"timeout_delay.h"


CSpi_Software::CSpi_Software(GPIO_TypeDef *pPortScl,	uint16_t pinScl,
														 GPIO_TypeDef *pPortCs,		uint16_t pinCs,
														 GPIO_TypeDef	*pPortMosi,	uint16_t pinMosi,
														 GPIO_TypeDef	*pPortMiso,	uint16_t pinMiso)
{
	spiIoInitial(pPortScl,	pinScl,
							 pPortCs,		pinCs,
							 pPortMosi,	pinMosi,
							 pPortMiso,	pinMiso);
}

void CSpi_Software::spiIoInitial(GPIO_TypeDef *pPortScl,	uint16_t pinScl,
																 GPIO_TypeDef *pPortCs,		uint16_t pinCs,
																 GPIO_TypeDef	*pPortMosi,	uint16_t pinMosi,
																 GPIO_TypeDef	*pPortMiso,	uint16_t pinMiso)
{
	if(pPortScl == GPIOA || pPortCs == GPIOA || pPortMosi == GPIOA || pPortMiso == GPIOA)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if(pPortScl == GPIOB || pPortCs == GPIOB || pPortMosi == GPIOB || pPortMiso == GPIOB)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if(pPortScl == GPIOC || pPortCs == GPIOC || pPortMosi == GPIOC || pPortMiso == GPIOC)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	else if(pPortScl == GPIOD || pPortCs == GPIOD || pPortMosi == GPIOD || pPortMiso == GPIOD)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	else if(pPortScl == GPIOE || pPortCs == GPIOE || pPortMosi == GPIOE || pPortMiso == GPIOE)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	else if(pPortScl == GPIOF || pPortCs == GPIOF || pPortMosi == GPIOF || pPortMiso == GPIOF)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	else if(pPortScl == GPIOG || pPortCs == GPIOG || pPortMosi == GPIOG || pPortMiso == GPIOG)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	else
		;

	pGpioPortSCL = pPortScl;
	GpioPinSCL = pinScl;
	pGpioPortCS = pPortCs;
	GpioPinCS = pinCs;
	pGpioPortMOSI = pPortMosi;
	GpioPinMOSI = pinMosi;
	pGpioPortMISO = pPortMiso;
	GpioPinMISO = pinMiso;
	
	mpIoStruct->GPIO_Pin = GpioPinSCL;
	mpIoStruct->GPIO_Mode = GPIO_Mode_Out_PP;
	mpIoStruct->GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(pGpioPortSCL, mpIoStruct);
	
	mpIoStruct->GPIO_Pin = GpioPinCS;
	mpIoStruct->GPIO_Mode = GPIO_Mode_Out_PP;
	mpIoStruct->GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(pGpioPortCS, mpIoStruct);
	
	mpIoStruct->GPIO_Pin = GpioPinMOSI;
	mpIoStruct->GPIO_Mode = GPIO_Mode_Out_PP;
	mpIoStruct->GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(pGpioPortMOSI, mpIoStruct);
	
	mpIoStruct->GPIO_Pin = GpioPinMISO;
	mpIoStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
	mpIoStruct->GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(pGpioPortMISO, mpIoStruct);
	
	csSetValue(Bit_SET);
}

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
			mosiSetValue(Bit_SET);
    }
		else
    {
			mosiSetValue(Bit_RESET);
    }
		SHORT_DELAY;
		csSetValue(Bit_RESET);
		DELAY;
		if(misoGetValue())
    {
			read |= mark;
    }
    else
			;
		sclSetValue(Bit_RESET);
		mark >>= 1;
	}
	return read;
}

