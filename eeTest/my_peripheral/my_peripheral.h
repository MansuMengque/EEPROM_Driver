#ifndef		__MY_PERIPHERAL_H__
#define		__MY_PERIPHERAL_H__

#include	"stm32f10x_gpio.h"
#ifdef __cplusplus
 extern "C" {
#endif

#define		LED_ON		Bit_RESET
#define		LED_OFF		Bit_SET

class Led
{
	protected:
		/// @brief 端口定义，PORT + PIN
		GPIO_TypeDef *pGpioLed;
		uint16_t GpioPinLed;
		GPIO_InitTypeDef* mpLedStruct;
		uint8_t led_id;
	
	protected:
		void ledSetOutput(void)
		{
			mpLedStruct->GPIO_Pin = GpioPinLed;
			mpLedStruct->GPIO_Mode = GPIO_Mode_Out_PP;
			mpLedStruct->GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(pGpioLed, mpLedStruct);
		}
		
	public:
		Led(GPIO_TypeDef 		*pPortLed,	uint16_t pinLed,
				 GPIO_InitTypeDef *pLedInitDef,
				 uint8_t 					id);
		void ledSetValue(BitAction value)
		{
			GPIO_WriteBit(pGpioLed,GpioPinLed,value);
		}
};

#define		BUTTON_DOWN			0
#define		BUTTON_UP		1
class Button
{
	protected:
		/// @brief 端口定义，PORT + PIN
		GPIO_TypeDef *pGpioButton;
		uint16_t GpioPinButton;
		GPIO_InitTypeDef* mpButtonStruct;
		uint8_t button_id;
	
	protected:
		void buttonSetInput(void)
		{
			mpButtonStruct->GPIO_Pin = GpioPinButton;
			mpButtonStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
			mpButtonStruct->GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(pGpioButton, mpButtonStruct);
		}

	
	public:
		Button(GPIO_TypeDef 		*pPortButton,	uint16_t pinButton,
					 GPIO_InitTypeDef *pButtonInitDef,
					 uint8_t 					id);
		uint8_t buttonGetValue(void)
		{
			return GPIO_ReadInputDataBit(pGpioButton, GpioPinButton);
		}
};
	 
#ifdef __cplusplus
}
#endif

#endif

