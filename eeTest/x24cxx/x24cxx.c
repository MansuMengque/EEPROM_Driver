#include	"x24cxx.h"
#include	"timeout_delay.h" 
#include	"stdint.h"
#include	"stm32f10x_gpio.h"
#include	"iic_software.h"
#include	"timeout_delay.h" 

//X24c16 Ft24c16;
/*
			X24CXX
	A0	|1	8|	VCC
	A1	|2	7|	WP
	A2	|3	6|	SCL
	GND	|4	5|	SDA
*/
volatile uint8_t temp = 0;

/* 页读时，将页的序号转换为地址号 */
addressType CX24cxx_Base::pageStartAddr(uint8_t page)
{
	addressType addr = 0;
	addr = (addressType)page * page_bytes;
	return addr;
}

dataType CX24cxx_Base::readCurrentAddress(uint8_t device_addr)
{
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr;
	device_address_byte.device_type = device_type;
	
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
dataType CX24cxx_Base::readRandom(uint8_t device_addr, addressType addr)
{
	err = 0;
	uint8_t addr_abits = addr>>(address_bytes*8); // 内存地址占用硬件地址位数
	
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // 保留被内存地址占用的硬件地址
	device_address_byte.device_type = device_type;
	
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
		case 1:// 发高位
			if(address_bytes > 1)
      {
				byteSet(addr>>8);
				if(ackGet() == nack)
				{
					err++;
					break;
				}
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
uint8_t CX24cxx_Base::writeRandom(uint8_t device_addr, addressType addr, dataType data)
{
	uint8_t err = 0;
	uint8_t addr_abits = addr>>(address_bytes*8); // 内存地址占用硬件地址位数

	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // 保留被内存地址占用的硬件地址
	device_address_byte.device_type = device_type;
	
  WRITE_ENABLE;
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
		case 1:// 发高位
			if(address_bytes > 1)
			{
				byteSet(addr);
				if(ackGet() == nack)
				{
					err++;
					break;
				}
			}
		case 2:// 发低位
			byteSet(addr);
			if(ackGet() == nack)
			{
				err++;
				break;
			}
		case 3:// 发数据
			byteSet(data);
			if(ackGet() == nack)
			{
				err++;
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
uint8_t CX24cxx_Base::writePage(uint8_t device_addr, addressType page_addr, dataType data)
{
	uint8_t err = 0;
	addressType addr = pageStartAddr(page_addr);
	uint8_t addr_abits = addr>>(address_bytes*8); // 内存地址占用硬件地址位数

	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // 保留被内存地址占用的硬件地址
	device_address_byte.device_type = device_type;
  WRITE_ENABLE;
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
		case 1:// 发高位
			if(address_bytes > 1)
      {
				byteSet(addr>>8);
				if(ackGet() == nack)
				{
					err++;
					break;
				}
      }
		case 2: // 发低位
			byteSet(addr);
			if(ackGet() == nack)
			{
				err ++;
				break;
			}
		case 3: // 发数据
			for(uint32_t i = 0; i < page_bytes; i++)
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
uint8_t CX24cxx_Base::readSequentialStart(uint8_t device_addr, addressType start_addr)
{
	err = 0;
	uint8_t addr_abits = start_addr>>(address_bytes*8); // 内存地址占用硬件地址位数

	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // 保留被内存地址占用的硬件地址
	device_address_byte.device_type = device_type;
	
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
		case 1:// 发高位
			if(address_bytes > 1)
      {
				byteSet(start_addr>>8);
				if(ackGet() == nack)
				{
					err++;
					break;
				}
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
dataType CX24cxx_Base::readSequential(void)
{
	ackSet(ack);
	read_data = byteGet();
	return read_data;
}

/* 连读结束
   结束连续读
   配合readSequentialStart和readSequential使用 */
void CX24cxx_Base::readSequentialStop(void)
{
	ackSet(nack);
	STOP;
}
void CX24cxx_Base::softReset(uint8_t device_addr)
{
	START;
	// 发18个1
	byteSet(0xFF);
	byteSet(0xFF);
	bitsSet(0xFF, 2);
	START;
	STOP;
}

