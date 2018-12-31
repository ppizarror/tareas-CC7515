from subprocess import Popen, check_output, check_call, PIPE, call
import time
datan = [64, 70, 80, 96, 100, 120, 128, 130, 140, 150, 160, 170, 180, 190, 196, 200, 210, 220, 224, 230, 240, 250, 256, 260, 270, 280, 288, 290, 300, 310, 320, 330, 340, 352, 360, 370, 380, 384, 400, 410, 416, 420, 430, 440, 448, 450, 460, 470, 480, 490, 500, 512]
# datan = [32, 40]
datatime = []

a = open('NxM.txt', 'w')
a.write('1000\n1000')
a.close()

for k in range(len(datan)):
    a = open('BLOCK_SIZE.txt', 'w')
    a.write('{0}'.format(datan[k]))
    a.close()
    for n in range(2):
        a = open('IF.txt', 'w')
        a.write('{0}'.format(n))
        a.close()
        your_exe_file_address = 'T3-CC7515-CUDA.exe'
        process = Popen([your_exe_file_address], stdout=PIPE, stderr=PIPE, shell=True)
        stdout, stderr = process.communicate()
        nops = int(str(stdout).split('\\r\\n')[5].split(': ')[1])
        print('bloque: {0}, ops: {1}, if: {2}'.format(datan[k], nops, n))
        datatime.append(nops)
        time.sleep(1)

# Escribe el archivo
a = open('resultsIF.txt', 'w')
i = 0
for k in range(len(datan)):
    a.write('{0}\t{1}\t{2}\n'.format(datan[k], datatime[i], datatime[i+1]))
    i = i + 2
a.close()
