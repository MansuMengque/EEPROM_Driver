#include	"iic_software.h"
#include	"timeout_delay.h" 

// Public-------------------------------------------------------------CIic_Software
CIic_Software::CIic_Software(void)
{
	read_data = 0;
	uint8_t ack = 0;
	uint8_t nack = 1;
}

CIic_Software::CIic_Software(GPIO_TypeDef 		*pPortScl,		uint16_t pinScl,
														 GPIO_TypeDef 		*pPortSda,		uint16_t pinSda,
														 GPIO_InitTypeDef *pSclInitDef,
														 GPIO_InitTypeDef *pSdaInitDef,
														 uint8_t					id)
{
	// ��ֵ���Ŷ˿ں����ź�
	pGpioPortSCL = pPortScl;
	GpioPinSCL = pinScl;
	pGpioPortSDA = pPortSda;
	GpioPinSDA = pinSda;
	// ��ֵ�Ĵ����ṹ��
	mpSclStruct = pSclInitDef;
	mpSdaStruct = pSdaInitDef;
	err = 0; 
	read_data = 0;
	uint8_t ack = 0;
	uint8_t nack = 1;

	iicIoInitial();
	SCL_HIGH;
	SDA_HIGH;
}
// Protected---------------------------------------------------------
void CIic_Software::start(void)
{
	DELAY;
	SDA_HIGH;
	SCL_HIGH;
	SHORT_DELAY;
	SDA_LOW;
	SHORT_DELAY;
	SCL_LOW;
}

void CIic_Software::stop(void)
{
	SCL_LOW;
	SDA_LOW;
	SHORT_DELAY;
	SCL_HIGH;
	SHORT_DELAY;
	SDA_HIGH;
}

/* ��������λ������ */
void CIic_Software::bitsSet(dataType data, uint8_t length)
{
	uint8_t mark = 1<<(length-1);
	while(mark)
	{	
		if(data & mark) SDA_HIGH;
		else SDA_LOW;
		DELAY;
		SCL_HIGH;
		DELAY;
		mark >>= 1;
		SCL_LOW;
	}
}

/* �����ֽ�
   SCL������д */
void CIic_Software::byteSet(dataType data)
{
	uint8_t mark = 1<<(8-1);
	while(mark)
	{	
		(data & mark) ? SDA_HIGH : SDA_LOW;
		DELAY;
		SCL_HIGH;
		SHORT_DELAY;
		mark >>= 1;
		SCL_LOW;
	}
	SDA_HIGH;
}

/* ��ȡ�ֽ�
   SCL�½��ض� */
dataType CIic_Software::byteGet(void)
{
	uint8_t mark = 1<<(8-1);
	uint8_t read_bits = 0;
	SDA_IN;
	while(mark)
	{	
		DELAY;
		SCL_HIGH;
		SHORT_DELAY;
		if(SDA) read_bits |= mark;
		SCL_LOW;
		mark >>= 1;
	}
	SDA_OUT;
	return read_bits;
}

/* ��ȡACK
   ����꣺
   ACK
   NACK */
uint8_t CIic_Software::ackGet(void)
{
	uint8_t read_ack = 1;
	SDA_IN;
	SCL_HIGH;
	SHORT_DELAY;
	read_ack = SDA;
	SCL_LOW;
	SDA_OUT;
	return read_ack;
}

/* ����ACK
   ����꣺ACK
   NACK */
void CIic_Software::ackSet(uint8_t ack)
{
	(ack == 1) ? SDA_HIGH : SDA_LOW;
	SHORT_DELAY;
	SCL_HIGH;
	DELAY;
	SCL_LOW;
	DELAY;
	SDA_HIGH;
}
