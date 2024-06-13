#ifndef		__X24CXX_H__
#define		__X24CXX_H__
#include	"iic_eeprom_base.h"

/*
			X24CXX
	A0	|1	8|	VCC
	A1	|2	7|	WP
	A2	|3	6|	SCL
	GND	|4	5|	SDA
*/



#ifdef __cplusplus
 extern "C" {
#endif

// 24cϵ�л��࣬��ֵ�������������ʵ��
class CX24cxx_Base : public CIic_Eeprom_Base
{
	public:
		CX24cxx_Base()
		{
			;
		}
		
		CX24cxx_Base(GPIO_TypeDef 		*pPortScl,		uint16_t pinScl,
								 GPIO_TypeDef 		*pPortSda,		uint16_t pinSda,
								 GPIO_TypeDef			*pPortWp,			uint16_t pinWp,
								 GPIO_InitTypeDef	*pSclInitDef,
								 GPIO_InitTypeDef	*pSdaInitDef,
								 GPIO_InitTypeDef	*pIoInitDef,
								 uint8_t 					id)
		{
			Iic.CIic_Port_Init(pPortScl,		pinScl,
												 pPortSda,		pinSda,
												 pSclInitDef,
												 pSdaInitDef,
												 id);

			pGpioPortWp = pPortWp;
			GpioPinWp = pinWp;
			
			ioInitial();
			mpIoStruct = pIoInitDef;
			mpIoStruct->GPIO_Pin = GpioPinWp;
			mpIoStruct->GPIO_Mode = GPIO_Mode_Out_PP;
			mpIoStruct->GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(pGpioPortWp, mpIoStruct);
		}

	
	protected:
		/// @brief SCL�˿ڶ��壬PORT + PIN
		GPIO_TypeDef *pGpioPortWp;
		uint16_t     GpioPinWp;
	
		GPIO_TypeDef *pGpioPortA2;
		uint16_t     GpioPinA2;
	
		GPIO_TypeDef *pGpioPortA1;
		uint16_t     GpioPinA1;
	
		GPIO_TypeDef *pGpioPortA0;
		uint16_t     GpioPinA0;
	
		// ���ýṹ�����ڸ�ֵiic����io��
		GPIO_InitTypeDef* mpIoStruct;

		// оƬ����
		addressType total_bytes;	// ���ֽ���
		addressType page_bytes;		// ��ҳ�ֽ���
		addressType all_pages;		// ��ҳ��
		uint8_t address_bytes;		// ��ַ��ռͨ���ֽ���
		uint8_t address_use_a;		// �ڴ��ַռ��Ӳ����ַλ��
		
		void ioInitial()
		{
			if(pGpioPortWp == GPIOA)
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			else if(pGpioPortWp == GPIOB)
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			else if(pGpioPortWp == GPIOC)
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			else if(pGpioPortWp == GPIOD)
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			else if(pGpioPortWp == GPIOE)
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			else if(pGpioPortWp == GPIOF)
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
			else if(pGpioPortWp == GPIOG)
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
			else
				;
		}

	public:
		addressType getTotalBytes()
		{
			return total_bytes;
		}
		
		addressType getPageBytes()
		{
			return page_bytes;
		}
		
		addressType getAllPages()
		{
			return all_pages;
		}
		
		void wpSetValue(BitAction value)
		{
			GPIO_WriteBit(pGpioPortWp,GpioPinWp,value);
		}
		
		addressType pageStartAddr(addressType page);
		dataType readCurrentAddress(uint8_t device_addr);
		dataType readRandom(uint8_t device_addr, addressType addr);
		uint8_t writeRandom(uint8_t device_addr, addressType addr, dataType data);
		uint8_t writePage(uint8_t device_addr, addressType page_addr, dataType data);
		uint8_t writePage(uint8_t device_addr, addressType page_addr, dataType data[]);
		uint8_t readSequentialStart(uint8_t device_addr, addressType start_addr);
		dataType readSequential(void);
		void readSequentialStop(void);
		void softReset(uint8_t device_addr);
		
};

// 3039 V2
class CX24c1024 : public CX24cxx_Base
{
	public:
		CX24c1024(void)
		{
			paraInitial();
		}
				
		CX24c1024(GPIO_TypeDef 		*pPortScl,	uint16_t pinScl,
						 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
						 GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						 GPIO_InitTypeDef	*pSclInitDef,
						 GPIO_InitTypeDef	*pSdaInitDef,
						 GPIO_InitTypeDef	*pIoInitDef,
						 uint8_t 				id)
	:CX24cxx_Base(pPortScl, pinScl,
								pPortSda, pinSda,
								pPortWp, 	pinWp,
								pSclInitDef,
								pSdaInitDef,
								pIoInitDef,
								id)
		{
			paraInitial();
		}
		
	protected:
		void paraInitial(void)
		{
			total_bytes = 131072;
			page_bytes = 256;
			all_pages = 512;
			address_bytes = 2;
			address_use_a = 1;
		}
};

// 3039
class CX24c512 : public CX24cxx_Base
{
	public:
		CX24c512(void)
		{
			paraInitial();
		}
				
		CX24c512(GPIO_TypeDef 		*pPortScl,	uint16_t pinScl,
						 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
						 GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						 GPIO_InitTypeDef	*pSclInitDef,
						 GPIO_InitTypeDef	*pSdaInitDef,
						 GPIO_InitTypeDef	*pIoInitDef,
						 uint8_t 				id)
	:CX24cxx_Base(pPortScl, pinScl,
								pPortSda, pinSda,
								pPortWp, 	pinWp,
								pSclInitDef,
								pSdaInitDef,
								pIoInitDef,
								id)
		{
			paraInitial();
		}
		
	protected:
		void paraInitial(void)
		{
			total_bytes = 65536;
			page_bytes = 128;
			all_pages = 512;
			address_bytes = 2;
			address_use_a = 0;
		}
};

// 3038/30c8
class CX24c256 : public CX24cxx_Base
{
	public:
		CX24c256(void)
		{
			paraInitial();
		}
				
		CX24c256(GPIO_TypeDef 		*pPortScl,	uint16_t pinScl,
						 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
						 GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						 GPIO_InitTypeDef	*pSclInitDef,
						 GPIO_InitTypeDef	*pSdaInitDef,
						 GPIO_InitTypeDef	*pIoInitDef,
						 uint8_t 				id)
	:CX24cxx_Base(pPortScl, pinScl,
								pPortSda, pinSda,
								pPortWp, 	pinWp,
								pSclInitDef,
								pSdaInitDef,
								pIoInitDef,
								id)
		{
			paraInitial();
		}
		
	protected:
		void paraInitial(void)
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
			paraInitial();
		}
				
		CX24c128(GPIO_TypeDef 		*pPortScl,	uint16_t pinScl,
						 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
						 GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						 GPIO_InitTypeDef	*pSclInitDef,
						 GPIO_InitTypeDef	*pSdaInitDef,
						 GPIO_InitTypeDef	*pIoInitDef,
						 uint8_t 				id)
	:CX24cxx_Base(pPortScl, pinScl,
								pPortSda, pinSda,
								pPortWp, 	pinWp,
								pSclInitDef,
								pSdaInitDef,
								pIoInitDef,
								id)
		{
			paraInitial();
		}
		
	protected:
		void paraInitial(void)
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
			paraInitial();
		}
				
		CX24c64(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
						 GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						 GPIO_InitTypeDef	*pSclInitDef,
						 GPIO_InitTypeDef	*pSdaInitDef,
						 GPIO_InitTypeDef	*pIoInitDef,
						 uint8_t 				id)
	:CX24cxx_Base(pPortScl, pinScl,
								pPortSda, pinSda,
								pPortWp, 	pinWp,
								pSclInitDef,
								pSdaInitDef,
								pIoInitDef,
								id)
		{
			paraInitial();
		}

	protected:
		void paraInitial(void)
		{
			total_bytes = 8192;
			page_bytes = 32;
			all_pages = 256;
			address_bytes = 2;
			address_use_a = 0;
		}
};

