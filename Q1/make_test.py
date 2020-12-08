import random
import string

l = []
for i in range(4,101):
    f = open('./tests/multi_thread/initial/{}.txt'.format(str(i)),'w')
    c=0
    while(c<1000):
        letters = string.ascii_lowercase
        word_len = random.choice(range(1,11))
        tmp = ''.join(random.choice(letters) for k in range(word_len))+'\n'
        if(tmp not in l):
            f.write(tmp)
            l.append(tmp)
            numbers = string.digits
            num_len = random.choice(range(1,3))
            f.write(''.join(random.choice(numbers) for k in range(num_len))+'\n')
            c+=1
    f.close()

for i in range(4,101):
    f = open('./tests/multi_thread/find/{}.txt'.format(str(i)),'w')
    for j in range(180):
        tmp_ind = random.choice(range(len(l)))
        f.write(l[tmp_ind])
        l.remove(l[tmp_ind])
    for j in range(40):
        letters = string.ascii_lowercase
        word_len = random.choice(range(1,11))
        tmp = ''.join(random.choice(letters) for k in range(word_len))+'\n'
        f.write(tmp)
    f.write("--\n")
    f.close()

