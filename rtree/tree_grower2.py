from rtree import index
import pandas as pd
import os
import time

#Sunarthsh pou kanei swap tis metavlhtes se periptwsh pou arxizoume apo thn megaluterh timh
def validate_and_fix_coordinates(minx, maxx, miny, maxy, mint, maxt):
    if minx > maxx:
        minx, maxx = maxx, minx
    if miny > maxy:
        miny, maxy = maxy, miny
    if mint > maxt:
        mint, maxt = maxt, mint
    return minx, maxx, miny, maxy, mint, maxt

index_name = '3d_index'

#Katharizoume to cache apo ta arxeia index
if os.path.exists(index_name + '.data') and os.path.exists(index_name + '.index'):
    os.remove(index_name + '.data')
    os.remove(index_name + '.index')

#Fortwnoume ta dedomena apo to csv
csv_file = '202102-divvy-tripdata.csv'
df = pd.read_csv(csv_file).head(1000)  # Read only the first 10 rows
df = df[['start_lat', 'end_lat', 'start_lng', 'end_lng', 'started_at', 'ended_at']]
df.columns = ['MinX', 'MaxX', 'MinY', 'MaxY', 'MinZ', 'MaxZ']

#Metatrepoume ta timestamps se integers
df['MinZ'] = pd.to_datetime(df['MinZ']).astype('int64') // 10**9
df['MaxZ'] = pd.to_datetime(df['MaxZ']).astype('int64') // 10**9

#Orizoume ta xarakthristika tou R-Tree
p = index.Property()
p.dimension = 3
p.dat_extension = 'data'
p.idx_extension = 'index'
idx3d = index.Index('3d_index', properties=p, leaf_capacity=3)



#Vazoume to kathe entry tou csv sto dentro, efoson ta perasoume apo elegxo
start_time = time.time()
for i, row in df.iterrows():
    try:
        minx, maxx, miny, maxy, mint, maxt = validate_and_fix_coordinates(row['MinX'], row['MaxX'], row['MinY'], row['MaxY'], row['MinZ'], row['MaxZ'])
        idx3d.insert(i, (minx, miny, mint, maxx, maxy, maxt))
        #Debug
        print(f"Inserted entry {i}: minx={minx}, maxx={maxx}, miny={miny}, maxy={maxy}, mint={mint}, maxz={maxt}")
    except Exception as e:
        pass
        #Merikes grammes apo to csv einai lathos kai petane errors, tis skiparoume (konta sto 0.01%)


end_time = time.time()
print("\n\nInsertion was a success!\nTime elapsed: ", {end_time-start_time}, "Seconds");

minx = float(input("Give the longitude minimum: "))
maxx = float(input("Give the longitude maximum: "))
miny = float(input("Give the latitude minimum: "))
maxy = float(input("Give the latitude maximum: "))
mint = float(input("Give the timestamp minimum: "))
maxt = float(input("Give the timestamp maximum: "))

start_time = time.time()
result = list(idx3d.intersection((minx, miny, mint, maxx, maxy, maxt)))
end_time = time.time()
print("\n\nIntersection indexes:", result, "\nTime elapsed: ", {end_time-start_time}, "Seconds")

