#ifndef __TIMER_H__
#define __TIMER_H__

///////////////////////////////////////////////////////////////////////////////
/* timer.h */

void          timer_init(void);					// 待ち時間発生初期化
void          wait(int msec);					// 待ち時間発生  引数に、必要なミリ秒を指定する

unsigned int  timer1_count;						// タイマー1のカウンタ
void          timer1_init(int interval);		// インターバルタイマー1を初期化
void          timer1_interval(int interval);	// インターバルを変更
void          timer1_start(void);				// タイマー1を開始
void          timer1_stop(void);				// タイマー1を停止

///////////////////////////////////////////////////////////////////////////////
#endif
