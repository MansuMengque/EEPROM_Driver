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

/* ����� 
   ���룺Ӳ����ַ���Ĵ��ַ������
   ���������*/
dataType CX24cxx_Base::readRandom(uint8_t device_addr, addressType addr)
{
	err = 0;
	uint8_t addr_abits = addr>>(address_bytes*8); // �ڴ��ַռ��Ӳ����ַλ��
	
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // �������ڴ��ַռ�õ�Ӳ����ַ
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
		case 1:// ����λ
			if(address_bytes > 1)
      {
				byteSet(addr>>8);
				if(ackGet() == nack)
				{
					err++;
					break;
				}
      }
		case 2:// ����λ
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

/* дָ��
   ���룺Ӳ����ַ���Ĵ��ַ������*/
uint8_t CX24cxx_Base::writeRandom(uint8_t device_addr, addressType addr, dataType data)
{
	uint8_t err = 0;
	uint8_t addr_abits = addr>>(address_bytes*8); // �ڴ��ַռ��Ӳ����ַλ��

	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // �������ڴ��ַռ�õ�Ӳ����ַ
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
		case 1:// ����λ
			if(address_bytes > 1)
			{
				byteSet(addr);
				if(ackGet() == nack)
				{
					err++;
					break;
				}
			}
		case 2:// ����λ
			byteSet(addr);
			if(ackGet() == nack)
			{
				err++;
				break;
			}
		case 3:// ������
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

/* ҳд
   ���룺Ӳ����ַ��ҳ�ţ����� */
uint8_t CX24cxx_Base::writePage(uint8_t device_addr, addressType page_addr, dataType data)
{
	uint8_t err = 0;
	addressType addr = pageStartAddr(page_addr);
	uint8_t addr_abits = addr>>(address_bytes*8); // �ڴ��ַռ��Ӳ����ַλ��

	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // �������ڴ��ַռ�õ�Ӳ����ַ
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
		case 1:// ����λ
			if(address_bytes > 1)
      {
				byteSet(addr>>8);
				if(ackGet() == nack)
				{
					err++;
					break;
				}
      }
		case 2: // ����λ
			byteSet(addr);
			if(ackGet() == nack)
			{
				err ++;
				break;
			}
		case 3: // ������
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


/* ������ʼ
   ���룺Ӳ����ַ����ʼ�Ĵ��ַ
   ���readSequential��readSequentialStopʹ�� */
uint8_t CX24cxx_Base::readSequentialStart(uint8_t device_addr, addressType start_addr)
{
	err = 0;
	uint8_t addr_abits = start_addr>>(address_bytes*8); // �ڴ��ַռ��Ӳ����ַλ��

	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr<<address_use_a | addr_abits; // �������ڴ��ַռ�õ�Ӳ����ַ
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
		case 1:// ����λ
			if(address_bytes > 1)
      {
				byteSet(start_addr>>8);
				if(ackGet() == nack)
				{
					err++;
					break;
				}
      }
		case 2:// ����λ
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

/* ����
   ������ʼ��ÿ����һ�α��������ɻ�ȡһ������
   ���readSequentialStart��readSequentialStopʹ�� */
dataType CX24cxx_Base::readSequential(void)
{
	ackSet(ack);
	read_data = byteGet();
	return read_data;
}

/* ��������
   ����������
   ���readSequentialStart��readSequentialʹ�� */
void CX24cxx_Base::readSequentialStop(void)
{
	ackSet(nack);
	STOP;
}
void CX24cxx_Base::softReset(uint8_t device_addr)
{
	START;
	// ��18��1
	byteSet(0xFF);
	byteSet(0xFF);
	bitsSet(0xFF, 2);
	START;
	STOP;
}

