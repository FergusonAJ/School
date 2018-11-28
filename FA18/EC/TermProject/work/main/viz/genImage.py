import pygame

with open('lastGen.txt', 'r') as fp:
    w = 0
    h = 0
    sw = 0
    sh = 0
    x = 0
    y = 0
    individuals = []
    curScore = None
    curInd = None
    for line in fp:
        line = line.strip()
        L = line.split()
        if len(L) == 2:
            if L[0] == 'FULL_BOARD_W_H:':
                L2 = L[1].split('x')
                w = int(L2[0])
                h = int(L2[1])
            elif L[0] == 'INDIVIDUAL_W_H:':
                L2 = L[1].split('x')
                sw = int(L2[0])
                sh = int(L2[1])
            elif L[0] == 'INDIVIDUAL_X_Y:':
                L2 = L[1].split('x')
                x = int(L2[0])
                y = int(L2[1])
        elif len(L) == 1:
            if L[0] != '':
                curScore = float(L[0])
        elif len(L) == 0:
            if curScore != None and curInd != None:
                individuals.append([curScore, curInd])
                curScore = None
                curInd = None
        else:
            if curInd == None:
                curInd = []
            for l in L:
                curInd.append(int(l))
           
pygame.init()
surf = pygame.Surface((w,h))

D = {}
for ind in individuals:
    surf.fill((0,0,0))
    for j in range(sh): 
        for i in range(sw):
            if ind[1][i + j * sw] == 1:
                surf.set_at((i + x,j + y),(255,255,255))
    num = 0
    if ind[0] in D:
        num = D[ind[0]]
    else:
        D[ind[0]] = 0
    pygame.image.save(surf, './images/' + str(ind[0]) + '_' + str(num) + '.png')
    D[ind[0]] += 1 
print('Done! Goodbye!')
