/*
 * header file which contains structure definitions of some very important structures like optab and symtab 
 * 
 */

#include<stdio.h>

typedef struct OPTABSTR
{
  char operand[5];
  unsigned int opcode;
}OPTAB;

typedef struct SYMTABSTR
{
    char name[10];
    unsigned int address;
    struct SYMTABSTR *next;
}SYMTAB;

typedef struct ARGTAB
{
	char arguements[10][7];
	int noOfArg;
	
}ARGTAB;

typedef struct DEFTAB
{
	char *prototype;
	FILE *start,*end;
}DEFTAB;

typedef struct NAMTAB
{
	char name[7];
	DEFTAB macroDef;
	ARGTAB macroArg;
	struct NAMTAB *next;
}NAMTAB;