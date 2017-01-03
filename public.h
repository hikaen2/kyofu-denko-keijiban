/* public.h                   */
/* control of global variable */

#ifndef __PUBLIC_H__
#define __PUBLIC_H__
	#define  MAX_WIDTH  228
	#define  MAX_HEIGHT  16
	#define  CHAR_WIDTH   8
	#define  CHAR_HEIGHT 16
#endif



#ifdef PUBLIC
	#define PUBEXT
#else
	#define PUBEXT extern
#endif

#include "bmp.h"
PUBEXT ABMP           Public_Abmp;
PUBEXT unsigned char  Public_Abmp_Data[MAX_WIDTH * MAX_HEIGHT / 8];
PUBEXT unsigned char* LED_Front_Upper;
PUBEXT unsigned char* LED_Front_Lower;
PUBEXT unsigned char* LED_Back_Upper;
PUBEXT unsigned char* LED_Back_Lower;
PUBEXT unsigned int   Public_Count;
PUBEXT unsigned int   Public_Scroll_Value;
PUBEXT char           FLG_autosync;
PUBEXT unsigned char  Public_Cursor_X;

