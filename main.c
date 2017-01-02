/********************************************/
/* ���|�d���f����2               2003/09/28 */
/*                              Suzuki Taro */
/*                                 H8-3048F */
/********************************************/
const char VersionString[] = {"Kyofu Denko Keijiban Version 2.0 Beta 4 Release 139"};

#include "3048.h"
#include "timer.h"
#include "sci.h"
#include "bmp.h"
#include "shell.h"

#define   PUBLIC
#include "public.h"

///////////////////////////////////////////////////////////////////////////////
// ���o�̓|�[�g������
void io_init(void)
{
	P1.DDR = 0xff;						// Port1 �o�� �f����LED
	P2.DDR = 0xff; P2.PCR.BYTE = 0x00;	// Port2 �o�� �f����LED   �v���A�b�voff
	PA.DDR = 0xff;						// PortA �o�� �f����LED
	PB.DDR = 0xff;						// PortB �o�� �f����LED
	P4.DDR = 0x00; P4.PCR.BYTE = 0xff;	// Port4 ���� ����psw1-4 �v���A�b�von
	P5.DDR = 0xff; P5.PCR.BYTE = 0x00;	// Port5 �o�� �\��LED     �v���A�b�voff
	P8.DDR = 0x00;						// Port8 ���� IRQ2,3
	
	LED_Front_Upper = (unsigned char*)&P1.DR.BYTE;
	LED_Front_Lower = (unsigned char*)&P2.DR.BYTE;
	LED_Back_Upper  = (unsigned char*)&PA.DR.BYTE;
	LED_Back_Lower  = (unsigned char*)&PB.DR.BYTE;
	return;
}

// Power On Self Test, �|�[�g1,2,A,B��LED�����点�邾��
void post(void)
{
	unsigned char i, j;
	sci1_printf("POST ");
	P1.DR.BYTE = P2.DR.BYTE = PA.DR.BYTE = PB.DR.BYTE = 0xff; wait(400);
	P1.DR.BYTE = P2.DR.BYTE = PA.DR.BYTE = PB.DR.BYTE = 0x00; wait(400);
	sci1_printf("P1"); j=1; for(i=0;i<=8;i++){ P1.DR.BYTE=j; j=j<<1; wait(40); }
	sci1_printf("2");  j=1; for(i=0;i<=8;i++){ P2.DR.BYTE=j; j=j<<1; wait(40); }
	sci1_printf("A");  j=1; for(i=0;i<=8;i++){ PA.DR.BYTE=j; j=j<<1; wait(40); }
	sci1_printf("B");  j=1; for(i=0;i<=8;i++){ PB.DR.BYTE=j; j=j<<1; wait(40); }
	sci1_printf(" OK.\n\n");
	return;
}

void irq_init(void)
{	
	INTC.ISCR.BYTE       = 0;	// IRQ Sense Control Register
	INTC.ISCR.BIT.IRQ3SC = 1;
	INTC.ISCR.BIT.IRQ2SC = 1;
	
	INTC.IER.BYTE        = 0;	// IRQ Enable Register
	INTC.IER.BIT.IRQ3E   = 1;
	INTC.IER.BIT.IRQ2E   = 1;
	
	//INTC.ISR.BYTE        = 0;	// IRQ Status Register
	//INTC.ISR.BIT.IRQ3F   = 1;
	//INTC.ISR.BIT.IRQ2F   = 1;
	
	INTC.IPRA.BYTE       = 0;	// IRQ Priority Level
	INTC.IPRA.BIT._IRQ23 = 1;
	
	return;
}

// �A�[���̃t�H�g�C���^���v�^����̊��荞��
void int_irq2(void)
{
	LED_Front_Upper = (unsigned char*)&P1.DR.BYTE;
	LED_Front_Lower = (unsigned char*)&P2.DR.BYTE;
	LED_Back_Upper  = (unsigned char*)&PA.DR.BYTE;
	LED_Back_Lower  = (unsigned char*)&PB.DR.BYTE;
	Public_Count += Public_Scroll_Value;
	if(Public_Count <  0)         Public_Count= MAX_WIDTH-1;
	if(Public_Count >= MAX_WIDTH) Public_Count=0;	// �X�N���[����
	
	timer1_start();	// �C���^�[�o���^�C�}�[���N��
	
	return;
}

// �A�[���̃t�H�g�C���^���v�^����̊��荞��
void int_irq3(void)
{
	LED_Front_Upper = (unsigned char*)&PA.DR.BYTE;
	LED_Front_Lower = (unsigned char*)&PB.DR.BYTE;
	LED_Back_Upper  = (unsigned char*)&P1.DR.BYTE;
	LED_Back_Lower  = (unsigned char*)&P2.DR.BYTE;
	Public_Count += Public_Scroll_Value;
	if(Public_Count <  0)         Public_Count= MAX_WIDTH-1;
	if(Public_Count >= MAX_WIDTH) Public_Count=0;	// �X�N���[����
	
	timer1_start();	// �C���^�[�o���^�C�}�[���N��
	
	return;
}

// ���C��
int main(void)
{
	char command_line[40];
	asm("andc.b #0x3f,ccr");			// ���荞�݂�L��
	initABMP(&Public_Abmp);
	Public_Count = 0;
    FLG_autosync = 1;
    Public_Scroll_Value = 1;
    
	io_init();							// ���o�̓|�[�g������
	timer_init();						// �^�C�}�[������
	sci1_init();						// �V���A���R���g���[��������
	sci1_printf("\n%s\n", VersionString);	// �o�[�W�����\��
	post();								// Power-On Self-Test
	//timer1_init(584);					// �^�C�}�[1������(interval)
	timer1_init(1586);
	irq_init();							// IRQ������
	
	// CommandPrompt
	for(;;){
		sci1_printf(">"); execute_command( sci1_strrx(command_line,40) );
	}
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

