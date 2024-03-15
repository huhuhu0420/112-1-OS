#include <fcntl.h>      // for open
#include <unistd.h>     // for close, read, write
#include <stdio.h>      // for printf, scanf
#include <sys/stat.h>   // for checking file existence
#include <errno.h>      // for error numbers
#include <string.h>     // for strerror

int main() {
    char source[256], destination[256];
    int source_fd, dest_fd, nread;
    char buffer[1024];

    // Prompt user for source file name
    printf("Enter the source file name: ");
    scanf("%s", source);

    // Check if source file exists
    if (access(source, F_OK) == -1) {
        printf("Error: Source file does not exist.\n");
        return 1;
    }

    // Prompt user for destination file name
    printf("Enter the destination file name: ");
    scanf("%s", destination);

    // Open source file
    source_fd = open(source, O_RDONLY);
    if (source_fd == -1) {
        printf("Error opening source file: %s\n", strerror(errno));
        return 1;
    }

    // Open (or create) destination file
    dest_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (dest_fd == -1) {
        printf("Error opening destination file: %s\n", strerror(errno));
        close(source_fd);
        return 1;
    }

    // Copy contents from source to destination
    while ((nread = read(source_fd, buffer, sizeof(buffer))) > 0) {
        if (write(dest_fd, buffer, nread) != nread) {
            printf("Error writing to destination file: %s\n", strerror(errno));
            close(source_fd);
            close(dest_fd);
            return 1;
        }
    }

    if (nread == -1) {
        printf("Error reading from source file: %s\n", strerror(errno));
    }

    // Close files
    close(source_fd);
    close(dest_fd);

    printf("File copy successful.\n");

    return 0;
}

