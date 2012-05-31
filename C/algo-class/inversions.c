/* Algorithm to calculate the number of inversions in an array.

Definition: given the array A = (1, 2, 3, ..., n) in any order, the pair
(i, j) is an inversion if and only if i < j and A[i] > A[j]

Uses: calculate similarity between two different arrays. Users could rate a set
of movies, for example, and then the number of inversions would be a measure of how
similar their likes are. The smaller the most similar.

*/

#include <stdio.h>
#include <stdlib.h>

int *
inversions(int *v, int n, int *result) {
    if (n < 2) {
        *result = 0;
        return v;
    }

    int half = n/2;

    int left_inversions;
    int right_inversions;

    int *left  = inversions(v, half, &left_inversions);
    int *right = inversions(&v[half], n - half, &right_inversions);

    /* Calculate split inversions */
    int *sorted = (int *) malloc(n*sizeof(int));
    int split_inversions = 0;
    int i, p = 0, q = 0;

    for (i = 0; i < n; ++i) {
        if (p == half) {
            sorted[i] = right[q++];

        } else if (q == n - half) {
            sorted[i] = left[p++];

        } else {
            if (left[p] <= right[q]) {
                sorted[i] = left[p++];
            } else {
                sorted[i] = right[q];
                split_inversions += half - p;
                ++q;
            }
        }
    }

    *result = left_inversions + right_inversions + split_inversions;

    return sorted;
}


int
main(void) {
    int v[] = { 1, 8, 5, 2, 4, 6 };
    int count;

    inversions(v, 6, &count);

    printf("Number of inversions: %d\n", count);

    return 0;
}
