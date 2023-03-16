#include <stdio.h>
#include <string>
#include <fstream>

using namespace std;
FILE* f1;
int line = 0, row = 0, val, length = 0;	//length为标识符个数
char ch = ' ';
struct {
	char name[21];
	int type;
	int addr;
}indent[1000];

struct st
{
	char name[21]; int code;
} sym;

char getchr()
{
	char ch = fgetc(f1);
	if (ch == '\n')
	{
		row = 1;
		line++;
	}
	else
		if (ch != ' ' && ch != '\t')
			row++;
	return(ch);
}

void error(int n)
{
	printf("There are %d-error\n", n);
	exit(0);
}

void getsym()
{
	char A[21];
	int k, i;
	static char reserved[15][10] = { "program","const","var","integer","long","procedure","if",
		 "then","while","do","read","write","begin","end","odd" };
	static char oprtr[17][3] = { "+","-","*","/","=","<>","<","<=",">",">=", ".",",",";",":",":=","(",")" };
	while (isspace(ch))	//是否为空格
	{
		ch = getchr();
	}
	if (isalpha(ch))	//是否为字母
	{
		k = 0;
		while (isalpha(ch) || isalnum(ch))	//向后取整个单词
		{
			if (isalpha(ch)) ch = tolower(ch);
			A[k++] = ch;
			ch = getchr();
		}
		A[k] = '\0';
		for (i = 0; i < 15; i++)
		{
			if (!strcmp(A, reserved[i]))		//检查是否为保留字
				break;
		}
		if (i < 15)
		{
			strcpy(sym.name, reserved[i]);
			sym.code = i + 1;
		}
		else
		{
			for (i = 0; i < length; i++)
			{
				if (!strcmp(A, indent[i].name))		//检查是否为已知标识符
				{
					break;
				}
			}
			if (i == length)
			{
				strcpy(indent[i].name, A);
			}
			strcpy(sym.name, indent[i].name);
			sym.code = 34;
		}
	}
	else if (isalnum(ch))	//识别数字
	{
		val = 0; k = 0;
		while (isalnum(ch))
		{
			val = val * 10 + ch - '0';	//每识别到一位数字就加在原数后
			sym.name[k++] = ch;
			ch = getchr();
		}
		sym.name[k] = '\0';
		sym.code = 33;
	}
	else
	{
		k = 0;
		if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '.' || ch == ','		//除上面几种情况之外的其他运算符的识别
			|| ch == ';' || ch == '(' || ch == ')')
		{
			A[k++] = ch;
			A[k] = '\0';
			ch = getchr();
			for (i = 0; i < 17; i++)
			{
				if (!strcmp(A, oprtr[i]))
				{
					strcpy(sym.name, A);
					sym.code = i + 16;
				}
			}
		}
		else
		{
			k = 0;
			if (ch == '>' || ch == ':')		//检查一个运算符之后是否还有组合的运算符
			{
				A[k++] = ch;
				ch = getchr();
				if (ch == '=')
				{
					A[k++] = ch;
					ch = getchr();
				}
			}
			else if (ch == '<')
			{
				A[k++] = ch;
				ch = getchr();
				if (ch == '=' || ch == '>')
				{
					A[k++] = ch;
					ch = getchr();
				}
			}
			else if (ch == -1)
			{
				strcpy(sym.name, "");
				sym.code = 35;
			}
			else
				error(1);
		}
		A[k] = '\0';
		for (i = 0; i < 17; i++)
			if (!strcmp(A, oprtr[i]))
			{
				strcpy(sym.name, A);
				sym.code = i + 16;
			}
	}
}


int main()
{
	f1 = fopen("D:\code.txt", "r");
	while (!feof(f1))
	{
		getsym();
		printf("%s %d\n", sym.name, sym.code);
	}
	fclose(f1);
}