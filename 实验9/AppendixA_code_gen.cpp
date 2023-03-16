#include <stdio.h>
#include <ctype.h>
#include <string.h>
//#include <process.h>
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
struct ta {char name[21];int type;int addr;int layer;int attrib;int arguaddr;}
   indent[1000];/*为标识符表*/
struct st
{char name[21];int code;} sym,syb;
int lenth=0;/*为标识符表长*/
FILE *f1,*f2;/* f1为输入文件指针,f2为输出文件指针*/
int line=0,row=0,layer=0,inlabel=0;
int top,a[20];
int readflag=0,writeflag=0,mulflag=0,divflag=0,flag;
unsigned long val;
struct tb {int addr;int type;int attrib;int layer;} 
  TermStack[1000];
int TermLenth=-1;
struct ss {int incode;int level;} ;
ss ariSignStack[1000]={{0,0}};
int ariSignStackLenth=0;
void getsym();char getchr();void error(int n);
void program();void proghead();
void block(int *);void consexpl(int *);
void consdefi(int *);void varexpl(int *);
void conssuff(int *);void vardefi(int *);
void varsuff(int *);void procdefi();
int typeil();void procedh(int *);
void procsuff();void assipro(int *);
void sentence(int *);void suffix();
void ifsent(int *);void read(int *);
void whilsent(int *);void idsuff(int *);
void write(int *);void compsent(int *);
void exprsuff(int *);void sentsuff(int *);
void conditio(int *);void termsuff(int *);
void express(int *);void term(int *);
void factsuff(int *);
void factor(int *);void addoper();
void muloper();void respoper(int *);
int argument(int *);
int fillin(st,int,int);
int lookup(st symb,int layer);
void genInstruction(int *);
int main(int argc,char *argv[])
  {char ft[20],*fc;
	if((f1=fopen(argv[1],"r"))==NULL)
      {printf("connot open the file\n");
        exit(0);
   }
   if(argc<=2)
     {strcpy(ft,argv[1]);
      if((fc=strchr(ft,'.'))!=NULL)
	     strcpy(fc,".asm");
      else
	     strcat(ft,".asm");
     }
   else
     strcpy(ft,argv[2]);
  if((f2=fopen(ft,"w"))==NULL)
      {printf("connot open the file\n");
        exit(0);
   }
  getsym();
  program();
  printf("the program is right\n");
  fclose(f1);
  fclose(f2);

  return 1;
  }
