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

		#define		SCL_HIGH				sclSetValue(Bit_SET)
		#define		SCL_LOW					sclSetValue(Bit_RESET)
		#define		CS_HIGH					csSetValue(Bit_SET)
		#define		CS_LOW					csSetValue(Bit_RESET)
		#define		MOSI_HIGH				mosiSetValue(Bit_SET)
		#define		MOSI_LOW				mosiSetValue(Bit_RESET)
		#define		MISO						misoGetValue()
	 
		#define		START						CS_LOW
		#define		STOP						CS_HIGH
	 
		#define		DELAY						delay_us(1)
		#define		SHORT_DELAY			delay_us(1)

	 class CSpi_Software
	 {
		 public:
			 CSpi_Software(void)
			 {
				 ;
			 }
		 
			 CSpi_Software(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
											GPIO_TypeDef 			*pPortCs,		uint16_t pinCs,
											GPIO_TypeDef			*pPortMosi,	uint16_t pinMosi,
											GPIO_TypeDef			*pPortMiso,	uint16_t pinMiso)
			 {
				pGpioPortSCL = pPortScl;
				 GpioPinSCL = pinScl;
				 pGpioPortCS = pPortCs;
				 GpioPinCS = pinCs;
				 pGpioPortMOSI = pPortMosi;
				 GpioPinMOSI = pinMosi;
				 pGpioPortMISO = pPortMiso;
				 GpioPinMISO = pinMiso;
			 }
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
		 
			// 初始化方向的结构
			GPIO_InitTypeDef* mpIoStruct;
		 
		 protected:
			uint8_t device_id; // 总线号
			uint8_t err; // 错误标记
			dataType read_data;
		 
		protected:
			void spiIoInitial(void)
			{
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
				
				CS_HIGH;
			}
			
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
	 };
	 
	 
#ifdef __cplusplus
}
#endif

#endif
