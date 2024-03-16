#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/time.h>

int main () {
    int n=0;
    while (n<1)
    {
        printf("Enter a number greater than 0: ");
        scanf("%d", &n);
    }
    
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }
    else if (pid == 0) {
        // child process
        printf("%d ", n);
        while (n != 1) {
            if (n % 2 == 0) {
                n = n / 2;
            } else {
                n = 3 * n + 1;
            }
            printf("%d ", n);
        }
        printf("\n");
    } else {
        // parent process
        wait(NULL);
    }
    
}