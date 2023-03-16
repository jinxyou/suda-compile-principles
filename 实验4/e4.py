#coding=utf-8

S = [(0, 'eps', 1), (1, 'eps', 2), (2, 'a', 3), (1, 'eps', 4), (4, 'b', 5), (3, 'eps', 6), (5, 'eps', 6), (6, 'eps', 1),
     (6, 'eps', 7), (0, 'eps', 7), (7, 'a', 8), (8, 'b', 9)]

def p_closure(p):
    T=[]
    T1=p
    
    while len(T)!=len(T1):
        T=[q for q in T1]
        
        for q in T:
            for t0,sym,t1 in S:
                if t0==q and sym=='eps' and t1 not in T1:
                    T1.append(t1)
    T1.sort()
    return T1

def move(T,a):
    R=[]
    for i in T:
       for t0,sym,t1 in S:
           if i==t0 and (sym==a or sym=='eps'):
                R.append(t1)
    return R

def nfa2dfa():
    D_tran = []
    D_states = []
    D_states.append(p_closure([0]))
    mark = 0
    while (len(D_states) > mark):
        T = D_states[mark]
        mark += 1
        for t0, sym, t1 in S:
            if sym != 'eps':
                U = p_closure(move(T, sym))
                if U == []:
                    continue
                if U not in D_states:
                    D_states.append(U)
                if sym != 'eps' and (T, sym, U) not in D_tran:
                    D_tran.append((T, sym, U))
    return (D_states, D_tran)

def print_tran(D_states, D_tran):
    states_map={}
    flag='A'
    for i in D_states:
        states_map[flag]=i
        flag=chr(ord(flag)+1)

    tran_map=[]
    t2,t3='',''
    for t0,sym,t1 in D_tran:
        for j in states_map:
            if t0==states_map[j]:
                t2=j
            if t1==states_map[j]:
                t3=j
        if t2!='' and t3!='' and (t2,sym,t3) not in tran_map:
            tran_map.append((t2,sym,t3))

    for state in states_map:
        print state,':',states_map[state]
    for t0,sym,t1 in tran_map:
        print t0,sym,t1

def match(seq,states,tran):
    cur=states[0]
    for i in seq:
        for t0,sym,t1 in tran:
            if t0==cur and i==sym:
                cur=t1
                break

    if cur==states[-1]:
        print seq,"接受"
        return True
    else:
        print seq, "不接受"
        return False

if __name__=="__main__":
    D_states, D_tran = nfa2dfa()
    print_tran(D_states,D_tran)

    match('aab',D_states,D_tran)
    match('ababab', D_states, D_tran)
    match('aabbab',D_states,D_tran)