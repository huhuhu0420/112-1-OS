#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
    unsigned int address;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <address>\n", argv[0]);
        return 1;
    }

    address = atoi(argv[1]);

    int page = address >> 12;
    int offset = address & 0xFFF;

    printf("The address %d contains:\n", address);
    printf("Page number: %d\n", page);
    printf("Offset: %d\n", offset);

    return 0;
}