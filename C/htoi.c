/* This program implements the function `htoi`, which
   converts a string of hexadecimal digits into its equivalent
   integer value. */

#include <stdio.h>
#include <ctype.h>

#define MAX_HEXA_LENGTH 255

enum hexa_chars { A = 10, B, C, D, E, F };

void print_help_message(void);
int htoi(char hexa[]);

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        print_help_message();
        return 1;
    }

    printf("%d\n", htoi(argv[1]));

    return 0;
}

void print_help_message() {
    printf("Usage:\n\t"
            "$ htoi 0x6a7f\n"
            "The \"0x\" prefix is not enforced.\n");
}

int htoi(char hexa[]) {
    char c;
    int n = 0;
    int i, start = 0;

    if (hexa[0] == '0' && tolower(hexa[1]) == 'x') {
        start = 2;
    }

    for (i = start; i < MAX_HEXA_LENGTH && hexa[i] != '\0'; ++i) {
        c = hexa[i];
        if (isdigit(c)) {
            n = 16*n + (c - '0');
        } else {
            if (tolower(c) == 'a') {
                n = 16*n + A;
            } else if (tolower(c) == 'b') {
                n = 16*n + B;
            } else if (tolower(c) == 'c') {
                n = 16*n + C;
            } else if (tolower(c) == 'd') {
                n = 16*n + D;
            } else if (tolower(c) == 'e') {
                n = 16*n + E;
            } else if (tolower(c) == 'f') {
                n = 16*n + F;
            } else {
                return 0;
            }
        }
    }

    return n;
}
