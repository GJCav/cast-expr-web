from random import randint

names = []
with open('classname.txt', 'r', encoding='utf-8') as f:
    for line in f:
        names.append(line.strip())


names.sort(key=lambda x: len(x), reverse=True)

objCnt = 2
objs = []
for i in range(objCnt):
    objs.append((f'objA{i}', names[i]))

out = open('cast-frag.cpp', 'w', encoding='utf-8')

for v, c in objs:
    out.write(f'C0 *{v} = new {c};\n')

out.write('\n')

cast_count = 10
def static_cast(varname: str, varclass: str):
    global out
    lvl = len(varclass) // 2

    for _ in range(cast_count):
        tgt = varclass[:randint(1, lvl)*2]
        out.write(f'static_cast<{tgt}*>({varname});\n')

def dyn_cast(varname: str, varclass: str):
    global out
    lvl = len(varclass) // 2

    for _ in range(cast_count):
        tgt = varclass[:randint(1, lvl)*2]
        out.write(f'assert(dynamic_cast<{tgt}*>({varname}) != nullptr);\n')

for v, c in objs:
    dyn_cast(v, c)
    out.write('\n')

out.close()