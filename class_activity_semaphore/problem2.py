import threading

a = threading.Semaphore(1)
b = threading.Semaphore(0)
c = threading.Semaphore(0)

def process1():
    while True:
        a.acquire() #wait(a)
        print("H", end="")
        print("E", end="")
        b.release() #signal(b)
        b.release() #signal(b)

def process2():
    while True:
        b.acquire() #wait(b)
        print("L", end="")
        c.release() #signal(c)

def process3():
    while True:
        c.acquire() #wait(c)
        c.acquire() #wait(c)
        print("O", end="")

t1 = threading.Thread(target=process1)
t2 = threading.Thread(target=process2)
t3 = threading.Thread(target=process3)

t1.start()
t2.start()
t3.start()