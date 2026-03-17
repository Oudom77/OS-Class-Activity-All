import threading
import time
import random

# Shared buffer
particle_buffer = []

# Semaphores
empty = threading.Semaphore(100)   # 100 empty particle slots
full = threading.Semaphore(0)      # 0 filled particle slots initially
mutex = threading.Semaphore(1)     # binary semaphore for buffer access

# Lock just for clean printing
print_lock = threading.Lock()

# Global particle counter to make particle names unique
particle_id = 1
particle_id_lock = threading.Lock()


def get_new_pair():
    global particle_id
    with particle_id_lock:
        p1 = f"P{particle_id}"
        particle_id += 1
        p2 = f"P{particle_id}"
        particle_id += 1
    return p1, p2


def producer(name):
    while True:
        # Produce a pair
        p1, p2 = get_new_pair()

        # Need 2 empty spaces
        empty.acquire()
        empty.acquire()

        # Exclusive access to buffer
        mutex.acquire()
        particle_buffer.append(p1)
        particle_buffer.append(p2)

        with print_lock:
            print(f"{name} produced pair ({p1}, {p2})")
            print(f"Buffer after produce: {particle_buffer}")
        mutex.release()

        # Signal that 2 particles are available
        full.release()
        full.release()

        time.sleep(random.uniform(0.5, 1.5))


def consumer():
    while True:
        # Need 2 particles available
        full.acquire()
        full.acquire()

        # Exclusive access to buffer
        mutex.acquire()
        p1 = particle_buffer.pop(0)
        p2 = particle_buffer.pop(0)

        with print_lock:
            print(f"Consumer fetched pair ({p1}, {p2})")
            print(f"Buffer after consume: {particle_buffer}")
            print("Packaged and shipped\n")
        mutex.release()

        # Signal that 2 spaces are now free
        empty.release()
        empty.release()

        time.sleep(random.uniform(1.0, 2.0))


# Create multiple producer threads
producers = []
for i in range(3):   # 3 producers
    t = threading.Thread(target=producer, args=(f"Producer-{i+1}",), daemon=True)
    producers.append(t)

# Create one consumer thread
consumer_thread = threading.Thread(target=consumer, daemon=True)

# Start all threads
for t in producers:
    t.start()

consumer_thread.start()

# Keep main thread alive
while True:
    time.sleep(1)