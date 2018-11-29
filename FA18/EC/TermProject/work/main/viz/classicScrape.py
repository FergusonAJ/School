import sys

icFP = open('../output/ic.csv', 'w')
rulesetFP = open('../output/ruleset.csv', 'w')

icFP.write('id,gen,value\n')
rulesetFP.write('id,gen,value\n')

maxGen = 5
if len(sys.argv) > 1:
    maxGen = int(sys.argv[1])

for i in range(maxGen):
    idx = 0
    with open('../output/IC_Gen_' + str(i) + '.txt', 'r') as tmpFP:
        for line in tmpFP:
            line = line.strip()
            L = line.split(' ')
            if L[0] == 'Fitness:':
                icFP.write(str(idx))
                icFP.write(',')
                icFP.write(str(i))
                icFP.write(',')
                icFP.write(L[1])
                icFP.write('\n')
                idx+=1
    
    idx = 0
    with open('../output/Ruleset_Gen_' + str(i) + '.txt', 'r') as tmpFP:
        for line in tmpFP:
            line = line.strip()
            L = line.split(' ')
            if L[0] == 'Fitness:':
                rulesetFP.write(str(idx))
                rulesetFP.write(',')
                rulesetFP.write(str(i))
                rulesetFP.write(',')
                rulesetFP.write(L[1])
                rulesetFP.write('\n')
                idx+=1

icFP.close()
rulesetFP.close()
