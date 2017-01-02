/**
 *	sci.h
 *	
 *	Suzuki Taro
 */


#ifndef __SCI_H__
#define __SCI_H__

////////////////////////////////////////////////////////////////////////////////
/* sci.h */

void           sci1_init(void);												// SCI1������
unsigned char  sci1_rx(void);												// SCI1����P������M
void           sci1_tx(char data);											// SCI1�ɂP�������M
unsigned char* sci1_strrx(char* str, int maxchar);							// SCI1���當�������M
void           sci1_strtx(const char* str);									// SCI1�ɕ�����𑗐M
unsigned int   sci1_sread(void* buf, unsigned int size, unsigned int n);	// SCI1����size*n Byte ��M
unsigned short sci1_sreadword(void);										// SCI1����2Byte��M
unsigned long  sci1_sreaddword(void);										// SCI1����4Byte��M
short          sci1_printf(const char *format, ...);

////////////////////////////////////////////////////////////////////////////////

#endif
