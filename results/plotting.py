
import matplotlib.pyplot as plt
import csv
import sys
x = []
y = []
  
with open(sys.argv[1],'r') as csvfile:
    lines = csv.reader(csvfile, delimiter=',')
    next(lines,None)
    for row in lines:
        x.append(int(row[0]))
        y.append(float(row[4]))
  
plt.plot(x, y, color = 'g', marker = 'o')
plt.xticks(rotation = 25)
plt.xlabel('Number of Processes')
plt.ylim(0,y[-1]+(y[-1]*20/100))
plt.ylabel('Time Execution')
plt.title('Weak Scalability', fontsize = 20)
plt.grid()
plt.show()
