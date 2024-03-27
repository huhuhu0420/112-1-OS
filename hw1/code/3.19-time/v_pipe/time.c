#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/time.h>

int main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        return 1;
    }

    int p1[2];
    if (pipe(p1) < 0) {
        fprintf(stderr, "Pipe failed\n");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }
    else if (pid == 0) {
        // child process
        struct timeval *start = (struct timeval*)malloc(sizeof(struct timeval));
        gettimeofday(start, NULL);
        write(p1[1], start, sizeof(struct timeval));
        close(p1[1]);
        execlp(argv[1], argv[1], NULL);
    } else {
        // parent process
        wait(NULL);
        struct timeval *end = (struct timeval*)malloc(sizeof(struct timeval));
        gettimeofday(end, NULL);

        struct timeval *start = (struct timeval*)malloc(sizeof(struct timeval));
        read(p1[0], start, sizeof(struct timeval));
        close(p1[0]);

        double elapsed = (end->tv_sec - start->tv_sec) * 1000.0;
        elapsed += (end->tv_usec - start->tv_usec) / 1000.0;
        printf("Elapsed time: %f ms\n", elapsed);

    }

}