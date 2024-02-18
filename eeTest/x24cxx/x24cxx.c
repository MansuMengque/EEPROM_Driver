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

/* ҳ��ʱ����ҳ�����ת��Ϊ��ַ�� */
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

/* ����� 
   ���룺Ӳ����ַ���Ĵ��ַ������
   ���������*/
dataType CX24cxx_Base::readRandom(uint8_t device_addr, addressType addr)
{
	Msg.err = 0;
	uint8_t addr_abits = addr>>(address_bytes*8); // �ڴ��ַռ��Ӳ����ַλ��
	device_addr >>= address_use_a; // ���Ӳ����ַ��λ
	
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // �������ڴ��ַռ�õ�Ӳ����ַ
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
		case 1:// ����λ
			if(address_bytes > 1)
      {
				Iic.byteSet(addr>>8);
				if(Iic.ackGet() == Iic.nack)
				{
					Msg.err++;
					break;
				}
      }
		case 2:// ����λ
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

/* дָ��
   ���룺Ӳ����ַ���Ĵ��ַ������*/
uint8_t CX24cxx_Base::writeRandom(uint8_t device_addr, addressType addr, dataType data)
{
	Msg.err = 0;
	uint8_t addr_abits = addr>>(address_bytes*8); // �ڴ��ַռ��Ӳ����ַλ��
	device_addr >>= address_use_a; // ���Ӳ����ַ��λ

	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // �������ڴ��ַռ�õ�Ӳ����ַ
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
		case 1:// ����λ
			if(address_bytes > 1)
			{
				Iic.byteSet(addr);
				if(Iic.ackGet() == Iic.nack)
				{
					Msg.err++;
					break;
				}
			}
		case 2:// ����λ
			Iic.byteSet(addr);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err++;
				break;
			}
		case 3:// ������
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

/* ҳд
   ���룺Ӳ����ַ��ҳ�ţ����� */
uint8_t CX24cxx_Base::writePage(uint8_t device_addr, addressType page_addr, dataType data)
{
	Msg.err = 0;
	addressType addr = pageStartAddr(page_addr);
	uint8_t addr_abits = addr>>(address_bytes*8); // �ڴ��ַռ��Ӳ����ַλ��
	device_addr >>= address_use_a; // ���Ӳ����ַ��λ
	
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // �������ڴ��ַռ�õ�Ӳ����ַ
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
		case 1:// ����λ
			if(address_bytes > 1)
      {
				Iic.byteSet(addr>>8);
				if(Iic.ackGet() == Iic.nack)
				{
					Msg.err++;
					break;
				}
      }
		case 2: // ����λ
			Iic.byteSet(addr);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err ++;
				break;
			}
		case 3: // ������
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


/* ������ʼ
   ���룺Ӳ����ַ����ʼ�Ĵ��ַ
   ���readSequential��readSequentialStopʹ�� */
uint8_t CX24cxx_Base::readSequentialStart(uint8_t device_addr, addressType start_addr)
{
	Msg.err = 0;
	uint8_t addr_abits = start_addr>>(address_bytes*8); // �ڴ��ַռ��Ӳ����ַλ��
	device_addr >>= address_use_a; // ���Ӳ����ַ��λ

	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // �������ڴ��ַռ�õ�Ӳ����ַ
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
		case 1:// ����λ
			if(address_bytes > 1)
      {
				Iic.byteSet(start_addr>>8);
				if(Iic.ackGet() == Iic.nack)
				{
					Msg.err++;
					break;
				}
      }
		case 2:// ����λ
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

/* ����
   ������ʼ��ÿ����һ�α��������ɻ�ȡһ������
   ���readSequentialStart��readSequentialStopʹ�� */
dataType CX24cxx_Base::readSequential(void)
{
	Iic.ackSet(Iic.ack);
	Msg.readData = Iic.byteGet();
	return Msg.readData;
}

/* ��������
   ����������
   ���readSequentialStart��readSequentialʹ�� */
void CX24cxx_Base::readSequentialStop(void)
{
	Iic.ackSet(Iic.nack);
	Iic.stop();
}
void CX24cxx_Base::softReset(uint8_t device_addr)
{
	Iic.start();
	// ��18��1
	Iic.byteSet(0xFF);
	Iic.byteSet(0xFF);
	Iic.bitsSet(0xFF, 2);
	Iic.start();
	Iic.stop();
}

