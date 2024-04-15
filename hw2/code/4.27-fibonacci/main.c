#include <stdio.h>
#include <pthread.h>
#include <sys/shm.h>
#include <stdlib.h>

#define MAX 50

typedef struct {
    int number;
    unsigned long *fib;
} args;

void* child (args *a) {
    for (int i = 2; i < a->number; i++) {
        a->fib[i] = a->fib[i - 1] + a->fib[i - 2];
    }
    pthread_exit(0);
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }
    else if (atoi(argv[1]) > MAX) {
        fprintf(stderr, "Number must be less than %d\n", MAX);
        return 1;
    }

    int shmid = shmget(567, sizeof(unsigned long) * MAX, IPC_CREAT | 0666);
    unsigned long *fib = (unsigned long*)shmat(shmid, NULL, 0);

    fib[0] = 0;
    fib[1] = 1;

    args a = { .number = atoi(argv[1]), .fib = fib };

    pthread_t tid;
    pthread_create(&tid, NULL, (void*)child, &a);

    pthread_join(tid, NULL);

    for (int i = 0; i < a.number; i++) {
        printf("%lu, ", a.fib[i]);
    }
}