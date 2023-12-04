#include		"my_peripheral.h"

Led::Led(GPIO_TypeDef 		*pPortLed,	uint16_t pinLed,
					 GPIO_InitTypeDef *pLedInitDef,
					 uint8_t 					id)
{
	pGpioLed = pPortLed;
	GpioPinLed = pinLed;
	mpLedStruct = pLedInitDef;
	led_id = id;
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
	buttonSetInput();
}

