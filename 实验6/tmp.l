%{
#include "stdio.h"
#define ID		1
#define NUMBER		2
#define PLUS		3
#define MINUS		4
#define TIMES		5
#define DIVIDE		6
#define LPAREN		7
#define RPAREN		8
#define LBRACE		9
#define RBRACE		10
#define ASSIGN		11
#define LESSTHAN		12
#define OSTREAM		13
#define QUOTATION	14
#define SEMICOLON	15
#define IF			16
#define ELSE		17
#define WHILE		18
#define INT		19
#define COUT		20
#define ENDL		21
#define SYMBOL		22
#define ERRORCHAR		23
#define RELATION		24
#define OPERATOR		25
int yylval;
%}

letter       [A-Za-z]
digit        [0-9]
delim	[ \t \n]
ws	{delim}+
id	{letter}({letter}|{digit})*
number	{digit}+(\.{digit}+)?(E[+-]?{digit}+)?

%%
{ws}	{;}
while	{yylval=WHILE; return (WHILE);}
if	{yylval=IF; return (IF);}
else	{yylval=ELSE; return (ELSE);}
cout	{yylval=COUT; return (COUT);}
endl	{yylval=ENDL; return (ENDL);}
int	{yylval=INT; return (INT);}
{id}	{yylval=installID(); return (ID);}
{number}	{yylval=installNum(); return (NUMBER);}
"+"	{yylval=PLUS; return (OPERATOR);}
"-"	{yylval=MINUS; return (OPERATOR);}
"*"	{yylval=TIMES; return (OPERATOR);}
"/"	{yylval=DIVIDE; return (OPERATOR);}
"("	{yylval=LPAREN; return (SYMBOL);}
")"	{yylval=RPAREN; return (SYMBOL);}
"{"	{yylval=LBRACE; return (SYMBOL);}
"}"	{yylval=RBRACE; return (SYMBOL);}
"="	{yylval=ASSIGN; return (OPERATOR);}
"<"	{yylval=LESSTHAN; return (RELATION);}
"<<"	{yylval=OSTREAM; return (OSTREAM);}
"\""	{yylval=QUOTATION; return (SYMBOL);}
";"	{yylval=SEMICOLON; return (SYMBOL);}
.	{yylval=ERRORCHAR; return ERRORCHAR;}
%%

int installID () {
	return ID;
}
 
int installNum () {
  return NUMBER;
}

int yywrap (){
  return 1;
}

int main() {
yyin=fopen("prog.txt","r");
int c=0;
while(c=yylex()){
	switch(c){
  	case ERRORCHAR: printf("(ERRORCHAR, \"%s\") \n", yytext);break;	  
    	case WHILE: printf("(WHILE, \"%s\") \n", yytext);break;
    	case IF: printf("(IF, \"%s\") \n", yytext);break;
   	case ELSE: printf("(ELSE, \"%s\") \n", yytext);break;
    	case NUMBER: printf("(NUM, \"%s\") \n", yytext);break;
    	case ID: printf("(ID, \"%s\") \n", yytext);break;
	case SYMBOL: printf("(SYMBOL,\"%s\")\n",yytext);break;
	case OSTREAM: printf("(OSTREAM,\"%s\")\n",yytext);break;
	case RELATION: printf("(RELATION,\"%s\")\n",yytext);break;
	case OPERATOR: printf("(OPERATOR,\"%s\")\n",yytext);break;
	case COUT: printf("(COUT,\"%s\")\n",yytext);break;
	case ENDL: printf("(ENDL,\"%s\")\n",yytext);break;
	case INT: printf("(INT,\"%s\")\n",yytext);break;
    	default:break;
  }
}
}
