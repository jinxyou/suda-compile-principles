import re

stri='maaamaaa'
pat=re.compile('m(.?)m(.?)')
re.sub(pat,r'\\1 a',stri)

print(stri)