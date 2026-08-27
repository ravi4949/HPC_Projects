#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t chopsticks[NUM_PHILOSOPHERS];

void *philosopher(void *arg) {
    int id = *(int *) arg;
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    for (int round = 0; round < 5; round++) {
        printf("Philosopher %d is thinking\n", id);
        fflush(stdout);
        usleep((rand() % 300 + 100) * 1000);

        printf("Philosopher %d wants to eat, picking up left chopstick %d\n", id, left);
        fflush(stdout);
        pthread_mutex_lock(&chopsticks[left]);

        printf("Philosopher %d picked up left chopstick %d, picking up right chopstick %d\n", id, left, right);
        fflush(stdout);
        pthread_mutex_lock(&chopsticks[right]);

        printf("Philosopher %d is eating\n", id);
        fflush(stdout);
        usleep((rand() % 300 + 100) * 1000);

        pthread_mutex_unlock(&chopsticks[right]);
        pthread_mutex_unlock(&chopsticks[left]);

        printf("Philosopher %d finished eating and put down both chopsticks\n", id);
        fflush(stdout);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    srand(42);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&chopsticks[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&chopsticks[i]);
    }

    printf("All philosophers have finished eating.\n");

    return 0;
}