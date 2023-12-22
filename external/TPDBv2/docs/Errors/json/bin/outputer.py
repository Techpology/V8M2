#!/usr/bin/env python3

import json

mainComp = '''
<div class="flex flex-row w-full flex flex-col">
  <h3 id="ID" class="text-2xl mb-4">TITLE1</h3>
  COMPS
</div>
'''

subComp = '''
    <div class="pl-8 mb-12">
      <p id="ID" class="text-lg font-bold">TITLE2</p>
      <p class="text-lg ml-10">
        DESCRIPTION
      </p>
      <p class="text-lg ml-10">
        <b>Args:</b><br/>
        <div style="margin-left: 60px;" >
            ARGS
        </div>
      </p>
       CODE_SNIPPETS
      <b>Notes:</b><br/>
        NOTES
        FOOTER
    </p>
  </div>
'''

subcomp_divider = '<div style="border-bottom: 2px dotted black; width: 100%; margin-bottom: 40px;"></div>'

argComp = '''
      <b>-</b>ARG<br/>
'''

codeComp = '''
<pre class="w-full bg-slate-200 rounded-lg flex flex-col mb-6">
    <code class="language-c line-numbers">
        CONTENT
    </code>
</pre>

'''

noteComp = '''<span style="margin-left: 30px;"></span><b>-</b> NOTE <br/><br/>'''


def getCodes(codes):
    code_snippets = ""
    for code in codes:
        lines = ""
        for line in code:
            lines += line+"\n"
        code_snippets+=f'{codeComp.replace("CONTENT",lines)}\n'
    return code_snippets

def getNotes(notes):
    notecomps = ""
    for note in notes:
        notecomps+=f"{noteComp.replace('NOTE',note)}\n"
    return notecomps

def getArgs(args):
    argcomps = ""
    for arg in args:
        argcomps+=f"{argComp.replace('ARG',arg)}"
    return argcomps

def getSubs(subs):
    subcomps = ""
    count = 0
    for sub in subs:
        print("set title")
        subcomp = subComp.replace("TITLE2",sub["title"])

        print("set id")
        subcomp = subcomp.replace("ID",sub["title"].split(" ")[0][1:])

        print("set desv")
        subcomp = subcomp.replace("DESCRIPTION",sub["Desc"].replace("\n","<br>"))

        print("set args")
        subcomp = subcomp.replace("ARGS",getArgs(sub["Args"]))
        if "code_snippets" in sub:
            print("set code snippets")
            subcomp = subcomp.replace("CODE_SNIPPETS",getCodes(sub["code_snippets"]))

        print("set notes")
        subcomp = subcomp.replace("NOTES",getNotes(sub["Notes"]))

        print("set footer")
        subcomp = subcomp.replace("FOOTER",sub["Footer"].replace("\n","<br>"))

        print("add sub comp")
        subcomps += (subcomp)
        # add divider for all but the last
        if count < len(subs)-1:
            subcomps+=subcomp_divider

        count +=1

    return subcomps

def getMainComp(main):
    comp = mainComp.replace("TITLE1",main["title"])
    comp = comp.replace("ID",main["title"].split(" ")[0][1:])
    comp = comp.replace("COMPS",getSubs(main["sub"]))

    return comp

def output(args):
    path = "./json.json"
    print("read json")
    with open(path,"r") as f:
        try:
            data = json.loads(f.read())
        except Exception as e:
            print(str(e))
            return 0

    print("start setting values")
    mains = ""
    for main in data["errors"]:
        mains += (getMainComp(main))

    print("retrive template html")
    text = ""
    with open("./bin/template.html","r") as f:
        text = f.read()
        text = text.replace("__ERRORS__",mains)

    print("write content to index.html")
    with open("index.html","w") as f:
        f.write(text)
