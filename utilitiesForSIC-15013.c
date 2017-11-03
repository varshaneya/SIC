/*						om sri sairam
 * This .c file contains the utilities for sic assembler and loader to work.
 * the utilities are:
 * String to number conversion
 * character to digit conversion
 * symbol table manipulation functions
 * assembling instructions
 * checking if an addressing mode is indexed or not
 * searching operand table
 * checking for comments
 * clearing the leading white spaces
 * calculating no of bytes to be allocated for a constant
 * address generation
 */

#define OPTABLEN 26

#include<stdio.h>
#include<stdlib.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"./SIC.h"

int char2num(char c);
unsigned int str2num(char num[],int base);
int insertSymtab(SYMTAB *symtab,char name[],unsigned int address);
SYMTAB* searchSymtab(SYMTAB *symtab,char name[]);
int searchOptab(OPTAB optab[],char name[]);
unsigned int noOfBytes(char line[]);
void clearWhiteSpaceFront(char line[]);
int checkComment(char line[]);
void* freeSymtab(SYMTAB *symtab);
int isSymbol(char sym[]);
unsigned int assembleInstr(OPTAB optab[],char operand[],unsigned int address,int isIndex);
int isIndexed(char operand[],char rawOperand[]);
unsigned int getAddress(SYMTAB *symtab, char sym[]);
char num2HexChar(int c);
void getOpcode(char hexOp[],unsigned int opcode);
void hex2bin(char hexChar,char binary[]);
void addressConversion(char hexAddr[],unsigned int address);
unsigned int handleConstants(char line[]);
void instructionPartition(char instr[], char *instrParts[],int n);

void instructionPartition(char instr[], char *instrParts[],int n)
{
    int i = 0,j=0,cnt;
    
    for(cnt = 0 ;cnt<n;cnt++)
    {
        while(instr[i] != ' ' && instr[i] != '\0' && instr[i] != '\n')
            {
                instrParts[cnt][j++] = instr[i++];
            }
        ++i;    
        instrParts[cnt][j] = '\0';
        clearWhiteSpaceFront(instrParts[cnt]);
        //puts(instrParts[cnt]);
        j=0;
    }
 
}


int char2num(char c)
{
    switch(c)
    {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'a': 
        case 'A': return 10;
        case 'b': 
        case 'B': return 11;
        case 'c': 
        case 'C': return 12;
        case 'd': 
        case 'D': return 13;
        case 'e': 
        case 'E': return 14;
        case 'f': 
        case 'F': return 15;
        default: return 0;
    }
    
    
}

unsigned int str2num(char num[],int base)
{
    int i,j;
    
    j = strlen(num) - 1;
    unsigned int ans = 0;
    
    for(i = 0; j>=0; i++,j--)
        ans += char2num(num[i]) * pow(base,j);
    
    return ans;
}

int insertSymtab(SYMTAB *symtab,char name[],unsigned int address)
{
	if(!strcmp(name," "))
		return 1;

    SYMTAB *temp, *start = symtab;
    
    temp = (SYMTAB *) malloc(sizeof(SYMTAB));
    
    if(!temp)
        return 0;
    
    temp->next = NULL;
    temp->address = address;
    strcpy(temp->name,name);
    
    while(start->next)
        start = start->next;
    
    start->next = temp;
    
    return 1;
    
}

SYMTAB* searchSymtab(SYMTAB *symtab,char name[])
{
    SYMTAB *temp = symtab->next;
    
    while(temp)
    {
        if(!strcmp(temp->name,name))    //element found
            return temp;
        
        temp = temp->next;
    }
    
    //printf("Symbol %s is not found\n",name);
    return NULL;       //element not found
}

int searchOptab(OPTAB optab[],char name[])
{
    int i;
    
    for(i = 0; i<OPTABLEN ;i++)
        if(!strcmp(optab[i].operand,name)) //if operand is found then return 1
            return 1;
    
    return 0;   //operand not found
    
}

unsigned int noOfBytes(char line[])
{
    int i,multiple;
    unsigned int byte = 0;
    
    if(line[0] == 'X' && line[1] == '\'')
    {
        i =2;
        
        while(line[i++] != '\'')
            byte++;
        
    return byte/2;
    }
    
    else if(line[0] == 'C' && line[1] == '\'')
    {
        i =2;
        
        while(line[i++] != '\'')
            byte++;
        
    return byte;
    }
    
    
}

void clearWhiteSpaceFront(char line[])
{
    int i;
    
    while(line[0] == ' ')
    {
        for(i = 1; i<=strlen(line);i++)
            line[i-1]=line[i];
    }
    
    
}

int checkComment(char line[])
{
    int i;
    
    clearWhiteSpaceFront(line);
    
    if(line[0] == '.')
        return 1;       //comments in SIC start with .
        
    else
        return 0;
        
    
    
}

void* freeSymtab(SYMTAB *symtab)
{
    SYMTAB *temp = NULL;
    
    while(symtab)
    {
        temp = symtab;
        symtab = symtab->next;
        free(temp);
        
    }
    
    return NULL;
}

unsigned int getAddress(SYMTAB *symtab, char sym[])
{
	SYMTAB *temp = searchSymtab(symtab,sym);
	
	if(temp)	//if the symbol is found...
		return temp->address;
	
	else
		return 0;
}

