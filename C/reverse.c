/* This program reverses its input, one line at a time */

#include <stdio.h>

#define MAXLEN 100

void reverse(char reversed[], char str[]);
int readline(char read[]);

int main() {
    char current_line[MAXLEN];
    char reversed[MAXLEN];

    while ((readline(current_line)) > 0) {
        reverse(reversed, current_line);
        printf("%s\n", reversed);
    }

    return 0;
}

void reverse(char reversed[], char string[]) {
    int i;
    int j;
    int aux;

    /* Copy our parameter */
    i = 0;
    while (i < MAXLEN && ((reversed[i] = string[i]) != '\0')) {
        ++i;
    }

    /* Revert */
    for (j = 0; j < i/2; ++j) {
        aux = reversed[i - j - 1];
        reversed[i - j - 1] = reversed[j];
        reversed[j] = aux;
    }
}

int readline(char read[]) {
    int c;
    int n_read = 0;

    for (c = getchar(); c != '\n' && c != EOF; c = getchar()) {
        read[n_read++] = c;
    }

    if (c == '\n') {
        read[n_read++] = c;
    }

    read[n_read] = '\0';

    return n_read;
}
