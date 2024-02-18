#include	"x34cxx.h"
#include	"iic_software.h"
#include	"stdint.h"
#include	"timeout_delay.h" 

/*
			X34CXX
	A0	|1	8|	VCC
	A1	|2	7|	NC
	A2	|3	6|	SCL
	GND	|4	5|	SDA
*/


//Public-------------------------------------------------------------

/* ��ȡ��ǰ����ҳ
   ����꣺	PAGE_GET_FIRST
			PAGE_GET_SECOND*/
uint8_t CFt34cxx_Base::pageAddrGet(void)
{
	uint8_t current_page;
	Iic.start();
	Iic.byteSet(page_get);
	current_page = Iic.ackGet();
	Iic.byteSet(0x00);
	Iic.ackGet(); // ������
	Iic.byteSet(0x00);
	Iic.ackGet(); // ������
	// �� stop ָ��
	return current_page;
}

/* ���������ַ�Զ��ж�Ӧ�л���ҳ
   ���룺��ַ*/
addressType CFt34cxx_Base::pageAddrAutoSet(addressType addr)
{
	Msg.err = 0;
	if(addr < 0x100)
	{
		pageAddrSet(page_set_first);
		if(pageAddrGet() == page_get_second)
		{
			Msg.err++;
		}
	}
	else if(addr < 0x1ff)
	{
		pageAddrSet(page_set_second);
		if(pageAddrGet() == page_get_first)
		{
			Msg.err++;
		}
		addr -= 0x100;
	}
	else
		;
	return addr;
}

/* ҳ��ʱ����ҳ�����ת��Ϊ��ַ�� */
addressType CFt34cxx_Base::pageStartAddr(uint8_t page)
{
	addressType addr = 0;
	addr = page * bytes_per_page;
	return addr;
}

/* �л�ǰ��벿��
   ����꣺PAGE_SET_FIRST
					PAGE_SET_SECOND*/
uint8_t CFt34cxx_Base::pageAddrSet(dataType cmd)
{
	Msg.err = 0;
	switch(0)
	{
		case 0:
			Iic.start();
			Iic.byteSet(cmd);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err++;
				break;
			}
		case 1:
			Iic.byteSet(0x00);
			Iic.ackGet(); // ������
		case 2:
			Iic.byteSet(0x00);
			Iic.ackGet(); // ������
		default:
			break;
		// �� stop ָ��
	}
	return Msg.err;
}

/* ����ǰ��ַ */
dataType CFt34cxx_Base::readCurrentAddress(uint8_t device_addr)
{
	Msg.err = 0;
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr;
	device_address_byte.device_type = device_type_rw;
	
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
dataType CFt34cxx_Base::readRandom(uint8_t device_addr, addressType addr)
{
	Msg.err = 0;
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr;
	device_address_byte.device_type = device_type_rw;
	
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
		case 2:// ����λ
			Iic.byteSet(addr);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err++;
				break;
			}
		case 3:
			Iic.start();
			device_address_byte.rw = rd;

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
uint8_t CFt34cxx_Base::writeRandom(uint8_t device_addr, addressType addr, dataType data)
{
	Msg.err = 0;
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr;
	device_address_byte.device_type = device_type_rw;
	
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
			Iic.byteSet(addr);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err++;
				break;
			}
		case 2:// ������
			Iic.byteSet(data);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err ++;
				break;
			}
		default:
			break;
	}
	Iic.stop();
	WAIT_DELAY;
	return Msg.err;
}

/* ҳд
   ���룺Ӳ����ַ��ҳ�ţ����� */
uint8_t CFt34cxx_Base::writePage(uint8_t device_addr, addressType page_addr, dataType data)
{
	Msg.err = 0;
	addressType addr = pageStartAddr(page_addr);
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr;
	device_address_byte.device_type = device_type_rw;
	
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
		case 1: // ����λ
			Iic.byteSet(addr);
			if(Iic.ackGet() == Iic.nack)
			{
				Msg.err ++;
				break;
			}
		case 2: // ������
			for(uint32_t i = 0; i < bytes_per_page; i++)
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
	WAIT_DELAY;
	return Msg.err;
}


/* ������ʼ
   ���룺Ӳ����ַ����ʼ�Ĵ��ַ
   ���readSequential��readSequentialStopʹ�� */
uint8_t CFt34cxx_Base::readSequentialStart(uint8_t device_addr, addressType start_addr)
{
	Msg.err = 0;
	device_address_byte.rw = wr;
	device_address_byte.device_addr = device_addr;
	device_address_byte.device_type = device_type_rw;
	
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
dataType CFt34cxx_Base::readSequential(void)
{
	Iic.ackSet(Iic.ack);
	Msg.readData = Iic.byteGet();
	return Msg.readData;
}

/* ��������
   ����������
   ���readSequentialStart��readSequentialʹ�� */
void CFt34cxx_Base::readSequentialStop(void)
{
	Iic.ackSet(Iic.nack);
	Iic.stop();
}

/* �����λ */
void CFt34cxx_Base::softReset(uint8_t device_addr)
{
	Iic.start();
	// ��18��1
	Iic.byteSet(0xFF);
	Iic.byteSet(0xFF);
	Iic.bitsSet(0xFF, 2);
	Iic.start();
	Iic.stop();
}

