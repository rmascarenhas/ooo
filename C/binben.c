/* This program is to be used to benchmark two different versions
   of the binary search, as proposed in "The C Programming Language",
   exercise 3-1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SET_SIZE 500000
#define EXECUTIONS 100000

int int_compare(const void *a, const void *b); /* Integer comparison function to be used with qsort */
int binbook(int x, long v[], long n); /* The version provided by the book */
int binmod(int x, long v[], long n); /* The version modified as proposed in the exercise */

int main() {
    long sorted[SET_SIZE];
    long i;
    clock_t start, end, time_book, time_mod;

    for (i = 0; i < SET_SIZE; ++i) {
        sorted[i] = i;
    }


    qsort(sorted, SET_SIZE, sizeof(int), int_compare);

    start = clock();
    for (i = 0; i < EXECUTIONS; ++i) {
        binbook(-1, sorted, SET_SIZE);
    }
    end = clock();
    time_book = end - start;

    start = clock();
    for (i = 0; i < EXECUTIONS; ++i) {
        binmod(-1, sorted, SET_SIZE);
    }
    end = clock();
    time_mod = end - start;

    printf("Set size: %ld.\n", SET_SIZE);
    printf("Algorith execution: %ld.\n\n", EXECUTIONS);
    printf("Results:\n\n\tBook version: %lu clock ticks.\n\tModified version: %lu clock ticks\n\n", time_book, time_mod);
    printf("Modified version was %.2lf times faster.\n", (double)time_book/time_mod);

    return 0;
}

int int_compare(const void *a, const void *b) {
    return (int) (*(long *)a - *(long *)b);
}

int binbook(int x, long v[], long n) {
    int low, high, mid;

    low = 0;
    high = n - 1;
    
    while (low <= high) {
        mid = (low + high)/2;
        if (x < v[mid]) {
            high = mid - 1;
        } else if (x > v[mid]) {
            low = mid + 1;
        } else {
            return mid;
        }
    }

    return -1;
}

int binmod(int x, long v[], long n) {
    int low, high, mid;

    low = 0;
    high = n - 1;
    mid = (low + high)/2;

    while (x != v[mid] && low <= high) {
        if (x < v[mid]) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }

        mid = (low + high)/2;
    }

    if (x == v[mid]) {
        return mid;
    } 

    return -1;
}
