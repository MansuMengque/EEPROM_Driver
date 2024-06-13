#ifndef		__SPI_SOFTWARE_H__
#define		__SPI_SOFTWARE_H__

#include	"stm32f10x.h"
#include	"stm32f10x_gpio.h"
#include	"stdint.h"
#include	"timeout_delay.h"

typedef	uint8_t		dataType;
typedef	uint32_t	addressType;

#ifdef __cplusplus
 extern "C" {
#endif
	 
		#define		DELAY						delay_us(1)
		#define		SHORT_DELAY			delay_us(1)

	 class CSpi_Software
	 {
		 public:
			 CSpi_Software(void)
			 {
				 ;
			 }
		 
			 CSpi_Software(GPIO_TypeDef *pPortScl,	uint16_t pinScl,
										 GPIO_TypeDef *pPortCs,		uint16_t pinCs,
										 GPIO_TypeDef	*pPortMosi,	uint16_t pinMosi,
										 GPIO_TypeDef	*pPortMiso,	uint16_t pinMiso);
			 
				void spiIoInitial(GPIO_TypeDef 	*pPortScl,	uint16_t pinScl,
													GPIO_TypeDef	*pPortCs,		uint16_t pinCs,
													GPIO_TypeDef	*pPortMosi,	uint16_t pinMosi,
													GPIO_TypeDef	*pPortMiso,	uint16_t pinMiso);
				
			// 初始化方向的结构
			GPIO_InitTypeDef mpIoStruct;
		 
			void sclSetValue(BitAction value)
			{
				GPIO_WriteBit(pGpioPortSCL,GpioPinSCL,value);
			}

			void csSetValue(BitAction value)
			{
				GPIO_WriteBit(pGpioPortCS,GpioPinCS,value);
			}
			
			void mosiSetValue(BitAction value)
			{
				GPIO_WriteBit(pGpioPortMOSI,GpioPinMOSI,value);
			}
			
			uint8_t misoGetValue(void)
			{
				return GPIO_ReadInputDataBit(pGpioPortMISO, GpioPinMISO);
			}
			
			dataType dataTransform(addressType data, uint8_t dataLength);
			
		 public:
			uint8_t device_id; // 总线号
			uint8_t err; // 错误标记
			dataType read_data;
		 
		 protected:
			/// @brief SCL端口定义，PORT + PIN
			GPIO_TypeDef *pGpioPortSCL;
			uint16_t     GpioPinSCL;
			/// @brief CS端口定义，PORT + PIN
			GPIO_TypeDef *pGpioPortCS;
			uint16_t     GpioPinCS;
			/// @brief MOSI端口定义，PORT + PIN
			GPIO_TypeDef *pGpioPortMOSI;
			uint16_t     GpioPinMOSI;
			/// @brief MISO端口定义，PORT + PIN
			GPIO_TypeDef *pGpioPortMISO;
			uint16_t     GpioPinMISO;
		 
	 };
	 
	 
#ifdef __cplusplus
}
#endif

#endif
