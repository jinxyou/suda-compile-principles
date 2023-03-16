# coding=UTF-8
import re
from fpdf import FPDF, HTMLMixin


class MyFPDF(FPDF, HTMLMixin):
    pass


def html2pdf(html_text, path):  # html格式转pdf函数
    pdf = MyFPDF()
    pdf.add_page()
    pdf.write_html(html_text)
    pdf.output(path, 'F')


def find_element(pat,text):     # 正则查找元素
    element=pat.findall(text)
    if len(element)==1:
        return element[0]
    elif len(element)>1:
        return element
    else:
        return ''

def clear_text(text):       # 清除文本中多余的代码
    lines=[]
    for line in text.split(r'\n'):
        line=line.strip()
        if len(line)>0:
            lines.append(line)
    text=' '.join(lines)
    lines = []
    for line in text.split(r'\t'):
        line=line.strip()
        if len(line)>0:
            lines.append(line)
    text=' '.join(lines)
    lines = []
    for line in text.split(r'\r'):
        line=line.strip()
        if len(line)>0:
            lines.append(line)
    text=' '.join(lines)
    lines = []
    for line in text.split('\\'):
        line=line.strip()
        if len(line)>0:
            lines.append(line)
    text=' '.join(lines)

    return text

if __name__ == "__main__":
    content=open('example.tex',"rb").read()
    content=str(content)

    pat_doc=re.compile(r'\\begin{document}(.+?)\\end{document}',re.S)       # 获取文档内容
    doc=find_element(pat_doc,content)
    print("doc:",doc)

    pat_title=re.compile(r'\\title{(.+?)}',re.S)        #获取标题
    title=find_element(pat_title,content)
    title='<h1>%s</h1>\n\n' % title
    doc=re.sub(pat_title,title,doc)
    # print("title:",title)

    pat_abstract=re.compile(r'\\begin{abstract}(.+?)\\end{abstract}',re.S)      # 获取摘要
    abstract=find_element(pat_abstract,content)
    abstract='<p>%s</p>\n\n' % abstract
    doc=re.sub(pat_abstract,abstract,doc)
    # print("abstract:",abstract)

    pat_section=re.compile(r'\\section{(.+?)}(.+?)\\section',re.S)      # 获取段落
    section_title, section_context = find_element(pat_section, content)
    section_title='<h2>%s</h2>\n\n' % section_title
    section_context='<p>%s</p>\n\n' % section_context
    doc=re.sub(pat_section,section_title+section_context,doc)
    # print("section:",section_title)
    # print(section_context)

    pat_subsection=re.compile(r'\\subsection{(.+?)}(.+?)\\subsection',re.S)     # 获取二级段落
    subsection_title, subsection_context = find_element(pat_subsection, content)
    subsection_title='<h3>%s</h3>\n\n' % subsection_title
    subsection_context='<p>%s</p>\n\n' % subsection_context
    doc=re.sub(pat_subsection,subsection_title+subsection_context,doc)
    # print("subsection:",subsection_title)
    # print(subsection_context)

    pat_subsubsection=re.compile(r'\\subsubsection{(.+?)}(.+?)\\subsubsection',re.S)        # 获取三级段落
    subsubsection_title, subsubsection_context = find_element(pat_subsubsection, content)
    subsubsection_title='<h4>%s</h4>\n\n' % subsubsection_title
    subsubsection_context='<p>%s</p>\n\n' % subsubsection_context
    doc=re.sub(pat_subsubsection,subsubsection_title+subsubsection_context,doc)
    # print("subsubsection:",subsubsection_title)
    # print(subsubsection_context)

    pat_itemize=re.compile(r'\\begin{itemize}(.+?)\\end{itemize}',re.S)     # 获取列表
    itemize=find_element(pat_itemize,content)
    itemize_text='<ol>'
    for i in itemize:
        temp=re.sub(r'\\item','',i)
        itemize_text += '<li>%s</li>' % temp
    itemize_text += '</ol>'
    doc=re.sub(pat_itemize,itemize_text,doc)
    # print("itemize:",itemize)

    pat_tabular=re.compile(r'\\begin{tabular}(.+?)\\end{tabular}',re.S)     # 获取表格
    tabular=find_element(pat_tabular,content)
    # print(tabular)

    pat_emph=re.compile(r'\\(emph){(.+?)}',re.S)        # 获取斜体字
    pat_emph.sub(r"<i>\2</i>",doc)


    # print(doc)
    doc=clear_text(doc)
    html2pdf(doc, 'result.pdf') # 输出