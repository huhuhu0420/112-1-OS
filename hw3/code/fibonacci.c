#include <stdio.h>
#include <pthread.h>
#include <sys/shm.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int number;
    long *fib;
} args;

void* child (args *a) {
    for (int i = 2; i < a->number; i++) {
        a->fib[i] = a->fib[i - 1] + a->fib[i - 2];
    }
    pthread_exit(0);
}

void print_fib (long *fib, int number) {
    int index = 0;
    while (index < number) {
        while (fib[index] == -1) {
        }
        printf("%ld ", fib[index]);
        index++;
    }    
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

    long fib[MAX];

    // init fib with -1
    for (int i = 0; i < MAX; i++) {
        fib[i] = -1;
    }

    fib[0] = 0;
    fib[1] = 1;

    args a = { .number = atoi(argv[1]), .fib = fib };

    pthread_t tid;
    pthread_create(&tid, NULL, (void*)child, &a);

    print_fib(a.fib, a.number);

    pthread_join(tid, NULL);
}