void program()
{int address=2*layer+6;
 proghead();
 block(&address);
 if(sym.code==DOT)
 {getsym();
 if(readflag)/*如调用输入子程序则生成其子程序*/
 {fprintf(f2,"n_read:xor ch,ch\nxor ax,ax\nxor dx,dx\n");
 fprintf(f2,"n_01:mov cl,[si]\ninc si\ncmp cl,13\n");
 fprintf(f2,"jz n_01\ncmp cl,32\njz n_01\ncmp cl,43\n");
 fprintf(f2,"jz n_02\ncmp cl,45\njnz n_03\ninc ch\n");
 fprintf(f2,"n_02:mov cl,[si]\ninc si\nn_03:sub cl,48\n");
 fprintf(f2,"cmp cl,9\njg n_error\ncmp cl,0\njl n_error\n");
 fprintf(f2,"n_04:add ax,ax\nadc dx,dx\npush dx\npush ax\n");
 fprintf(f2,"add ax,ax\nadc dx,dx\nadd ax,ax\nadc dx,dx\n");
 fprintf(f2,"pop bx\nadd ax,bx\npop bx\nadc dx,bx\n");
 fprintf(f2,"add al,cl\nadc ah,0\nadc dx,0\nmov cl,[si]\n");
 fprintf(f2,"inc si\nsub cl,48\ncmp cl,9\njg n_05\n");
 fprintf(f2,"cmp cl,0\njnl n_04\nn_05:and ch,ch\n");
 fprintf(f2,"jz n_06\nxor dx,0ffffh\nneg ax\ncmc\n");
 fprintf(f2,"adc dx,0\njmp n_06\nn_error:mov dx,offset error\n");
 fprintf(f2,"mov ah,9\nint 21h\nn_06:ret\n");
 }
 if(writeflag)/*如调用输出子程序则生成其子程序*/
 {fprintf(f2,"n_write:mov si,offset conter\ndec si\n");
 fprintf(f2,"mov [si],BYTE PTR '$'\ndec si\nmov [si],BYTE PTR 32\ndec si\n");
 fprintf(f2,"xor bh,bh\nand dx,dx\njns n_07\nxor dx,0ffffh\nneg ax\ncmc\nadc dx,0\n");
 fprintf(f2,"inc bh\nn_07:xor bl,bl\nmov cx,32\nn_08:rcl ax,1\n");
 fprintf(f2,"rcl dx,1\nrcl bl,1\ncmp bl,10\njl n_09\n");
 fprintf(f2,"sub bl,10\nn_09:cmc\nloop n_08\nrcl ax,1\n");
 fprintf(f2,"rcl dx,1\nadd bl,48\ndec si\nmov [si],bl\n");
 fprintf(f2,"and ax,ax\njnz n_07\nand dx,dx\njnz n_07\n");
 fprintf(f2,"and bh,bh\njz n_010\ndec si\n");
 fprintf(f2,"mov [si],BYTE PTR '-'\n");
 fprintf(f2,"n_010:mov dx,si\nmov ah,09h\nint 21h\nret\n");
 }
 if(divflag)/*如调用四字节的除法子程序则生成其子程序*/
 {fprintf(f2,"n_divide:mov BYTE PTR conter,32\n");
 fprintf(f2,"mov BYTE PTR conter+1,0\nand dx,dx\njns n_011\n");
 fprintf(f2,"xor dx,0ffffh\nneg ax\ncmc\nadc dx,0\n");
 fprintf(f2,"xor BYTE PTR conter+1,0ffh\nn_011:mov bl,[si+2]\n");
 fprintf(f2,"and bl,bl\njns n_012\nxor WORD PTR [si+2],0ffffh\n");
 fprintf(f2,"neg WORD PTR [si]\ncmc\nadc WORD PTR [si+2],0\n");
 fprintf(f2,"xor BYTE PTR conter+1,0ffh\nn_012:\nxor bx,bx\n");
 fprintf(f2,"xor cx,cx\nn_013:rcl ax,1\nrcl dx,1\nrcl bx,1\n");
 fprintf(f2,"rcl cx,1\ncmp cx,WORD PTR [si+2]\njl n_014\n");
 fprintf(f2,"jg n_015\ncmp bx,WORD PTR [si]\njl n_014\n");
 fprintf(f2,"n_015:sub bx,WORD PTR [si]\nsbb cx,WORD PTR [si+2]\n");
 fprintf(f2,"n_014:cmc\ndec BYTE PTR conter\njnz n_013\n");
 fprintf(f2,"rcl ax,1\nrcl dx,1\nmov bl,BYTE PTR [conter+1]\nand bl,bl\n");
 fprintf(f2,"jz n_016\nxor dx,0ffffh\nneg ax\ncmc\nadc dx,0\n");
 fprintf(f2,"n_016:ret\n");
 }
 if(mulflag)/*如调用四字节的乘法子程序则生成其子程序*/
 {fprintf(f2,"n_muliti:mov BYTE PTR conter,0\n");
 fprintf(f2,"and dx,dx\njns n_017\nxor dx,0ffffh\nneg ax\n");
 fprintf(f2,"cmc\nadc dx,0\nxor BYTE PTR conter+1,0ffh\n");
 fprintf(f2,"n_017:mov bl,[si+2]\nand bl,bl\njns n_018\n");
 fprintf(f2,"xor WORD PTR [si+2],0ffffh\nneg WORD PTR [si]\n");
 fprintf(f2,"cmc\nadc WORD PTR [si+2],0\nxor BYTE PTR conter,0ffh\n");
 fprintf(f2,"n_018:mov cx,ax\nmov ax,dx\nmul WORD PTR [si]\n");
 fprintf(f2,"mov bx,ax\nmov ax,cx\nmul WORD PTR [si+2]\n");
 fprintf(f2,"add bx,ax\nmov ax,cx\nmul WORD PTR [si]\n");
 fprintf(f2,"add dx,bx\nmov bl,BYTE PTR conter\nand bl,bl\njz n_019\n");
 fprintf(f2,"xor dx,0ffffh\nneg ax\ncmc\nadc dx,0\nn_019:ret\n");
 }
 fprintf(f2,"prog_seg ends\nmain endp\nend n_start\n");
 }
 else
   error(2);
}
void proghead()
{if(sym.code==PROGRAM)
 {fprintf(f2,"data_seg segment\ndata db 65000 dup(0)\n");
  fprintf(f2,"buffer db 256 dup(0)\nconter dw 0\nerror db \'Input data error$\'\n"); 
  fprintf(f2,"data_seg ends\nstack_seg segment\ndw 4096 dup(0)\ntos label word\n");
  fprintf(f2,"stack_seg ends\nprog_seg segment\n");
  fprintf(f2,"main proc far\nassume cs:prog_seg,ds:data_seg,ss:stack_seg\n");
  fprintf(f2,"n_start:\nmov ax,stack_seg\nmov ss,ax\nmov sp,offset tos\n");
  fprintf(f2,"push ds\nxor ax,ax\npush ax\n");
  fprintf(f2,"mov ax,data_seg\nmov ds,ax\nmov di,offset data\n");
  /*生成汇编语言定义的数据段、堆栈段和代码段*/
  getsym();
  if(sym.code==ID)
  {fillin(sym,2,layer);/*为过程标识符填表*/
   getsym();
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
void block(int *paddress)
{int n,m;
 consexpl(paddress);
 varexpl(paddress);
 m=inlabel++;
 fprintf(f2,"jmp n_%d\n",m);/*跳过过程说明中的代码*/
 procdefi();
 n=inlabel++;
 fprintf(f2,"n_%d:call FAR PTR n_%d\n",m,n);
 /*生成一条调用取本分程序所需存储空间的指令*/
 compsent(paddress);
 fprintf(f2,"mov di,[di]\nret\n");
 fprintf(f2,"n_%d:mov ax,%d\nadd ax,di\nmov [di+2],ax\nret\n",n,*paddress);
 /*生成反回调用程序，以及取本分程序所需存储空间的子程序*/

}
void consexpl(int *paddress)
{if(sym.code==CONST)
{getsym();
 consdefi(paddress);
 conssuff(paddress);
 if(sym.code==SEM)
   getsym();
 else
   error(6);
}
}
void consdefi(int *paddress)
{if(sym.code==ID)
 {int n=fillin(sym,0,layer);
  getsym();
  if(sym.code==EQU)
  {getsym();
   if(sym.code==INT)
   {getsym();
	indent[n].addr=*paddress;
   if(val>65535)
   {fprintf(f2,"mov ax,%u\nmov [di+%u],ax\n",val%65536,*paddress);
    fprintf(f2,"mov ax,%u\nmov [di+%u],ax\n",val/65536,*paddress);
    *paddress+=4;
    indent[n].type=1;
   }
   else
   {fprintf(f2,"mov ax,%u\nmov [di+%u],ax\n",val%65536,*paddress);
   *paddress+=2;
    indent[n].type=0;
   }/*产生将常量放入相应存储空间的指令*/
   }
   else error(7);
   }
   else
      error(8);
  }
  else
	error(9);
}

void conssuff(int *paddress)
{if(sym.code==COM)
{getsym();
  consdefi(paddress);
 conssuff(paddress);
// if(sym.code==SEM)
//   getsym();
// else
//   error(10);
}
}
void varexpl(int *paddress)
{if(sym.code==VAR)
{
 flag=1;/*区别变量说明中的标识符表和read语句中的标识符表*/
 getsym();
 vardefi(paddress);
 varsuff(paddress);
}
}
void vardefi(int *paddress)
{int n;
if(sym.code==ID)
{top=0;
 a[top++]=fillin(sym,1,layer);
 /*建立标识符链表，供以后填写类型*/
 getsym();
 idsuff(paddress);
 if(sym.code==COL)
 {getsym();
  n=typeil();
  if(sym.code==SEM)
     getsym();
 }
 else
 error(11);
}
for(int i=0;i<top;i++)
{indent[a[i]].type=n;
 indent[a[i]].addr=*paddress;
 *paddress+=2*(n+1);
}/*对标识符链表，填写类型和运行时的相对地址*/

}
void varsuff(int *paddress)
{if(sym.code==ID)
{vardefi(paddress);
 varsuff(paddress);
}
} 
int typeil()
{int n=sym.code==INTEGER?0:1;
 if(sym.code==INTEGER||sym.code==LONG)
   getsym();
 else
   error(12);
 return(n);
}
void procdefi()
{int address=0;
if(sym.code==PROCEDURE)
{procedh(&address);
 block(&address);
 if(sym.code==SEM)
 {getsym();
  procsuff();
 }
 else
	 error(13);
for(int i=lenth-1;i>=0;i--)/*删除已处理过程中的符号*/
  if(indent[i].layer==layer)
    lenth--;
  layer--;
}
}
void procedh(int *paddress)
{int n;
 if(sym.code==PROCEDURE)
{getsym();
 if(sym.code==ID)
 {syb=sym;
  n=fillin(sym,2,layer);
  indent[n].attrib=2;/*设置为无参过程*/
  getsym();
  layer++;
  *paddress=2*layer+6;
  /*本层常量、变量的的起始地址
  前面的单元保存老的SP、本层数据的顶部和活动记录*/
  if(sym.code==SEM)
	  getsym();
  else
	if(sym.code==LBR)
	{indent[n].arguaddr=argument(paddress);
	 indent[n].attrib=3;/*设置为有参过程*/
	 if(sym.code==SEM)
        getsym();
	 else
        error(16);
	}
	else
	  error(15);
 }
 else
	 error(14);
}
fprintf(f2,"%s:mov [di+%d],di\n",syb.name,2*layer+4);
/*生成过程标识和进入过程保护老的SP的指令*/
}
int argument(int *paddress)
{int n,m;
 if(sym.code==LBR)
     { getsym();
       if(sym.code==ID)
           {n=fillin(sym,1,layer);/*将参数填写符号表*/
		    getsym();
            if(sym.code==COL)
              { getsym();
                m=typeil();
				indent[n].type=m;
				indent[n].addr=*paddress;
                *paddress+=2*(m+1);/*为参数分配运行时的地址*/
                if(sym.code==RBR)
				   getsym();
                else error(19);
              }
            else error(18);
           }
       else  error(17);
       }
	   return(n);
}
void procsuff()
{int address=0;
if(sym.code==PROCEDURE)
{procedh(&address);
 block(&address);
 if(sym.code==SEM)
 {getsym();
  procsuff();
 }
 else
	 error(20);
}
}
void assipro(int *paddress)
{int n,k;
if(sym.code==ID)
{n=lookup(sym,layer);
 getsym();
 if(sym.code==ASS)
 {getsym();
  express(paddress);
 if(indent[n].type==0)
 {fprintf(f2,"mov bx,[di+%d]\n",TermStack[TermLenth].layer+4);
  fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth--].addr);
  fprintf(f2,"mov bx,[di+%d]\n",2*indent[n].layer+4);
  fprintf(f2,"mov [bx+%d],ax\n",indent[n].addr);
 }
 else
 {fprintf(f2,"mov bx,[di+%d]\n",TermStack[TermLenth].layer+4);
  fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth].addr);
  if(TermStack[TermLenth].type==1)
    fprintf(f2,"mov dx,[bx+%d]\n",TermStack[TermLenth].addr+2);
  else
    fprintf(f2,"cwd\n");
  TermLenth--;
  fprintf(f2,"mov bx,[di+%d]\n",2*indent[n].layer+4);
  fprintf(f2,"mov [bx+%d],ax\n",indent[n].addr);
  fprintf(f2,"mov [bx+%d],dx\n",indent[n].addr+2);
 }
 }/*产生赋值语句的指令*/
  else
  if(sym.code==LBR)
  {ariSignStack[++ariSignStackLenth].incode=LBR;
  ariSignStack[ariSignStackLenth].level=1;
  getsym();
  express(paddress);
  if(sym.code==RBR)
  {while(ariSignStack[ariSignStackLenth].level>1)
	   genInstruction(paddress);
   ariSignStackLenth--;
   getsym();
    if(indent[n].attrib==3)
	{if(indent[indent[n].arguaddr].type==0)
	   {fprintf(f2,"mov ax,[di+%d]\n",TermStack[TermLenth--].addr);
	    fprintf(f2,"mov bx,[di+2]\nmov [bx+%d],ax\n",indent[indent[n].arguaddr].addr);
	}
	 else
	 {fprintf(f2,"mov ax,[di+%d]\n",TermStack[TermLenth].addr);
	  fprintf(f2,"mov dx,[di+%d]\n",TermStack[TermLenth--].addr+2);
	  fprintf(f2,"mov bx,[di+2]\nmov [bx+%d],ax\n",indent[indent[n].arguaddr].addr);
	  fprintf(f2,"mov [bx+%d],dx\n",indent[indent[n].arguaddr].addr);
	 }
     fprintf(f2,"mov [bx],di\nmov si,bx\n");
     for(k=0;k<layer;k++)
	   fprintf(f2,"mov ax,[di+%d]\nmov [si+%d],al\n",2*k+4,2*k+4);
	 fprintf(f2,"mov di,bx\ncall FAR PTR %s\n",indent[n].name);
	}/*产生有参过程调用指令*/
    else error(22);
   }
   else error(21);
  }
  else
  {if(indent[n].attrib==2)
	{fprintf(f2,"mov bx,[di+2]\nmov [bx+2],di\n");
     for(k=0;k<layer;k++)
	   fprintf(f2,"mov ax,[di+%d]\nmov [si+%d],al\n",2*k+4,2*k+4);
	 fprintf(f2,"mov di,bx\ncall FAR PTR %s\n",indent[n].name);
  }/*/*产生无参过程调用指令*/
  else error(23);
  }
}
}


