#ifndef		__X24CXX_H__
#define		__X24CXX_H__
#include	"stm32f10x.h"
#include	"stm32f10x_gpio.h"
#include	"stdint.h"
#include	"iic_software.h"
#include	"timeout_delay.h" 


/*
			X24CXX
	A0	|1	8|	VCC
	A1	|2	7|	WP
	A2	|3	6|	SCL
	GND	|4	5|	SDA
*/

#pragma anon_unions
typedef union
{
	struct
	{
		unsigned rw						:1; 
		unsigned device_addr	:3;
		unsigned device_type	:4;
	};
	uint8_t byte;
}Function24c_Union;


#define		WRITE_DISABLE		wpSetValue(Bit_SET)
#define		WRITE_ENABLE		wpSetValue(Bit_RESET)

typedef struct structReturn
{
	dataType readData;
	uint8_t err;
}ReadMsg;

#ifdef __cplusplus
 extern "C" {
#endif

// 24c系列基类，赋值语句在派生类中实现
class CX24cxx_Base : public CIic_Software
{
	public:
		CX24cxx_Base(GPIO_TypeDef 		*pPortScl,	uint16_t pinScl,
								 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
								 GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
								 GPIO_TypeDef			*pPortA2,		uint16_t pinA2,
								 GPIO_TypeDef			*pPortA1,		uint16_t pinA1,
								 GPIO_TypeDef			*pPortA0,		uint16_t pinA0,
								 GPIO_InitTypeDef	*pSclInitDef,
								 GPIO_InitTypeDef	*pSdaInitDef,
								 GPIO_InitTypeDef	*pIoInitDef,
								 uint8_t 					id)
	:CIic_Software(pPortScl, pinScl,
								 pPortSda, pinSda,
								 pSclInitDef,
								 pSdaInitDef,
								 id)
	{
		pGpioPortWp = pPortWp;
		GpioPinWp = pinWp;
		pGpioPortA2 = pPortA2;
		GpioPinA2 = pinA2;
		pGpioPortA1 = pPortA1;
		GpioPinA1 = pinA1;
		pGpioPortA0 = pPortA0;
		GpioPinA0 = pinA0;
	}
	
	
	protected:
		/// @brief SCL端口定义，PORT + PIN
		GPIO_TypeDef *pGpioPortWp;
		uint16_t     GpioPinWp;
	
		GPIO_TypeDef *pGpioPortA2;
		uint16_t     GpioPinA2;
	
		GPIO_TypeDef *pGpioPortA1;
		uint16_t     GpioPinA1;
	
		GPIO_TypeDef *pGpioPortA0;
		uint16_t     GpioPinA0;
	
		// 公用结构，用于赋值iic以外io口
		GPIO_InitTypeDef* mpIoStruct;
	protected:
		/* 功能设置 */
		uint8_t rd;
		uint8_t wr;
		uint8_t device_type; // 1010

		// 芯片参数
		addressType total_bytes; // 总字节数
		addressType page_bytes; // 单页字节数
		addressType all_pages; // 总页数
		uint8_t address_bytes; // 地址所占通信字节数
		uint8_t address_use_a; // 内存地址占用硬件地址位数

	public:
		CX24cxx_Base()
		{
			// 功能设置
			rd = 1;
			wr = 0;
			device_type = 0x0A; // 1010表示EEPROM
		}
		Function24c_Union device_address_byte;
	
	public:
		void eeIoInitial(void)
		{
			iicIoInitial(); // IIC的IO口初始化
			mpIoStruct->GPIO_Pin = GpioPinWp;
			mpIoStruct->GPIO_Mode = GPIO_Mode_Out_PP;
			mpIoStruct->GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(pGpioPortWp, mpIoStruct);
			
			mpIoStruct->GPIO_Pin = GpioPinA2;
			mpIoStruct->GPIO_Mode = GPIO_Mode_Out_PP;
			mpIoStruct->GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(pGpioPortA2, mpIoStruct);
			
			mpIoStruct->GPIO_Pin = GpioPinA1;
			mpIoStruct->GPIO_Mode = GPIO_Mode_Out_PP;
			mpIoStruct->GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(pGpioPortA1, mpIoStruct);
			
			mpIoStruct->GPIO_Pin = GpioPinA0;
			mpIoStruct->GPIO_Mode = GPIO_Mode_Out_PP;
			mpIoStruct->GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(pGpioPortA0, mpIoStruct);
		}
				
		void wpSetValue(BitAction value)
		{
			GPIO_WriteBit(pGpioPortWp,GpioPinWp,value);
		}
		addressType pageStartAddr(uint8_t page);

		dataType readCurrentAddress(uint8_t device_addr);
		dataType readRandom(uint8_t device_addr, addressType addr);
		uint8_t writeRandom(uint8_t device_addr, addressType addr, dataType data);
		uint8_t writePage(uint8_t device_addr, addressType page_addr, dataType data);
		uint8_t readSequentialStart(uint8_t device_addr, addressType start_addr);
		dataType readSequential(void);
		void readSequentialStop(void);
		void softReset(uint8_t device_addr);
		
};

// 3038/30c8
class CX24c256 : public CX24cxx_Base
{
	public:
		CX24c256(void)
		{
			total_bytes = 32768;
			page_bytes = 64;
			all_pages = 512;
			address_bytes = 2;
			address_use_a = 0;
		}
		
		CX24c256(GPIO_TypeDef 		*pPortScl,	uint16_t pinScl,
						 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
						 GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						 GPIO_TypeDef			*pPortA2,		uint16_t pinA2,
						 GPIO_TypeDef			*pPortA1,		uint16_t pinA1,
						 GPIO_TypeDef			*pPortA0,		uint16_t pinA0,
						 GPIO_InitTypeDef	*pSclInitDef,
						 GPIO_InitTypeDef	*pSdaInitDef,
						 GPIO_InitTypeDef	*pIoInitDef,
						 uint8_t 					id)
		:CX24cxx_Base(pPortScl, pinScl,
								  pPortSda, pinSda,
								  pPortWp, pinWp,
								  pPortA2, pinA2,
								  pPortA1, pinA1,
								  pPortA0, pinA0,
								  pSclInitDef,
								  pSdaInitDef,
								  pIoInitDef,
								  id)
		{
			total_bytes = 32768;
			page_bytes = 64;
			all_pages = 512;
			address_bytes = 2;
			address_use_a = 0;
		}
};

// 3037
class CX24c128 : public CX24cxx_Base
{
	public:
		CX24c128(void)
		{
			total_bytes = 16384;
			page_bytes = 64;
			all_pages = 256;
			address_bytes = 2;
			address_use_a = 0;
		}
		
		CX24c128(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						GPIO_TypeDef 			*pPortSda,	uint16_t pinSda,
						GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						GPIO_TypeDef			*pPortA2,		uint16_t pinA2,
						GPIO_TypeDef			*pPortA1,		uint16_t pinA1,
						GPIO_TypeDef			*pPortA0,		uint16_t pinA0,
						GPIO_InitTypeDef	*pSclInitDef,
						GPIO_InitTypeDef	*pSdaInitDef,
						GPIO_InitTypeDef	*pIoInitDef,
						uint8_t 					id)
		:CX24cxx_Base(pPortScl, pinScl,
								  pPortSda, pinSda,
								  pPortWp, pinWp,
								  pPortA2, pinA2,
								  pPortA1, pinA1,
								  pPortA0, pinA0,
								  pSclInitDef,
								  pSdaInitDef,
								  pIoInitDef,
								  id)
		{
			total_bytes = 16384;
			page_bytes = 64;
			all_pages = 256;
			address_bytes = 2;
			address_use_a = 0;
		}
};

// 3036
class CX24c64 : public CX24cxx_Base
{
	public:
		CX24c64(void)
		{
			total_bytes = 8192;
			page_bytes = 32;
			all_pages = 256;
			address_bytes = 2;
			address_use_a = 0;
		}
		
		CX24c64(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						GPIO_TypeDef 			*pPortSda,	uint16_t pinSda,
						GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						GPIO_TypeDef			*pPortA2,		uint16_t pinA2,
						GPIO_TypeDef			*pPortA1,		uint16_t pinA1,
						GPIO_TypeDef			*pPortA0,		uint16_t pinA0,
						GPIO_InitTypeDef	*pSclInitDef,
						GPIO_InitTypeDef	*pSdaInitDef,
						GPIO_InitTypeDef	*pIoInitDef,
						uint8_t 					id)
		:CX24cxx_Base(pPortScl, pinScl,
								  pPortSda, pinSda,
								  pPortWp, pinWp,
								  pPortA2, pinA2,
								  pPortA1, pinA1,
								  pPortA0, pinA0,
								  pSclInitDef,
								  pSdaInitDef,
								  pIoInitDef,
								  id)
		{
			total_bytes = 8192;
			page_bytes = 32;
			all_pages = 256;
			address_bytes = 2;
			address_use_a = 0;
		}
};

// 3034
class CX24c16 : public CX24cxx_Base
{
	public:
		CX24c16(void)
		{
			total_bytes = 2048;
			page_bytes = 16;
			all_pages = 128;
			address_bytes = 1;
			address_use_a = 0;
		}
		
		CX24c16(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						GPIO_TypeDef 			*pPortSda,	uint16_t pinSda,
						GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						GPIO_TypeDef			*pPortA2,		uint16_t pinA2,
						GPIO_TypeDef			*pPortA1,		uint16_t pinA1,
						GPIO_TypeDef			*pPortA0,		uint16_t pinA0,
						GPIO_InitTypeDef	*pSclInitDef,
						GPIO_InitTypeDef	*pSdaInitDef,
						GPIO_InitTypeDef	*pIoInitDef,
						uint8_t 					id)
		:CX24cxx_Base(pPortScl, pinScl,
								  pPortSda, pinSda,
								  pPortWp, pinWp,
								  pPortA2, pinA2,
								  pPortA1, pinA1,
								  pPortA0, pinA0,
								  pSclInitDef,
								  pSdaInitDef,
								  pIoInitDef,
								  id)
		{
			total_bytes = 2048;
			page_bytes = 16;
			all_pages = 128;
			address_bytes = 1;
			address_use_a = 0;
		}
};


#ifdef __cplusplus
}
#endif


#endif
