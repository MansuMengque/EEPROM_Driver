#ifndef		__EEPROM_BASE_H__
#define		__EEPROM_BASE_H__
#include	"stdint.h"
typedef	uint8_t		dataType;
typedef	uint32_t	addressType;

#ifdef __cplusplus
 extern "C" {
#endif

enum Protocol_Type
{
	IIC,
	SPI
};
	 
typedef struct structReturn
{
	dataType readData;
	uint8_t err;
}ReadMsg;


class CEeprom_Base
{
	public:
	CEeprom_Base()
	{
		Msg.err = 0;
		Msg.readData = 0;
	}
	dataType getReadData(void){ return Msg.readData;}
	uint8_t getErrSign(void){return Msg.err;}
	Protocol_Type getProtocol(void){return protocol;}
	
	protected:
	/* π¶ƒ‹…Ë÷√ */
	Protocol_Type protocol;
	ReadMsg Msg;
};
	 
#ifdef __cplusplus
}
#endif

#endif
