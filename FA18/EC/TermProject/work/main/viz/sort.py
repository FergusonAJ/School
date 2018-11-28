
D = {}

with open('tmp.txt') as fp:
    for line in fp:
        if line.strip() != '':
            if line not in D:
                D[line] = 0
            D[line] += 1
L = []
for k in D:
    L.append((k, D[k]))
L.sort(key=lambda x: x[1])
for x in reversed(L):
    print(x[0], x[1])
            
