import os
import random

f = open(os.getcwd()+'\\dump.csv', mode='w')


for j in range(30000000):
    lat = round(random.uniform(30, 35), 4)
    long = round(random.uniform(-120, -115), 4)
    rent = int(round(random.uniform(0, 3000), 0))
    output = str(lat)+","+str(long)+","+str(rent)+"\n"
    f.write(output)