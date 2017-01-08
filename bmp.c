/*
 * bmp.c
 */

#include "public.h"
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////

static void bset(unsigned char* data, unsigned char nbit)
{
    asm("bset %0l,@%1"::"r"(nbit),"r"(data));
}

static void bclr(unsigned char* data, unsigned char nbit)
{
    asm("bclr %0l,@%1"::"r"(nbit),"r"(data));
}

// bのnbit目のbitを返す
static uint8_t getbit(uint8_t b, uint8_t n)
{
    return (b & (1 << (7 - n))) == 0 ? 0 : 1;
}

// dataのnbit目にbitを格納
static void letbit(unsigned char* data, int nbit, char bit)
{
    if (bit == 1) {
        bset(&data[nbit / 8], nbit % 8);
    } else {
        bclr(&data[nbit / 8], nbit % 8);
    }
}

// ABMPの座標(x,y)にbitを格納する
static void pset(int x, int y, char b)
{
    letbit(VRAM, y + x * 16, b);
}

// ABMPをSCI1から読む
int readABMP(void)
{
    int abmpx;
    int abmpy;
  
    // ヘッダ読み込み
          if( sci1_sreadword()  != 0x4d42 )      return 1;  // bmfh.bfType
              sci1_sreaddword();                            // bmfh.bfSize
              sci1_sreadword();                             // bmfh.bfReserved1
              sci1_sreadword();                             // bmfh.bfReserved2
              sci1_sreaddword();                            // bmfh.bfOffBits
              sci1_sreaddword();                            // bmih.biSize
      abmpx = sci1_sreaddword();                            // bmih.biWidth
      abmpy = sci1_sreaddword(); if(abmpy < 0)   return 2;  // bmih.biHeight
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

    long linesize   = ((abmpx + 31) / 32) * 32;

    for (int y = abmpy - 1; y >= 0; y--) {
        for (int x = 0;  x < linesize; x += 8) {
            uint8_t byte = sci1_rx();
            for (int i = 0; i < 8; i++) {
                pset(x + i, y, getbit(byte, i));
            }
        }
    }
    return 0;
}

// ABMPをダンプする
void dumpABMP(void)
{
    //sci1_printf("abmp = 0x%08x\n", abmp);
    //sci1_printf("x=%d, y=%d, size=%d, data=0x%08x\n", abmp->x, abmp->y, abmp->size, Public_vram);

    sci1_printf("data[] = {\n");
    for (int i = 0; i < MAX_WIDTH * 2 ; i++) {
        sci1_printf("0x%02x,", VRAM[i]);
    }
    sci1_printf("};\n");
}

///////////////////////////////////////////////////////////////////////////////
