#include <stdio.h>
#include <ctype.h>
#include <string.h>
//#include <process.h>/*VC++使用该文件*/
#include<stdlib.h>
#define lenth1 15
#define lenth2 17
struct {char name[21];int type;int addr;}
   indent[1000];/*为标识符表*/
struct st
{char name[21];int code;} sym;
int lenth=0;/*为标识符表长*/
FILE *f1,*f2;/* f1为输入文件指针,f2输出文件指针*/
int line=0,row=0,val;
void getsym();
char getchr();
void error(int);
int main(int argc,char *argv[])
  {/*argc表示参数的个数、argv[0]、argv[1]、argv[2]……
   表示运行程序时输入的字符串，包括执行文件名，   调试时其参数设置，VC++6.0
在菜单Project下的子菜单Settings中的Debug选项卡中Program arguments输入对话框
中设置Turbo C 2.0在Options菜单下的子菜单Options中输入对话框中设置   */
   char ft[12],*fc;
   if((f1=fopen(argv[1],"r"))==NULL)
      {printf("connot open the file\n");
        exit(0);
		/*如不能打开输入文件，显示出错信息退出*/
      }      /*当参数个数小于等于2时，用户没有指出输出文件此时生成一个与输入
文件名相同而后缀名不同的输出文件名；否则用户已输入输出文件名故直接使用*/
   if(argc<=2)
     {strcpy(ft,argv[1]);
      if((fc=strchr(ft,'.'))!=NULL)
	     strcpy(fc,".mid");
      else
	     strcat(ft,".mid");
     }
   else
     strcpy(ft,argv[2]);
if((f2=fopen(ft,"w"))==NULL) /*打开输出文件*/
      {printf("connot open the file\n");
        exit(0);
      }
 while(!feof(f1))
 {getsym();
  printf("%s %d\n",sym.name,sym.code);
  fprintf(f2,"%s %d\n",sym.name,sym.code);
 }
 fclose(f1); fclose(f2);
return(1);
 }

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
	    {
			n=0;
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
 
