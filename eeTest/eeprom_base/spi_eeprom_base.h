#ifndef		__SPI_EEPROM_BASE_H__
#define		__SPI_EEPROM_BASE_H__
#include	"eeprom_base.h"
#include	"spi_software.h"

#ifdef __cplusplus
 extern "C" {
#endif

class CSpi_Eeprom_Base : public CEeprom_Base
{
public:
	CSpi_Eeprom_Base():CEeprom_Base()
	{
		protocol = SPI;
	}
	CSpi_Software Spi;
};
	 
#ifdef __cplusplus
}
#endif

#endif
