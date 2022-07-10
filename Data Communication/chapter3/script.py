import os  #for exit
import time
from matplotlib.pyplot import MultipleLocator
import matplotlib.pyplot as plt

os.system("gcc prog2.c")

rate = []
N=80
os.system('rm -r output.txt')
time = 0
for i in range(80):
        time = i*0.5
        os.system('./a.out 50 0.2 0.2 {} 2'.format(time))
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号
rate = []
N=80
with open('output3_nobuffer.txt','r') as f:
    while(1):
        fstr = f.readline()
        if(fstr == ''):
            break
        recv_base = fstr.split()[0]
        t = fstr.split()[1]
        rate.append([recv_base,t])
print(rate)
x=[i*0.5 for i in range(N)]
y= []
z= []
for i in range(len(rate)):
    y.append(int(rate[i][0]))
    z.append(float(rate[i][1]))


rate2 = []
with open('output3_buffer.txt','r') as f:
    while(1):
        fstr = f.readline()
        if(fstr == ''):
            break
        recv_base = fstr.split()[0]
        t = fstr.split()[1]
        rate2.append([recv_base,t])

x2=[i*0.5 for i in range(N)]
y2= []
z2= []
for i in range(len(rate2)):
    y2.append(int(rate2[i][0]))
    z2.append(float(rate2[i][1]))


plt.figure(1,figsize=(10, 10), dpi=100)
plt.scatter(x, y, c='red',label='No Buffer')
plt.scatter(x, y2, c='blue',label='Buffer')

plt.xticks(range(0, 40, 10))
plt.yticks(range(0, int(max(y)), int(int(max(y))/10)))
plt.xlabel("lambda值", fontdict={'size': 16})
plt.ylabel("B向layer总发送次数", fontdict={'size': 16})
plt.title("lambda值与B向layer总发送次数关系图", fontdict={'size': 20})


plt.figure(2,figsize=(10, 10), dpi=100)

plt.scatter(x, z, c='red',label='No Buffer')
plt.scatter(x, z2, c='blue',label='Buffer')

plt.yticks(range(0, int(max(z2)), int(int(max(z2))/10)))
plt.xlabel("lambda值", fontdict={'size': 16})
plt.ylabel("程序运行时间", fontdict={'size': 16})
plt.title("lambda值与运行时间关系", fontdict={'size': 20})
plt.legend(loc='best')

plt.show()