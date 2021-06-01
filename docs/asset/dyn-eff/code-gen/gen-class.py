from pprint import pprint
from random import randint
from io import TextIOWrapper

class Node:
    def __init__(self, f, id: int) -> None:
        if f == None:
            self.name = f'C{id}'
            self.fthname = None
        else:
            self.name = f'{f.name}C{id}'
            self.fthname = f.name
        self.childs = []
        self.funcs = []
        self.genfuncs()

    def stretch(self, cnt=10) -> None:
        for id in range(cnt):
            c = Node(self, id)
            self.childs.append(c)

    def genfuncs(self, cnt=5) -> None:
        for id in range(cnt):
            self.funcs.append((f'f{id}', randint(1, 50)))

    def __str__(self) -> str:
        if self.fthname == None:
            buf = f'class {self.name}' + ' {\n'
        else:
            buf  = f'class {self.name}: public {self.fthname}' + '{\n'
        buf +=  'public:\n'
        buf +=  '    virtual void whoami() {cout << "' + self.name + '" << endl;}\n'
        for f, d in self.funcs:
            buf += '    virtual void ' + f + '() {cout << "' + str(d) + '" << endl;}\n'
        buf += '};'
        return buf

# build tree
def BuildTree(h: Node, maxLevel, curLevel) -> None:
    h.stretch()

    if curLevel >= maxLevel:
        return
    c = h.childs[randint(0, len(h.childs)-1)]
    BuildTree(c, maxLevel, curLevel+1)

root = Node(None, 0)
BuildTree(root, 100, 1)

names = []
def OutputTree(h: Node, out: TextIOWrapper):
    names.append(h.name)

    out.write(str(h))
    out.write('\n')
    for c in h.childs:
        OutputTree(c, out)

with open('lotsofclass.h', 'w', encoding='utf-8') as out:
    out.write('#include <iostream>\n')
    out.write('using namespace std;\n\n')
    OutputTree(root, out)

with open('classname.txt', 'w', encoding='utf-8') as out:
    for n in names:
        out.write(n)
        out.write('\n')

