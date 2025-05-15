
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3

int buffer[BUFFER_SIZE]; // Transaction queue
int in = 0, out = 0;

pthread_mutex_t lock;
sem_t empty, full;

void *producer(void *arg) {
    int id = *(int *)arg;
    while (1) {
        int transaction = rand() % 100 + 1;

        sem_wait(&empty);
        pthread_mutex_lock(&lock);

        buffer[in] = transaction;
        printf("Producer %d produced: %d → buffer[%d]\n", id, transaction, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&lock);
        sem_post(&full);

        sleep(rand() % 2 + 1);
    }
    return NULL;
}

void *consumer(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&lock);

        int transaction = buffer[out];
        printf("Consumer %d consumed: %d ← buffer[%d]\n", id, transaction, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&lock);
        sem_post(&empty);

        sleep(rand() % 2 + 1);
    }
    return NULL;
}

int main() {
    pthread_t prod_threads[NUM_PRODUCERS];
    pthread_t cons_threads[NUM_CONSUMERS];
    int ids[NUM_PRODUCERS > NUM_CONSUMERS ? NUM_PRODUCERS : NUM_CONSUMERS];

    pthread_mutex_init(&lock, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        ids[i] = i + 1;
        pthread_create(&prod_threads[i], NULL, producer, &ids[i]);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        ids[i] = i + 1;
        pthread_create(&cons_threads[i], NULL, consumer, &ids[i]);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(prod_threads[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(cons_threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
