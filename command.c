///////////////////////////////////////////////////////////////////////////////
/* command.c */

#include "3048.h"
#include <stdlib.h>
#include "command.h"
#include "sci.h"
#include "bmp.h"
#include "public.h"

///////////////////////////////////////////////////////////////////////////////
int cmd_dump( int argc, char* argv[] )
{
    dumpABMP(&Public_Abmp);
    sci1_printf("done.\n\n");
    return 0;
}

int cmd_exit( int argc, char* argv[] )
{
    sci1_printf("The system is halted.\n\n");
    exit(0);
}

int cmd_help( int argc, char* argv[] )
{
/*
    int i;
    for( i=0; i<CommandNumber; i++ ){
        sci1_printf(CommandTable[i].word); sci1_printf(", ");
    }
    sci1_printf("\n\n");
*/
    return 0;
}

int cmd_read( int argc, char* argv[] )
{
    int ret;
    sci1_printf("Send bitmap file.\n");
    ret = readABMP(&Public_Abmp);
    if(ret == 0) sci1_printf("done.\n\n");
    else         sci1_printf("error:%d.\n\n", ret);
    return ret;
}

int cmd_run( int argc, char* argv[] )
{
    //timer1_start();
    //irq_init();
    sci1_printf("done.\n");
    return 0;
}

int cmd_sync( int argc, char* argv[] )
{
    sci1_printf("ITU1.GRA = %d\n", ITU1.GRA);
    sci1_printf("done.\n");
    return 0;
}

int cmd_lock( int argc, char* argv[] )
{
    FLG_autosync = 0;
    sci1_printf("done.\n"); 
    return 0;
}

int cmd_unlock( int argc, char* argv[] )
{
    FLG_autosync = 1;
    sci1_printf("done.\n");
    return 0;
}

int cmd_power( int argc, char* argv[] )
{
    sci1_printf("done.\n");
    return 0;
}

int cmd_ver( int argc, char* argv[] )
{
    extern const char VersionString[];
    sci1_printf("%s\n\n", VersionString);
    return 0;
}

int cmd_reverse( int argc, char* argv[] )
{
    if     (Public_Scroll_Value ==  0) Public_Scroll_Value =  1;
    else if(Public_Scroll_Value ==  1) Public_Scroll_Value = -1;
    else if(Public_Scroll_Value == -1) Public_Scroll_Value =  1;
    return 0;
}

int cmd_stop( int argc, char* argv[] )
{
    Public_Scroll_Value = 0;
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
