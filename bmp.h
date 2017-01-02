#ifndef __BMP_H__
#define __BMP_H__

///////////////////////////////////////////////////////////////////////////////
/* bmp.h */

typedef struct {										// reArrayed Bitmap 並び替えDIB!?
	long				x, y;
	unsigned long		size;
	unsigned char*		data;
} ABMP;

void initABMP(ABMP* abmp);								// ABMPを初期化する
int  readABMP(ABMP* abmp);								// ABMPをSCI1から読む
void dumpABMP(const ABMP* abmp);						// ABMPをダンプ
int  getpointABMP(const ABMP* abmp, int x, int y);		// ABMPの座標(x,y)の値を返す
void letpointABMP(ABMP* abmp, int x, int y, char bit);	// ABMPの座標(x,y)にbitを格納する
void printlineABMP(const ABMP* abmp, unsigned int line, unsigned char* upper, unsigned char* lower);

///////////////////////////////////////////////////////////////////////////////
#endif
