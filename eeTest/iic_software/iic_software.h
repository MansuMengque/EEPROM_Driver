#ifndef		__IIC_SOFTWARE_H__
#define		__IIC_SOFTWARE_H__

#include	"stm32f10x.h"
#include	"stm32f10x_gpio.h"
#include	"stdint.h"
#include	"timeout_delay.h"

typedef	uint8_t		dataType;
typedef	uint32_t	addressType;

#ifdef __cplusplus
 extern "C" {
#endif

	// 此处修改iic速度
	#define		IIC_DELAY						delay_us(1) // scl 高电平
	#define		IIC_SHORT_DELAY			delay_us(1) // scl 低电平
	#define		IIC_WAIT_DELAY			delay_ms(3)

	class CIic_Software
	{
		protected:
			/// @brief SCL端口定义，PORT + PIN
			GPIO_TypeDef *pGpioPortSCL;
			uint16_t     GpioPinSCL;
			/// @brief SDA端口定义，PORT + PIN
			GPIO_TypeDef *pGpioPortSDA;
			uint16_t     GpioPinSDA;
			// 修改方向用的结构
			GPIO_InitTypeDef* mpSclStruct;
			GPIO_InitTypeDef* mpSdaStruct;

		public:
			uint8_t device_id; // 总线号
			uint8_t ack; // 有回应
			uint8_t nack; // 无回应
			
			uint8_t err; // 错误标记
			dataType read_data;

		protected:
			void iicIoInitial(void)
			{
				mpSdaStruct->GPIO_Pin = GpioPinSDA;
				mpSdaStruct->GPIO_Mode = GPIO_Mode_Out_OD;
				mpSdaStruct->GPIO_Speed = GPIO_Speed_10MHz;
				GPIO_Init(pGpioPortSDA, mpSdaStruct);
				
				mpSclStruct->GPIO_Pin = GpioPinSCL;
				mpSclStruct->GPIO_Mode = GPIO_Mode_Out_OD;
				mpSclStruct->GPIO_Speed = GPIO_Speed_10MHz;
				GPIO_Init(pGpioPortSCL, mpSclStruct);
			}
			
			void sdaSetInput(void)
			{
				mpSdaStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
				GPIO_Init(pGpioPortSDA, mpSdaStruct);
			}
			
			void sdaSetOutput(void)
			{
				mpSdaStruct->GPIO_Mode = GPIO_Mode_Out_OD;
				GPIO_Init(pGpioPortSDA, mpSdaStruct);
			}
			
			void sdaSetValue(BitAction value)
			{
				GPIO_WriteBit(pGpioPortSDA,GpioPinSDA,value);
			}
			
			uint8_t sdaGetValue(void)
			{
				return GPIO_ReadInputDataBit(pGpioPortSDA, GpioPinSDA);
			}
			
			void sclSetInput(void)
			{
				mpSclStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
				GPIO_Init(pGpioPortSCL, mpSclStruct);
			}
			
			void sclSetOutput(void)
			{
				mpSclStruct->GPIO_Mode = GPIO_Mode_Out_OD;
				GPIO_Init(pGpioPortSCL, mpSclStruct);
			}
			
			void sclSetValue(BitAction value)
			{
				GPIO_WriteBit(pGpioPortSCL, GpioPinSCL, value);
			}

			uint8_t sclGetValue(void)
			{
				return GPIO_ReadInputDataBit(pGpioPortSCL, GpioPinSCL);
			}

		public:
			CIic_Software();
			CIic_Software(GPIO_TypeDef 		*pPortScl,	uint16_t pinScl,
									 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
									 GPIO_InitTypeDef *pSclInitDef,
									 GPIO_InitTypeDef *pSdaInitDef,
									 uint8_t 					id);
			
			void CIic_Port_Init(GPIO_TypeDef 		*pPortScl,	uint16_t pinScl,
									 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
									 GPIO_InitTypeDef *pSclInitDef,
									 GPIO_InitTypeDef *pSdaInitDef,
									 uint8_t 					id);
		
			void start(void);
			void stop(void);
			void bitsSet(dataType data, uint8_t length);
			void byteSet(dataType data);
			dataType byteGet(void);
			uint8_t ackGet(void);
			void ackSet(uint8_t ack);
			uint8_t idGet(void){return device_id;}
	};

	 
	 
#ifdef __cplusplus
}
#endif

#endif

