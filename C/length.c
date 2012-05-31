/* Draws a vertical histogram of the length of its input */

#include <stdio.h>

/* as we can use no more than arrays, assume there won't be any word longer 
   than MAX_LENGTH */
#define MAX_LENGTH 100

#define ZERO 0
#define NONZERO 1

main() {
    int words_by_length[MAX_LENGTH];
    int occurrences[MAX_LENGTH]; /* indicates whether or not a certain length occurred in the input */
    int current_length = 0;
    int largest = 0;
    int c;
    int i, j;

    for (i = 0; i < MAX_LENGTH; ++i) {
        words_by_length[i] = 0;
    }

    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (current_length > 0) {
                ++words_by_length[current_length];

                current_length = 0;
            }
        } else {
            ++current_length;
        }
    }

    for (i = 0; i < MAX_LENGTH; ++i) {
        if (words_by_length[i] == 0) {
            occurrences[i] = ZERO;
        } else {
            occurrences[i] = NONZERO;

            if (words_by_length[i] > largest) {
                largest = words_by_length[i];
            }
        }
    }

    for (i = largest; i > 0; --i) {
        for (j = 0; j < MAX_LENGTH; ++j) {
            if (occurrences[j]) {
                if (words_by_length[j] >= i) {
                    printf("|");
                }

                printf("\t");
            }
        }
        printf("\n");
    }

    for (j = 0; j < MAX_LENGTH; ++j) {
        if (occurrences[j]) {
            printf("%d\t", j);
        }
    }

    printf("\n");
}
