
///////////////////////////////////////////////////////////////////////////////
/* timer.c */

#include "3048.h"
#include "timer.h"
#include "public.h"

///////////////////////////////////////////////////////////////////////////////

void timer_init(void){
	ITU0.TCR.BYTE     =   0x23;					// GRA�R���y�A�}�b�` clock 1/8 = 2MHz
	ITU0.GRA          = 0x07d0;					// GRA��2000�ɐݒ� = 1ms
	ITU.TSTR.BIT.STR0 =      0;					// �J�E���g��~���
	return;
}
void wait(int msec){
	int i;
	ITU.TSTR.BIT.STR0 = 1;						// ITU0 TCNT�J�E���g�J�n
	for(i=0; i<msec; i++){
		do{ }while(ITU0.TSR.BIT.IMFA == 0);		// TCNT = GRA�ɂȂ�܂ő҂�(1ms)
		ITU0.TSR.BIT.IMFA = 0;					// ���m�t���O��߂��čĊJ
	}
	ITU.TSTR.BIT.STR0 = 0;						// ITU0 TCNT�J�E���g�I��
	return;
}

///////////////////////////////////////////////////////////////////////////////
// timer1 (�C���^�[�o���^�C�}�[)
void timer1_init(int interval){					// �C���^�[�o���^�C�}�[1��������
	timer1_count      = MAX_WIDTH/2;
	ITU1.TCR.BYTE     = 0x23;					// GRA�R���y�A�}�b�` clock 1/8 = 2MHz
	ITU1.GRA          = interval;				// GRA��interval�ɐݒ�
	ITU1.TIER.BYTE    = 0xF9;					// ITU1��GRA�ɂ��R���y�A�}�b�`�����݂�����
	ITU.TSTR.BIT.STR1 = 0;						// �J�E���g��~���
}
void timer1_interval(int interval){				// �^�C�}�[1�̃C���^�[�o����ݒ�
	ITU1.GRA = interval;
}
void timer1_start(void){						// �^�C�}�[1���J�n
	ITU1.TCNT         = 0;
	ITU.TSTR.BIT.STR1 = 1;						// ITU1 TCNT�J�E���g�J�n

	if(FLG_autosync){							// Interval Calibration
		ITU1.GRA = (unsigned int) ((float)ITU1.GRA * ((float)timer1_count/(MAX_WIDTH/2+1)) +0.5);
	}
	
	timer1_count = 0;
	return;
}
void timer1_stop(void){							// �^�C�}�[1���~
	ITU.TSTR.BIT.STR1 = 0;						// �J�E���g��~���
}
void int_imia1(void)							// �^�C�}�[1�C���^�[�o�����荞��
{
	printlineABMP(&Public_Abmp, Public_Count + timer1_count,               LED_Front_Upper, LED_Front_Lower);
	printlineABMP(&Public_Abmp, Public_Count + timer1_count+(MAX_WIDTH/2), LED_Back_Upper,  LED_Back_Lower);
	if(timer1_count < 255) timer1_count++;	
	ITU1.TSR.BIT.IMFA = 0; 						// �����݌��m�t���O��߂��čĊJ
}

///////////////////////////////////////////////////////////////////////////////

