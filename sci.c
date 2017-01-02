/**
 *	sci.c
 *	
 *	Suzuki Taro
 */



#include "3048.h"
#include "sci.h"
#include "sci_printf.c"
#include "timer.h"

////////////////////////////////////////////////////////////////////////////////

#define                       Qsize 81
static unsigned char          Qbuf[Qsize];
static unsigned char*         Qend;
static unsigned char*         Qin;
static unsigned char*         Qout;
static volatile unsigned char Qenable;

volatile unsigned char CTRL_C;

////////////////////////////////////////////////////////////////////////////////

// SCI1���犄����
void int_rxi1(void)
{
	switch(SCI1.SSR.BIT.RDRF){			// ��M��Ԕ���
	case 1:								// RDRF=1 �����M
		if(Qin == Qout && Qenable){
			sci1_printf("Buffer overflow.\n\n");
		}else{
			*Qin = SCI1.RDR;			// data ���o��/
			if(*Qin == 0x03) CTRL_C = 1;
			else             CTRL_C = 0;
			Qin++; if(Qin == Qend) Qin = Qbuf;
			Qenable = 1;
		}
		SCI1.SSR.BIT.RDRF = 0;			// ��M�t���O�N���A
		break;
	default:							// �G���[������
		SCI1.SSR.BYTE &= 0xc7;			// �G���[�t���O�N���A
		break;
	}
	return;
}

// SCI1������
void sci1_init(void)
{
	SCI1.SCR.BYTE = 0x00;				// SCI1�ݒ� stop,�����N���b�N
	SCI1.SMR.BYTE = 0x00;				// data8.stop1,pari non
	//SCI1.BRR = 51;					// 9600bps
	//SCI1.BRR = 207;					// 2400bps
	SCI1.BRR = 25;						// 19200bps
	wait(1);
	SCI1.SCR.BYTE = 0x30;				// Tx,Rx�L�� ,�����ݖ���
	SCI1.SSR.BYTE &= 0x80;				// �G���[�t���O�̃N���A
	
	Qenable = 0;
	Qin     = Qbuf;
	Qout    = Qbuf;
	Qend    = Qbuf + Qsize;
	SCI1.SCR.BIT.RIE = 1;				// �V���A���|�[�g�P�����݋���
	
	return;
}

// SCI1����P������M
unsigned char sci1_rx(void)
{
	unsigned char data;	
	
	while(Qenable == 0);					// ��M�ƃG���[�̃t���O�����܂ő҂�
	data = *Qout;							// �f�[�^���󂯎��data�ɕۑ�
	Qout++; if(Qout == Qend) Qout = Qbuf;
	if(Qout == Qin) Qenable=0;
	
	return data;
}

// SCI1�ɂP�������M
void sci1_tx(char data)
{
	while(SCI1.SSR.BIT.TDRE == 0);				// �����M�f�[�^��������܂ő҂�
	SCI1.TDR = data;							// ���M�f�[�^�̃Z�b�g
	SCI1.SSR.BIT.TDRE = 0;						// ���M�t���O�̃N���A
	return;
}

// SCI1���當�������M
void sci1_strtx(const char *str)
{
	while(*str != '\0'){						// ������\0�ɂȂ�܂ŌJ��Ԃ�
		sci1_tx(*str);							// 1�������M
		str++;									// ���̕����Ɉڂ�
	}
	return;
}

// SCI1����maxchar-1�����܂ŁA�܂��͕����܂ł̕������ǂݍ���
unsigned char* sci1_strrx(char* str, int maxchar)
{
	unsigned char data = '\0';
	int  i;
	for(i=0; i<maxchar-1 && data!='\r'; i++){	// ���^�[�����͂����o
		data = sci1_rx();						// 1������M
		if(data != '\r'){						// ���������͓ǂݍ��܂Ȃ�
			sci1_tx(data);
			str[i] = data;						// 1�����𕶎���Ɋi�[
		}else{
			i--;
		}
	}
	str[i] = '\0';								// �ǂݍ��񂾕�����̍Ō��'\0'��t������
	sci1_printf("\n");
	return str;									// �ߒl:�|�C���^str���̂���
}

// fread�ۂ�
unsigned int sci1_sread(void* buf, unsigned int size, unsigned int n)
{
	char* cbuf = (char*)buf;
	size_t i;
	for(i=0; i<size*n; i++){
		cbuf[i] = sci1_rx();
	}
	return i;
}

// DWORD��SCI1����Ǎ�
unsigned short sci1_sreadword(void)
{
	unsigned short var;
	var  = (unsigned short) sci1_rx();
	var += (unsigned short) sci1_rx() * 0x100;
	return var;
}

// WORD��SCI1����Ǎ�
unsigned long sci1_sreaddword(void)
{
	unsigned long var;
	var  = (unsigned long) sci1_rx();
	var += (unsigned long) sci1_rx() * 0x100;
	var += (unsigned long) sci1_rx() * 0x10000;
	var += (unsigned long) sci1_rx() * 0x1000000;
	return var;
}
