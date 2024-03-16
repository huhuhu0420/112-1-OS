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
    pid_t pid;
    int fd[2];
    char buffer[100];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    pipe(fd);
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }
    else if (pid == 0) {
        // child process
        close(fd[0]);
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            fprintf(stderr, "Error opening file\n");
            return 1;
        }
        while (fgets(buffer, 100, file) != NULL) {
            write(fd[1], buffer, 100);
        }
        close(fd[1]);
        fclose(file);
    } else {
        // parent process
        int status;
        wait(&status);
        if (status != 0) {
            fprintf(stderr, "Error copying file\n");
            return 1;
        }
        close(fd[1]);
        FILE *file = fopen(argv[2], "w");
        if (file == NULL) {
            fprintf(stderr, "Error opening file\n");
            return 1;
        }
        while (read(fd[0], buffer, 100) > 0) {
            fputs(buffer, file);
        }
        close(fd[0]);
    }
}