/*									om sri sai ram
 * this is pass-2 assembler for SIC architecture
 * Author: varshaneya
 * Rollno: 15013
 * 
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdlib.h>

#include"./SIC.h"

#define MAXLEN 32
#define MAXTXTRCDSIZE 10

extern int searchOptab(OPTAB optab[],char name[]);
extern int isSymbol(char sym[]);
extern int isIndexed(char operand[],char rawOperand[]);
extern unsigned int getAddress(SYMTAB *symtab, char sym[]);
extern unsigned int assembleInstr(OPTAB optab[],char operand[],unsigned int address,int isIndex);
extern unsigned int handleConstants(char line[]);
extern void* freeSymtab(SYMTAB *symtab);
extern unsigned int str2num(char num[],int base);
extern void instructionPartition(char instr[], char *instrParts[],int n);
extern SYMTAB* searchSymtab(SYMTAB *symtab,char name[]);


void sicPass2(char source[],OPTAB optab[],SYMTAB *symtab)
{
    FILE *inFile,*objectFile;
    inFile = fopen(source,"r");
    
    source[strlen(source)-3] = 'o';
    source[strlen(source)-2] = 'b';
    source[strlen(source)-1] = 'j';
    
       
    objectFile = fopen(source,"w");
    
    if(inFile == NULL)
    {
        printf("Intermediate file does not exist...\n");
        return;
    }
    
    char *line,*operand;
    size_t size = MAXLEN;
    unsigned int sizeofFile,assmInstr[10],address,stAddTxtRcd,START;
    
    line = (char*) malloc (MAXLEN * sizeof(char));
    
    int i,flag,startEncountered = 0,isIndex=0,txtRcdCnt = 0;
    char *instrParts[4];
	
    
    for(i = 0;i<4;i++)
        instrParts[i] = (char *) malloc(MAXLEN * sizeof(char));
	
	operand = (char *) malloc(MAXLEN * sizeof(char));
    
    getline(&line,&size,inFile);
	line[strlen(line)-1] = '\0';
    
    sizeofFile = str2num(line,16);
    
    while(!feof(inFile))
    {
        getline(&line,&size,inFile);
       
        instructionPartition(line,instrParts,4);
        
        if(!strcmp(instrParts[2],"START"))
        {
            START = str2num(instrParts[3],16);
            fprintf(objectFile,"H%6s%06X%06X",instrParts[1],START,sizeofFile);
            stAddTxtRcd = START;
			continue;
	}
	
	if(strcmp(instrParts[2],"END"))		//if END directive is not encountered then perform the following
	{
		if(searchOptab(optab,instrParts[2]) && strcmp(instrParts[3]," "))	//if opcode is found then do this..
		{
			if(isSymbol(instrParts[3]))
			{
				isIndex = isIndexed(operand,instrParts[3]);
				
				if(searchSymtab(symtab,operand))
					address = getAddress(symtab,instrParts[3]);
				
				else if(!strcmp(instrParts[2],"RSUB"))
					address = 0;
					
				else
				{
					address = 0;
					printf("Error. undefined symbol %s\n",operand);
					return;
				}
				
			}
			
			else
				address = 0;
			
			assmInstr[txtRcdCnt++] = assembleInstr(optab,instrParts[2],address,isIndex);
		}
	
		else if(!strcmp(instrParts[2],"WORD") || !strcmp(instrParts[2],"BYTE"))
			assmInstr[txtRcdCnt++] = handleConstants(instrParts[3]);
		
	}
		
		
	if(txtRcdCnt >= MAXTXTRCDSIZE || !strcmp(instrParts[2],"END"))
	{
		fprintf(objectFile,"\nT%06X%02X",stAddTxtRcd,txtRcdCnt*3);
		stAddTxtRcd += (3 * txtRcdCnt);		//since all instructions are of equal length ( = 3 bytes)...
		
		i =0;
		while(i<txtRcdCnt)
		{
			fprintf(objectFile,"%06X",assmInstr[i]);
			i++;
		}
		
		txtRcdCnt = 0;
		
		
		if(!strcmp(instrParts[2],"END"))
		{
			fprintf(objectFile,"\nE%06X",START);
			break;
		}

	}
		
	
        
        
    }
    
    for(i = 0;i<4;i++)
        free(instrParts[i]);
    
    fclose(inFile);
    fclose(objectFile);
    free(line);
    freeSymtab(symtab);
    
    
    source[strlen(source)-3] = 'i';
    source[strlen(source)-2] = 'n';
    source[strlen(source)-1] = 't';
    
    remove(source);
}
