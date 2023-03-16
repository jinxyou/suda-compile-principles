class Parser:
    result=''
    lookahead=''

    def __init__(self):
        self.lookahead=input("中缀表达式转后缀表达式，请每行输入中缀表达式中的一个数字或符号，输入任意其他符号结束表达式\n")

    def expr(self):
        self.term()
        while(True):
            if self.lookahead == '+':
                self.match('+')
                self.term()
                self.result+='+ '
            elif self.lookahead == '-':
                self.match('-')
                self.term()
                self.result+='- '
            else:
                return

    def term(self):
        if ord(self.lookahead)>=48 and ord(self.lookahead)<=57:
            self.result+=self.lookahead+' '
            self.match(self.lookahead)
        else:
            print("Syntax error")

    def match(self,t):
        if self.lookahead==t:
            self.lookahead=input()
        else:
            print("Syntax error")

if(__name__=="__main__"):
    parse = Parser()
    parse.expr()
    parse.result+="\n"
    print("后缀表达式为"+parse.result)