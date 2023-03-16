#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define PROGRAM 1
#define CONST 2
#define VAR 3
#define INTEGER 4
#define LONG 5
#define PROCEDURE 6
#define IF 7
#define THEN 8
#define WHILE 9
#define DO 10
#define READ 11
#define WRITE 12
#define BEGIN 13
#define END 14
#define ODD 15
#define ADD 16
#define SUB 17
#define MUL 18
#define DIV 19
#define EQU 20
#define NEQ 21
#define LES 22
#define LEQ 23
#define LAG 24
#define GEQ 25
#define DOT 26
#define COM 27
#define SEM 28
#define COL 29
#define ASS 30
#define LBR 31
#define RBR 32
#define INT 33
#define ID  34
#define EIN 35
#define lenth1 15
#define lenth2 17
struct {char name[21];int type;int addr;}
   indent[1000];/*为标识符表*/
struct st
{char name[21];int code;} sym;
int lenth=0;/*为标识符表长*/
FILE *f1;/* f1为输入文件指针*/
int line=0,row=0,val;
void getsym();char getchr();void error(int);
void program();void proghead();
void block();void consexpl();
void consdefi();void varexpl();
void conssuff();void vardefi();
void varsuff();void procdefi();
void typeil();void procedh();
void procsuff();void assipro();
void sentence();void suffix();
void ifsent();void read();
void whilsent();void idsuff();
void write();void compsent();
void exprsuff();void sentsuff();
void conditio();void termsuff();
void express();void term();
void factsuff();
void factor();void addoper();
void muloper();void respoper();
void argument();
int main(int argc,char *argv[])
{
	if((f1=fopen(argv[1],"r"))==NULL)
    {printf("connot open the file\n");
    exit(0);
	}
getsym();
program();
printf("the program is right\n");
fclose(f1);
return 1;
}
/* getsym()、getchr()、error(int);见词法分析程序*/
void getsym()
{static char a[lenth1][10]={"program","const","var","integer","long","procedure","if",
		 "then","while","do","read","write","begin","end","odd"
		 },
 d[lenth2][3]={"+","-","*","/","=","<>","<","<=",">",">=", ".",",",";",":",":=","(",")"},
 str[21],ch=' ';
 int i,n;
 while(isspace(ch))
	 ch=getchr();
 if(isalpha(ch))
   {n=0;
	while(isalpha(ch)||isalnum(ch))
	  {if(isalpha(ch))
	     ch=tolower(ch);
	   str[n++]=ch;
	   ch=getchr();
	   }
	str[n]='\0';
	for(i=0;i<lenth1;i++)
	  if(!strcmp(str,a[i])) break;
	if(i<lenth1)
	{strcpy(sym.name,a[i]);
	 sym.code=i+1;
	}
	else
	   {for(i=0;i<lenth;i++)
	      if(!strcmp(str,indent[i].name)) break;
	    if(i==lenth)
	       strcpy(indent[i].name,str);
		strcpy(sym.name,indent[i].name);
	    sym.code=34;
	   }
	 }
    else if(isalnum(ch))
	  { val=0;n=0;
	    while(isalnum(ch))
		{val=val*10+ch-'0';
		 sym.name[n++]=ch;
		 ch=getchr();
		 }
		sym.name[n]='\0';
	    sym.code=33;
       }
	   else
	    {n=0;
	     if(ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='='||ch=='.'||ch==','
		   ||ch==';'||ch=='('||ch==')')
		{str[n++]=ch;str[n]='\0';
		ch=getchr();
		for(i=0;i<lenth2;i++)
	      if(!strcmp(str,d[i]))
		  {strcpy(sym.name,str);
	       sym.code=i+16;
		  }	 
	   }
	   else {n=0;
		     if (ch=='>'||ch==':')
		      {str[n++]=ch;
		       if((ch=getchr())=='=')
			   {str[n++]=ch;ch=getchr();
			   }
			  }
			else 
			if(ch=='<')
			   {str[n++]=ch;ch=getchr();
		       if(ch=='='||ch=='>')
			   {str[n++]=ch;ch=getchr();
			   }
			}	       	 
		   else if(ch==-1)
		   {strcpy(sym.name,"");
	       sym.code=35;
		   }
		   else
              error(1);
		 } 
	     str[n]='\0';
		 for(i=0;i<lenth2;i++)
	     if(!strcmp(str,d[i]))
		 {strcpy(sym.name,str);
	     sym.code=i+16;
		 }
	   }
}
char getchr()
{char ch=fgetc(f1);
 if(ch=='\n')
 {row=1;
  line++;
 }
 else
	if(ch!=' '&&ch!='\t')
		 row++;
 return(ch);
}
void error(int n)
{printf("There are %d-error\n", n);
 exit(0);
}

