#ifndef		__X93CXX_H__
#define		__X93CXX_H__
#include	"stm32f10x.h"
#include	"stm32f10x_gpio.h"
#include	"stdint.h"
#include	"timeout_delay.h"
#include	"spi_software.h"
#include	"spi_eeprom_base.h"

/*
			X93CX6
	CS	|1	8|	VCC
	SCL	|2	7|	NC
	DI	|3	6|	ORG
	DO	|4	5|	GND
*/

// 是否在CS下降并上升后DO输出busy信号
#define		WAIT_AFTER_CS_FALL	0
#define		ORG_HIGH						orgSetValue(Bit_SET)
#define		ORG_LOW							orgSetValue(Bit_RESET)

class CX93cxx_Base : public CSpi_Eeprom_Base
{
	public:
		CX93cxx_Base(void)
		{
			op_read = 2;
			op_ewen = 0;
			op_ewds = 0;
			op_erase = 3;
			op_write = 1;
			op_eral = 0;
			op_wral = 0;
			
			address_ewen = 3;
			address_ewds = 2;
			address_eral = 0;
			address_wral = 1;
						
			words_length_x8 = 8;
			words_length_x16 = 16;

			x8 = 0;
			x16 = 1;
		}
		
		CX93cxx_Base(GPIO_TypeDef 		*pPortScl,	uint16_t pinScl,
								 GPIO_TypeDef 		*pPortCs,		uint16_t pinCs,
								 GPIO_TypeDef			*pPortMosi,	uint16_t pinMosi,
								 GPIO_TypeDef			*pPortMiso,	uint16_t pinMiso,
								 GPIO_TypeDef			*pPortORG,	uint16_t pinOrg,
								 GPIO_InitTypeDef	*pIoInitDef)
		{
			Spi.spiIoInitial(pPortScl,	pinScl,
											 pPortCs,		pinCs,
											 pPortMosi,	pinMosi,
											 pPortMiso,	pinMiso);
			
			pGpioPortORG = pPortORG;
			GpioPinORG = pinOrg;
			mpIoStruct->GPIO_Pin = GpioPinORG;
			mpIoStruct->GPIO_Mode = GPIO_Mode_Out_PP;
			mpIoStruct->GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(pGpioPortORG, mpIoStruct);
			
			op_read = 2;
			op_ewen = 0;
			op_ewds = 0;
			op_erase = 3;
			op_write = 1;
			op_eral = 0;
			op_wral = 0;
			
			address_ewen = 3;
			address_ewds = 2;
			address_eral = 0;
			address_wral = 1;
						
			words_length_x8 = 8;
			words_length_x16 = 16;

			x8 = 0;
			x16 = 1;
		}
		
		/* 底层函数 */
		uint8_t RequestWaiting(void);
		void Delay(uint8_t time);
		void start(void);
		void stop(void);

		/* 完整功能函数
		   cmd开头函数是规格书标准函数 */
		dataType cmdREAD(addressType addr, uint8_t wordsLength);
		uint8_t cmdWRITE(addressType addr, dataType data, uint8_t wordsLength);
		void readDataStart(addressType addr, uint8_t wordsLength);
		void cmdEWDS(uint8_t wordsLength);
		void cmdEWEN(uint8_t wordsLength);
		uint8_t cmdERASE(addressType addr, uint8_t wordsLength);
		uint8_t cmdERAL(uint8_t wordsLength);
		uint8_t cmdWRAL(dataType data, uint8_t wordsLength);
		uint8_t writeDataAll(dataType data, uint8_t wordsLength);
		uint8_t eraseDataAll(uint8_t wordsLength);

	protected:
			/// @brief ORG端口定义，PORT + PIN
			GPIO_TypeDef *pGpioPortORG;
			uint16_t     GpioPinORG;
	
			// 初始化方向的结构
			GPIO_InitTypeDef* mpIoStruct;

	protected:
		uint8_t op_current;
		uint8_t address_length_current;
		uint8_t words_length_current;
		addressType address_numbers_current;
		uint8_t x8;
		uint8_t x16;
		// 仅发送OP前两位，后面0个数按不同芯片参数发送
		uint8_t op_read;
		uint8_t op_ewen;
		uint8_t op_ewds;
		uint8_t op_erase;
		uint8_t op_write;
		uint8_t op_eral;
		uint8_t op_wral;

