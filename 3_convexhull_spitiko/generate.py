import os
import random

f = open(os.getcwd()+'\\airbnb_listings_usa.csv', mode='w')

f.write("latitude,longitude,price\n")
for j in range(1000000):
    lat = round(random.uniform(30, 100), 4)
    long = round(random.uniform(-120, 180), 4)
    rent = int(round(random.uniform(0, 3000), 0))
    output = str(lat)+","+str(long)+","+str(rent)+"\n"
    f.write(output)