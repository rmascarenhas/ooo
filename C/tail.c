/* Stripped-down version of the `tail` Unix command */

#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_SIZE       1000
#define TAIL_DEFAULT_LENGTH 10
#define MAX_ALLOWED_TAIL    10000

#define ABORT(str) \
    printf("%s\n", str);   \
    return 1;

enum Bool { FALSE, TRUE };

static const char usage_mesg[] = "Usage: _tail [-n] file";

int readline(char *buf, FILE *fd);
int tail(char *filename, int n_lines, char **buf);

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        ABORT(usage_mesg);
    }

    char *filename;
    int length = TAIL_DEFAULT_LENGTH;
    char *buf[MAX_ALLOWED_TAIL];
    enum Bool filename_given = FALSE;
    int i;

    while (--argc > 0) {
        if ((*++argv)[0] == '-') {
            length = -1 * atoi(*argv);
        } else {
            filename = *argv;
            filename_given = TRUE;
        }
    }

    if (!filename_given || length > MAX_ALLOWED_TAIL) {
        ABORT(usage_mesg);
    }

    int buffered_lines = tail(filename, length, buf);

    for (i = 0; i < buffered_lines; ++i) {
        printf("%s", buf[i]);
    }

    return 0;
}

int readline(char *buf, FILE *fd) {
    char c;
    int i = 0;

    while ((c = fgetc(fd)) != EOF && c != '\n' && i < MAX_LINE_SIZE) {
        buf[i++] = c;
    }

    if (c == '\n') {
        buf[i++] = c;
    }

    buf[i] = '\0';

    return i;
}

int tail(char *filename, int n_lines, char **buf) {
    FILE *fd = fopen(filename, "r");
    if (fd == NULL) {
        ABORT("FATAL ERROR: Could not open file.");
    }

    int count = 0;
    char *lines[MAX_ALLOWED_TAIL];

    do {
        lines[count] = (char *) malloc(MAX_LINE_SIZE);
    } while ((readline(lines[count], fd)) > 0 && ++count < MAX_ALLOWED_TAIL);

    int start = (count < n_lines) ? 0 : count - n_lines;

    int i, j = 0;
    for (i = start; i < count; ++i) {
        buf[j++] = lines[i];
    }

    return j;
}
