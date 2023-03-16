#coding=utf-8

S=[] # Status
ST=[] # temp status
T=0
K=0

input='(a|b)*'
n=len(input)

def get_current(input,i):
    return input[i]

def get_next(input,i):
    if i<len(input)-1:
        return input[i+1]
    else:
        return ''

def get_last(input, i):
    if i>0:
        return input[i-1]
    else:
        return ''

def add_to_status(t0,ch,t1):
    S.append((t0,ch,t1))

def check_vocab(ch):
    if ch>='a' and ch<='z':
        return True
    else:
        return False

def match(input,T,K):
    for i in range(n):
        cur=get_current(input, i)
        next=get_next(input, i)
        last=get_last(input, i)

        if check_vocab(cur):
            if next=='*':
                add_to_status(T, 'eps', K + 1)
                add_to_status(K + 1, cur, K + 1)
                add_to_status(K + 1, 'eps', K + 2)
                K = K + 2
                T = K
            elif next!='*' and next !='|' and last!='|':
                add_to_status(T, cur, K + 1)
                K += 1
                T = K

        if cur=='(':
            ST.append(T)
            add_to_status(T, 'eps', K + 1)
            K += 1
            T = K

        if cur == ')':
            if next != '*':
                pass
            else:
                add_to_status(T, 'eps', ST[-1] + 1)
                add_to_status(T, 'eps', K + 1)
                add_to_status(ST[-1], 'eps', K + 1)
                K += 1
                T = K
                ST.pop()


        if cur == '|':  # 检测当前符号是否为·|如果是，则获取前一个字母及后一个字母构造NFA
            add_to_status(T, 'eps', K + 1)
            add_to_status(K + 1, last, K + 2)
            add_to_status(T, 'eps', K + 3)
            add_to_status(K + 3, next, K + 4)
            add_to_status(K + 2, 'eps', K + 5)
            add_to_status(K + 4, 'eps', K + 5)
            K += 5
            T = K

if __name__=="__main__":
    match(input,T,K)
    for t0,ch,t1 in S:
        print '%s\t[%s]\t%s' %(t0,ch,t1)