int isSymbol(char sym[])
{
	int i;
	
	if(!strcmp(sym,"") || !strcmp(sym,"\n"))
		return 1;
	
	
	for(i = 0; i<strlen(sym);i++)
		if((sym[i]>='A' && sym[i]<='Z')||(sym[i]>='a' && sym[i]<='z')||(sym[i]>='0' && sym[i]<='9')|| sym[i] == ',')
			continue;
		else
			return 0;
		
	return 1;	
}

void getOpcode(char hexOp[],unsigned int opcode)
{
	int temp,i=1;
	
	while(opcode)
	{
		temp = opcode%16;
		hexOp[i--] = num2HexChar(temp) ;
		opcode /= 16;
		
	}
	
	hexOp[2] = '\0';

	
}

void hex2bin(char hexChar,char binary[])
{
switch(hexChar)
{
	case '0': strcpy(binary,"0000"); break;
	case '1': strcpy(binary,"0001"); break;	
	case '2': strcpy(binary,"0010"); break;	
	case '3': strcpy(binary,"0011"); break;	
	case '4': strcpy(binary,"0100"); break;
	case '5': strcpy(binary,"0101"); break;
	case '6': strcpy(binary,"0110"); break;
	case '7': strcpy(binary,"0111"); break;
	case '8': strcpy(binary,"1000"); break;
	case '9': strcpy(binary,"1001"); break;
	case 'a': 
	case 'A': strcpy(binary,"1010"); break;
	case 'b': 
	case 'B': strcpy(binary,"1011"); break;
	case 'c': 
	case 'C': strcpy(binary,"1100"); break;
	case 'd': 
	case 'D': strcpy(binary,"1101"); break;
	case 'e': 
	case 'E': strcpy(binary,"1110"); break;
	case 'f': 
	case 'F': strcpy(binary,"1111"); break;
}


}

void addressConversion(char hexAddr[],unsigned int address)
{
	int temp,i=3;
	
	while(address)
	{
		temp = address%16;
		hexAddr[i--] = num2HexChar(temp) ;
		address /= 16;
		
	}
	
	for(i=0;i<4;i++)
		if(hexAddr[i] == 's')
			hexAddr[i]='0';
	
	hexAddr[4] = '\0';
	
}

unsigned int assembleInstr(OPTAB optab[],char operand[],unsigned int address,int isIndex)
{
	char instr[25]="\0",hexOp[3],binary[5],hexAddr[5] = "ssss",finalHexInstr[7];
	int i,j,k,flag =0;
	unsigned int opcode,ans;
	
	if(!strcmp(operand,"RSUB"))
	{
//		printf("RSUB reached\n");
		flag = 1;
	}
	
	for(i = 0; i<OPTABLEN ;i++)
		if(!strcmp(optab[i].operand,operand))
		{
			opcode = optab[i].opcode;
			break;
		}
	
	
	getOpcode(hexOp,opcode);
	
	hex2bin(hexOp[0],binary);
	strcpy(instr,binary);
	
	hex2bin(hexOp[1],binary);
	strcat(instr,binary);
	
	if(isIndex == 1)	//if indexed addressing is used then set x bit to 1 otherwise set it to zero
		instr[8] = '1';
	else
		instr[8] = '0';
	
	addressConversion(hexAddr,address);
	
	hex2bin(hexAddr[0],binary);
	
	for(i=9,j=1;i<=11;i++,j++)
		instr[i] = binary[j];
	
	//puts(hexAddr);
	
	for(i=1;i<=3;i++)
	{
		hex2bin(hexAddr[i],binary);
		strcat(instr,binary);
	}
		
	//puts(instr);
	instr[24] = '\0';

	for(i=0,k=0;k<strlen(instr);i++)
	{
		for(j=3,ans=0;j>=0;j--,k++)
			ans += char2num(instr[k]) * pow(2,j) ;
	
		finalHexInstr[i] = num2HexChar(ans);
		
	//	if(flag == 1)
	//	printf("%u\n",ans);
	}

	finalHexInstr[i] = '\0';
		
	//puts(finalHexInstr);
	return str2num(finalHexInstr,16);
	
}

int isIndexed(char operand[],char rawOperand[])
{
	int i;
	for(i = 0; i< strlen(rawOperand); i++)
	{
		if(rawOperand[i] == ',')
		{
			operand[i] = '\0';
			return 1;			
		}
		
		operand[i] = rawOperand[i];
	}
	
	operand[i] = '\0';
	return 0;
}


char num2HexChar(int c)
{
    switch(c)
    {
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 10: return 'A';
        case 11: return 'B';
        case 12: return 'C';
        case 13: return 'D';
        case 14: return 'E';
        case 15: return 'F';
    }
    
    
}

unsigned int handleConstants(char line[])
{
	
	char temp[3];

	if(line[0] == 'C')
		return line[2];


	else if(line[0] == 'X')
	{
		temp[0]=line[2];
		temp[1]=line[3];
		temp[2]='\0';
		return str2num(temp,16);

	}

	else
	{
		int j = strlen(line)-1;
		unsigned int objCode= 0;
		int i = 0;
		while(i<=j)
			objCode += char2num(line[i++]) * pow(10,j--);


	}

}
