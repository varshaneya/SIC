/*					om sri sairam
 * This is the main contro file from where the pass-1 and pass-2 assemblers are called
 * Author: varshaneya
 * Rollno: 15013
 * 
 */

#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include"./SIC.h"

#define MAXLEN 32

extern SYMTAB* sicPass1(char source[],OPTAB optab[]);
extern void sicPass2(char source[],OPTAB optab[],SYMTAB *symtab);

void main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Please enter the program name to be assembled after ./SICASM\n");
		return;
		
	}
	
    OPTAB optab[]={
        {"ADD",0x18},{"LDA",0x0},{"SUB",0x1c},{"STA",0x0c},{"AND",0x40},{"COMP",0x28},{"DIV",0x24},{"J",0x3c},{"JEQ",0x30},{"JGT",0x34},{"JLT",0x38},{"JSUB",0x48},{"LDCH",0x50},
        {"LDL",0x08},{"LDX",0x04},{"MUL",0x20},{"OR",0x44},{"RD",0xd8},{"RSUB",0x4c},{"STCH",0x54},{"STL",0x14},{"STS",0x7c},{"STX",0x10},{"TD",0xe0},{"TIX",0x2c},{"WD",0xdc}
    }; //initialise operand table
    
    SYMTAB *symtab = NULL;
    
    char source[MAXLEN];
    
    strcpy(source,argv[1]);
    
    symtab = sicPass1(source,optab);
    
    if(!symtab)     //some error encountered in pass 1
      return;
    
    sicPass2(source,optab,symtab);
    
}
