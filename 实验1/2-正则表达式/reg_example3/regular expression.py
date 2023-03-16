#-*- coding:UTF-8 -*-
import re


def verify_image(name):
    pat=re.compile(r'(.+\.jp(e)?g$)|(.\.gif)|(.\.bmp)')
    result=pat.match(name)
    if result==None:
        print("匹配失败")
    else:
        print("匹配成功")

def match_date(date):
    pat=re.compile('^(([469]|(11))/([1-9]|[12][0-9]|30)|([13578]|10|12)/([1-9]|[12][0-9]|30|31)|2/([1-9]|1[0-9]|2[0-8]))/([1-9]|[1-9]\d|[1-9]\d\d|[1-9]\d\d\d)$')
    result=pat.match(date)
    if result==None:
        print("匹配失败")
    else:
        print("匹配成功")

def fetch_region_num(num):
    pat=re.compile('^(0\d{2,3})(\d{8})')
    matchobj=pat.match(num)
    if matchobj==None:
        print("匹配失败")
    else:
        result = matchobj.group(1)
        print(result)

def fetch_linktext():
    html_content = open('list.html', 'r').read()
    pat=re.compile(r'href="(http://[\w\d\./]+)"')
    result=pat.findall(html_content)
    print(result)

def fetch_content():
    html_content = open('list.html', 'r').read()
    print(html_content)
    pat=re.compile(r'<div class="text-title">([^<])<span class="article-tag">')
    matchobj = pat.match(html_content)
    if matchobj==None:
        print("匹配失败")
    else:
        result = matchobj.group()
        print(result)

if __name__=="__main__":
    fetch_content()