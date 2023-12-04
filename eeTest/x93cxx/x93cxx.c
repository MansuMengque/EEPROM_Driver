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
{	// 用来产生或等待回应，调用Timer产生相应超时
	uint8_t err = 2;
	u32 temp;
	DELAY;
	// 在x93cxx.c中修改
	#if(WAIT_AFTER_CS_FALL == 1)
	CS_LOW;
	DELAY;
	CS_HIGH;
	#endif
	
	waitStart(2); // 等待时间为ms
	do
	{
		temp=SysTick->CTRL;
		if(MISO == 0)
		{
			err = 1;
			break;
		}
	}while((temp&0x01)&&!(temp&(1<<16)));          // 等待时间到达
	
	waitStart(10); // 等待时间为ms
	do
	{
		temp=SysTick->CTRL;
		if(MISO == 1 && err == 1)
		{
			err = 0;
			break;
		}
	}while((temp&0x01)&&!(temp&(1<<16)));          // 等待时间到达
	timeOutRest();
	
	if(err > 0)
	{
		printf("等超时\t");
	}
	
	return err;
}

void CX93cxx_Base::start(void)
{ 	// 拉高CS产生开始信号
	START;
	// 操作SCL和MOSI发送一位开始位
	SCL_LOW;
	MOSI_HIGH;
	DELAY;
	SCL_HIGH;
	DELAY;
	SCL_LOW;
	DELAY;
}

void CX93cxx_Base::stop(void)
{ // 拉低CS产生结束信号
	DELAY;
	CS_LOW;
	DELAY;
}

dataType CX93cxx_Base::cmdREAD(addressType addr, uint8_t wordsLength)
{ // 发指令，发地址，读数据
	wordsLengthSet(wordsLength);
	start();
	opSend(op_read);
	addressSend(addr);
	read_data = dataGet();
	stop();
	return read_data;
}

void CX93cxx_Base::readDataStart(addressType addr, uint8_t wordsLength)
{
	wordsLengthSet(wordsLength);
	start();
	opSend(op_read);
	addressSend(addr);
}

void CX93cxx_Base::cmdEWEN(uint8_t wordsLength)
{ // 发指令，发地址
	wordsLengthSet(wordsLength);
	start();
	opSend(op_ewen);
	addressSend(address_ewen);
	stop();
}

void CX93cxx_Base::cmdEWDS(uint8_t wordsLength)
{ // 发指令，发地址
	wordsLengthSet(wordsLength);
	start();
	opSend(op_ewds);
	addressSend(address_ewds);
	stop();
}

uint8_t CX93cxx_Base::cmdERASE(addressType addr, uint8_t wordsLength)
{ // 发指令，发地址，返回芯片响应状态
	uint8_t err = 0;
	wordsLengthSet(wordsLength);
	start();
	opSend(op_erase);
	addressSend(addr);
	err = RequestWaiting();
	STOP;
	if(err > 0)
	{
		printf("擦超时\t");
	}
	return err;
}

uint8_t CX93cxx_Base::cmdWRITE(addressType addr, dataType data, uint8_t wordsLength)
{ // 发指令，发地址，发数据
	uint8_t err = 0;
	wordsLengthSet(wordsLength);
	start();
	opSend(op_write);
	addressSend(addr);
	dataSend(data);
	err = RequestWaiting();
	stop();
	if(err > 0)
	{
		printf("写超时\t");
	}
	return err;
}

uint8_t CX93cxx_Base::cmdERAL(uint8_t wordsLength)
{ // 发指令，发地址
	uint8_t err = 0;
	wordsLengthSet(wordsLength);
	start();
	opSend(op_eral);
	addressSend(address_eral);
	err = RequestWaiting();
	stop();
	if(err > 0)
	{
		printf("擦超时\t");
	}
	return err;
}

uint8_t CX93cxx_Base::cmdWRAL(dataType data, uint8_t wordsLength)
{
	uint8_t err = 0;
	wordsLengthSet(wordsLength);
	start();
	opSend(op_wral);
	addressSend(address_wral);
	dataSend(data);
	err = RequestWaiting();
	stop();
	if(err > 0)
	{
		printf("写超时\t");
	}
	return err;
}

uint8_t CX93cxx_Base::writeDataAll(dataType data, uint8_t wordsLength)
{ // 全片低压写
	uint8_t err = 0;
	for(addressType i = 0; i < address_numbers_current; i ++)
	{
		if(cmdWRITE(i, data, wordsLength))
		{
			err = 1;
			break;
		}
	}
	if(err > 0)
	{
		printf("写超时\t");
	}
	return err;
}

uint8_t CX93cxx_Base::eraseDataAll(uint8_t wordsLength)
{ // 全片低压擦
	uint8_t err = 0;
	for(addressType i = 0; i < address_numbers_current; i ++)
	{
		if(cmdERASE(i, wordsLength))
		{
			err = 1;
			break;
		}
	}
	if(err > 0)
	{
		printf("擦超时\t");
	}
	return err;
}


