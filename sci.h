/**
 *  sci.h
 *  
 *  Suzuki Taro
 */


#ifndef __SCI_H__
#define __SCI_H__

////////////////////////////////////////////////////////////////////////////////
/* sci.h */

void           sci1_init(void);                                             // SCI1初期化
unsigned char  sci1_rx(void);                                               // SCI1から１文字受信
void           sci1_tx(char data);                                          // SCI1に１文字送信
unsigned char* sci1_strrx(char* str, int maxchar);                          // SCI1から文字列を受信
void           sci1_strtx(const char* str);                                 // SCI1に文字列を送信
unsigned int   sci1_sread(void* buf, unsigned int size, unsigned int n);    // SCI1からsize*n Byte 受信
unsigned short sci1_sreadword(void);                                        // SCI1から2Byte受信
unsigned long  sci1_sreaddword(void);                                       // SCI1から4Byte受信
short          sci1_printf(const char *format, ...);

////////////////////////////////////////////////////////////////////////////////

#endif
