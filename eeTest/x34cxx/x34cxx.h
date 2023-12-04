#ifndef		__X34CXX_H__
#define		__X34CXX_H__
#include	"stm32f10x.h"
#include	"stm32f10x_gpio.h"
#include	"stdint.h"
#include	"timeout_delay.h"
#include	"iic_software.h"

/*
			X34CXX
	A0	|1	8|	VCC
	A1	|2	7|	NC
	A2	|3	6|	SCL
	GND	|4	5|	SDA
	
	MCUͨ��IIC�ӿڷ������ݣ�
	A0/A1/A2�ڰ���Ӳ�����ӣ�
	һ��IIC��8��EE��
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
	 
/* ��Ҫ��ѹ
   Quadrant0		00h to 7Fh
   Quadrant1 		80h to FFh
   Quadrant2		00h to 7Fh
   Quadrant3 		80h to FFh */
//#define		RSWP_SET_Quadrant0	(dataType)0x62
//#define		RSWP_SET_Quadrant1	(dataType)0x68
//#define		RSWP_SET_Quadrant2	(dataType)0x6a
//#define		RSWP_SET_Quadrant3	(dataType)0x60
//#define		RSWP_CLEAR					(dataType)0x66

class CFt34c04 : public CIic_Software
{
	public:
		/* �������� */
		uint32_t bytes_per_page;
		uint32_t bytes_per_half_memory;
		uint32_t page_in_memory;
		uint32_t page_in_half_memory;
	
		/* ��ҳ���� */
		dataType page_set_first;
		dataType page_set_second;
		dataType page_get;
	
		/* �������� */
		uint8_t rd;
		uint8_t wr;
		uint8_t device_type_rw; // 1010

		// ��ȡ
		dataType page_get_first;
		dataType page_get_second;

		void paraInitial(void);
	
	public:
		Function34c_Union device_address_byte;
		void testErrSignReset(void)
		{
			test_err_sign = 0;
		}
		void testErrSignSet()
		{
			test_err_sign = 1;
		}
		uint8_t testErrSignGet()
		{
			return test_err_sign;
		}
	
	public:
		CFt34c04();
		CFt34c04(GPIO_TypeDef 		*pPortScl,	uint16_t pinScl,
						GPIO_TypeDef 			*pPortSda,	uint16_t pinSda,
						GPIO_InitTypeDef	*pSclInitDef,
						GPIO_InitTypeDef	*pSdaInitDef,
						uint8_t 					id);
		
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

#ifdef __cplusplus
}
#endif

#endif
