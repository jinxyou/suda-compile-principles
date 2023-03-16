def error():    # 处理错误
    print("Syntax error!")


def getsymbols(w):  # 处理输入字符串
    line = w.split(" ")
    line.append("$")
    symbols, expr = [], []
    for i in line:
        if len(i) == 1 and ord('0') <= ord(i) <= ord('9'):
            symbols.append("id")
            expr.append(i)
        elif i == "id":
            symbols.append("id")
            expr.append('0')
        else:
            symbols.append(i)
            expr.append(i)
    return symbols, expr


def print_table(stack, w):  # 打印成表
    s = ''
    for i in stack:
        s += str(i) + " "
    print("%-25s" % s, end='')
    s = ''
    for i in w:
        s += str(i) + " "
    print("%20s" % s, end='')

# 处理规约时的数值计算
def f1(b):
    return str(int(b[0]) + int(b[2]))


def f2(b):
    return b[0]


def f3(b):
    return str(int(b[0]) * int(b[2]))


def f4(b):
    return b[0]


def f5(b):
    return b[1]


def f6(b):
    return b[0]

# SLR过程
def SLR(l):
    print("输入:",l)
    w, e = getsymbols(l)
    i = 0
    sym = e[i]
    ip = w[i]
    stack = [0]
    val = []
    action = [{'id': ['s', 5], '(': ['s', 4]},              # action表
              {'+': ['s', 6], '$': ['acc']},
              {'+': ['r', 2], '*': ['s', 7], ')': ['r', 2], '$': ['r', 2]},
              {'+': ['r', 4], '*': ['r', 4], ')': ['r', 4], '$': ['r', 4]},
              {'id': ['s', 5], '(': ['s', 4]},
              {'+': ['r', 6], '*': ['r', 6], ')': ['r', 6], '$': ['r', 6]},
              {'id': ['s', 5], '(': ['s', 4]},
              {'id': ['s', 5], '(': ['s', 4]},
              {'+': ['s', 6], ')': ['s', 11]},
              {'+': ['r', 1], '*': ['s', 7], ')': ['r', 1], '$': ['r', 1]},
              {'+': ['r', 3], '*': ['r', 3], ')': ['r', 3], '$': ['r', 3]},
              {'+': ['r', 5], '*': ['r', 5], ')': ['r', 5], '$': ['r', 5]}]
    goto = [{'E': 1, 'T': 2, 'F': 3},       # goto表
            {},
            {},
            {},
            {'E': 8, 'T': 2, 'F': 3},
            {},
            {'T': 9, 'F': 3},
            {'F': 10}]
    syntax = [[],       # 规约表
              ["E", ["E", "+", "T"]],
              ["E", ["T"]],
              ["T", ["T", "*", "F"]],
              ["T", ["F"]],
              ["F", ["(", "E", ")"]],
              ["F", ["id"]]]
    val_syntax = [[], f1, f2, f3, f4, f5, f6]   # 规约表对应的数值转换函数

    print("{:^25s} {:^20s} {:^10s}".format("栈", "输入", "动作"))

    while 1:    # SLR分析过程
        s = stack[-1]
        a = ip
        print_table(stack, w[i::])
        if action[s].get(a) is not None:
            if action[s][a][0] == 's':
                stack.append(a)
                val.append(sym)
                s1 = action[s][a][1]
                stack.append(s1)
                if i + 1 != len(w):
                    i += 1
                    sym = e[i]
                    ip = w[i]
                print("%-10s" % "移进")
            elif action[s][a][0] == 'r':
                rule = syntax[action[s][a][1]]
                A = rule[0]
                beta = rule[1]
                for j in range(2 * len(beta)):
                    stack.pop()
                b = []
                for j in range(len(beta)):
                    b.append(val.pop())
                s1 = stack[-1]
                stack.append(A)
                stack.append(goto[s1][A])
                val.append(val_syntax[action[s][a][1]](b))
                print("%-10s" % ("按" + A + "->" + " ".join(beta) + "规约"))
            elif action[s][a][0] == 'acc':
                print("%-10s" % "接受")
                print("结果:",val[-1])
                return
            else:
                error()
                return
        else:
            error()
            return

SLR("id + id * id")
SLR("id *+ id + id")
SLR("8 + 5 * 2")
