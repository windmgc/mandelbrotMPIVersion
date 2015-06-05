__author__ = 'windmgc'

import os
import numpy as np
import pylab as pl

xasix=[]
mandelResult=[]

for i in range(2,25,1):
    average=0
    for j in range(1,11,1):
        command = "source /etc/profile; mpirun -n "+ str(i) +" /home/windmgc/mandel.out"  #Change this line to the address of executable file.
        strResult = os.popen(command).read()
        strSplit = strResult.split("\n")
        # print strSplit[0]
        average+=float(strSplit[0])

    average= average/10.0
    mandelResult.append(average)
    print "Thread %d finished.",i

for j in range(2,25,1):
    xasix.append(j)

print xasix
print mandelResult

pl.plot(xasix,mandelResult)
pl.show()
