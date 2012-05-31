/* Prints its input one word per line */

#include <stdio.h>

#define BLANK 0
#define CHAR  1

main() {
    int current_char;
    int state = BLANK;

    while ((current_char = getchar()) != EOF) {
        if (current_char == ' ' || current_char == '\t' || current_char == '\n') {
            if (state == CHAR) {
                putchar('\n');
                state = BLANK;
            }
        } else {
            putchar(current_char);
            state = CHAR;
        }
    }
}
