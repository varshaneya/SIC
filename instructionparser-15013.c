/*
this program takes a SIC instrucition and parses it to give the value of opcode and other fields

Done by 

varshaneya v
15013

*/

#include<stdio.h>
#include<string.h>

int checkInstruction(char ins[])
{

int i,flag = 0;

for(i=0; i< strlen(ins) ; i++)
	if((ins[i] >= 48 && ins[i] <= 57) || (ins[i] >= 65 && ins[i] <= 70) || (ins[i] >= 97 && ins[i] <= 102))
	++flag;


if(flag == strlen(ins))
return 1;

else 
return 0;

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

void convert2MacInstr(char hexIns[],char macIns[])
{

int i;

char binary[5];	//4 bits to represent a hex digit
macIns[0] = '\0';

for(i = 0; i< strlen(hexIns); i++)
{
	hex2bin(hexIns[i],binary);
	strcat(macIns,binary);
 //       puts(macIns);
}

macIns[32] = '\0';
}

void main()
{
char hexInstruction[9];	//max size of instruction in sic is 6

printf("Welcome to instruction parser\n");
printf("Enter a SIC/XE instruction in hex format: ");
scanf("%s",hexInstruction);

if(!checkInstruction(hexInstruction))	//check if the given input is a valid hex code
{
printf("Wrong hex code!! exiting\n");
return;		//exit program if it is an invalid hex code
}

char macInstruction[33],opcode[9];	//max size of machine instruction in SIC/XE is 32
int ctr;

convert2MacInstr(hexInstruction,macInstruction);	//converts the instruction in hex to machine code

//puts(macInstruction);

printf("\nThe instruction is successfully parsed and the different parts are: \n");

if(strlen(hexInstruction) == 2) //format 1 instruction
{
 printf("\nInstruction format 1 \nopcode is: %s\n",macInstruction);
 return;    //exit out of main
}

else if(strlen(hexInstruction) == 4)    //format 2 instruction
{
 printf("\nInstruction format 2 \nopcode is: ");
 
 for(ctr = 0; ctr<=7; ctr++)
     opcode[ctr] = macInstruction[ctr];
 opcode[ctr] = '\0';
 
 puts(opcode);
 
 printf("\nr1 = ");
 while(ctr<12)
 {
     printf("%c",macInstruction[ctr++]);
 }
 
 printf("\nr2 = ");
  while(ctr<16)
 {
     printf("%c",macInstruction[ctr++]);
 }
 
 putchar('\n');
 
 return;    //exit out of main
  
}

char n,i,x,b,p,e,disp[21];

for(ctr = 0; ctr<=5; ctr++)
opcode[ctr] = macInstruction[ctr];

opcode[ctr] = '\0';

n = macInstruction[ctr++];
i = macInstruction[ctr++];
x = macInstruction[ctr++];
b = macInstruction[ctr++];
p = macInstruction[ctr++];
e = macInstruction[ctr++];

if(e == '0')
printf("\nFormat 3 instruction\n");

else if(e == '1')
printf("\nFormat 4 instruction\n");

printf("opcode: ");
puts(opcode);

printf("n = %c\ni = %c\nx = %c\nb = %c\np = %c\ne = %c\n",n,i,x,b,p,e);

printf("Disp = ");

int dctr = 0;

for( ;ctr<strlen(macInstruction);ctr++)
{
	if(e == '0' && ctr == 25)
	{
	disp[dctr] = '\0';
	break;
	}

disp[dctr++] = macInstruction[ctr];
//printf("%c",disp[ctr]);

}

disp[dctr] = '\0';

puts(disp);

}
