/* public.h                   */
/* control of global variable */

#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#define MAX_WIDTH  228
#define MAX_HEIGHT 16
#define CHAR_WIDTH  8
#define CHAR_HEIGHT 16

extern unsigned char VRAM[MAX_WIDTH * MAX_HEIGHT / 8];
extern unsigned int OFFSET;
extern unsigned int DELTA;
extern char AUTOSYNC;
extern int LINE1;
extern int LINE2;

/*
 * timer
 */
void timer0_init(void);                // 待ち時間発生初期化
void wait(int msec);                   // 待ち時間発生  引数に、必要なミリ秒を指定する
void timer1_init(int interval);        // インターバルタイマー1を初期化
void timer1_interval(int interval);    // インターバルを変更
void timer1_start(void);               // タイマー1を開始

/*
 * shell
 */
int execute_command(char* command_line);

/*
 * bmp
 */
int readABMP(void);                    // ABMPをSCI1から読む
void dumpABMP(void);                   // ABMPをダンプ

/*
 * sci
 */
void           sci1_init(void);                                           // SCI1初期化
unsigned char  sci1_rx(void);                                             // SCI1から１文字受信
void           sci1_tx(char data);                                        // SCI1に１文字送信
unsigned char* sci1_strrx(char* str, int maxchar);                        // SCI1から文字列を受信
void           sci1_strtx(const char* str);                               // SCI1に文字列を送信
unsigned int   sci1_sread(void* buf, unsigned int size, unsigned int n);  // SCI1からsize*n Byte 受信
unsigned short sci1_sreadword(void);                                      // SCI1から2Byte受信
unsigned long  sci1_sreaddword(void);                                     // SCI1から4Byte受信
short          sci1_printf(const char *format, ...);

#endif