void sentence(int *paddress)
{if(sym.code==ID)
   assipro(paddress);
 else if(sym.code==IF)
        ifsent(paddress);
      else if(sym.code==WHILE)
                whilsent(paddress);
	       else if(sym.code==READ)
                     read(paddress);
		        else if(sym.code==WRITE)
                          write(paddress);
				     else if(sym.code==BEGIN)
						     compsent(paddress);
}
void suffix(int *paddress)
{if(sym.code==ASS)
{getsym();
 express(paddress);
}
else if(sym.code==LBR)
        { getsym();
          express(paddress);
          if(sym.code==RBR)
               getsym();
          else error(24);
}
}
void ifsent(int *paddress)
{int n;
 if(sym.code==IF)
 {getsym();
 conditio(paddress);
 if(sym.code==THEN)
 {fprintf(f2,"and ax,ax\njnz n_%d\n",inlabel++);
  n=inlabel++;
  fprintf(f2,"jmp n_%d\nn_%d:nop\n",n,n-1);
  getsym();
  sentence(paddress);
  fprintf(f2,"n_%d:nop\n",n);
 }
 else
	 error(26);
}
else error(25);
}
void whilsent(int *paddress)
{int n,m,l;
 if(sym.code==WHILE)
 {n=inlabel++;
  fprintf(f2,"n_%d:nop\n",n);
  getsym();
  conditio(paddress);
  m=inlabel++;
  fprintf(f2,"and ax,ax\njnz n_%d\n",m);
  l=inlabel++;
  fprintf(f2,"jmp n_%d\nn_%d:nop\n",l,m);
  if(sym.code==DO)
  {getsym();
   sentence(paddress);
   fprintf(f2,"jmp n_%d\nn_%d:nop\n",n,l);
  }
  else
	 error(28);
}
else error(27);
}
void read(int *paddress)
{int n;
 if(sym.code==READ)
{fprintf(f2,"mov dx,offset buffer\nmov si,dx\nmov [si],BYTE PTR 255\n");
 fprintf(f2,"mov ah,0ah\nint 21h\ninc si\ninc si\npush si\n");
 flag=2;
 getsym();
 if(sym.code==LBR)
 {getsym();
  if(sym.code==ID)
  {n=lookup(sym,layer);
   if(indent[n].type==0)
   {fprintf(f2,"pop si\ncall FAR PTR n_read\npush si\n");
    fprintf(f2,"mov bx,[di+%d]\n",2*indent[n].layer+4);
    fprintf(f2,"mov [bx+%d],ax\n",indent[n].addr);
	readflag=1;
   }
   else
     if(indent[n].type==1)
	 {fprintf(f2,"pop si\ncall FAR PTR n_read\npush si\n");
      fprintf(f2,"mov bx,[di+%d]\n",2*indent[n].layer+4);
      fprintf(f2,"mov [bx+%d],ax\n",indent[n].addr);
      fprintf(f2,"mov [bx+%d],dx\n",indent[n].addr+2);
      readflag=1;
	 }
	 else
	   error(29);
   getsym();
   idsuff(paddress);
   if(sym.code==RBR)
   {getsym();
    fprintf(f2,"pop si\n");
   }
   else error(33);
  }
  else error(32);
 }
 else error(31);
}
else error(30);
}
void idsuff(int *paddress)
{int n;
 if(sym.code==COM)
{getsym();
 if(sym.code==ID)
 {if(flag==1)
    a[top++]=fillin(sym,0,layer );
  else
  {n=lookup(sym,layer);
   if(indent[n].type==0)
	{fprintf(f2,"call FAR PTR n_read\n");
    fprintf(f2,"mov bx,[di+%d]\n",2*indent[n].layer+4);
    fprintf(f2,"mov [bx+%d],ax\n",indent[n].addr);
	readflag=1;
   }
   else
     if(indent[n].type==1)
	  {fprintf(f2,"call FAR PTR n_read\n");
      fprintf(f2,"mov bx,[di+%d]\n",2*indent[n].layer+4);
      fprintf(f2,"mov [bx+%d],ax\n",indent[n].addr);
      fprintf(f2,"mov [bx+%d],dx\n",indent[n].addr+2);
      readflag=1;
	 }
	 else
	   error(34);
  }
  getsym();
  idsuff(paddress);
 }
 else
	 error(35);
}
}
void write(int *paddress)
{if(sym.code==WRITE)
{fprintf(f2,"mov ah,2\nmov dl,10\nint 21h\n");
 fprintf(f2,"mov ah,2\nmov dl,13\nint 21h\n");
 getsym();
 if(sym.code==LBR)
 {getsym();
  express(paddress);
  exprsuff(paddress);
  if(sym.code==RBR)
    getsym();
  else error(38);
 }
  else error(37);
 }
 else error(36);
}
void compsent(int *paddress)
{if(sym.code==BEGIN)
{getsym();
 sentence(paddress);
 sentsuff(paddress);
 if(sym.code==END)
   getsym();
 else error(40);
}
else error(39);
}

