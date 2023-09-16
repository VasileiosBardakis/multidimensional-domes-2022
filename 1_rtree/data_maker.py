import random
import csv

#Sunarthsh pou ftiaxnei ena tuxaio trajectory
def traj_maker():
    min_x = random.uniform(-180.0, 180.0)  # Longitude
    max_x = random.uniform(min_x, 180.0)
    min_y = random.uniform(-90.0, 90.0)    # Latitude
    max_y = random.uniform(min_y, 90.0)
    min_z = random.uniform(0, 1000000)     # Timestamp
    max_z = random.uniform(min_z, 1000000)
    return [min_x, max_x, min_y, max_y, min_z, max_z]

#Dhmiourgw N grammes
num_entries = 1000
trajectory_data = [traj_maker() for _ in range(num_entries)]

#Metaferw ta apotelesmata se ena csv arxeio
with open('synthetic_spatio_temporal_data.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['MinX', 'MaxX', 'MinY', 'MaxY', 'MinZ', 'MaxZ']) #H prwth grammh
    writer.writerows(trajectory_data)

print(f"Generated {num_entries} entries successfully!")