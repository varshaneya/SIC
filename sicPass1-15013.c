/*									om sri sai ram
 * this is pass-1 assembler for SIC architecture
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

extern int checkComment(char line[]);
extern void clearWhiteSpaceFront(char line[]);
extern unsigned int str2num(char num[],int base);
extern int insertSymtab(SYMTAB *symtab,char name[],unsigned int address);
extern SYMTAB* searchSymtab(SYMTAB *symtab,char name[]);
extern int searchOptab(OPTAB optab[],char name[]);
extern void instructionPartition(char instr[], char *instrParts[],int n);
extern unsigned int noOfBytes(char line[]);

SYMTAB* sicPass1(char source[],OPTAB optab[])
{
    FILE *srcFile, *intFile;
    srcFile = fopen(source,"r");

   
	source[strlen(source)-3] = 'i';
    source[strlen(source)-2] = 'n';
    source[strlen(source)-1] = 't';
    
    intFile = fopen(source,"w+");    //create an intermediate file for pass2
    
    if(srcFile == NULL)    //exit out of the program if the input does noe exist
    {
     printf("Source file does not exist... exiting\n");
     return NULL;
    }
    
    char *line;
    size_t size = MAXLEN;
    
    line = (char*) malloc (MAXLEN * sizeof(char));
    
    int i,flag,startEncountered = 0;
    char *instrParts[3];
    unsigned int LOCCTR,START;    //location counter and starting address
    
    for(i = 0;i<3;i++)
        instrParts[i] = (char *) malloc(MAXLEN * sizeof(char));
    
    //initialise the symbol table
    
    SYMTAB *symtab;
    
    symtab = (SYMTAB *) malloc (sizeof(SYMTAB));
    
    if(!symtab)
    {
        printf("Insufficient memory...\n");
        return NULL;
    }
    
    strcpy(symtab->name,"start");
    symtab->address = 0;
    symtab->next = NULL;

	fseek(intFile,7,SEEK_SET);	//leave initial space of 7 characters for the size of the file
    
    while(!feof(srcFile))    //loop till the end of source file
    {
        getline(&line,&size,srcFile);
        
        instructionPartition(line,instrParts,3);
        
        if(checkComment(line))      //the line checks for comments
            continue;
        
        if(!strcmp(instrParts[1],"START"))  //if opcode == "Start"
        {
            LOCCTR = str2num(instrParts[2],16);
			START = LOCCTR;
            ++startEncountered;
            fprintf(intFile,"%x %s",START,line);
            continue;
        }
        
        else if(!startEncountered)
        {
            START = LOCCTR = 0;
            fprintf(intFile,"%x %s",START,line);
	    	continue;
        }
        
        if(strcmp(instrParts[1],"END"))  //if the starting line is not END
        {
            if(strcmp(instrParts[0],""))
            {
                if(searchSymtab(symtab,instrParts[0]) == NULL)
                {
                    flag = insertSymtab(symtab,instrParts[0],LOCCTR);
                    
                    if(flag == 0)
                    {
                        printf("No memory left to create variables\n");
                        return NULL;
                    }
                    
                }
                
                else
                {
                    printf("Duplicate symbol %s detected\n",instrParts[0]);
                    return NULL;
                }
                                
            }
            
            if(searchOptab(optab,instrParts[1]))
			{
                fprintf(intFile,"%x  %s",LOCCTR,line);
				LOCCTR +=3;
			}
            
            else if(!strcmp(instrParts[1],"WORD"))
			{
                fprintf(intFile,"%x %s",LOCCTR,line);
				LOCCTR +=3;
			}
            
            else if(!strcmp(instrParts[1],"RESW"))
			{
           		fprintf(intFile,"%x %s",LOCCTR,line);
				LOCCTR += (3 * str2num(instrParts[2],10));
			}
            
            else if(!strcmp(instrParts[1],"BYTE"))
			{
                fprintf(intFile,"%x %s",LOCCTR,line);
				LOCCTR += noOfBytes(instrParts[2]);
			}
			
			else if(!strcmp(instrParts[1],"RESB"))
			{
				fprintf(intFile,"%x %s",LOCCTR,line);
				LOCCTR += str2num(instrParts[2],10);
				
			}
            
            else if(!strcmp(instrParts[0],"START"))
                printf("Invalid operand code %s\n",instrParts[1]);
        
        }
        
        
        else if(!strcmp(instrParts[1],"END"))
        {
	    	fprintf(intFile,"%x  %s",LOCCTR - 3,line);
	    	rewind(intFile);
            fprintf(intFile,"%06x\n",(LOCCTR - START));
        }
    }
        
    fclose(srcFile);    //close the source file
    fclose(intFile);    //close the intermediate file
    free(line);     //free up the dynamically allocated array
    for(i = 0;i<3;i++)
        free(instrParts[i]);
    
    return symtab;      
        
        
}
