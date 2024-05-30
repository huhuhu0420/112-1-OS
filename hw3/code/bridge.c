#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

enum DIR {
    NORTH = 0,
    SOUTH = 1
};

void enterBridge(enum DIR direction) {
    int time = rand() % 3;
    printf("Farmer from %s is entering the bridge\n", direction == NORTH ? "NORTH" : "SOUTH");
    sleep(time);
    printf("Farmer from %s has left the bridge\n", direction == NORTH ? "NORTH" : "SOUTH");
}

void* Farmer(enum DIR direction) {
    pthread_mutex_lock(&mutex);
    enterBridge(direction);
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

int main () {
    pthread_t tids[10];
    for (int i = 0; i < 10; i++) {
        enum DIR direction = rand() % 2 == 0 ? NORTH : SOUTH;
        pthread_create(&tids[i], NULL, (void*)Farmer, (void*)direction);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(tids[i], NULL);
    }
}