class CX24c32 : public CX24cxx_Base
{
	public:
		CX24c32(void)
		{
			paraInitial();
		}
				
		CX24c32(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
						 GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						 GPIO_InitTypeDef	*pSclInitDef,
						 GPIO_InitTypeDef	*pSdaInitDef,
						 GPIO_InitTypeDef	*pIoInitDef,
						 uint8_t 				id)
	:CX24cxx_Base(pPortScl, pinScl,
								pPortSda, pinSda,
								pPortWp, 	pinWp,
								pSclInitDef,
								pSdaInitDef,
								pIoInitDef,
								id)
		{
			paraInitial();
		}

	protected:
		void paraInitial(void)
		{
			total_bytes = 4096;
			page_bytes = 32;
			all_pages = 128;
			address_bytes = 2;
			address_use_a = 0;
		}
};

// 3024
class CX24c16 : public CX24cxx_Base
{
	public:
		CX24c16(void)
		{
			paraInitial();
		}
		
		CX24c16(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
						 GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						 GPIO_InitTypeDef	*pSclInitDef,
						 GPIO_InitTypeDef	*pSdaInitDef,
						 GPIO_InitTypeDef	*pIoInitDef,
						 uint8_t 				id)
	:CX24cxx_Base(pPortScl, pinScl,
								pPortSda, pinSda,
								pPortWp, 	pinWp,
								pSclInitDef,
								pSdaInitDef,
								pIoInitDef,
								id)
		{
			paraInitial();
		}
		
