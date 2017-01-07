///////////////////////////////////////////////////////////////////////////////
/* bmp.c */

#include "bmp.h"
#include "sci.h"
#include "public.h"
#include "bmp_def.h"

static __inline__ void bset(unsigned char* data, unsigned char nbit) __attribute__ ((always_inline));
static __inline__ void bclr(unsigned char* data, unsigned char nbit) __attribute__ ((always_inline));
static __inline__ unsigned char btst(const unsigned char* data, unsigned char nbit) __attribute__ ((always_inline));
static __inline__ char getbit(const unsigned char* data, int nbit);     // dataのnbit目のbitを返す
static __inline__ void letbit(unsigned char* data, int nbit, char bit); // dataのnbit目にbitを格納

///////////////////////////////////////////////////////////////////////////////

__inline__ void bset(unsigned char* data, unsigned char nbit)
{
    asm __volatile__ ("bset %0l,@%1"::"r"(nbit),"r"(data));
    return;
}
__inline__ void bclr(unsigned char* data, unsigned char nbit)
{
    asm __volatile__ ("bclr %0l,@%1"::"r"(nbit),"r"(data));
    return;
}
__inline__ unsigned char btst(const unsigned char* data, unsigned char nbit)
{
    unsigned char __ccr;
    asm __volatile__ (
        "btst   %1l,@%c2:8 \n\t"
        "stc    ccr,%0l \n\t"
        "and.b  #4:8,%0l"
        : "=r"(__ccr) : "r"(nbit), "r"(data) : "r0"
    );
    return __ccr;
}

// dataのnbit目のbitを返す
__inline__ char getbit(const unsigned char* data, int nbit)
{
    return btst(&data[nbit/8], 7-(nbit%8));
}

/*
// dataのnbit目のbitを返す
__inline__ char getbit(const unsigned char* data, int nbit)
{
    unsigned char mask = 1;
    mask = mask << (7-(nbit%8));
    return (data[nbit/8] & mask)!=0;
}
*/
// dataのnbit目にbitを格納
__inline__ void letbit(unsigned char* data, int nbit, char bit)
{
    if(bit) bset(&data[nbit/8], nbit%8);
    else    bclr(&data[nbit/8], nbit%8);
    return;
}

///////////////////////////////////////////////////////////////////////////////
// ABMPを初期化する
void initABMP(ABMP* abmp)
{
    int i;
    abmp->data = Public_Abmp_Data;
    for(i=0;   i<default_size; i++){
        abmp->data[i] = default_data[i];
    }
    abmp->size = default_size;
    abmp->x    = default_x;
    abmp->y    = default_y;
    return;
}

// ABMPの座標(x,y)の値を返す 多分未完成
int getpointABMP(const ABMP* abmp, int x, int y)
{
    if(x < 0 || abmp->x <= x) return 0;
    if(y < 0 || abmp->y <= y) return 0;
    return getbit(abmp->data, x*y+y);
}

// ABMPの座標(x,y)にbitを格納する
void letpointABMP(ABMP* abmp, int x, int y, char bit)
{
    if(x < 0 || abmp->x <= x) return;
    if(y < 0 || abmp->y <= y) return;
    letbit(abmp->data, y+x*abmp->y, bit);
    return;
}

// ABMPをSCI1から読む
int readABMP(ABMP* abmp)
{
    long linesize;
    int x,y,i;
    unsigned char temp;
    
    // ヘッダ読み込み
          if( sci1_sreadword()  != 0x4d42 )      return 1;  // bmfh.bfType
              sci1_sreaddword();                            // bmfh.bfSize
              sci1_sreadword();                             // bmfh.bfReserved1
              sci1_sreadword();                             // bmfh.bfReserved2
              sci1_sreaddword();                            // bmfh.bfOffBits
              sci1_sreaddword();                            // bmih.biSize
    abmp->x = sci1_sreaddword();                            // bmih.biWidth
    abmp->y = sci1_sreaddword(); if(abmp->y < 0) return 2;  // bmih.biHeight
          if( sci1_sreadword()  != 1 )           return 3;  // bmih.biPlanes
          if( sci1_sreadword()  != 1 )           return 4;  // bmih.biBitCount
          if( sci1_sreaddword() != 0 )           return 5;  // bmih.biCompression
              sci1_sreaddword();                            // bmih.biSizeImage
              sci1_sreaddword();                            // bmih.biXPelsPerMeter
              sci1_sreaddword();                            // bmih.biYPelsPerMeter
              sci1_sreaddword();                            // bmih.biClrUsed
              sci1_sreaddword();                            // bmih.biClrImportant
              sci1_sreaddword();                            // rgbq[0]
              sci1_sreaddword();                            // rgbq[1]

    linesize   = ((abmp->x+31)/32) * 32;
    abmp->size = abmp->x  * abmp->y / 8;

    for(y=abmp->y-1; y>=0; y-- ){
    for(x=0;  x<linesize;  x+=8){
        temp = sci1_rx();
        for(i=0; i<8; i++) {letpointABMP(abmp, x+i, y, getbit(&temp,i)); }
    }
    }
    return 0;
}

// ABMPをダンプする
void dumpABMP(const ABMP* abmp)
{
    int i;
    sci1_printf("abmp = 0x%08x\n", abmp);
    sci1_printf("x=%d, y=%d, size=%d, data=0x%08x\n", abmp->x, abmp->y, abmp->size, abmp->data);

    sci1_printf("data[] = {\n");
    for(i=0; i<abmp->size; i++){
        sci1_printf("0x%02x,", abmp->data[i]);
    }
    sci1_printf("};\n");
    return;
}

// ABMPをLEDに表示する
void printlineABMP(const ABMP* abmp, unsigned int line, unsigned char* upper, unsigned char* lower)
{
    line = line % MAX_WIDTH;
    if(line >= abmp->x) return;
    *upper = abmp->data[line*2];
    *lower = abmp->data[line*2+1];
    return;
}

///////////////////////////////////////////////////////////////////////////////