void exprsuff(int *paddress)
{if(TermStack[TermLenth].type==0)
 {fprintf(f2,"mov ax,[di+%d]\n",TermStack[TermLenth--].addr);
  fprintf(f2,"cwd\ncall FAR PTR n_write\n");
  writeflag=1;
 }
   else
     if(TermStack[TermLenth].type==1)
	 {fprintf(f2,"mov ax,[di+%d]\n",TermStack[TermLenth].addr);
      fprintf(f2,"mov dx,[di+%d]\n",TermStack[TermLenth--].addr+2);
      fprintf(f2,"call FAR PTR n_write\n");
	  writeflag=1;
	 }
	 else
	   error(41);
if(sym.code==COM)
{getsym();
 express(paddress);
 exprsuff(paddress);
 }
}
void sentsuff(int *paddress)
{if(sym.code==SEM)
{getsym();
 sentence(paddress);
 sentsuff(paddress);
}
}
void conditio(int *paddress)
{int n,m,l;char s[4];
if(sym.code==ODD)
{getsym();
 express(paddress);
 fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
 fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth--].addr);
 n=inlabel++;
 fprintf(f2,"and ax,1\njz n_%d\nmov ax,0ffh\nn_%d:mov [di+%d],ax\n",n,n,*paddress);
 TermStack[++TermLenth].addr=*paddress;
 TermStack[TermLenth].attrib=1;
 TermStack[TermLenth].layer=layer;
 TermStack[TermLenth].type=0;
 *paddress+=2;
}
else
{express(paddress);
 respoper(paddress);
 express(paddress);
 switch(ariSignStack[ariSignStackLenth--].incode)
 {case EQU:strcpy(s,"jne");break;
  case NEQ:strcpy(s,"je");break;
  case LES:strcpy(s,"jge");break;
  case LEQ:strcpy(s,"jg");break;
  case LAG:strcpy(s,"jle");break;
  case GEQ:strcpy(s,"jl");
 }
 if(TermStack[TermLenth].type==0&&TermStack[TermLenth-1].type==0)
 {fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
 fprintf(f2,"mov dx,[bx+%d]\n",TermStack[TermLenth--].addr);
 fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
 fprintf(f2,"xor ax,ax\ncmp [bx+%d],dx\n",TermStack[TermLenth--].addr);
 n=inlabel++;
 fprintf(f2,"%s n_%d\n",s,n);
 fprintf(f2,"mov ax,-1\nn_%d:nop\n",n);
}
else
{fprintf(f2,"mov si,[di+%d]\n",2*TermStack[TermLenth].layer+4);
 fprintf(f2,"mov ax,[si+%d]\n",TermStack[TermLenth].addr);
 if(TermStack[TermLenth].type==1)
 fprintf(f2,"mov dx,[si+%d]\n",TermStack[TermLenth].addr+2);
 else
 fprintf(f2,"cwd\n");
 TermLenth--;
 fprintf(f2,"mov si,[di+%d]\n",2*TermStack[TermLenth].layer+4);
 fprintf(f2,"mov bx,[si+%d]\n",TermStack[TermLenth].addr);
 if(TermStack[TermLenth].type==1)
 fprintf(f2,"mov cx,[si+%d]\n",TermStack[TermLenth].addr+2);
 else
 {n=inlabel++;
  fprintf(f2,"xor cx,cx\nand bx,bx\njns n_%d\ndec cx\nn_%d:nop\n",n,n);
 }
 TermLenth--;
 n=inlabel++;m=inlabel++;l=inlabel++;
 fprintf(f2,"cmp cx,dx\nje n_%d\n%s n_%d\njmp n_%d\n",n,s,m,l);
 fprintf(f2,"n_%d:cmp bx,ax\n%s n_%d\nn_%d:mov ax,-1\n",n,s,m,l);
 n=inlabel++;
 fprintf(f2,"jmp n_%d\nn_%d:xor ax,ax\nn_%d:nop\n",n,m,n);
}
}
}/*生成计算逻辑值的指令*/
void express(int *paddress)
{if(sym.code==ADD||sym.code==SUB)
{if(sym.code==SUB)
{ariSignStack[++ariSignStackLenth].incode=SUB;
 ariSignStack[ariSignStackLenth].level=5;};
 getsym();
}
 term(paddress);
 termsuff(paddress);
 while(ariSignStack[ariSignStackLenth].level>=3)
	   genInstruction(paddress);
 /*生成运算符栈中优先级大于等于3的指令*/
}
void term(int *paddress)
{factor(paddress);
 factsuff(paddress);
}
void termsuff(int *paddress)
{if(sym.code==ADD||sym.code==SUB)
{if(ariSignStack[ariSignStackLenth].level<3)
  {ariSignStack[++ariSignStackLenth].incode=sym.code;
   ariSignStack[ariSignStackLenth].level=3;
  }
  else
	while(ariSignStack[ariSignStackLenth].level>=3)
	   genInstruction(paddress);
 getsym();
 factor(paddress);
 factsuff(paddress);
}
}
void factsuff(int *paddress)
{if(sym.code==MUL||sym.code==DIV)
{if(ariSignStack[ariSignStackLenth].level<4)
  {ariSignStack[++ariSignStackLenth].incode=sym.code;
   ariSignStack[ariSignStackLenth].level=4;
  } /*生成运算符栈中优先级大于等于4的指令*/

  else
	while(ariSignStack[ariSignStackLenth].level>=4)
	   genInstruction(paddress);
 getsym();
 factor(paddress);
 factsuff(paddress);
}
}
void factor(int *paddress)
{int n;
 if(sym.code==ID)
 {n=lookup(sym,layer);
 TermStack[++TermLenth].addr=indent[n].addr;
 TermStack[TermLenth].type=indent[n].type;
 TermStack[TermLenth].layer=indent[n].layer;
 getsym();
 factsuff(paddress);
 }
 else if(sym.code==INT)
 {if(val>65535)
   {fprintf(f2,"mov ax,%u\nmov [di+%u],ax\n",val%65536,*paddress);
    fprintf(f2,"mov dx,%u\nmov [di+%u],ax\n",val/65536,*paddress+2);
    TermStack[++TermLenth].addr=*paddress;
    TermStack[TermLenth].type=1;
    TermStack[TermLenth].layer=layer;
    *paddress+=4;
    }
 else
    {
    fprintf(f2,"mov ax,%u\nmov [di+%d],ax\n",val,*paddress);
    TermStack[++TermLenth].addr=*paddress;
    TermStack[TermLenth].type=0;
    TermStack[TermLenth].layer=layer;
    *paddress+=2;
 }/*生成将常量放入相应的存储空间的指令*/
  getsym();
  factsuff(paddress);
 }
 else 
 if(sym.code==LBR)
 {ariSignStack[++ariSignStackLenth].incode=LBR;
  ariSignStack[ariSignStackLenth].level=1;
  getsym();
  express(paddress);
  if(sym.code==RBR)
  {while(ariSignStack[ariSignStackLenth].level>1)
	   genInstruction(paddress);
   ariSignStackLenth--;
   /*当前输入符号为），生成（前所有运算符的指令，并将（退栈*/
   getsym();
  }
  else error(28);
 }
 else error(27);
}

