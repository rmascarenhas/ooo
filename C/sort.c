/* Stripped-down version of the `sort` Unix command */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES_SORT  500000 /* refuse to sort more than that */
#define MAX_LINE_SIZE   10000

#define ABORT(str) \
    printf("%s\n", str); \
    return 1;

int readlines(char *lines[], int nlines);
int readline(char *line);
void writelines(char *lines[], int nlines);

void sort(void *lineptr[], int left, int right, int (*compare)(const void *, const void *));
int numcmp(char *, char *);
int numcmp_r(char *, char *); /* reverse order */

static const char usage_mesg[] = "Usage: _sort [-r -n] [file].";

enum Bool { NO, YES };

int main(int argc, char *argv[]) {
    char *lines[MAX_LINES_SORT];
    enum Bool numerical = NO, reverse = NO;
    int nlines, c;
    int (*compare_func)(const void *, const void *);

    while (--argc > 0 && (*++argv)[0] == '-') {
        while (c = *++argv[0]) {
            switch(c) {
                case 'n':
                    numerical = YES;
                    break;

                case 'r':
                    reverse = YES;
                    break;

                default:
                    ABORT(usage_mesg);
            }
        }
    }

    if (numerical) {
        if (reverse) {
            compare_func = numcmp_r;
        } else {
            compare_func = numcmp;
        }
    } else {
        compare_func = strcmp;
    }

    if ((nlines = readlines(lines, MAX_LINES_SORT)) >= 0) {
        sort((void **) lines, 0, nlines-1, compare_func);
        writelines(lines, nlines);
    } else {
        ABORT("_sort: input too big.");
    }

    return 0;
}

int readline(char *buf) {
    int c;
    int i = 0;

    while ((c = getchar()) != EOF && c != '\n') {
        buf[i++] = c;
    }

    if (c == '\n') {
        buf[i++] = c;
    }

    buf[i] = '\0';

    return i;
}

int readlines(char *buf[], int nlines) {
    int i = 0;

    do {
        buf[i] = (char *) malloc(MAX_LINE_SIZE);
    } while (readline(buf[i]) > 0 && ++i < nlines);

    return i;
}

void writelines(char *buf[], int nlines) {
    int i;
    for (i = 0; i < nlines; ++i) {
        printf("%s", buf[i]);
    }
}

void sort(void *v[], int left, int right, int (*compare)(const void *, const void *)) {
    int i, last;
    void swap(void *v[], int, int);

    if (left >= right) {
        return;
    }

    swap(v, left, (left + right)/2);
    last = left;

    for (i = left + 1; i <= right; ++i) {
        if ((*compare)(v[i], v[left]) < 0) {
            swap(v, ++last, i);
        }
    }

    swap(v, left, last);
    sort(v, left, last-1, compare);
    sort(v, last+1, right, compare);
}

int numcmp(char *s1, char *s2) {
    double v1, v2;

    v1 = atof(s1);
    v2 = atof(s2);

    if (v1 < v2) {
        return -1;
    } else if (v1 > v2) {
        return 1;
    } else {
        return 0;
    }
}

int numcmp_r(char *s1, char *s2) {
    return -1 * numcmp(s1, s2);
}

void swap(void *v[], int i, int j) {
    void *tmp;

    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}
