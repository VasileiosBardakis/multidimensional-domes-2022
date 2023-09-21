import matplotlib.pyplot as plt  # TODO: What else other than pyplot?

import csv

# all points
x_all = []
y_all = []
with open('points.csv', 'r') as allPointsFile:  
    plots = csv.reader(allPointsFile, delimiter = ',')
      
    for row in plots:
        x_all.append(float(row[0]))
        y_all.append(float(row[1]))

# convex hull points using red color
x_convex = []
y_convex = []
with open('convex.csv', 'r') as convexFile:  
    plots = csv.reader(convexFile, delimiter = ',')
      
    for row in plots:
        x_convex.append(float(row[0]))
        y_convex.append(float(row[1]))

plt.scatter(x_all,y_all, color = 'g', s=2)
plt.scatter(x_convex,y_convex, color = 'r', s=3)
plt.xlabel("latitude")
plt.ylabel("longitude")
plt.legend()
plt.show()