void addoper(int *paddress)
{if(sym.code==ADD||sym.code==SUB)
{ if(ariSignStack[++ariSignStackLenth].level<2)
  {ariSignStack[++ariSignStackLenth].incode=(sym.code==ADD?ADD:SUB);
   ariSignStack[++ariSignStackLenth].level=2;
  }
  else
	while(ariSignStack[ariSignStackLenth].level>=2)
	   genInstruction(paddress);
 getsym();
}
 else error(30);
}
void muloper(int *paddress)
{if(sym.code==MUL||sym.code==DIV)
{if(ariSignStack[++ariSignStackLenth].level<4)
  {ariSignStack[++ariSignStackLenth].incode=(sym.code==ADD?ADD:SUB);
   ariSignStack[++ariSignStackLenth].level=2;
  }
  else
	while(ariSignStack[ariSignStackLenth].level>=2)
	   genInstruction(paddress);
	getsym();
}
else error(31);
}
void respoper(int *paddress)
{if(sym.code>=EQU&&sym.code<=GEQ)
{if(ariSignStack[ariSignStackLenth].level<2)
  {ariSignStack[++ariSignStackLenth].incode=sym.code;
   ariSignStack[ariSignStackLenth].level=2;
  }
  else
	while(ariSignStack[ariSignStackLenth].level>=2)
	   genInstruction(paddress);
 getsym();
}
 else error(32);
}
int fillin(st symb,int attrib,int layer)
{int i;
for(i=0;i<lenth;i++)
  if(!strcmp(indent[i].name,symb.name)&&(indent[i].layer==layer))
	break;
if(i>=lenth)
 {strcpy(indent[lenth++].name,symb.name);
  indent[i].attrib=attrib;
  indent[i].layer=layer;
 }
 else error(0);
return(i);
}
int lookup(st symb,int layer)
{int i;
while(layer>=0)
{for(i=0;i<lenth;i++)
 if((strcmp(indent[i].name,symb.name)==0) && (indent[i].layer==layer))
	  goto L;
 layer--;
 }
 if(i>=lenth)
    printf("The ID-%s is not declared\n",symb.name);
L:return(i);
}

