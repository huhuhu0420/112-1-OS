#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/time.h>

int create_shared_memory(size_t size) {
    // shared memory
    int shmid;
    key_t key;
    char *shm, *s;

    key = 567;
    if ((shmid = shmget(key, size, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        return 0;
    }
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        return 0;
    }
    return shmid;
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        return 1;
    }

    int shmid = create_shared_memory(sizeof(struct timeval));
    struct timeval *start = (struct timeval*)shmat(shmid, NULL, 0);

    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }
    else if (pid == 0) {
        // child process
        gettimeofday(start, NULL);
        execlp(argv[1], argv[1], NULL);
    } else {
        // parent process
        wait(NULL);
        struct timeval *end;
        gettimeofday(end, NULL);

        double elapsed = (end->tv_sec - start->tv_sec) * 1000.0;
        elapsed += (end->tv_usec - start->tv_usec) / 1000.0;
        printf("Elapsed time: %f ms\n", elapsed);

        shmdt(start);
        shmctl(shmid, IPC_RMID, NULL);
    }

}