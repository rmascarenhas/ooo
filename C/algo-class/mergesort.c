#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void
generate_random_array(int *buf, int n) {
    srand(time(NULL));
    int i;
    for (i = 0; i < n; ++i) {
        buf[i] = rand() % n;
    }
}

void *
mergesort(void *v, int n, int size, int (*compare)(const void *, const void *)) {
    if (n < 2) {
        return;
    }

    int half = n/2;
    int comp_result;
    char *base = (char *) v;
    char *left_base, *right_base;

    left_base  = (char *) mergesort((void *) base, half, size,  compare);
    right_base = (char *) mergesort((void *) &base[half * size], n - half, size,  compare);

    char *sorted = (char *) malloc(n * size);

    int i, p = 0, q = 0;
    for (i = 0; i < n; ++i) {
        if (p == half) {
            // sorted[i] = second_half[q++];
            memcpy(sorted + i*size, right_base + q*size, size);
            ++q;
        } else if (q == n - half) {
            // sorted[i] = first_half[p++];
            memcpy(sorted + i*size, left_base + p*size, size);
            ++p;
        } else {
            comp_result = (*compare)((const void *) &left_base[p*size], (const void *) &right_base[q*size]);
            if (comp_result <= 0) {
                // sorted[i] = first_half[p++];
                memcpy(sorted + i*size, left_base + p*size, size);
                ++p;
            } else {
                // sorted[i] = second_half[q++];
                memcpy(sorted + i*size, right_base + q*size, size);
                ++q;
            }
        }
    }

    return (void *) sorted;
}

int int_cmp(const void *a, const void *b) {
    int x = *((int *) a);
    int y = *((int *) b);

    return x - y;
}

static const int EXECUTIONS = 10;         /* Number of executions performed to extract the average */
static const int DATA_SIZE  = 1000000;    /* Size of the random array generated                    */

int
main(int argc, char *argv[]) {
    printf("Sorting algorithms\n");
    printf("==================\n");

    printf("\nTest: %ld executions with %ld random elements.\n", 
            (long) EXECUTIONS, (long) DATA_SIZE);

    printf("\nGenerating random arrays and sorting... This can take some time.\n");

    int *data = (int *) malloc(DATA_SIZE*sizeof(int));
    int (*cmp_func)(const void *, const void *) = int_cmp;
    time_t start, end;
    unsigned long total = 0;
    int i;
    for (i = 0; i < EXECUTIONS; ++i) {
        generate_random_array(data, DATA_SIZE);
        time(&start);
        mergesort((void *) data, DATA_SIZE, sizeof(int), cmp_func);
        time(&end);
        total += difftime(end, start);
    }

    printf("\n\nResults\n");
    printf("-------\n\n");


    printf("\tAlgorithm\t\t|\t\tAverage (secs)\n");
    printf("\t------------------------+-----------------------------\n");
    printf("\tmergesort\t\t|\t\t%lf\n", (double) total/EXECUTIONS);

    return EXIT_SUCCESS;
}
