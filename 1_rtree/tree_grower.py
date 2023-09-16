from rtree import index
import pandas as pd
import os
import time

index_name = '3d_index'

#Katharizoume to cache apo ta arxeia index
if os.path.exists(index_name + '.data') and os.path.exists(index_name + '.index'):
    os.remove(index_name + '.data')
    os.remove(index_name + '.index')

#Fortwnoume ta dedomena apo to csv kai ta vazoume sto 3D R-tree
csv_file = "synthetic_spatio_temporal_data.csv"
df = pd.read_csv(csv_file)

start_time = time.time()
#Orizoume ta xarakthristika tou R-Tree
p = index.Property()
p.dimension = 3
p.dat_extension = 'data'
p.idx_extension = 'index'
idx3d = index.Index('3d_index', properties=p, leaf_capacity=3)

for i, row in df.iterrows():
    minx, maxx, miny, maxy, mint, maxt = row['MinX'], row['MaxX'], row['MinY'], row['MaxY'], row['MinZ'], row['MaxZ']
    idx3d.insert(i, (minx, miny, mint, maxx, maxy, maxt))
    
    #Debug
    #print(f"Inserted entry {i}: minx={minx}, maxx={maxx}, miny={miny}, maxy={maxy}, mint={mint}, maxz={maxt}")


end_time = time.time()
print("Insertion was a success!\nTime elapsed: ", {end_time-start_time}, "Seconds");
#
minx = float(input("Give the longitude minimum: "))
maxx = float(input("Give the longitude maximum: "))
miny = float(input("Give the latitude minimum: "))
maxy = float(input("Give the latitude maximum: "))
mint = float(input("Give the timestamp minimum: "))
maxt = float(input("Give the timestamp maximum: "))
result = list(idx3d.intersection((minx, miny, mint, maxx, maxy, maxt)))
print("Intersection indexes:", result)

