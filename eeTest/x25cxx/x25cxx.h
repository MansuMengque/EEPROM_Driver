#ifndef		__X25CXX_H__
#define		__X25CXX_H__
#include	"stm32f10x.h"
#include	"stm32f10x_gpio.h"
#include	"sys.h"
#include	"timeout_delay.h"
#include	"spi_software.h"

/*		X25CXX
	#CS	|1	8|	VCC
	SO	|2	7|	#HOLD
	#WP	|3	6|	SCL
	GND	|4	5|	SI
*/
	
#define			WP_HIGH				wpSetValue(Bit_SET)
#define			WP_LOW				wpSetValue(Bit_RESET)
#define			HOLD_HIGH			holdSetValue(Bit_SET)
#define			HOLD_LOW			holdSetValue(Bit_RESET)
	
#pragma anon_unions
typedef union
{
	struct
	{
		unsigned n_rdy				:1; 
		unsigned wen					:1;
		unsigned bp						:2;
		unsigned null_bits		:3;
		unsigned wpen					:1;
	};
	
	uint8_t reg;
	
}Status_Reg_Union;

#ifdef __cplusplus
 extern "C" {
#endif
	 
class CX25cxx_Base : public CSpi_Software
{
	public:
		/// @brief WP端口定义，PORT + PIN
		GPIO_TypeDef *pGpioPortWP;
		uint16_t     GpioPinWP;
		/// @brief HOLD端口定义，PORT + PIN
		GPIO_TypeDef *pGpioPortHOLD;
		uint16_t     GpioPinHOLD;
	
	public:
		void eeIoInitial(void)
		{
			spiIoInitial();
			mpIoStruct->GPIO_Pin = GpioPinWP;
			mpIoStruct->GPIO_Mode = GPIO_Mode_Out_PP;
			mpIoStruct->GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(pGpioPortWP, mpIoStruct);
			
			mpIoStruct->GPIO_Pin = GpioPinHOLD;
			mpIoStruct->GPIO_Mode = GPIO_Mode_Out_PP;
			mpIoStruct->GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(pGpioPortHOLD, mpIoStruct);
			
			WP_HIGH;
			HOLD_HIGH;
		}
	protected:
		void eeInitial(void);
		// Instruction Format
		uint8_t op_length;
		uint8_t op_wren;
		uint8_t op_wrdi;
		uint8_t op_rdsr;
		uint8_t op_wrsr;
		uint8_t op_read;
	  uint8_t op_write;
	
		uint8_t addr_length;
	
		// Status Regsiter 
		Status_Reg_Union status_byte;
		// | WPEN | X | X | X | BP1 | BP0 | WEN | #RDY |
		uint8_t stu_reg_rdy_rdy;
		uint8_t stu_reg_rdy_bsy;
		uint8_t stu_reg_wen_diasble;
		uint8_t stu_reg_wen_enable;
		uint8_t stu_reg_bp_lev0;
		uint8_t stu_reg_bp_lev1;
		uint8_t stu_reg_bp_lev2;
		uint8_t stu_reg_bp_lev3;
		uint8_t stu_wpen_unprotected;
		uint8_t stu_wpen_protected;
	
		/* Memory Orgnizaton */
		addressType total_bytes;
		addressType page_nums;
		addressType bytes_in_page;
		uint8_t address_length;

	protected:
		void wpSetValue(BitAction value)
		{
			GPIO_WriteBit(pGpioPortWP,GpioPinWP,value);
		}
		void holdSetValue(BitAction value)
		{
			GPIO_WriteBit(pGpioPortHOLD,GpioPinHOLD,value);
		}
		
		void dataSend(dataType data)
		{
			dataTransform(data, 8);
		}
		
		void addrSend(addressType addr)
		{
			dataTransform(addr, addr_length);
		}
		
		dataType dataRead(void)
		{
			read_data = dataTransform(0x00, 8);
			return read_data;
		}
		
		void opSendWren(void)
		{
			dataTransform(op_wren, op_length);
		}
		void opSendWrdi(void)
		{
			dataTransform(op_wrdi, op_length);
		}
		void opSendRdsr(void)
		{
			dataTransform(op_rdsr, op_length);
		}
		void opSendWrsr(void)
		{
			dataTransform(op_wrsr, op_length);
		}
		void opSendRead(void)
		{
			dataTransform(op_read, op_length);
		}
		void opSendWrite(void)
		{
			dataTransform(op_write, op_length);
		}
		
	public:
		void cmdWREN(void);
		void cmdWRDI(void);
		dataType cmdRDSR(void);
		dataType cmdWRSR(dataType statu_reg);
		dataType cmdREAD(addressType addr);
		void cmdWRITE(addressType my_addr, dataType my_data);
		uint8_t writeData(addressType my_addr, dataType my_data);
		dataType writePage(addressType addr, dataType my_data);
		uint8_t timeOutWait(void);
		addressType pageStartAddr(uint8_t page);
};

class CX25C256 : public CX25cxx_Base
{
	public:
		CX25C256(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
							GPIO_TypeDef 			*pPortCs,		uint16_t pinCs,
							GPIO_TypeDef			*pPortMosi,	uint16_t pinMosi,
							GPIO_TypeDef			*pPortMiso,	uint16_t pinMiso,
							GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
							GPIO_TypeDef			*pPortHold,	uint16_t pinHold,
							GPIO_InitTypeDef	*pIoInitDef,
							uint8_t 					id)
		{
			
			// 赋值引脚端口和引脚号
			pGpioPortSCL = pPortScl;
			GpioPinSCL = pinScl;
			pGpioPortCS = pPortCs;
			GpioPinCS = pinCs;
			pGpioPortMOSI = pPortMosi;
			GpioPinMOSI = pinMosi;
			pGpioPortMISO = pPortMiso;
			GpioPinMISO = pinMiso;
			pGpioPortWP = pPortWp;
			GpioPinWP = pinWp;
			pGpioPortHOLD = pPortHold;
			GpioPinHOLD = pinHold;

			/* Memory Orgnizaton */
			addressType total_bytes = 32768;
			addressType page_nums = 512;
			addressType bytes_in_page = 64;
			uint8_t address_length = 8;
		}
};

class CX25C64 : public CX25cxx_Base
{
	public:
		CX25C64(GPIO_TypeDef 			*pPortScl,	uint16_t pinScl,
						GPIO_TypeDef 			*pPortCs,		uint16_t pinCs,
						GPIO_TypeDef			*pPortMosi,	uint16_t pinMosi,
						GPIO_TypeDef			*pPortMiso,	uint16_t pinMiso,
						GPIO_TypeDef			*pPortWp,		uint16_t pinWp,
						GPIO_TypeDef			*pPortHold,	uint16_t pinHold,
						GPIO_InitTypeDef	*pIoInitDef,
						uint8_t 					id)
		{
			
			// 赋值引脚端口和引脚号
			pGpioPortSCL = pPortScl;
			GpioPinSCL = pinScl;
			pGpioPortCS = pPortCs;
			GpioPinCS = pinCs;
			pGpioPortMOSI = pPortMosi;
			GpioPinMOSI = pinMosi;
			pGpioPortMISO = pPortMiso;
			GpioPinMISO = pinMiso;
			pGpioPortWP = pPortWp;
			GpioPinWP = pinWp;
			pGpioPortHOLD = pPortHold;
			GpioPinHOLD = pinHold;

			/* Memory Orgnizaton */
			addressType total_bytes = 8192;
			addressType page_nums = 256;
			addressType bytes_in_page = 32;
			uint8_t address_length = 8;
		}
};

#ifdef __cplusplus
}
#endif

#endif