	protected:
		void paraInitial(void)
		{
			total_bytes = 2048;
			page_bytes = 16;
			all_pages = 128;
			address_bytes = 1;
			address_use_a = 3;
		}
};

class CX24c08 : public CX24cxx_Base
{
	public:
		CX24c08(void)
		{
			paraInitial();
		}
		
		CX24c08(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
						 GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						 GPIO_InitTypeDef	*pSclInitDef,
						 GPIO_InitTypeDef	*pSdaInitDef,
						 GPIO_InitTypeDef	*pIoInitDef,
						 uint8_t 				id)
	:CX24cxx_Base(pPortScl, pinScl,
								pPortSda, pinSda,
								pPortWp, 	pinWp,
								pSclInitDef,
								pSdaInitDef,
								pIoInitDef,
								id)
		{
			paraInitial();
		}
		
	protected:
		void paraInitial(void)
		{
			total_bytes = 1024;
			page_bytes = 16;
			all_pages = 64;
			address_bytes = 1;
			address_use_a = 2;
		}
};

class CX24c04 : public CX24cxx_Base
{
	public:
		CX24c04(void)
		{
			paraInitial();
		}
		
		CX24c04(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
						 GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						 GPIO_InitTypeDef	*pSclInitDef,
						 GPIO_InitTypeDef	*pSdaInitDef,
						 GPIO_InitTypeDef	*pIoInitDef,
						 uint8_t 				id)
	:CX24cxx_Base(pPortScl, pinScl,
								pPortSda, pinSda,
								pPortWp, 	pinWp,
								pSclInitDef,
								pSdaInitDef,
								pIoInitDef,
								id)
		{
			paraInitial();
		}
		
	protected:
		void paraInitial(void)
		{
			total_bytes = 512;
			page_bytes = 16;
			all_pages = 32;
			address_bytes = 1;
			address_use_a = 1;
		}
};

class CX24c02 : public CX24cxx_Base
{
	public:
		CX24c02(void)
		{
			paraInitial();
		}
		
		CX24c02(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						 GPIO_TypeDef 		*pPortSda,	uint16_t pinSda,
						 GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						 GPIO_InitTypeDef	*pSclInitDef,
						 GPIO_InitTypeDef	*pSdaInitDef,
						 GPIO_InitTypeDef	*pIoInitDef,
						 uint8_t 				id)
	:CX24cxx_Base(pPortScl, pinScl,
								pPortSda, pinSda,
								pPortWp, 	pinWp,
								pSclInitDef,
								pSdaInitDef,
								pIoInitDef,
								id)
		{
			paraInitial();
		}
		
	protected:
		void paraInitial(void)
		{
			total_bytes = 256;
			page_bytes = 16;
			all_pages = 16;
			address_bytes = 1;
			address_use_a = 0;
		}
};

#ifdef __cplusplus
}
#endif


#endif
