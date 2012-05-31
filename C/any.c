/* This program implements the function `any` which is very
   similar to the function strpbrk from <string.h>

   any(s1, s2)

   returns the location of the first character in s1 from string s2,
   or -1 if s1 contains no characters from s2. */

#include <stdio.h>

void print_help_message(void);
int any(char s1[], char s2[]);

int main(int argc, char* argv[]) {
    if (argc <= 2) {
        print_help_message();
        return 1;
    }

    printf("%d\n", any(argv[1], argv[2]));

    return 0;
}

void print_help_message() {
    printf("Usage:\n\t"
            "$ any s1 s2\n"
            "Returns the location of the first character in s1 from s2, "
            "or -1 if s1 contains no characters from s2.\n");
}

int any(char s1[], char s2[]) {
    int p1, p2;

    p1 = p2 = 0;

    while (s1[p1] != '\0') {
        while (s2[p2] != '\0' && s1[p1] != s2[p2])
            ++p2;

        if (s2[p2] != '\0') {
            return p1;
        }

        ++p1;
        p2 = 0;
    }

    return -1;
}
