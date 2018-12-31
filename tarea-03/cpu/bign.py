from subprocess import Popen, check_output, check_call, PIPE, call
import time
import math
# datan = [10, 25, 50, 75, 100, 250, 500, 750, 1000, 2500, 5000, 7500, 10000]
datan = [5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25]
datatime = []

for n in range(len(datan)):
    nn = datan[n]
    a = open('NxM.txt', 'w')
    a.write('{0}\n{0}'.format(nn))
    a.close()
    process = Popen(['MAIN.exe'], stdout=PIPE, stderr=PIPE, shell=True)
    stdout, stderr = process.communicate()
    try:
        nops = int(str(stdout).split('\\r\\n')[2].split(': ')[1])
    except:
        nops = -1
    print('{0}x{1}, ops: {2}'.format(nn, nn, nops))
    datatime.append(nops)
    time.sleep(1)

# Escribe el archivo
a = open('bign.txt', 'w')
i = 0
for n in range(len(datan)):
    if n < (len(datan) - 1):
        a.write('{0}\t'.format(datatime[i]))
    else:
        a.write('{0}'.format(datatime[i]))
    i = i + 1
    if n < (len(datan)-1):
        a.write('\n')
a.close()
