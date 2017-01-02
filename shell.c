/**
 *	shell.c
 *	
 *	Suzuki Taro
 */


///////////////////////////////////////////////////////////////////////////////

#include "3048.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "command.h"
#include "sci.h"

// �R�}���h���C���W�J
int commandline_deployment(char* command_line, int* argc, char* argv[]);

///////////////////////////////////////////////////////////////////////////////
// �R�}���h�e�[�u�����}�N���Ő���
typedef struct {
	char* name;
	int (*command)( int argc, char* argv[] );
} CMDTBL;
const CMDTBL CommandTable[] = {
	#define CMD(a)  { #a, cmd_ ## a },
	#include "command_def.h"
	#undef CMD
};
const unsigned char CommandNumber = sizeof(CommandTable) / sizeof(CommandTable[0]);


///////////////////////////////////////////////////////////////////////////////
#define ARGC_MAX 16

int execute_command(char* command_line)
{
	int   i, ret;
	int   argc;
	char* argv[ ARGC_MAX ];
	
	commandline_deployment( command_line, &argc, argv );
	if( argc==0 )  return 0;
	
	for( i=0; i<CommandNumber; i++ ){
		if( strcmp(argv[0], CommandTable[i].name) == 0 ){
			ret = CommandTable[i].command( argc, argv );
			sci1_printf("\n");
			return ret;
		}
	}
	
	sci1_printf("command not found.\n\n");
	return 1;
}

int commandline_deployment(char* command_line, int* argc, char* argv[])
{
	int i, j, ofs, length;
	
	length = strlen( command_line );
	for( i=0; command_line[i]!='\0'; i++ ){
		if( command_line[i]=='\b' ){			// �o�b�N�X�y�[�X�W�J
			ofs = (i!=0);
			for(j=i-ofs; j<=length-1-ofs; j++){
				command_line[j] = command_line[j+1+ofs];
			}
			length -= 1+ofs;
			i -= 1+ofs;
		}
		else if( 0x01 <= command_line[i] && command_line[i] <= 0x1f ){	// �ςȂ̍폜
			for(j=i; j<=length-1; j++){
				command_line[j] = command_line[j+1];
			}
			i--;
		}
	}
	
	*argc = 0;  i = 0;
	while( command_line[i]!='\0' ){
		while( command_line[i]==' ' && command_line[i]!='\0' ){			// �X�y�[�X����
			command_line[i] = '\0';
			i++;
		}
		if( command_line[i]!='\0' ){
			if( *argc < ARGC_MAX ) argv[(*argc)++] = &command_line[i];	// �R�}���h���C���W�J
			i++;
		}
		while( command_line[i]!=' ' && command_line[i]!='\0' ){			// ������ǂݔ�΂�
			i++;
		}
	}
	
	return *argc;
}


///////////////////////////////////////////////////////////////////////////////
