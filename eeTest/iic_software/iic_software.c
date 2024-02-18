#include	"iic_software.h"
#include	"timeout_delay.h" 
#include	"stm32f10x_rcc.h"
// Public-------------------------------------------------------------CIic_Software
CIic_Software::CIic_Software(void)
{
	read_data = 0;
	ack = 0;
	nack = 1;
}

CIic_Software::CIic_Software(GPIO_TypeDef 		*pPortScl,		uint16_t pinScl,
														 GPIO_TypeDef 		*pPortSda,		uint16_t pinSda,
														 GPIO_InitTypeDef *pSclInitDef,
														 GPIO_InitTypeDef *pSdaInitDef,
														 uint8_t					id)
{
	CIic_Port_Init(pPortScl,		pinScl,
								 pPortSda,		pinSda,
								 pSclInitDef,
								 pSdaInitDef,
								 id);
}

void CIic_Software::CIic_Port_Init(GPIO_TypeDef 		*pPortScl,		uint16_t pinScl,
														 GPIO_TypeDef 		*pPortSda,		uint16_t pinSda,
														 GPIO_InitTypeDef *pSclInitDef,
														 GPIO_InitTypeDef *pSdaInitDef,
														 uint8_t					id)
{
	if(pPortScl == GPIOA || pPortSda == GPIOA)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if(pPortScl == GPIOB || pPortSda == GPIOB)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if(pPortScl == GPIOC || pPortSda == GPIOC)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	else if(pPortScl == GPIOD || pPortSda == GPIOD)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	else if(pPortScl == GPIOE || pPortSda == GPIOE)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	else if(pPortScl == GPIOF || pPortSda == GPIOF)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	else if(pPortScl == GPIOG || pPortSda == GPIOG)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	else
		;

	// 赋值引脚端口和引脚号
	pGpioPortSCL = pPortScl;
	GpioPinSCL = pinScl;
	pGpioPortSDA = pPortSda;
	GpioPinSDA = pinSda;
	// 赋值寄存器结构体
	mpSclStruct = pSclInitDef;
	mpSdaStruct = pSdaInitDef;
	err = 0; 
	read_data = 0;
	ack = 0;
	nack = 1;

	iicIoInitial();
	sclSetValue(Bit_SET);
	sdaSetValue(Bit_SET);
}

// Protected---------------------------------------------------------
void CIic_Software::start(void)
{
	IIC_DELAY;
	sdaSetValue(Bit_SET);
	sclSetValue(Bit_SET);
	IIC_SHORT_DELAY;
	sdaSetValue(Bit_RESET);
	IIC_SHORT_DELAY;
	sclSetValue(Bit_RESET);
}

void CIic_Software::stop(void)
{
	sclSetValue(Bit_RESET);
	sdaSetValue(Bit_RESET);
	IIC_SHORT_DELAY;
	sclSetValue(Bit_SET);
	IIC_SHORT_DELAY;
	sdaSetValue(Bit_SET);
}

/* 发送任意位数数据 */
void CIic_Software::bitsSet(dataType data, uint8_t length)
{
	uint8_t mark = 1<<(length-1);
	while(mark)
	{	
		if(data & mark) sdaSetValue(Bit_SET);
		else sdaSetValue(Bit_RESET);
		IIC_DELAY;
		sclSetValue(Bit_SET);
		IIC_DELAY;
		mark >>= 1;
		sclSetValue(Bit_RESET);
	}
}

/* 发送字节
   SCL上升沿写 */
void CIic_Software::byteSet(dataType data)
{
	uint8_t mark = 1<<(8-1);
	while(mark)
	{	
		(data & mark) ? sdaSetValue(Bit_SET) : sdaSetValue(Bit_RESET);
		IIC_DELAY;
		sclSetValue(Bit_SET);
		IIC_SHORT_DELAY;
		mark >>= 1;
		sclSetValue(Bit_RESET);
	}
	sdaSetValue(Bit_SET);
}

/* 获取字节
   SCL下降沿读 */
dataType CIic_Software::byteGet(void)
{
	uint8_t mark = 1<<(8-1);
	uint8_t read_bits = 0;
	
	sdaSetInput();
	while(mark)
	{	
		IIC_DELAY;
		sclSetValue(Bit_SET);
		IIC_SHORT_DELAY;
		if(sdaGetValue()) read_bits |= mark;
		sclSetValue(Bit_RESET);
		mark >>= 1;
	}
	sdaSetOutput();
	return read_bits;
}

/* 读取ACK
   输出宏：
   ACK
   NACK */
uint8_t CIic_Software::ackGet(void)
{
	uint8_t read_ack = 1;
	sdaSetInput();
	sclSetValue(Bit_SET);
	IIC_SHORT_DELAY;
	read_ack = sdaGetValue();
	sclSetValue(Bit_RESET);
	sdaSetOutput();
	return read_ack;
}

/* 发送ACK
   输入宏：ACK
   NACK */
void CIic_Software::ackSet(uint8_t ack)
{
	(ack == 1) ? sdaSetValue(Bit_SET) : sdaSetValue(Bit_RESET);
	IIC_SHORT_DELAY;
	sclSetValue(Bit_SET);
	IIC_DELAY;
	sclSetValue(Bit_RESET);
	IIC_DELAY;
	sdaSetValue(Bit_SET);
}
