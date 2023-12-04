#include	"sys.h" 
#include	"timeout_delay.h" 

volatile uint16_t tick;
volatile uint16_t poweron;

static u8  fac_us=0;												// us��ʱ������			   
static u16 fac_ms=0;												// ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��

void delay_init(uint16_t sysclock)
{	//ѡ���ڲ�ʱ�� PLL = HIRC / 2 * 12 = 48M, HCLK / 8 = 6M
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=sysclock/8;												//Ϊϵͳʱ�ӵ�1/8  
	fac_ms=(u16)fac_us*1000;									//��OS��,����ÿ��ms��Ҫ��systickʱ����
}								    

/* ��ʱnus
   nusΪҪ��ʱ��us��. */
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 								// ʱ�����	  		 
	SysTick->VAL=0x00;        								// ��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	// ��ʼ����	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));			// �ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	// �رռ�����
	SysTick->VAL =0X00;      					 				// ��ռ�����	 
}
/* ��ʱnms
   ע��nms�ķ�Χ
   SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
   nms<=0xffffff*8*1000/SYSCLK
   SYSCLK��λΪHz,nms��λΪms
   ��72M������,nms<=1864 */
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;						// ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;												// ��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	// ��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));			// �ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	// �رռ�����
	SysTick->VAL =0X00;       								// ��ռ�����	  	    
} 

void poweronDelay(void)
{
	delay_ms(1000);
}

void waitStart(u32 nms)
{
	SysTick->LOAD = nms*fac_ms;								// ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;												// ��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	// ��ʼ����  
}

void waitBusy(u32 nms)
{
	u32 temp;
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));			// �ȴ�ʱ�䵽��
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	// �رռ�����
	SysTick->VAL =0X00;       								// ��ռ�����
}

void timeOutRest(void)
{
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	// �رռ�����
	SysTick->VAL =0x00;                       // ��ռ�����
}
