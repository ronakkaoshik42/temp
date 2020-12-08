import matplotlib.pyplot as plt

def get_values(filename):
  f = open(filename,'r')
  l = f.read().split()

  x = []
  y = []

  for i in range(len(l)):
    x.append(float(l[i].split(',')[0]))
    y.append(float(l[i].split(',')[1]))

  return x,y

fig1 = plt.gcf()
x1,y1 = get_values('./plot/s_lock_write.txt')
x2,y2 = get_values('./plot/rw_lock_write.txt')
x3,y3 = get_values('./plot/hoh_lock_write.txt')
plt.plot(x1,y1,label = 's lock')
plt.plot(x2,y2,label = 'rw lock')
plt.plot(x3,y3,label = 'hoh lock')
plt.xlabel("Number of threads")
plt.ylabel("Time taken")
plt.title("Write workload")
plt.legend(loc="upper right")
plt.show()
plt.draw()
fig1.savefig("write_load.png", dpi=100)

fig2 = plt.gcf()
x1,y1 = get_values('./plot/s_lock_read.txt')
x2,y2 = get_values('./plot/rw_lock_read.txt')
x3,y3 = get_values('./plot/hoh_lock_read.txt')
plt.plot(x1,y1,label = 's lock')
plt.plot(x2,y2,label = 'rw lock')
plt.plot(x3,y3,label = 'hoh lock')
plt.xlabel("Number of threads")
plt.ylabel("Time taken")
plt.title("Write workload")
plt.legend(loc="upper right")
plt.show()
plt.draw()
fig1.savefig("read_load.png", dpi=100)

fig3 = plt.gcf()
x1,y1 = get_values('./plot/s_lock_read_write.txt')
x2,y2 = get_values('./plot/rw_lock_read_write.txt')
x3,y3 = get_values('./plot/hoh_lock_read_write.txt')
plt.plot(x1,y1,label = 's lock')
plt.plot(x2,y2,label = 'rw lock')
plt.plot(x3,y3,label = 'hoh lock')
plt.xlabel("Number of threads")
plt.ylabel("Time taken")
plt.title("Write workload")
plt.legend(loc="upper right")
plt.show()
plt.draw()
fig1.savefig("read_write_load.png", dpi=100)