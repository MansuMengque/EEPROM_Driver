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
	Msg.err = 0;
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr;
	device_address_byte.device_type = eeprom;
	
	switch(0)
	{
		case 0:
			Iic.start();
			Iic.byteSet(device_address_byte.byte);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err++;
				break;
			}
		case 1:
			Msg.readData = Iic.byteGet();
			if(Iic.ackGet() == Iic.nack)
			{
				; //no Iic.ack
			}
		default:
			break;
	}
	Iic.stop();
	return Msg.readData;
}

/* 随机读 
   输入：硬件地址，寄存地址，数据
   输出：数据*/
dataType CX24cxx_Base::readRandom(uint8_t device_addr, addressType addr)
{
	Msg.err = 0;
	uint8_t addr_abits = addr>>(address_bytes*8); // 内存地址占用硬件地址位数
	device_addr >>= address_use_a; // 清除硬件地址低位
	
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // 保留被内存地址占用的硬件地址
	device_address_byte.device_type = eeprom;
	
	switch(0)
	{
		case 0:
			Iic.start();
			Iic.byteSet(device_address_byte.byte);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err++;
				break;
			}
		case 1:// 发高位
			if(address_bytes > 1)
      {
				Iic.byteSet(addr>>8);
				if(Iic.ackGet() == Iic.nack)
				{
					Msg.err++;
					break;
				}
      }
		case 2:// 发低位
			Iic.byteSet(addr);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err++;
				break;
			}
		case 3:
			device_address_byte.rw = rd;
			Iic.start();
			Iic.byteSet(device_address_byte.byte);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err++;
				break;
			}
		case 5:
			Msg.readData = Iic.byteGet();
			if(Iic.ackGet() == Iic.nack)
			{
				//no Iic.ack
			}
	}
	Iic.stop();
	return Msg.readData;
}

/* 写指令
   输入：硬件地址，寄存地址，数据*/
uint8_t CX24cxx_Base::writeRandom(uint8_t device_addr, addressType addr, dataType data)
{
	Msg.err = 0;
	uint8_t addr_abits = addr>>(address_bytes*8); // 内存地址占用硬件地址位数
	device_addr >>= address_use_a; // 清除硬件地址低位

	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // 保留被内存地址占用的硬件地址
	device_address_byte.device_type = eeprom;
	
  wpSetValue(Bit_RESET);
	switch(0)
	{
		case 0:
			Iic.start();
			Iic.byteSet(device_address_byte.byte);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err++;
				break;
			}
		case 1:// 发高位
			if(address_bytes > 1)
			{
				Iic.byteSet(addr);
				if(Iic.ackGet() == Iic.nack)
				{
					Msg.err++;
					break;
				}
			}
		case 2:// 发低位
			Iic.byteSet(addr);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err++;
				break;
			}
		case 3:// 发数据
			Iic.byteSet(data);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err++;
				break;
			}
		default:
			break;
	}
	Iic.stop();
	IIC_WAIT_DELAY;
	return Msg.err;
}

/* 页写
   输入：硬件地址，页号，数据 */
uint8_t CX24cxx_Base::writePage(uint8_t device_addr, addressType page_addr, dataType data)
{
	Msg.err = 0;
	addressType addr = pageStartAddr(page_addr);
	uint8_t addr_abits = addr>>(address_bytes*8); // 内存地址占用硬件地址位数
	device_addr >>= address_use_a; // 清除硬件地址低位
	
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // 保留被内存地址占用的硬件地址
	device_address_byte.device_type = eeprom;
  wpSetValue(Bit_RESET);
	switch(0)
	{
		case 0:
			Iic.start();
			Iic.byteSet(device_address_byte.byte);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err ++;
				break;
			}
		case 1:// 发高位
			if(address_bytes > 1)
      {
				Iic.byteSet(addr>>8);
				if(Iic.ackGet() == Iic.nack)
				{
					Msg.err++;
					break;
				}
      }
		case 2: // 发低位
			Iic.byteSet(addr);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err ++;
				break;
			}
		case 3: // 发数据
			for(uint32_t i = 0; i < page_bytes; i++)
			{
				Iic.byteSet(data);
				if(Iic.ackGet() == Iic.nack)
				{
					Msg.err ++;
					break;
				}
			}
		default:
			break;
	}
	Iic.stop();
	IIC_WAIT_DELAY;
	return Msg.err;
}


/* 连读开始
   输入：硬件地址，起始寄存地址
   配合readSequential和readSequentialStop使用 */
uint8_t CX24cxx_Base::readSequentialStart(uint8_t device_addr, addressType start_addr)
{
	Msg.err = 0;
	uint8_t addr_abits = start_addr>>(address_bytes*8); // 内存地址占用硬件地址位数
	device_addr >>= address_use_a; // 清除硬件地址低位

	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // 保留被内存地址占用的硬件地址
	device_address_byte.device_type = eeprom;
	
	switch(0)
	{
		case 0:
			Iic.start();
			Iic.byteSet(device_address_byte.byte);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err++;
				break;
			}
		case 1:// 发高位
			if(address_bytes > 1)
      {
				Iic.byteSet(start_addr>>8);
				if(Iic.ackGet() == Iic.nack)
				{
					Msg.err++;
					break;
				}
      }
		case 2:// 发低位
			Iic.byteSet(start_addr);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err++;
				break;
			}
		case 3:
			Iic.start();
			device_address_byte.rw = rd;

			Iic.byteSet(device_address_byte.byte);
		default:
			break;
	}
	if(Msg.err != 0)
	{
		Iic.stop();
	}
	return Msg.err;
}

/* 连读
   连读开始后，每调用一次本函数即可获取一个数据
   配合readSequentialStart和readSequentialStop使用 */
dataType CX24cxx_Base::readSequential(void)
{
	Iic.ackSet(Iic.ack);
	Msg.readData = Iic.byteGet();
	return Msg.readData;
}

/* 连读结束
   结束连续读
   配合readSequentialStart和readSequential使用 */
void CX24cxx_Base::readSequentialStop(void)
{
	Iic.ackSet(Iic.nack);
	Iic.stop();
}
void CX24cxx_Base::softReset(uint8_t device_addr)
{
	Iic.start();
	// 发18个1
	Iic.byteSet(0xFF);
	Iic.byteSet(0xFF);
	Iic.bitsSet(0xFF, 2);
	Iic.start();
	Iic.stop();
}

