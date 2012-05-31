/* Clone of the tee(1) command. Only supported option is -a */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

static const size_t READ_SIZE = 1024; /* how many bytes each read(2) will read from stdin */
static const int    MAX_FILES = 256;  /* write output to at most MAX_FILES files          */

static void pexit(const char *s); /* prints error message and exits with failure status */
static void usage();              /* prints usage message and exits                     */

int
main(int argc, char *argv[]) {
    char buf[READ_SIZE];    /* buffer for reading from STDIN                */
    int fds[MAX_FILES];     /* file descriptor for each file we are writing */ 
    int n_files = 0;        /* how many files are we writing to             */
    int append = 0;         /* does not append by default                   */
    int i;                  /* index helper for loops                       */
    char option;            /* used for the return of getopt(3) and read(2) */
    ssize_t r;              /* number of bytes read from read(2)            */
    ssize_t w;              /* number of bytes written by write(2)          */
    int c;                  /* return value from close(2)                   */
    int fd_;                /* helper variable to hold file descriptors     */
    int open_flags;         /* flags used to open each of the files         */
    mode_t mode;            /* permission of the create files               */

    open_flags = O_CREAT | O_WRONLY;
    mode       = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; /* rw-rw-rw- */

    while ((option = getopt(argc, argv, "a")) != -1) {
        switch(option) {
            case 'a':
                append = 1;
                break;

            case '?':
                if (isprint(optopt)) {
                    fprintf(stderr, "Unknown option: `%c'.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option character: `\\x%x'.\n", optopt);
                }

                return 1;


            default:
                abort();
        }
    }

    if (append) {
        open_flags |= O_APPEND;
    } else {
        open_flags |= O_TRUNC;
    }

    /* Opens each file specified */
    for (i = optind; i < argc; ++i) {
        fd_ = open(argv[i], open_flags, mode);
        
        if (fd_ == -1) {
            pexit("open");
        }

        fds[n_files++] = fd_;
    }

    if (n_files == 0) {
        usage(argv[0]);
    }

    /* Read from STDIN and write to each file */
    while ((r = read(STDIN_FILENO, buf, READ_SIZE)) > 0) {
        for (i = 0; i < n_files; ++i) {
            /* write to stdout */
            w = write(STDOUT_FILENO, buf, (size_t) r);
            
            if (w == -1) {
                pexit("write");
            }

            /* write to files */
            w = write(fds[i], buf, (size_t) r);

            if (w == -1) {
                pexit("write");
            }
        }
    }

    if (r == -1) {
        pexit("read");
    }


    /* Close opened files */
    for (i = 0; i < n_files; ++i) {
        c = close(fds[i]);

        if (c == -1) {
            pexit("close");
        }
    }


    return EXIT_SUCCESS;
}


void pexit(const char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}


void usage(char *basename) {
    fprintf(stderr, "Usage: %s [-a] file\n", basename);
    exit(EXIT_FAILURE);
}
