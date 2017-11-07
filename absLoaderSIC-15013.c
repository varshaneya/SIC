/*
 * program to simulate absolute loader for SIC architecture
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

extern unsigned int str2num(char num[],int base);
extern void hex2bin(char hexChar,char binary[]);
extern int char2num(char c);

typedef struct memoryInstruction
{
	unsigned byte : 24;
}memoryInstruction;

void loadInstrInMem(memoryInstruction *progArea,memoryInstruction instruction)
{
	static unsigned int counter = 0;
	*(progArea + counter) = instruction;
	++counter;
}

int main(int argc,char* argv[])
{
	if(argc != 2)
	{
		printf("Enter fullpath of object file after ./SICLDR\n");
		return -1;
	}
	
	FILE *objFile;
	objFile = fopen(argv[1],"r");
	
	char c,line[7],progName[7],temp[3];
	unsigned int START=0,txtRcdSize,progLen,i,j,stAddRcd,nxtRcd = 0;
	char instr[25];
	
	memoryInstruction *progArea = NULL;
	memoryInstruction memInstr;
	
	if(objFile == NULL)
	{
		printf("Error... object file does not exist\n");
		return -1;
	}
	
	c = fgetc(objFile);
	
	if(c != 'H')
	{
		printf("Error in object file.. header record not defined correctly\n");
		fclose(objFile);
		return -1;
	}
	
	fread(progName,sizeof(char),6,objFile);	//get the program name from the object file
	
	fread(line,sizeof(char),6,objFile); //get the address of the program
	START = str2num(line,16);
	
	fread(line,sizeof(char),6,objFile); //get the program length
	progLen = str2num(line,16);
	
	if(progLen <= 0 )
	{
		printf("Error in program length\n");
		fclose(objFile);
		return -1;
	}
	
	//allocate space in memeory for the instructions to be loaded
	
	progArea = malloc(sizeof(struct memoryInstruction) * progLen);
	
	if(progArea == NULL)
	{
		printf("Memory cannot be allocated for the program to be loaded.. so exiting\n");
		fclose(objFile);
		return -1;
	}
	
	//printf("%d\n",sizeof(struct memoryInstruction));
	
	c = fgetc(objFile);	//ignore the linefeed at the end of the line
	
	while(1)
	{
		c = fgetc(objFile);
		
		if(c == 'E')	//if an end record is encountered then break out of the loop...
			break;
		
		else if(c == 'T')
		{
			fread(line,sizeof(char),6,objFile);
			stAddRcd = str2num(line,16);
			
			fread(temp,sizeof(char),2,objFile);
			temp[2]='\0';
			txtRcdSize = str2num(temp,16);

			
			for(i=0;i<txtRcdSize;i=i+3)
			{
				fread(line,sizeof(char),6,objFile);
				line[6]='\0';
				memInstr.byte = 0;
				
				for(j = 0; j<6; j++)
				{
					memInstr.byte += char2num(line[j]);
					if(j!=5)
						memInstr.byte <<= 4;
				}
				
				loadInstrInMem(progArea,memInstr);
			}
			
			//break;
			c = fgetc(objFile);	//ignore the linefeed at the end of the line
		}
		
		else
		{
			printf("Error in header file\n");
			break;
		}
		
	}
	
	free(progArea);
	fclose(objFile);
	return 0;	
}