#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <stdatomic.h>

#define MAX_RESOURCES 5

int available_resources = MAX_RESOURCES;
atomic_int mutex = 1;

void wait(atomic_int* mutex) {
    while (atomic_exchange(mutex, 0) == 0);
}

void signal(atomic_int* mutex) {
    atomic_store(mutex, 1);
}

int decrease_count(int count) {
    if (available_resources < count) {
        printf("decrease: %d, not enough resources: %d\n", count, available_resources);
        return -1;
    }
    else {
        // wait(&mutex);
        available_resources -= count;
        // signal(&mutex);
        printf("decrease: %d, available resources: %d\n", count, available_resources);
        return 0;
    }
}

int increase_count(int count) {
    // wait(&mutex);
    available_resources += count;
    // signal(&mutex);
    printf("increase: %d, available resources: %d\n", count, available_resources);
    return 0;
}

void use (int count) {
    wait(&mutex);
    int resut = decrease_count(count);
    signal(&mutex);
    if (resut == 0) {
        printf("use: %d\n", count);
        sleep(rand() % 2 + 1);
        wait(&mutex);
        increase_count(count);
        signal(&mutex);
    }
}

int main() {

    #pragma omp parallel for
    for (int i = 0; i < 5; i++) {
        sleep(rand() % 6 + 1);
        use(i % MAX_RESOURCES + 1);
    }

    #pragma omp barrier

    printf("Available resources: %d\n", available_resources);

    return 0;
}
