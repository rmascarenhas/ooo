/* Sample program to demonstrate the use of readv(2) and writev(2) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>

static void usage(char *basename);
static void pexit(const char *mesg);
static void read_from_file(int fd); /* Performs scather input on file descriptor fd */
static void write_to_file(int fd);  /* Performs gather output from file descriptor fd */

/* We can read or write to the file. In any case, we will use three buffers:
        * a character string
        * an integer
        * a double
*/

/* Global buffers - this is only an example and avoid extra typing */
struct iovec iov[3];

#define BUF1_SIZ 32
char buf1[BUF1_SIZ] = "Some random string";
int buf2;
double buf3;

int
main(int argc, char *argv[]) {
    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        usage(argv[0]);
    }

    int open_flags = O_CREAT | O_APPEND | O_RDWR;
    mode_t mode    = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    int fd;
    if ((fd = open(argv[2], open_flags, mode)) == -1) {
        pexit("open");
    }

    if (strcmp(argv[1], "r") == 0) {
        read_from_file(fd);
        
    } else if (strcmp(argv[1], "w") == 0) {
        write_to_file(fd);

    } else {
        usage(argv[0]);
    }

    if (close(fd) == -1) {
        pexit("close");
    }

    return EXIT_SUCCESS;
}

void usage(char *basename) {
    fprintf(stderr, "Usage: %s [r,w] file\n", basename);
    exit(EXIT_FAILURE);
}

void pexit(const char *mesg) {
    perror(mesg);
    exit(EXIT_FAILURE);
}

void setup_iov(void) {
    iov[0].iov_base = buf1;
    iov[0].iov_len = BUF1_SIZ;

    iov[1].iov_base = &buf2;
    iov[1].iov_len  = sizeof(buf2);

    iov[2].iov_base = &buf3;
    iov[3].iov_len  = sizeof(buf3);
}

void read_from_file(int fd) {
    setup_iov();
    ssize_t bytes_read;

    if ((bytes_read = readv(fd, iov, 3)) == -1) {
        pexit("readv");
    }

    printf("Content read (%ld bytes):\n\t", (long) bytes_read);
    printf("Buf1: %s\n\t", buf1);
    printf("Buf2: %d\n\t", buf2);
    printf("Buf3: %lf\n", buf3);
}

void write_to_file(int fd) {
    buf2 = 4;
    buf3 = 3.14;

    setup_iov();

    ssize_t bytes_written;

    if ((bytes_written = writev(fd, iov, 3)) == -1) {
        pexit("writev");
    }

    printf("Content written (%ld bytes):\n\t", (long) bytes_written);
    printf("Buf1: %s\n\t", buf1);
    printf("Buf2: %d\n\t", buf2);
    printf("Buf3: %lf\n", buf3);
}
