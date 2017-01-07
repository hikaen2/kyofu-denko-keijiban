/********************************************/
/* 恐怖電光掲示板2               2003/09/28 */
/*                              Suzuki Taro */
/*                                 H8-3048F */
/********************************************/
const char VersionString[] = {"Kyofu Denko Keijiban Version 2.0 Beta 4 Release 139"};

#include "3048.h"
#include "timer.h"
#include "sci.h"
#include "bmp.h"
#include "shell.h"

#define   PUBLIC
#include "public.h"

///////////////////////////////////////////////////////////////////////////////
// 入出力ポート初期化
void io_init(void)
{
    P1.DDR = 0xff;                      // Port1 出力 掲示板LED
    P2.DDR = 0xff; P2.PCR.BYTE = 0x00;  // Port2 出力 掲示板LED   プルアップoff
    PA.DDR = 0xff;                      // PortA 出力 掲示板LED
    PB.DDR = 0xff;                      // PortB 出力 掲示板LED
    P4.DDR = 0x00; P4.PCR.BYTE = 0xff;  // Port4 入力 操作用sw1-4 プルアップon
    P5.DDR = 0xff; P5.PCR.BYTE = 0x00;  // Port5 出力 表示LED     プルアップoff
    P8.DDR = 0x00;                      // Port8 入力 IRQ2,3
    
    LED_Front_Upper = (unsigned char*)&P1.DR.BYTE;
    LED_Front_Lower = (unsigned char*)&P2.DR.BYTE;
    LED_Back_Upper  = (unsigned char*)&PA.DR.BYTE;
    LED_Back_Lower  = (unsigned char*)&PB.DR.BYTE;
    return;
}

// Power On Self Test, ポート1,2,A,BのLEDを光らせるだけ
void post(void)
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
    return;
}

void irq_init(void)
{   
    INTC.ISCR.BYTE       = 0;   // IRQ Sense Control Register
    INTC.ISCR.BIT.IRQ3SC = 1;
    INTC.ISCR.BIT.IRQ2SC = 1;
    
    INTC.IER.BYTE        = 0;   // IRQ Enable Register
    INTC.IER.BIT.IRQ3E   = 1;
    INTC.IER.BIT.IRQ2E   = 1;
    
    //INTC.ISR.BYTE        = 0; // IRQ Status Register
    //INTC.ISR.BIT.IRQ3F   = 1;
    //INTC.ISR.BIT.IRQ2F   = 1;
    
    INTC.IPRA.BYTE       = 0;   // IRQ Priority Level
    INTC.IPRA.BIT._IRQ23 = 1;
    
    return;
}

// アームのフォトインタラプタからの割り込み
void int_irq2(void)
{
    LED_Front_Upper = (unsigned char*)&P1.DR.BYTE;
    LED_Front_Lower = (unsigned char*)&P2.DR.BYTE;
    LED_Back_Upper  = (unsigned char*)&PA.DR.BYTE;
    LED_Back_Lower  = (unsigned char*)&PB.DR.BYTE;
    Public_Count += Public_Scroll_Value;
    if(Public_Count <  0)         Public_Count= MAX_WIDTH-1;
    if(Public_Count >= MAX_WIDTH) Public_Count=0;   // スクロール量
    
    timer1_start(); // インターバルタイマーを起動
    
    return;
}

// アームのフォトインタラプタからの割り込み
void int_irq3(void)
{
    LED_Front_Upper = (unsigned char*)&PA.DR.BYTE;
    LED_Front_Lower = (unsigned char*)&PB.DR.BYTE;
    LED_Back_Upper  = (unsigned char*)&P1.DR.BYTE;
    LED_Back_Lower  = (unsigned char*)&P2.DR.BYTE;
    Public_Count += Public_Scroll_Value;
    if(Public_Count <  0)         Public_Count= MAX_WIDTH-1;
    if(Public_Count >= MAX_WIDTH) Public_Count=0;   // スクロール量
    
    timer1_start(); // インターバルタイマーを起動
    
    return;
}

// メイン
int main(void)
{
    char command_line[40];
    asm("andc.b #0x3f,ccr");            // 割り込みを有効
    initABMP(&Public_Abmp);
    Public_Count = 0;
    FLG_autosync = 1;
    Public_Scroll_Value = 1;
    
    io_init();                          // 入出力ポート初期化
    timer_init();                       // タイマー初期化
    sci1_init();                        // シリアルコントローラ初期化
    sci1_printf("\n%s\n", VersionString);   // バージョン表示
    post();                             // Power-On Self-Test
    //timer1_init(584);                 // タイマー1初期化(interval)
    timer1_init(1586);
    irq_init();                         // IRQ初期化
    
    // CommandPrompt
    for(;;){
        sci1_printf(">"); execute_command( sci1_strrx(command_line,40) );
    }
    
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

