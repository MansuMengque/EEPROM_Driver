#include	"x25cxx.h"
#include	"timeout_delay.h" 
#include	"stdint.h"
#include	"stm32f10x_gpio.h"
#include	"spi_software.h"

/*
			X25CXX
	#CS	|1	8|	VCC
	SO	|2	7|	#HOLD
	#WP	|3	6|	SCL
	GND	|4	5|	SI
*/

addressType CX25cxx_Base::pageStartAddr(uint8_t page)
{
	addressType addr = 0;
	addr = (addressType)page * bytes_in_page;
	return addr;
}

void CX25cxx_Base::eeInitial(void)
{
	op_length = 8;	
	op_wren = 0x06;	// 0B00000110 write enable
	op_wrdi = 0x04; // 0B00000100 write disable
	op_rdsr = 0x05;	// 0B00000101 read status register
	op_wrsr = 0x01;	// 0B00000001 write status register
	op_read = 0x03;	// 0B00000011 read data
	op_write = 0x02;// 0B00000010 write data	
	
	stu_reg_rdy_rdy = 0;
	stu_reg_rdy_bsy = 1;
	stu_reg_wen_diasble = 0;
	stu_reg_wen_enable = 1;
	stu_reg_bp_lev0 = 0;
	stu_reg_bp_lev1 = 1;
	stu_reg_bp_lev2 = 2;
	stu_reg_bp_lev3 = 3;
	stu_wpen_unprotected = 0;
	stu_wpen_protected = 1;
}


/* �ȴ��Ա�̽���
   0��ʾ��ɣ�1��ʾ�ȴ���ʱ */
uint8_t CX25cxx_Base::timeOutWait(void)
{
	err = 0;
	uint16_t counter = 0;
	status_byte.reg = cmdRDSR();
	while(status_byte.n_rdy == stu_reg_rdy_bsy)
	{
		status_byte.reg = cmdRDSR();
		delay_us(100);
		counter++;
		if(counter >= 60)
		{
			err = 1;
			break;
		}
		else
			;
	}
	return err;
}

/* ������׼дʹ������ʱ�� */
void CX25cxx_Base::cmdWREN(void)
{
	START;
	opSendWren();
	STOP;
}

/* ������׼д��������ʱ�� */
void CX25cxx_Base::cmdWRDI(void)
{
	START;
	opSendWrdi();
	STOP;
}

/* ������׼��״̬�Ĵ���ʱ�� */
dataType CX25cxx_Base::cmdRDSR(void)
{
	START;
	opSendRdsr();
	read_data = dataRead();
	STOP;
	return read_data;
}

/* ������׼д״̬�Ĵ���ʱ�� */
dataType CX25cxx_Base::cmdWRSR(dataType statu_reg)
{
	START;
	opSendWrsr();
	dataSend(statu_reg);
	STOP;
	return timeOutWait();
}

/* ������׼������ʱ�� */
dataType CX25cxx_Base::cmdREAD(addressType addr)
{//��ָ�����ַ��������
	START;
	opSendRead();
	addrSend(addr);
	read_data =  dataRead();
	STOP;
	return read_data;
}

/* ������׼д����ʱ�� */
void CX25cxx_Base::cmdWRITE(addressType my_addr, dataType my_data)
{
	START;
	opSendWrite();
	addrSend(my_addr);
	dataSend(my_data);
	STOP;
}

/* д����
   �ɹ�����0�� ��ʱ����1 */
uint8_t CX25cxx_Base::writeData(addressType my_addr, dataType my_data)
{
	cmdWRITE(my_addr, my_data);
	return timeOutWait();
}

dataType CX25cxx_Base::writePage(addressType addr, dataType my_data)
{
	START;
	opSendWrite();
	addrSend(addr);
	for(uint8_t j = 0; j < bytes_in_page; j++)
	{
		dataSend(my_data);
	}
	STOP;
	return timeOutWait();
}
