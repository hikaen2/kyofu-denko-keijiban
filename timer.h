#ifndef __TIMER_H__
#define __TIMER_H__

///////////////////////////////////////////////////////////////////////////////
/* timer.h */

void          timer_init(void);					// �҂����Ԕ���������
void          wait(int msec);					// �҂����Ԕ���  �����ɁA�K�v�ȃ~���b���w�肷��

unsigned int  timer1_count;						// �^�C�}�[1�̃J�E���^
void          timer1_init(int interval);		// �C���^�[�o���^�C�}�[1��������
void          timer1_interval(int interval);	// �C���^�[�o����ύX
void          timer1_start(void);				// �^�C�}�[1���J�n
void          timer1_stop(void);				// �^�C�}�[1���~

///////////////////////////////////////////////////////////////////////////////
#endif
