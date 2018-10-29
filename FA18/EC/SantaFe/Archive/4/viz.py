import pygame
import imageio

pygame.init()

scale = 16


states = []
curState = []
with open('best.txt') as fp:
    for line in fp:
        line = line.strip()
        if line == '':
            if curState != []:
                states.append(curState)
                curState = []
        else:
            isState = True
            for c in line:
                if c not in '01<>^v':
                    isState = False
                    break
            if isState:
                curState.append(line)

surf = pygame.Surface((len(states[0][0]) * scale, len(states[0]) * scale))
images = []
for stateNum in range(len(states)):
    surf.fill((200,200,200))
    state = states[stateNum]
    for j in range(len(state)):
        line = state[j]
        for i in range(len(line)):
            if line[i] == '0':
                pygame.draw.rect(surf, (0,0,0), (i * scale + 1, j * scale + 1, scale-2, scale-2))
            elif line[i] == "1":
                pygame.draw.rect(surf, (255,255,0), (i * scale + 1, j * scale + 1, scale-2, scale-2))
            else:
                pygame.draw.rect(surf, (0,0,0), (i * scale + 1, j * scale + 1, scale-2, scale-2))
                tl = (i * scale + 1, j * scale + 1)
                tr = (i * scale + scale-1, j * scale + 1)
                bl = (i * scale + 1, j * scale + scale-1)
                br = (i * scale + scale-1, j * scale + scale-1)
                t = (i * scale + (scale//2), j * scale + 1)
                b = (i * scale + (scale//2), j * scale + scale-1)
                l = (i * scale + 1, j * scale + (scale//2))
                r = (i * scale + scale-1, j * scale + (scale//2))
                if  line[i] == "v":
                    pygame.draw.polygon(surf, (255,0,0), (tl, tr, b))
                elif  line[i] == ">":
                    pygame.draw.polygon(surf, (255,0,0), (tl, r, bl))
                elif  line[i] == "<":
                    pygame.draw.polygon(surf, (255,0,0), (tr, br, l))
                elif  line[i] == "^":
                    pygame.draw.polygon(surf, (255,0,0), (bl, t, br))
    pygame.image.save(surf, './images/' + str(stateNum) + '.png')
    images.append(imageio.imread('./images/' + str(stateNum) + '.png'))

imageio.mimsave('./viz.gif', images)
