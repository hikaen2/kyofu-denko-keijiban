#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "public.h"
#include "font_abmp.h"

int abmp_putchar(int c)
{
	short i;
	for(i=0; i<CHAR_WIDTH; i++){
		Public_Abmp.data[Public_Cursor_X * CHAR_WIDTH + i] = Font_Abmp_Data[c * CHAR_WIDTH + i];
	}
	Public_Cursor_X++;
	return 0;
}

#endif


