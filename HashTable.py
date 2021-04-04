n, q = (int(j) for j in input().split())
A = [[] for i in range(n)]


def hash(x):
    return (x % n)


def find(x):
    i = hash(x)
    while A[i] != []:
        if A[i] == x:
            return(1)
        i = hash(i+1)
    return(0)


def add(x):
    i = hash(x)
    while A[i] != [] and A[i] != 'deleted':
        if A[i] == x:
            return(i)
        i = hash(i+1)
    A[i] = x
    return(i)


def remove(x):
    i = hash(x)
    while A[i] != []:
        if A[i] == x:
            A[i] = 'deleted'
            return(1)
        i = hash(i+1)
    return(0)


for i in range(q):
    word, num = (str(j) for j in input().split())
    if word == 'add':
        print(add(int(num)))
    if word == 'find':
        print(find(int(num)))
    if word == 'remove':
        print(remove(int(num)))