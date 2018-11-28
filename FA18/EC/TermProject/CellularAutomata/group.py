import pygame
import time
import random

SCREEN_WIDTH = 512
SCREEN_HEIGHT = 512

pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))

states = []
curState = []
with open('field.txt', 'r') as fp:
    for line in fp:
        stripped = line.strip()
        nonData = False
        for c in stripped:
            if c not in '.XO!':
                nonData = True
        if nonData or len(stripped) == 0:
            if curState != []:
                states.append(curState)
                curState = []
            continue
        curState.append(stripped)
numCols = len(states[0][0])
numRows = len(states[0])
print(numCols, 'x', numRows)
colSpacing = SCREEN_WIDTH // numCols
rowSpacing = SCREEN_HEIGHT // numRows


groupMap = {}
groupData = []
groupStates = []
with open('groups.txt', 'r') as fp:
    for line in fp:
        stripped = line.strip()
        nonData = False
        for c in stripped:
            if c not in 'x: 0123456789':
                nonData = True
        if nonData:
            if groupData != []:
                groupStates.append(groupData)
                groupData = []
            continue
        if len(stripped) > 1:
            L1 = stripped.split(':')
            L2 = L1[0].split('x')
            x = int(L2[0])
            y = int(L2[1])
            L3 = L1[1].strip().split()
            if len(L3) > 0:
                group = L3[0]
                if '0' in L3:
                    L3.remove('0')
                if group not in groupMap and len(L3) == 1:
                    groupMap[group] = (random.randint(0,255),random.randint(0,255),random.randint(0,255))
                groupData.append([x,y,L3]) 
print('Groups loaded!')
 
curState = 0
play = False
done = False
while not done:
    evt = pygame.event.poll()
    if evt.type == pygame.KEYDOWN:
        if evt.key == pygame.K_ESCAPE or evt.key == pygame.K_q:
            done = True
        elif evt.key == pygame.K_SPACE or evt.key == pygame.K_RIGHT:
            curState += 1
            if curState >= len(states):
                curState = 0
        elif evt.key == pygame.K_LEFT:
            curState -= 1
            if curState < 0:
                curState = len(states) - 1
        elif evt.key == pygame.K_RETURN:
            play = not play
        elif evt.key == pygame.K_r:
            curState = 0
    screen.fill((0,0,0))
    for x,y,group in groupStates[curState]:
        col = [0,0,0]
        for g in group:
            col[0] += groupMap[g][0]
            col[1] += groupMap[g][1]
            col[2] += groupMap[g][2]
        col[0] /= len(group)
        col[1] /= len(group)
        col[2] /= len(group)
        pygame.draw.rect(screen, col, (colSpacing * x, rowSpacing * y, colSpacing, rowSpacing))
    pygame.display.flip()
    if play:
        time.sleep(0.01)
        curState += 1
        if curState >= len(states):
            curState = 0

pygame.quit()
