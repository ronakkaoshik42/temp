import os
import matplotlib.pyplot as plt


# S lock

os.system("rm -rf ./plot/")
os.system("mkdir ./plot")

os.system("gcc -o a plot_s_lock.c trie.c trie.h -g -pthread")

for i in range(2,100):
    os.system('./a '+str(i))

os.system("gcc -o a plot_rw_lock.c trie.c trie.h -g -pthread")

for i in range(2,100):
    os.system('./a '+str(i))

os.system("gcc -o a plot_hoh_lock.c trie.c trie.h -g -pthread")

for i in range(2,100):
    os.system('./a '+str(i))
