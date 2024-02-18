#include	"my_peripheral.h"
#include	"stm32f10x_rcc.h"

Led::Led(GPIO_TypeDef 		*pPortLed,	uint16_t pinLed,
					 GPIO_InitTypeDef *pLedInitDef,
					 uint8_t 					id)
{
	pGpioLed = pPortLed;
	GpioPinLed = pinLed;
	mpLedStruct = pLedInitDef;
	led_id = id;
	if(pPortLed == GPIOA)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if(pPortLed == GPIOB)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if(pPortLed == GPIOC)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	else if(pPortLed == GPIOD)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	else if(pPortLed == GPIOE)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	else if(pPortLed == GPIOF)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	else if(pPortLed == GPIOG)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	else
		;

	ledSetValue(LED_OFF);
	ledSetOutput();
}

Button::Button(GPIO_TypeDef 		*pPortButton,	uint16_t pinButton,
							 GPIO_InitTypeDef *pButtonInitDef,
							 uint8_t 					id)
{
	pGpioButton = pPortButton;
	GpioPinButton = pinButton;
	mpButtonStruct = pButtonInitDef;
	button_id = id;
	
	if(pPortButton == GPIOA)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if(pPortButton == GPIOB)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if(pPortButton == GPIOC)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	else if(pPortButton == GPIOD)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	else if(pPortButton == GPIOE)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	else if(pPortButton == GPIOF)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	else if(pPortButton == GPIOG)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	else
		;

	buttonSetInput();
}

