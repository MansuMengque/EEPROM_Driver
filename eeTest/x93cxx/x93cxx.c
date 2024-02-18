#include	"x93cxx.h"
#include	"timeout_delay.h" 
#include	"stdint.h"
#include	"stm32f10x_gpio.h"
#include	"usart.h"
#include	"spi_software.h"

/*
			93CX6
	CS	|1	8|	VCC
	SCL	|2	7|	NC
	DI	|3	6|	ORG
	DO	|4	5|	GND
*/

uint8_t CX93cxx_Base::RequestWaiting(void)
{	// ����������ȴ���Ӧ������Timer������Ӧ��ʱ
	Msg.err = 2;
	u32 temp;
	DELAY;
	// ��x93cxx.c���޸�
	#if(WAIT_AFTER_CS_FALL == 1)
	Spi.csSetValue(Bit_RESET);
	DELAY;
	Spi.csSetValue(Bit_RESET);
	#endif
	
	waitStart(2); // �ȴ�ʱ��Ϊms
	do
	{
		temp=SysTick->CTRL;
		if(Spi.misoGetValue() == 0)
		{
			Msg.err = 1;
			break;
		}
	}while((temp&0x01)&&!(temp&(1<<16)));          // �ȴ�ʱ�䵽��
	
	waitStart(10); // �ȴ�ʱ��Ϊms
	do
	{
		temp=SysTick->CTRL;
		if(Spi.misoGetValue() == 1 && Msg.err == 1)
		{
			Msg.err = 0;
			break;
		}
	}while((temp&0x01)&&!(temp&(1<<16)));          // �ȴ�ʱ�䵽��
	timeOutRest();
	
	if(Msg.err > 0)
	{
		printf("�ȳ�ʱ\t");
	}
	
	return Msg.err;
}

void CX93cxx_Base::start(void)
{ 	// ����CS������ʼ�ź�
	Spi.csSetValue(Bit_RESET);
	// ����SCL��MOSI����һλ��ʼλ
	Spi.sclSetValue(Bit_RESET);
	Spi.mosiSetValue(Bit_SET);
	DELAY;
	Spi.sclSetValue(Bit_SET);
	DELAY;
	Spi.sclSetValue(Bit_RESET);
	DELAY;
}

void CX93cxx_Base::stop(void)
{ // ����CS���������ź�
	DELAY;
	Spi.csSetValue(Bit_RESET);
	DELAY;
}

dataType CX93cxx_Base::cmdREAD(addressType addr, uint8_t wordsLength)
{ // ��ָ�����ַ��������
	wordsLengthSet(wordsLength);
	start();
	opSend(op_read);
	addressSend(addr);
	Msg.readData = dataGet();
	stop();
	return Msg.readData;
}

void CX93cxx_Base::readDataStart(addressType addr, uint8_t wordsLength)
{
	wordsLengthSet(wordsLength);
	start();
	opSend(op_read);
	addressSend(addr);
}

void CX93cxx_Base::cmdEWEN(uint8_t wordsLength)
{ // ��ָ�����ַ
	wordsLengthSet(wordsLength);
	start();
	opSend(op_ewen);
	addressSend(address_ewen);
	stop();
}

void CX93cxx_Base::cmdEWDS(uint8_t wordsLength)
{ // ��ָ�����ַ
	wordsLengthSet(wordsLength);
	start();
	opSend(op_ewds);
	addressSend(address_ewds);
	stop();
}

uint8_t CX93cxx_Base::cmdERASE(addressType addr, uint8_t wordsLength)
{ // ��ָ�����ַ������оƬ��Ӧ״̬
	Msg.err = 0;
	wordsLengthSet(wordsLength);
	start();
	opSend(op_erase);
	addressSend(addr);
	Msg.err = RequestWaiting();
	stop();
	if(Msg.err > 0)
	{
		printf("����ʱ\t");
	}
	return Msg.err;
}

uint8_t CX93cxx_Base::cmdWRITE(addressType addr, dataType data, uint8_t wordsLength)
{ // ��ָ�����ַ��������
	Msg.err = 0;
	wordsLengthSet(wordsLength);
	start();
	opSend(op_write);
	addressSend(addr);
	dataSend(data);
	Msg.err = RequestWaiting();
	stop();
	if(Msg.err > 0)
	{
		printf("д��ʱ\t");
	}
	return Msg.err;
}

uint8_t CX93cxx_Base::cmdERAL(uint8_t wordsLength)
{ // ��ָ�����ַ
	Msg.err = 0;
	wordsLengthSet(wordsLength);
	start();
	opSend(op_eral);
	addressSend(address_eral);
	Msg.err = RequestWaiting();
	stop();
	if(Msg.err > 0)
	{
		printf("����ʱ\t");
	}
	return Msg.err;
}

uint8_t CX93cxx_Base::cmdWRAL(dataType data, uint8_t wordsLength)
{
	Msg.err = 0;
	wordsLengthSet(wordsLength);
	start();
	opSend(op_wral);
	addressSend(address_wral);
	dataSend(data);
	Msg.err = RequestWaiting();
	stop();
	if(Msg.err > 0)
	{
		printf("д��ʱ\t");
	}
	return Msg.err;
}

uint8_t CX93cxx_Base::writeDataAll(dataType data, uint8_t wordsLength)
{ // ȫƬ��ѹд
	Msg.err = 0;
	for(addressType i = 0; i < address_numbers_current; i ++)
	{
		if(cmdWRITE(i, data, wordsLength))
		{
			Msg.err = 1;
			break;
		}
	}
	if(Msg.err > 0)
	{
		printf("д��ʱ\t");
	}
	return Msg.err;
}

uint8_t CX93cxx_Base::eraseDataAll(uint8_t wordsLength)
{ // ȫƬ��ѹ��
	Msg.err = 0;
	for(addressType i = 0; i < address_numbers_current; i ++)
	{
		if(cmdERASE(i, wordsLength))
		{
			Msg.err = 1;
			break;
		}
	}
	if(Msg.err > 0)
	{
		printf("����ʱ\t");
	}
	return Msg.err;
}


