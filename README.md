## 🧵 Multithreaded Data Pipeline with Synchronization – (C, POSIX Threads)

This project implements a multithreaded data pipeline using the classic **producer-consumer model**. Built in C with **POSIX threads**, the program uses **semaphores** and **mutexes** to synchronize access to a shared circular buffer. Multiple **producers** generate **transactions, while consumers process them concurrently in a thread-safe environment.

---

## 📦 Features

- Multiple producer and consumer threads
- Circular buffer implementation
- Thread-safe synchronization with:
  - **``pthread_mutex_t``** (mutex)
  - **``sem_t``** (semaphores)
- Real-time console output of operations

---

## 🧠 How It Works

- **Producers** generate random transactions and insert them into a circular buffer.
- **Consumers** remove and process items from the buffer.
- A **mutex** ensures exclusive access to the buffer.
- **Semaphores** manage the count of available and used buffer slots.

The program creates multiple producer and consumer threads that communicate through a shared circular buffer. Producers generate random transactions and insert them into the buffer if space is available. Consumers remove transactions from the buffer when data is present. Access to the buffer is synchronized using a mutex to prevent race conditions, while semaphores manage the number of available and occupied slots. This setup ensures safe, concurrent data flow between producers and consumers in a classic multithreaded pipeline.

``` C
void *producer(void *arg) {
    int item = generate_random(); // simulate a new transaction

    sem_wait(&empty);             // wait for empty slot
    pthread_mutex_lock(&lock);    // lock the buffer

    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&lock);  // unlock the buffer
    sem_post(&full);              // signal item added
}

void *consumer(void *arg) {
    sem_wait(&full);              // wait for item to consume
    pthread_mutex_lock(&lock);    // lock the buffer

    int item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&lock);  // unlock the buffer
    sem_post(&empty);             // signal slot freed
}
```
This captures:

- Circular buffer usage (in, out)

- Semaphores for full/empty slots

- Mutex for safe access

- Data flow between producer and consumer

---

## 🛠️ How to build

``` bash
gcc producer_consumer.c -o producer_consumer -lpthread
```
This compiles the program in order to produce an executable but since we are running **kernal operations** you will need to run more commands.

---

## ▶️ How to run

```bash
chmod +x producer_consumer
./producer_consumer
```
The program has been designed to run continuosly to stop at anytime **``ctrl+C``**

---

## 💻 Output

![image](https://github.com/rishabhkalia6/Multithreaded-Data-Pipeline-with-Synchronization-C-POSIX-Threads-/blob/main/screenshots/Screenshot%20from%202025-05-17%2011-56-33.png?raw=true)


This screenshot shows the real-time terminal output of the **multithreaded producer-consumer pipeline**, implemented in C using POSIX threads, semaphores, and mutex locks.

- Multiple **producer threads** are generating random transaction values and inserting them into a fixed-size circular buffer.

- Concurrently, multiple consumer threads are removing and processing these values.

- The buffer is synchronized using:

  - A mutex lock to ensure exclusive access

  - Two semaphores: **``empty``** (tracks available slots) and **``full**`` (tracks filled slots)

The output clearly shows:

- Which producer inserted which value and at which buffer index

- Which consumer removed a value and from which buffer index

- Continuous activity between producers and consumers, demonstrating proper synchronization and buffer state cycling

This confirms that the data pipeline operates correctly, without race conditions or overflows, showcasing effective multithreading and inter-thread communication.

