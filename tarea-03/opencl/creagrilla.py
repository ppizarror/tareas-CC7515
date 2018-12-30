from subprocess import Popen, check_output, check_call, PIPE, call
import time
datan = [100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
datatime = []

for m in range(len(datan)):
    for n in range(len(datan)):
        a = open('NxM.txt', 'w')
        a.write('{0}\n'.format(datan[n]))
        a.write('{0}\n'.format(datan[m]))
        a.close()
        your_exe_file_address = 'T3-CC7515-OPENCL.exe'
        process = Popen([your_exe_file_address], stdout=PIPE, stderr=PIPE, shell=True)
        stdout, stderr = process.communicate()
        nops = int(str(stdout).split('\\r\\n')[2].split(': ')[1])
        print('{0}x{1}, ops: {2}'.format(datan[n], datan[m], nops))
        datatime.append(nops)
        time.sleep(1)

# Escribe el archivo
a = open('results.txt', 'w')
i = 0
for m in range(len(datan)):
    for n in range(len(datan)):
        if n < (len(datan) - 1):
            a.write('{0}\t'.format(datatime[i]))
        else:
            a.write('{0}'.format(datatime[i]))
        i = i + 1
    if m < (len(datan)-1):
        a.write('\n')
a.close()
