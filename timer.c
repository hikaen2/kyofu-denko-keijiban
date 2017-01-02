
///////////////////////////////////////////////////////////////////////////////
/* timer.c */

#include "3048.h"
#include "timer.h"
#include "public.h"

///////////////////////////////////////////////////////////////////////////////

void timer_init(void){
	ITU0.TCR.BYTE     =   0x23;					// GRAコンペアマッチ clock 1/8 = 2MHz
	ITU0.GRA          = 0x07d0;					// GRAを2000に設定 = 1ms
	ITU.TSTR.BIT.STR0 =      0;					// カウント停止状態
	return;
}
void wait(int msec){
	int i;
	ITU.TSTR.BIT.STR0 = 1;						// ITU0 TCNTカウント開始
	for(i=0; i<msec; i++){
		do{ }while(ITU0.TSR.BIT.IMFA == 0);		// TCNT = GRAになるまで待つ(1ms)
		ITU0.TSR.BIT.IMFA = 0;					// 検知フラグを戻して再開
	}
	ITU.TSTR.BIT.STR0 = 0;						// ITU0 TCNTカウント終了
	return;
}

///////////////////////////////////////////////////////////////////////////////
// timer1 (インターバルタイマー)
void timer1_init(int interval){					// インターバルタイマー1を初期化
	timer1_count      = MAX_WIDTH/2;
	ITU1.TCR.BYTE     = 0x23;					// GRAコンペアマッチ clock 1/8 = 2MHz
	ITU1.GRA          = interval;				// GRAをintervalに設定
	ITU1.TIER.BYTE    = 0xF9;					// ITU1のGRAによるコンペアマッチ割込みを許可
	ITU.TSTR.BIT.STR1 = 0;						// カウント停止状態
}
void timer1_interval(int interval){				// タイマー1のインターバルを設定
	ITU1.GRA = interval;
}
void timer1_start(void){						// タイマー1を開始
	ITU1.TCNT         = 0;
	ITU.TSTR.BIT.STR1 = 1;						// ITU1 TCNTカウント開始

	if(FLG_autosync){							// Interval Calibration
		ITU1.GRA = (unsigned int) ((float)ITU1.GRA * ((float)timer1_count/(MAX_WIDTH/2+1)) +0.5);
	}
	
	timer1_count = 0;
	return;
}
void timer1_stop(void){							// タイマー1を停止
	ITU.TSTR.BIT.STR1 = 0;						// カウント停止状態
}
void int_imia1(void)							// タイマー1インターバル割り込み
{
	printlineABMP(&Public_Abmp, Public_Count + timer1_count,               LED_Front_Upper, LED_Front_Lower);
	printlineABMP(&Public_Abmp, Public_Count + timer1_count+(MAX_WIDTH/2), LED_Back_Upper,  LED_Back_Lower);
	if(timer1_count < 255) timer1_count++;	
	ITU1.TSR.BIT.IMFA = 0; 						// 割込み検知フラグを戻して再開
}

///////////////////////////////////////////////////////////////////////////////

