
func_names = ['INVALID', 'IF', 'RIGHT', 'LEFT', 'FORWARD', 'IS_FOOD_FORWARD', 'SEQ']

class Node:
    def __init__(self, node, level, parent, index):
        self.node = node
        self.level = level
        self.parent = parent
        self.children = []
        self.index = str(index)
    def getString(self):
        s = ''
        s += '  ' * self.level
        s += str(self.node)     
        s += ' -> '     
        s += str(self.level)  
        s += '\n'
        for c in self.children:
            s += c.getString()
        return s
    def getDotString(self):
        s = ''
        s += '\t' + self.index + '[label = ' + func_names[self.node] + ']\n'
        for c in self.children:
            s += '\t' + self.index + ' -> ' + c.index + '\n'
        for c in self.children:
            s += c.getDotString()
        return s
    def print(self):
        print('  ' * level, node, '->', level) 
        print('children:', len(self.children))
        
root = None
current = None
idx = 0
with open('best.txt') as fp:
    for line in fp:
        if '(' in line and ')' in line:
            line = line.strip()
            sp = line.split(' ')
            node = int(sp[0])
            level = int(sp[1][1:-1])
            n = Node(node, level, None, idx)
            idx+=1
            if root == None:
                root = n
            else:
                while current.level + 1 > n.level:
                    current = current.parent
                current.children.append(n)
                n.parent = current 
            current = n
#print(root.getString())
with open('tree.dot', 'w+') as fp:
    fp.write('digraph G{\n')
    fp.write(root.getDotString())
    fp.write('}')

