#ifndef __BMP_H__
#define __BMP_H__

///////////////////////////////////////////////////////////////////////////////
/* bmp.h */

typedef struct {										// reArrayed Bitmap ���ёւ�DIB!?
	long				x, y;
	unsigned long		size;
	unsigned char*		data;
} ABMP;

void initABMP(ABMP* abmp);								// ABMP������������
int  readABMP(ABMP* abmp);								// ABMP��SCI1����ǂ�
void dumpABMP(const ABMP* abmp);						// ABMP���_���v
int  getpointABMP(const ABMP* abmp, int x, int y);		// ABMP�̍��W(x,y)�̒l��Ԃ�
void letpointABMP(ABMP* abmp, int x, int y, char bit);	// ABMP�̍��W(x,y)��bit���i�[����
void printlineABMP(const ABMP* abmp, unsigned int line, unsigned char* upper, unsigned char* lower);

///////////////////////////////////////////////////////////////////////////////
#endif
