#ifndef		__X34CXX_H__
#define		__X34CXX_H__
#include	"stm32f10x.h"
#include	"stm32f10x_gpio.h"
#include	"stdint.h"
#include	"iic_eeprom_base.h"
#include	"timeout_delay.h"

/*
			X34CXX
	A0	|1	8|	VCC
	A1	|2	7|	NC
	A2	|3	6|	SCL
	GND	|4	5|	SDA
	
	MCU通过IIC接口发送数据，
	A0/A1/A2在板上硬件连接，
	一组IIC挂8个EE。
*/
#define		START						start()
#define		STOP						stop()

#define		SCL_HIGH				sclSetValue(Bit_SET)
#define		SCL_LOW					sclSetValue(Bit_RESET)
#define		SDA_HIGH				sdaSetValue(Bit_SET)
#define		SDA_LOW					sdaSetValue(Bit_RESET)

#define		SDA							sdaGetValue()
#define		SDA_IN					sdaSetInput()
#define		SDA_OUT					sdaSetOutput()
#define		SCL_OUT					sclSetOutput()

#define		DELAY						delay_us(2)
#define		SHORT_DELAY			delay_us(2)
#define		WAIT_DELAY			delay_ms(3)

#ifdef __cplusplus
 extern "C" {
#endif
	 
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
}Function34c_Union;
	 
/* 需要高压
   Quadrant0		00h to 7Fh
   Quadrant1 		80h to FFh
   Quadrant2		00h to 7Fh
   Quadrant3 		80h to FFh */
//#define		RSWP_SET_Quadrant0	(dataType)0x62
//#define		RSWP_SET_Quadrant1	(dataType)0x68
//#define		RSWP_SET_Quadrant2	(dataType)0x6a
//#define		RSWP_SET_Quadrant3	(dataType)0x60
//#define		RSWP_CLEAR					(dataType)0x66


class CFt34cxx_Base : public CIic_Eeprom_Base
{
	public:
		CFt34cxx_Base();
		CFt34cxx_Base(GPIO_TypeDef 			*pPortScl,		uint16_t pinScl,
									GPIO_TypeDef 			*pPortSda,		uint16_t pinSda,
									GPIO_InitTypeDef	*pSclInitDef,
									GPIO_InitTypeDef	*pSdaInitDef,
									uint8_t 					id)
	{
		Iic.CIic_Port_Init(pPortScl,		pinScl,
											 pPortSda,		pinSda,
											 pSclInitDef,
											 pSdaInitDef,
											 id);
	}
	
		public:
		/* 基础配置 */
		uint32_t bytes_per_page;
		uint32_t bytes_per_half_memory;
		uint32_t page_in_memory;
		uint32_t page_in_half_memory;
	
		/* 切页设置 */
		dataType page_set_first;
		dataType page_set_second;
		dataType page_get;
	
		/* 功能设置 */
		uint8_t rd;
		uint8_t wr;
		uint8_t device_type_rw; // 1010

		// 读取
		dataType page_get_first;
		dataType page_get_second;
	
	public:
		Function34c_Union device_address_byte;
	
		addressType pageAddrAutoSet(addressType addr);
		addressType pageStartAddr(uint8_t page);
		uint8_t pageAddrSet(dataType cmd);
		uint8_t pageAddrGet(void);
	
		dataType readCurrentAddress(uint8_t device_addr);
		dataType readRandom(uint8_t device_addr, addressType addr);
		uint8_t writeRandom(uint8_t device_addr, addressType addr, dataType data);
		uint8_t writePage(uint8_t device_addr, addressType page_addr, dataType data);
		uint8_t readSequentialStart(uint8_t device_addr, addressType start_addr);
		dataType readSequential(void);
		void readSequentialStop(void);
		void softReset(uint8_t device_addr);

};

class CFt34c04 : public CFt34cxx_Base
{
		public:
		CFt34c04();
		CFt34c04(GPIO_TypeDef 		*pPortScl,	uint16_t pinScl,
						GPIO_TypeDef 			*pPortSda,	uint16_t pinSda,
						GPIO_InitTypeDef	*pSclInitDef,
						GPIO_InitTypeDef	*pSdaInitDef,
						uint8_t 					id);
		
	protected:
		void paraInitial(void)
		{
			Msg.err = 0;
			Msg.readData = 0;
			
			// 基础配置
			bytes_per_page = 16;
			bytes_per_half_memory = 256;
			page_in_memory = 32;
			page_in_half_memory = 16;
			
			// 切页设置
			page_set_first = 0x6c;
			page_set_second = 0x6e;
			page_get = 0x6d;
			
			// 读取
			dataType page_get_first	= 0;
			dataType page_get_second = 1;
		}
};

#ifdef __cplusplus
}
#endif

#endif
