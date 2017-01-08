/*
 * 恐怖電光掲示板2
 * 2003/09/28
 * Suzuki Taro
 * H8-3048F
 */

const char* VersionString = "Kyofu Denko Keijiban Version 2.0 Beta 4 Release 139";

#include "3048.h"
#include "public.h"

/**
 * 入出力ポート初期化
 */
static void io_init(void)
{
    P1.DDR = 0xff;                      // Port1 出力 掲示板LED
    P2.DDR = 0xff; P2.PCR.BYTE = 0x00;  // Port2 出力 掲示板LED   プルアップoff
    PA.DDR = 0xff;                      // PortA 出力 掲示板LED
    PB.DDR = 0xff;                      // PortB 出力 掲示板LED
    P4.DDR = 0x00; P4.PCR.BYTE = 0xff;  // Port4 入力 操作用sw1-4 プルアップon
    P5.DDR = 0xff; P5.PCR.BYTE = 0x00;  // Port5 出力 表示LED     プルアップoff
    P8.DDR = 0x00;                      // Port8 入力 IRQ2,3
}

/**
 * Power On Self Test, ポート1,2,A,BのLEDを光らせるだけ
 */
static void post(void)
{
    unsigned char i, j;
    sci1_printf("POST ");
    P1.DR.BYTE = P2.DR.BYTE = PA.DR.BYTE = PB.DR.BYTE = 0xff; wait(400);
    P1.DR.BYTE = P2.DR.BYTE = PA.DR.BYTE = PB.DR.BYTE = 0x00; wait(400);
    sci1_printf("P1"); j=1; for(i=0;i<=8;i++){ P1.DR.BYTE=j; j=j<<1; wait(40); }
    sci1_printf("2");  j=1; for(i=0;i<=8;i++){ P2.DR.BYTE=j; j=j<<1; wait(40); }
    sci1_printf("A");  j=1; for(i=0;i<=8;i++){ PA.DR.BYTE=j; j=j<<1; wait(40); }
    sci1_printf("B");  j=1; for(i=0;i<=8;i++){ PB.DR.BYTE=j; j=j<<1; wait(40); }
    sci1_printf(" OK.\n\n");
}

static void irq_init(void)
{   
    INTC.ISCR.BYTE       = 0;   // IRQ Sense Control Register
    INTC.ISCR.BIT.IRQ3SC = 1;
    INTC.ISCR.BIT.IRQ2SC = 1;
    
    INTC.IER.BYTE        = 0;   // IRQ Enable Register
    INTC.IER.BIT.IRQ3E   = 1;
    INTC.IER.BIT.IRQ2E   = 1;
    
    INTC.IPRA.BYTE       = 0;   // IRQ Priority Level
    INTC.IPRA.BIT._IRQ23 = 1;
}

static int mod(int a, int b) {
  int mod = a % b;
  if (b > 0 ? mod < 0 : mod > 0) {
    mod += b;
  }
  return mod;
}

/**
 * アームのフォトインタラプタからの割り込み
 */
void int_irq2(void)
{
    LINE1 = 0;
    OFFSET = mod(OFFSET + DELTA, MAX_WIDTH);
    timer1_start(); // インターバルタイマーを起動
}

/**
 * アームのフォトインタラプタからの割り込み
 */
void int_irq3(void)
{
    LINE2 = 0;
    OFFSET = mod(OFFSET + DELTA, MAX_WIDTH);
    timer1_start(); // インターバルタイマーを起動
}

/**
 * メイン
 */
int main(void)
{
    char command_line[40];
    asm("andc.b #0x3f,ccr");            // 割り込みを有効
    io_init();                          // 入出力ポート初期化
    timer0_init();                      // タイマー初期化
    sci1_init();                        // シリアルコントローラ初期化
    sci1_printf("\n%s\n", VersionString);   // バージョン表示
    post();                             // Power-On Self-Test
    timer1_init(1586);                  // タイマー1初期化(interval)
    irq_init();                         // IRQ初期化

    // CommandPrompt
    for (;;) {
        sci1_printf(">");
	execute_command(sci1_strrx(command_line, 40));
    }

    return 0;
}
