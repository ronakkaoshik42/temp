#!/usr/bin/env python
# coding: utf-8

import numpy as np 
import pandas as pd 
import matplotlib.pyplot as plt

data = pd.read_csv("output.csv",header =None)
a = np.arange(1,101)
del data[100]

plt.plot(a,data.loc[0],marker='o', color='yellow', label='FIFO')
plt.plot(a,data.loc[3],color='red', label='random')
plt.plot(a,data.loc[6],marker ='x', color='green', label='lru')
plt.plot(a,data.loc[9],color='blue', label='approx lru')
plt.xlabel("Cache Size (Blocks)")
plt.ylabel("Hit Rate (%)")
plt.legend(loc='upper left')
plt.title("Sequential Workload")
plt.show()


plt.plot(a,data.loc[1],marker='o', color='yellow', label='FIFO')
plt.plot(a,data.loc[4],color='red', label='random')
plt.plot(a,data.loc[7],marker ='x', color='green', label='lru')
plt.plot(a,data.loc[10],color='blue', label='approx lru')
plt.xlabel("Cache Size (Blocks)")
plt.ylabel("Hit Rate (%)")
plt.legend(loc='upper left')
plt.title("Random Workload")
plt.show()


plt.plot(a,data.loc[2],marker='o', color='yellow', label='FIFO')
plt.plot(a,data.loc[5],color='red', label='random')
plt.plot(a,data.loc[8],marker ='x', color='green', label='lru')
plt.plot(a,data.loc[11],color='blue', label='approx lru')
plt.xlabel("Cache Size (Blocks)")
plt.ylabel("Hit Rate (%)")
plt.legend(loc='upper left')
plt.title("80-20 Workload")
plt.show()




