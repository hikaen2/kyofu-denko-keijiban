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

// SCI1から割込み
void int_rxi1(void)
{
	switch(SCI1.SSR.BIT.RDRF){			// 受信状態判定
	case 1:								// RDRF=1 正常受信
		if(Qin == Qout && Qenable){
			sci1_printf("Buffer overflow.\n\n");
		}else{
			*Qin = SCI1.RDR;			// data 取り出し/
			if(*Qin == 0x03) CTRL_C = 1;
			else             CTRL_C = 0;
			Qin++; if(Qin == Qend) Qin = Qbuf;
			Qenable = 1;
		}
		SCI1.SSR.BIT.RDRF = 0;			// 受信フラグクリア
		break;
	default:							// エラー発生時
		SCI1.SSR.BYTE &= 0xc7;			// エラーフラグクリア
		break;
	}
	return;
}

// SCI1初期化
void sci1_init(void)
{
	SCI1.SCR.BYTE = 0x00;				// SCI1設定 stop,内部クロック
	SCI1.SMR.BYTE = 0x00;				// data8.stop1,pari non
	//SCI1.BRR = 51;					// 9600bps
	//SCI1.BRR = 207;					// 2400bps
	SCI1.BRR = 25;						// 19200bps
	wait(1);
	SCI1.SCR.BYTE = 0x30;				// Tx,Rx有効 ,割込み無効
	SCI1.SSR.BYTE &= 0x80;				// エラーフラグのクリア
	
	Qenable = 0;
	Qin     = Qbuf;
	Qout    = Qbuf;
	Qend    = Qbuf + Qsize;
	SCI1.SCR.BIT.RIE = 1;				// シリアルポート１割込み許可
	
	return;
}

// SCI1から１文字受信
unsigned char sci1_rx(void)
{
	unsigned char data;	
	
	while(Qenable == 0);					// 受信とエラーのフラグが立つまで待つ
	data = *Qout;							// データを受け取りdataに保存
	Qout++; if(Qout == Qend) Qout = Qbuf;
	if(Qout == Qin) Qenable=0;
	
	return data;
}

// SCI1に１文字送信
void sci1_tx(char data)
{
	while(SCI1.SSR.BIT.TDRE == 0);				// 未送信データが送られるまで待つ
	SCI1.TDR = data;							// 送信データのセット
	SCI1.SSR.BIT.TDRE = 0;						// 送信フラグのクリア
	return;
}

// SCI1から文字列を受信
void sci1_strtx(const char *str)
{
	while(*str != '\0'){						// 文字が\0になるまで繰り返す
		sci1_tx(*str);							// 1文字送信
		str++;									// 次の文字に移る
	}
	return;
}

// SCI1からmaxchar-1文字まで、または復改までの文字列を読み込む
unsigned char* sci1_strrx(char* str, int maxchar)
{
	unsigned char data = '\0';
	int  i;
	for(i=0; i<maxchar-1 && data!='\r'; i++){	// リターン入力を検出
		data = sci1_rx();						// 1文字受信
		if(data != '\r'){						// 復改文字は読み込まない
			sci1_tx(data);
			str[i] = data;						// 1文字を文字列に格納
		}else{
			i--;
		}
	}
	str[i] = '\0';								// 読み込んだ文字列の最後に'\0'を付加する
	sci1_printf("\n");
	return str;									// 戻値:ポインタstrそのもの
}

// freadぽい
unsigned int sci1_sread(void* buf, unsigned int size, unsigned int n)
{
	char* cbuf = (char*)buf;
	size_t i;
	for(i=0; i<size*n; i++){
		cbuf[i] = sci1_rx();
	}
	return i;
}

// DWORDをSCI1から読込
unsigned short sci1_sreadword(void)
{
	unsigned short var;
	var  = (unsigned short) sci1_rx();
	var += (unsigned short) sci1_rx() * 0x100;
	return var;
}

// WORDをSCI1から読込
unsigned long sci1_sreaddword(void)
{
	unsigned long var;
	var  = (unsigned long) sci1_rx();
	var += (unsigned long) sci1_rx() * 0x100;
	var += (unsigned long) sci1_rx() * 0x10000;
	var += (unsigned long) sci1_rx() * 0x1000000;
	return var;
}