void genInstruction(int *paddress)
{int n;
switch(ariSignStack[ariSignStackLenth].incode)
{case ADD:{if(ariSignStack[ariSignStackLenth].level!=5)
		   	 if(TermStack[TermLenth].type==0&&
				TermStack[TermLenth-1].type==0)
			   {fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov dx,[bx+%d]\n",TermStack[TermLenth--].addr);
               fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth--].addr);
               fprintf(f2,"add ax,dx\nmov [di+%d],ax\n",*paddress);
               TermStack[++TermLenth].addr=*paddress;
               TermStack[TermLenth].attrib=1;
               TermStack[TermLenth].layer=layer;
               TermStack[TermLenth].type=0;
			   *paddress+=2;
               }
			   else
			   {fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth].addr);
			   if(TermStack[TermLenth].type==1)
			   fprintf(f2,"mov dx,[bx+%d]\n",TermStack[TermLenth].addr+2);
			   else fprintf(f2,"cwd\n");
			   TermLenth--;
			   fprintf(f2,"mov [di+%d],ax\nmov [di+%d],dx\n",*paddress,*paddress+2);
               fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
			   fprintf(f2,"add ax,[bx+%d]\n",TermStack[TermLenth].addr);
			   if(TermStack[TermLenth].type==1)
                  fprintf(f2,"adc dx,[bx+%d]\n",TermStack[TermLenth].addr+2);
			   else 
				   fprintf(f2,"adc dx,0\n");
               TermLenth--;
			   fprintf(f2,"mov [di+%d],ax\nmov [di+%d],dx\n",*paddress,*paddress+2);
               TermStack[++TermLenth].addr=*paddress;
               TermStack[TermLenth].attrib=1;
               TermStack[TermLenth].layer=layer;
               TermStack[TermLenth].type=1;
			   *paddress+=4;
               }
			   break;
}
  case SUB:{if(ariSignStack[ariSignStackLenth].level==5)
			   if(TermStack[TermLenth].type==0)
			   {
               fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth--].addr);
               fprintf(f2,"neg ax\nmov [di+address],ax\n");
               TermStack[++TermLenth].addr=*paddress;
               TermStack[TermLenth].attrib=1;
               TermStack[TermLenth].layer=layer;
               TermStack[TermLenth].type=0;
			   *paddress+=2;}
			   else
			   {fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth].addr);
               fprintf(f2,"mov dx,[bx+%d]\n",TermStack[TermLenth].addr+2);
               fprintf(f2,"xor dx,0ffffh\nneg ax\ncmc\nadc dx,0\n");
			   fprintf(f2,"mov [di+%d],ax\nmov [di+%d],dx\n",*paddress,*paddress+2);
               TermStack[++TermLenth].addr=*paddress;
               TermStack[TermLenth].attrib=1;
               TermStack[TermLenth].layer=layer;
               TermStack[TermLenth].type=1;
			   *paddress+=4;
               }
			else
			   if(TermStack[TermLenth].type==0&&
				  TermStack[TermLenth-1].type==0)
			   {fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov dx,[bx+%d]\n",TermStack[TermLenth--].addr);
               fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth--].addr);
               fprintf(f2,"sub ax,dx\nmov [di+%d],ax\n",*paddress);
               TermStack[++TermLenth].addr=*paddress;
               TermStack[TermLenth].attrib=1;
               TermStack[TermLenth].layer=layer;
               TermStack[TermLenth].type=0;
			   *paddress+=2;
               }
			   else
			   {fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth].addr);
			   if(TermStack[TermLenth].type==1)
               fprintf(f2,"mov dx,[bx+%d]\n",TermStack[TermLenth].addr+2);
			   else fprintf(f2,"cwd\n");
               TermLenth--;
               fprintf(f2,"mov si,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov bx,[si+%d]\nsub bx,ax\n",TermStack[TermLenth].addr);
			   if(TermStack[TermLenth].type==1)
               fprintf(f2,"mov cx,[si+%d]\n",TermStack[TermLenth].addr+2);
			   else 
			   {n=inlabel++;
				fprintf(f2,"xor cx,cx\ntest bx,bx\njns n_%d\ndec cx\nn_%d:nop\n",n,n);
			   }
               fprintf(f2,"sbb cx,dx\n");
			   fprintf(f2,"mov [di+%d],bx\nmov [di+%d],cx\n",*paddress,*paddress+2);
               TermStack[++TermLenth].addr=*paddress;
               TermStack[TermLenth].attrib=1;
               TermStack[TermLenth].layer=layer;
               TermStack[TermLenth].type=1;
			   *paddress+=4;
               }
			   break;
}
  case MUL: if(TermStack[TermLenth].type==0&&
				TermStack[TermLenth-1].type==0)
			   {
               fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov cx,[bx+%d]\n",TermStack[TermLenth--].addr);
               fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth--].addr);
               fprintf(f2,"imul cx\nmov [di+%d],ax\n",*paddress);
               TermStack[++TermLenth].addr=*paddress;
               TermStack[TermLenth].attrib=1;
               TermStack[TermLenth].layer=layer;
               TermStack[TermLenth].type=0;
			   *paddress+=2;}
			   else
			   {fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
                fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth].addr);
			    if(TermStack[TermLenth].type==1)
                fprintf(f2,"mov dx,[bx+%d]\n",TermStack[TermLenth].addr+2);
			    else fprintf(f2,"cwd\n");
                TermLenth--;

                fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
			    if(TermStack[TermLenth].type==1)
                fprintf(f2,"mov si,bx\nadd si,%d\n",TermStack[TermLenth].addr);
                else
				{fprintf(f2,"mov [di+%d],ax\nmov [di+%d],dx\n",*paddress,*paddress+2);
			    fprintf(f2,"mov si,di\nadd si,%d\n",*paddress);
			    fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
    		    fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth].addr);
				}
                TermLenth--;
			    fprintf(f2,"call FAR PTR n_muliti\n");
				mulflag=1;
			    fprintf(f2,"mov [di+%d],ax\nmov [di+%d],dx\n",*paddress,*paddress+2);
			    TermStack[++TermLenth].addr=*paddress;
                TermStack[TermLenth].attrib=1;
                TermStack[TermLenth].layer=layer;
                TermStack[TermLenth].type=1;
			    *paddress+=4;
			   }
			   break;
			   
			   
  case DIV:{if(TermStack[TermLenth].type==0&&
				TermStack[TermLenth-1].type==0)
			   {fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov cx,[bx+%d]\n",TermStack[TermLenth--].addr);
               fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth--].addr);
               fprintf(f2,"cwd\nidiv cx\nmov [di+%d],ax\n",*paddress);
               TermStack[++TermLenth].addr=*paddress;
               TermStack[TermLenth].attrib=1;
               TermStack[TermLenth].layer=layer;
               TermStack[TermLenth].type=0;
			   *paddress+=2;}
			   else
			   {fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth].addr);
			   if(TermStack[TermLenth].type==1)
               fprintf(f2,"mov dx,[bx+%d]\n",TermStack[TermLenth].addr+2);
			   else fprintf(f2,"cwd\n");
               TermLenth--;
               fprintf(f2,"mov [di+%d],ax\nmov [di+%d],dx\n",*paddress,*paddress+2);
			   fprintf(f2,"mov si,di\nadd si,%d\n",*paddress);
               fprintf(f2,"mov bx,[di+%d]\n",2*TermStack[TermLenth].layer+4);
               fprintf(f2,"mov ax,[bx+%d]\n",TermStack[TermLenth].addr);
			   if(TermStack[TermLenth].type==1)
               fprintf(f2,"mov dx,[bx+%d]\n",TermStack[TermLenth].addr+2);
			   else fprintf(f2,"cwd\n");
               TermLenth--;
			   fprintf(f2,"call FAR PTR n_divide\n");
			   divflag=1;
			   fprintf(f2,"mov [di+%d],ax\nmov [di+%d],dx\n",*paddress,*paddress+2);
	           TermStack[++TermLenth].addr=*paddress;
               TermStack[TermLenth].attrib=1;
               TermStack[TermLenth].layer=layer;
               TermStack[TermLenth].type=1;
			   *paddress+=4;
			   }
			   break;
			   }
	  }
   ariSignStackLenth--;
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
		   else if(ch=-1)
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