void program()
{
	proghead();
	block();
	if(sym.code==DOT)
		getsym();
	else
		error(2);
}
void proghead()
{if(sym.code==PROGRAM)
 {getsym();
  if(sym.code==ID)
  {getsym();
   if(sym.code==SEM)
       getsym();
   else
    error(5);
  }
  else
	error(4);
 }
 else
  error(3);
}
void block()
{consexpl();
 varexpl();
 procdefi();
 compsent();
}
void consexpl()
{if(sym.code==CONST)
{getsym();
 consdefi();
 conssuff();
 if(sym.code==SEM)
   getsym();
 else
   error(6);
}
}
void consdefi()
{if(sym.code==ID)
 {getsym();
  if(sym.code==EQU)
  {getsym();
   if(sym.code==INT)
     getsym();
   else
      error(9);
  }
  else
	error(8);
}else error(7);
}
void conssuff()
{if(sym.code==COM)
{getsym();
  consdefi();
 conssuff();
// if(sym.code==SEM)
//   getsym();
// else
//   error(10);
}
}
void varexpl()
{if(sym.code==VAR)
{getsym();
 vardefi();
 varsuff();
}
}
void vardefi()
{if(sym.code==ID)
{getsym();
 idsuff();
 if(sym.code==COL)
 {getsym();
  typeil();
  if(sym.code==SEM)
   getsym();
  else
   error(12);
}
 else
	 error(11);
}
}
void varsuff()
{if(sym.code==ID)
{vardefi();
 varsuff();
}
}

void typeil()
{if(sym.code==INTEGER||sym.code==LONG)
   getsym();
 else
	 error(13);
}
void procdefi()
{if(sym.code==PROCEDURE)
{procedh();
 block();
 if(sym.code==SEM)
 {getsym();
  procsuff();
 }
 else
	 error(14);
}
}
void procedh()
{if(sym.code==PROCEDURE)
{getsym();
 if(sym.code==ID)
 {getsym();
  argument();
  if(sym.code==SEM)
	  getsym();
  else
	  error(16);
 }
 else
	 error(15);
}
}
void argument()
{if(sym.code==LBR)
     { getsym();
       if(sym.code==ID)
           {getsym();
            if(sym.code==COL)
              { getsym();
                typeil();
                if(sym.code==RBR)
                   getsym();
                else error(19);
              }
            else error(18);
           }
       else  error(17);
       }
}
void procsuff()
{if(sym.code==PROCEDURE)
{procedh();
 block();
 if(sym.code==SEM)
 {getsym();
  procsuff();
 }
 else
	 error(20);
}
}
void assipro()
{if(sym.code==ID)
{getsym();
 suffix();
}
else
  error(21);
}
void sentence()
{if(sym.code==ID)
   assipro();
 else if(sym.code==IF)
        ifsent();
      else if(sym.code==WHILE)
                whilsent();
	       else if(sym.code==READ)
                     read();
		        else if(sym.code==WRITE)
                          write();
				     else if(sym.code==BEGIN)
						     compsent();
}
void suffix()
{if(sym.code==ASS)
{getsym();
 express();
}
else if(sym.code==LBR)
        { getsym();
          express();
          if(sym.code==RBR)
               getsym();
          else error(22);
}
}
void ifsent()
{if(sym.code==IF)
{getsym();
 conditio();
 if(sym.code==THEN)
 {getsym();
 sentence();
 }
 else
	 error(24);
}
else error(23);
}
void whilsent()
{if(sym.code==WHILE)
{getsym();
 conditio();
 if(sym.code==DO)
 {getsym();
 sentence();
 }
 else
	 error(26);
}
else error(25);
}
void read()
{if(sym.code==READ)
{getsym();
 if(sym.code==LBR)
 {getsym();
  if(sym.code==ID)
  {getsym();
   idsuff();
   if(sym.code==RBR)
    getsym();
   else error(27);
  }
  else error(30);
 }
 else error(29);
}
else error(28);
}
void idsuff()
{if(sym.code==COM)
{getsym();
 if(sym.code==ID)
 {getsym();
  idsuff();
 }
 else
	 error(31);
}
}
void write()
{if(sym.code==WRITE)
{getsym();
 if(sym.code==LBR)
 {getsym();
  express();
  exprsuff();
  if(sym.code==RBR)
    getsym();
  else error(34);
 }
  else error(33);
 }
 else error(32);
}
void compsent()
{if(sym.code==BEGIN)
{getsym();
 sentence();
 sentsuff();
 if(sym.code==END)
   getsym();
 else error(36);
}
else error(35);
}
void exprsuff()
{if(sym.code==COM)
{getsym();
 express();
 exprsuff();
 }
}
void sentsuff()
{if(sym.code==SEM)
{getsym();
 sentence();
 sentsuff();
}
}
void conditio()
{if(sym.code==ODD)
{getsym();
 express();
}
else
{express();
 respoper();
 express();
}
}
void express()
{if(sym.code==ADD||sym.code==SUB)
 getsym();
 term();
 termsuff();
}
void term()
{factor();
 factsuff();
}
void termsuff()
{if(sym.code==ADD||sym.code==SUB)
{getsym();
 factor();
 factsuff();
}
}
void factsuff()
{if(sym.code==MUL||sym.code==DIV)
{getsym();
 factor();
 factsuff();
}
}
void factor()
{if(sym.code==ID)
{getsym();
 factsuff();
 }
 else if(sym.code==INT)
 {getsym();
  factsuff();
 }
 else if(sym.code==LBR)
 {getsym();
  express();
  if(sym.code==RBR)
    getsym();
  else error(38);
 }
 else error(37);
}
void addoper()
{if(sym.code==ADD||sym.code==SUB)
   getsym();
 else error(39);
}
void muloper()
{if(sym.code==MUL||sym.code==DIV)
 getsym();
else error(40);
}
void respoper()
{if(sym.code>=EQU&&sym.code<=GEQ)
  getsym();
 else error(41);
}

