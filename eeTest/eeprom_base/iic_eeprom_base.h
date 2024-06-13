#ifndef		__IIC_EEPROM_BASE_H__
#define		__IIC_EEPROM_BASE_H__
#include	"eeprom_base.h"
#include	"iic_software.h"

typedef	uint8_t		dataType;
typedef	uint32_t	addressType;

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
}Device_Addr_Byte_Union;

class CIic_Eeprom_Base : public CEeprom_Base
{
	public:
		CIic_Eeprom_Base():CEeprom_Base()
		{
			// 功能设置
			rd = 1;
			wr = 0;
			eeprom = 0x0A; // 1010表示EEPROM

			protocol = IIC;
		}
		CIic_Software Iic;
		
	protected:
		uint8_t rd;
		uint8_t wr;
		uint8_t eeprom; // 1010
		Device_Addr_Byte_Union device_address_byte;
};
	 
#ifdef __cplusplus
}
#endif

#endif
