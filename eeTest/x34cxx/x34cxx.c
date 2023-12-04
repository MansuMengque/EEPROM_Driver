#include	"x34cxx.h"
#include	"timeout_delay.h" 
#include	"stdint.h"

/*
			X34CXX
	A0	|1	8|	VCC
	A1	|2	7|	NC
	A2	|3	6|	SCL
	GND	|4	5|	SDA
*/

//Protected----------------------------------------------------------
void CFt34c04::paraInitial(void)
{
	err = 0;
	read_data = 0;
	test_err_sign = 0;
	
	// 基础配置
	bytes_per_page = 16;
	bytes_per_half_memory = 256;
	page_in_memory = 32;
	page_in_half_memory = 16;
	
	// 切页设置
	page_set_first = 0x6c;
	page_set_second = 0x6e;
	page_get = 0x6d;
	
	// 功能设置
	rd = 1;
	wr = 0;
	device_type_rw = 0x0A; // 1010
	
	// 读取
	dataType page_get_first	= 0;
	dataType page_get_second = 1;
}

//Public-------------------------------------------------------------
CFt34c04::CFt34c04()
{
	;
}

CFt34c04::CFt34c04(GPIO_TypeDef 		*pPortScl,		uint16_t pinScl,
								 GPIO_TypeDef 		*pPortSda,		uint16_t pinSda,
								 GPIO_InitTypeDef *pSclInitDef,
								 GPIO_InitTypeDef *pSdaInitDef,
								 uint8_t					id)
{
	// 赋值引脚端口和引脚号
	pGpioPortSCL = pPortScl;
	GpioPinSCL = pinScl;
	pGpioPortSDA = pPortSda;
	GpioPinSDA = pinSda;
	// 赋值寄存器结构体
	mpSclStruct = pSclInitDef;
	mpSdaStruct = pSdaInitDef;

	device_id = id;
	paraInitial();
	SCL_HIGH;
	SDA_HIGH;
	SDA_OUT;
	SCL_OUT;
}

/* 获取当前所在页
   输出宏：	PAGE_GET_FIRST
			PAGE_GET_SECOND*/
uint8_t CFt34c04::pageAddrGet(void)
{
	uint8_t current_page;
	START;
	byteSet(page_get);
	current_page = ackGet();
	byteSet(0x00);
	ackGet(); // 不关心
	byteSet(0x00);
	ackGet(); // 不关心
	// 无 stop 指令
	return current_page;
}

/* 根据输入地址自动判断应切换的页
   输入：地址*/
addressType CFt34c04::pageAddrAutoSet(addressType addr)
{
	err = 0;
	if(addr < 0x100)
	{
		pageAddrSet(page_set_first);
		if(pageAddrGet() == page_get_second)
		{
			err++;
		}
	}
	else if(addr < 0x1ff)
	{
		pageAddrSet(page_set_second);
		if(pageAddrGet() == page_get_first)
		{
			err++;
		}
		addr -= 0x100;
	}
	else
		;
	return addr;
}

/* 页读时，将页的序号转换为地址号 */
addressType CFt34c04::pageStartAddr(uint8_t page)
{
	addressType addr = 0;
	addr = page * bytes_per_page;
	return addr;
}

/* 切换前后半部分
   输入宏：PAGE_SET_FIRST
					PAGE_SET_SECOND*/
uint8_t CFt34c04::pageAddrSet(dataType cmd)
{
	err = 0;
	switch(0)
	{
		case 0:
			START;
			byteSet(cmd);
			if(ackGet() == nack)
			{
				err++;
				break;
			}
		case 1:
			byteSet(0x00);
			ackGet(); // 不关心
		case 2:
			byteSet(0x00);
			ackGet(); // 不关心
		default:
			break;
		// 无 stop 指令
	}
	return err;
}

/* 读当前地址 */
dataType CFt34c04::readCurrentAddress(uint8_t device_addr)
{
	err = 0;
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr;
	device_address_byte.device_type = device_type_rw;
	
	switch(0)
	{
		case 0:
			START;
			byteSet(device_address_byte.byte);
			if(ackGet() == nack)
			{
				err++;
				break;
			}
		case 1:
			read_data = byteGet();
			if(ackGet() == nack)
			{
				; //no ack
			}
		default:
			break;
	}
	STOP;
	return read_data;
}