		uint8_t address_ewen;
		uint8_t address_ewds;
		uint8_t address_eral;
		uint8_t address_wral;
		uint8_t address_length_x8;
		uint8_t address_length_x16;
		addressType address_numbers_x8;
		addressType address_numbers_x16;

		uint8_t words_length_x8;
		uint8_t words_length_x16;

	public:
		void wordsLengthSet(uint8_t mode)
		{
			if(mode == x8)
			{
				orgSetValue(Bit_RESET);
				address_length_current = address_length_x8;
				address_numbers_current = address_numbers_x8;
				words_length_current = words_length_x8;
			}
			else if(mode == x16)
			{
				orgSetValue(Bit_SET);
				address_length_current = address_length_x16;
				address_numbers_current = address_numbers_x16;
				words_length_current = words_length_x16;
			}
		}
		
		void orgSetValue(BitAction value)
		{
			GPIO_WriteBit(pGpioPortORG,GpioPinORG,value);
		}

		void opSend(uint8_t op)
		{
			op_current = op;
			Spi.dataTransform(op, 2);
		}

		void addressSend(addressType addr)
		{
			Spi.dataTransform(addr, address_length_current);
		}

		void addressSend(uint8_t op_addr)
		{
			Spi.dataTransform(op_addr, 2);
			Spi.dataTransform(0x0000, address_length_current - 2);
		}

		void dataSend(dataType data)
		{
			Spi.dataTransform(data, words_length_current);
		}

		dataType dataGet(void)
		{
			return Spi.dataTransform(0x0000, words_length_current);
		}
};

class CX93C46 : public CX93cxx_Base
{
	public:
		CX93C46(void)
		{
			address_length_x8 = 7;
			address_length_x16 = 6;
			address_numbers_x8 = 128;
			address_numbers_x16 = 64;
		}
		
		CX93C46(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						GPIO_TypeDef 			*pPortCs,		uint16_t pinCs,
						GPIO_TypeDef			*pPortMosi,	uint16_t pinMosi,
						GPIO_TypeDef			*pPortMiso,	uint16_t pinMiso,
						GPIO_TypeDef			*pPortORG,	uint16_t pinOrg,
						GPIO_InitTypeDef	*pIoInitDef)
		:CX93cxx_Base(pPortScl,	pinScl,
									 pPortCs,		pinCs,
									 pPortMosi,	pinMosi,
									 pPortMiso,	pinMiso,
									 pPortORG,	pinOrg,
									 pIoInitDef)
		{
			address_length_x8 = 7;
			address_length_x16 = 6;
			address_numbers_x8 = 128;
			address_numbers_x16 = 64;
		}
};

class CX93C56 : public CX93cxx_Base
{
	public:
		CX93C56(void)
		{
			address_length_x8 = 9;
			address_length_x16 = 8;
			address_numbers_x8 = 256;
			address_numbers_x16 = 128;
		}
		
		CX93C56(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						GPIO_TypeDef 			*pPortCs,		uint16_t pinCs,
						GPIO_TypeDef			*pPortMosi,	uint16_t pinMosi,
						GPIO_TypeDef			*pPortMiso,	uint16_t pinMiso,
						GPIO_TypeDef			*pPortORG,	uint16_t pinOrg,
						GPIO_InitTypeDef	*pIoInitDef)
		:CX93cxx_Base(pPortScl,	pinScl,
									 pPortCs,		pinCs,
									 pPortMosi,	pinMosi,
									 pPortMiso,	pinMiso,
									 pPortORG,	pinOrg,
									 pIoInitDef)
		{
			address_length_x8 = 9;
			address_length_x16 = 8;
			address_numbers_x8 = 256;
			address_numbers_x16 = 128;
		}
};

class CX93C57 : public CX93cxx_Base
{
	public:
		CX93C57(void)
		{
			address_length_x8 = 8;
			address_length_x16 = 7;
			address_numbers_x8 = 256;
			address_numbers_x16 = 128;
		}
		
