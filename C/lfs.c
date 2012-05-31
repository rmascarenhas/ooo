/* Example program using LFS (Large File Summit) specification */

/* Uncomment the following line if the program is not compiled with
   -D_FILE_OFFSET_BITS=64 */

// #define _FILE_OFFSET_BITS 64


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

static void usage(char *basename);
static void pexit(const char *mesg);

int
main(int argc, char *argv[]) {
    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        usage(argv[0]);
    }

    int fd = open(argv[1], O_WRONLY);
    if (fd == -1) {
        pexit("open");
    }

    lseek(fd, 0, SEEK_END);
    if ((write(fd, argv[2], strlen(argv[2]))) == -1) {
        pexit("write");
    }


    if ((close(fd)) == -1) {
        pexit("close");
    }

    return EXIT_SUCCESS;
}

void usage(char *basename) {
    fprintf(stderr, "Usage: %s big-file data\n", basename);
    exit(EXIT_FAILURE);
}

void pexit(const char *mesg) {
    perror(mesg);
    exit(EXIT_FAILURE);
}
