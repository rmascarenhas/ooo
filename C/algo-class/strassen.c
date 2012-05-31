/* Multiplies two matrices using the Strassen's algorithm (1969)

Given two matrices X and Y such that:

X = ( A  B ),   Y = ( E  F )
    ( C  D )        ( G  H )

where A, B, ..., H are quadrants, then if we take

P1 = A(F - H)
P2 = (A + B)H
P3 = (C + D)E
P4 = D(G - E)
P5 = (A + D)(E + H)
P6 = (B - D)(G + H)
P7 = (A - C)(E + F)

then our product X * Y can be expressed as:

X * Y = ( P5 + P4 - P2 + P6         P1 + P2         )
        (   P3 + P4             P1 + P5 - P3 + P7   )

and we have only 7 recursive calls.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define M_ADD 0
#define M_SUB 1

typedef int elem_t;

typedef struct {
    elem_t **data;     /* pointer to our actual matrix */
    size_t length;   /* assuming square matrices only */
} matrix_t;

void
mprint(matrix_t m) {
    int i, j;
    for (i = 0; i < m.length; ++i) {
        printf("\t");
        for (j = 0; j < m.length; ++j) {
            printf("%ld\t", (long) m.data[i][j]);
        }
        printf("\n");
    }
}

matrix_t *
mallocate(size_t length) {
    matrix_t *m;
    m = (matrix_t *) malloc(sizeof(matrix_t));
    m->data = (elem_t **) malloc(length*sizeof(elem_t *));

    int i;
    for (i = 0; i < length; ++i) {
        m->data[i] = (elem_t *) malloc(length*sizeof(elem_t));
    }

    m->length = length;

    return m;
}

void
mfree(matrix_t *m) {
    int i;
    for (i = 0; i < m->length; ++i) {
        free(m->data[i]);
    }

    free(m);
}

void
mapply(matrix_t x, matrix_t y, matrix_t **result, int op) {
    *result = mallocate(x.length);
    if (x.length != y.length) {
        (*result)->data = NULL;
        return;
    }

    int i, j;
    for (i = 0; i < x.length; ++i) {
        for (j = 0; j < x.length; ++j) {
            if (op == M_ADD) {
                (*result)->data[i][j] = x.data[i][j] + y.data[i][j];
            } else if (op == M_SUB) {
                (*result)->data[i][j] = x.data[i][j] - y.data[i][j];
            }
        }
    }
}

void
msum(matrix_t *x, matrix_t *y, matrix_t **result) {
    mapply(*x, *y, result, M_ADD);
}

void
msub(matrix_t *x, matrix_t *y, matrix_t **result) {
    mapply(*x, *y, result, M_SUB);
}

/* The following set of functions return each of the four quarters of a matrix */
matrix_t *
first_quarter(matrix_t *m) {
    matrix_t *a;
    int half = m->length / 2;
    a = mallocate(half);

    int i, j;
    for (i = 0; i < half; ++i) {
        for (j = 0; j < half; ++j) {
            a->data[i][j] = m->data[i][j];
        }
    }

    return a;
}

matrix_t *
second_quarter(matrix_t *m) {
    matrix_t *b;
    int half = m->length / 2;
    b = mallocate(half);

    int i, j;
    for (i = 0; i < half; ++i) {
        for (j = 0; j < half; ++j) {
            b->data[i][j] = m->data[i][half+j];
        }
    }

    return b;
}

matrix_t *
third_quarter(matrix_t *m) {
    matrix_t *c;
    int half = m->length / 2;
    c = mallocate(half);

    int i, j;
    for (i = 0; i < half; ++i) {
        for (j = 0; j < half; ++j) {
            c->data[i][j] = m->data[half+i][j];
        }
    }

    return c;
}

matrix_t *
fourth_quarter(matrix_t *m) {
    matrix_t *d;
    int half = m->length / 2;
    d = mallocate(half);

    int i, j;
    for (i = 0; i < half; ++i) {
        for (j = 0; j < half; ++j) {
            d->data[i][j] = m->data[half+i][half+j];
        }
    }

    return d;
}

void
strassen(matrix_t *x, matrix_t *y, matrix_t **result) {
    *result = mallocate(x->length);

    if (x->length < 2) {
        (*result)->data[0][0] = x->data[0][0] * y->data[0][0];
        (*result)->length = 1;
        return;
    }

    int i, j;
    int half = x->length / 2;

    /* quarters of x */
    matrix_t *A, *B, *C, *D;

    /* quarters of y */
    matrix_t *E, *F, *G, *H;

    A = first_quarter(x);
    B = second_quarter(x);
    C = third_quarter(x);
    D = fourth_quarter(x);

    E = first_quarter(y);
    F = second_quarter(y);
    G = third_quarter(y);
    H = fourth_quarter(y);

    matrix_t *p1, *p2, *p3, *p4, *p5, *p6, *p7;

    /* used to store temporary matrixes from sums and subtractions */
    matrix_t *temp1, *temp2;

    /* P1 */
    msub(F, H, &temp1);
    strassen(A, temp1, &p1);

    /* P2 */
    msum(A, B, &temp1);
    strassen(temp1, H, &p2);

    /* P3 */
    msum(C, D, &temp1);
    strassen(temp1, E, &p3);

    /* P4 */
    msub(G, E, &temp1);
    strassen(D, temp1, &p4);

    /* P5 */
    msum(A, D, &temp1);
    msum(E, H, &temp2);
    strassen(temp1, temp2, &p5);

    /* P6 */
    msub(B, D, &temp1);
    msum(G, H, &temp2);
    strassen(temp1, temp2, &p6);

    /* P7 */
    msub(A, C, &temp1);
    msum(E, F, &temp2);
    strassen(temp1, temp2, &p7);

    /* temp1 = P5 + P4 - P2 + P6 */
    msum(p5, p4, &temp1);
    msub(temp1, p2, &temp2);
    msum(temp2, p6, &temp1);

    for (i = 0; i < half; ++i) {
        for (j = 0; j < half; ++j) {
            (*result)->data[i][j] = temp1->data[i][j];
        }
    }

    /* temp1 = P1 + P2 */
    msum(p1, p2, &temp1);

    for (i = 0; i < half; ++i) {
        for (j = 0; j < half; ++j) {
            (*result)->data[i][j+half] = temp1->data[i][j];
        }
    }

    /* temp1 = P3 + P4 */
    msum(p3, p4, &temp1);

    for (i = 0; i < half; ++i) {
        for (j = 0; j < half; ++j) {
            (*result)->data[i+half][j] = temp1->data[i][j];
        }
    }

    /* temp1 = P1 + P5 - P3 - P7 */
    msum(p1, p5, &temp1);
    msub(temp1, p3, &temp2);
    msub(temp2, p7, &temp1);

    for (i = 0; i < half; ++i) {
        for (j = 0; j < half; ++j) {
            (*result)->data[i+half][j+half] = temp1->data[i][j];
        }
    }
}

static const int M_SIZE = 1024;

int
main(int argc, char *argv[]) {
    srand(time(NULL));

    matrix_t *x, *y;
    x = mallocate(M_SIZE);
    y = mallocate(M_SIZE);

    int i, j;
    for (i = 0; i < M_SIZE; ++i) {
        for (j = 0; j < M_SIZE; ++j) {
            x->data[i][j] = rand() % 10;
            y->data[i][j] = rand() % 10;
        }
    }

    printf("The product of\n");
    mprint(*x);
    printf("\nand\n");
    mprint(*y);
    printf("\nis:\n");

    matrix_t *result;
    strassen(x, y, &result);

    mprint(*result);

    return 0;
}