		CX93C57(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						GPIO_TypeDef 			*pPortCs,		uint16_t pinCs,
						GPIO_TypeDef			*pPortMosi,	uint16_t pinMosi,
						GPIO_TypeDef			*pPortMiso,	uint16_t pinMiso,
						GPIO_TypeDef			*pPortORG,	uint16_t pinOrg,
						GPIO_InitTypeDef	*pIoInitDef)
		:CX93cxx_Base(pPortScl,	pinScl,
									 pPortCs,		pinCs,
									 pPortMosi,	pinMosi,
									 pPortMiso,	pinMiso,
									 pPortORG,	pinOrg,
									 pIoInitDef)
		{
			address_length_x8 = 8;
			address_length_x16 = 7;
			address_numbers_x8 = 256;
			address_numbers_x16 = 128;
		}
};

class CX93C66 : public CX93cxx_Base
{
	public:
		CX93C66(void)
		{
			address_length_x8 = 9;
			address_length_x16 = 8;
			address_numbers_x8 = 512;
			address_numbers_x16 = 256;
		}
		
		CX93C66(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						GPIO_TypeDef 			*pPortCs,		uint16_t pinCs,
						GPIO_TypeDef			*pPortMosi,	uint16_t pinMosi,
						GPIO_TypeDef			*pPortMiso,	uint16_t pinMiso,
						GPIO_TypeDef			*pPortORG,	uint16_t pinOrg,
						GPIO_InitTypeDef	*pIoInitDef)
		:CX93cxx_Base(pPortScl,	pinScl,
									 pPortCs,		pinCs,
									 pPortMosi,	pinMosi,
									 pPortMiso,	pinMiso,
									 pPortORG,	pinOrg,
									 pIoInitDef)
		{
			address_length_x8 = 9;
			address_length_x16 = 8;
			address_numbers_x8 = 512;
			address_numbers_x16 = 256;
		}
};

class CX93C76 : public CX93cxx_Base
{
	public:
		CX93C76(void)
		{
			address_length_x8 = 11;
			address_length_x16 = 10;
			address_numbers_x8 = 1024;
			address_numbers_x16 = 256;
		}
		
		CX93C76(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						GPIO_TypeDef 			*pPortCs,		uint16_t pinCs,
						GPIO_TypeDef			*pPortMosi,	uint16_t pinMosi,
						GPIO_TypeDef			*pPortMiso,	uint16_t pinMiso,
						GPIO_TypeDef			*pPortORG,	uint16_t pinOrg,
						GPIO_InitTypeDef	*pIoInitDef)
		:CX93cxx_Base(pPortScl,	pinScl,
									 pPortCs,		pinCs,
									 pPortMosi,	pinMosi,
									 pPortMiso,	pinMiso,
									 pPortORG,	pinOrg,
									 pIoInitDef)
		{
			address_length_x8 = 11;
			address_length_x16 = 10;
			address_numbers_x8 = 1024;
			address_numbers_x16 = 256;
		}
};

class CX93C86 : public CX93cxx_Base
{
	public:
		CX93C86(void)
		{
			address_length_x8 = 11;
			address_length_x16 = 10;
			address_numbers_x8 = 2048;
			address_numbers_x16 = 1024;
		}
		
		CX93C86(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						GPIO_TypeDef 			*pPortCs,		uint16_t pinCs,
						GPIO_TypeDef			*pPortMosi,	uint16_t pinMosi,
						GPIO_TypeDef			*pPortMiso,	uint16_t pinMiso,
						GPIO_TypeDef			*pPortORG,	uint16_t pinOrg,
						GPIO_InitTypeDef	*pIoInitDef)
		:CX93cxx_Base(pPortScl,	pinScl,
									 pPortCs,		pinCs,
									 pPortMosi,	pinMosi,
									 pPortMiso,	pinMiso,
									 pPortORG,	pinOrg,
									 pIoInitDef)
		{
			address_length_x8 = 11;
			address_length_x16 = 10;
			address_numbers_x8 = 2048;
			address_numbers_x16 = 1024;
		}
};



#endif