/* 随机读 
   输入：硬件地址，寄存地址，数据
   输出：数据*/
dataType CFt34c04::readRandom(uint8_t device_addr, addressType addr)
{
	err = 0;
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr;
	device_address_byte.device_type = device_type_rw;
	
	switch(0)
	{
		case 0:
			START;
			byteSet(device_address_byte.byte);
			if(ackGet() == nack)
			{
				err++;
				break;
			}
		case 2:// 发低位
			byteSet(addr);
			if(ackGet() == nack)
			{
				err++;
				break;
			}
		case 3:
			START;
			device_address_byte.rw = rd;

			byteSet(device_address_byte.byte);
			if(ackGet() == nack)
			{
				err++;
				break;
			}
		case 5:
			read_data = byteGet();
			if(ackGet() == nack)
			{
				//no ack
			}
	}
	STOP;
    return read_data;
}

/* 写指令
   输入：硬件地址，寄存地址，数据*/
uint8_t CFt34c04::writeRandom(uint8_t device_addr, addressType addr, dataType data)
{
	uint8_t err = 0;
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr;
	device_address_byte.device_type = device_type_rw;
	
	switch(0)
	{
		case 0:
			START;
			byteSet(device_address_byte.byte);
			if(ackGet() == nack)
			{
				err++;
				break;
			}
		case 1:// 发低位
			byteSet(addr);
			if(ackGet() == nack)
			{
				err++;
				break;
			}
		case 2:// 发数据
			byteSet(data);
			if(ackGet() == nack)
			{
				err ++;
				break;
			}
		default:
			break;
	}
	STOP;
	WAIT_DELAY;
	return err;
}

/* 页写
   输入：硬件地址，页号，数据 */
uint8_t CFt34c04::writePage(uint8_t device_addr, addressType page_addr, dataType data)
{
	uint8_t err = 0;
	addressType addr = pageStartAddr(page_addr);
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr;
	device_address_byte.device_type = device_type_rw;
	
	switch(0)
	{
		case 0:
			START;
			byteSet(device_address_byte.byte);
			if(ackGet() == nack)
			{
				err ++;
				break;
			}
		case 1: // 发低位
			byteSet(addr);
			if(ackGet() == nack)
			{
				err ++;
				break;
			}
		case 2: // 发数据
			for(uint32_t i = 0; i < bytes_per_page; i++)
			{
				byteSet(data);
				if(ackGet() == nack)
				{
					err ++;
					break;
				}
			}
		default:
			break;
	}
	STOP;
	WAIT_DELAY;
	return err;
}


/* 连读开始
   输入：硬件地址，起始寄存地址
   配合readSequential和readSequentialStop使用 */
uint8_t CFt34c04::readSequentialStart(uint8_t device_addr, addressType start_addr)
{
	err = 0;
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr;
	device_address_byte.device_type = device_type_rw;
	
	switch(0)
	{
		case 0:
			START;
			byteSet(device_address_byte.byte);
			if(ackGet() == nack)
			{
				err++;
				break;
			}
		case 2:// 发低位
			byteSet(start_addr);
			if(ackGet() == nack)
			{
				err++;
				break;
			}
		case 3:
			START;
			device_address_byte.rw = rd;

			byteSet(device_address_byte.byte);
		default:
			break;
	}
	if(err != 0)
	{
		STOP;
	}
	return err;
}

/* 连读
   连读开始后，每调用一次本函数即可获取一个数据
   配合readSequentialStart和readSequentialStop使用 */
dataType CFt34c04::readSequential(void)
{
	ackSet(ack);
	read_data = byteGet();
	return read_data;
}

/* 连读结束
   结束连续读
   配合readSequentialStart和readSequential使用 */
void CFt34c04::readSequentialStop(void)
{
	ackSet(nack);
	STOP;
}

/* 软件复位 */
void CFt34c04::softReset(uint8_t device_addr)
{
	START;
	// 发18个1
	byteSet(0xFF);
	byteSet(0xFF);
	bitsSet(0xFF, 2);
	START;
	STOP;
}

