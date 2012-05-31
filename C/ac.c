/* Extremely simple calculator. (Simpler version of `bc').
 * Supports only expression in Reverse Polish Notation.
 *
 * Usage:
 *
 * >> 3 3 +
 * => 6
 *
 * The result of the previous calculation is availabe using '_'
 * >> _ 2 *
 * => 12
 *
 * Type 'quit' to leave.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 100
#define MAXVARS 100
#define NUMBER '0'
#define COMMAND 'c'
#define UNDERSCORE '_'
#define VAR 'v'
#define MAXVAL 100
#define BUFSIZE 100

#define ABORT(mesg) printf("%s\n", mesg); exit(1);

int getop(char[]);
void push(double);
double pop(void);
int getch(void);
void ungetch(int);

char mesg[MAXOP];
double vars[MAXVARS];
int varsp = 0;

double last;

int main() {
    int type;
    double op2;
    char s[MAXOP];
    
    printf("> ");

    while ((type = getop(s)) != EOF && strcmp(s, "quit") != 0) {
        switch(type) {
            case NUMBER:
                push(atof(s));
                break;

            case COMMAND:
                if (strcmp(s, "store") == 0) {
                    push(vars[varsp++] = pop());
                    printf("Variable name is %c.\n", varsp - 1 + 'a');
                } else { 
                    sprintf(mesg, "ac: unknown command %s.", s);
                    ABORT(mesg);
                }
                break;

            case UNDERSCORE:
                push(last);
                break;

            case VAR:
                push(vars[s[0] - 'a']);
                break;

            case '+':
                push(pop() + pop());
                break;

            case '*':
                push(pop() * pop());
                break;

            case '-':
                push(-1*(pop() - pop()));
                break;

            case '/':
                if ((op2 = pop()) == 0.0) {
                    ABORT("ac: Zero division error.");
                }

                push(pop() / op2);
                break;

            case '\n':
                printf("=> %.8g\n\n> ", last = pop());
                break;

            default:
                sprintf(mesg, "ac: unknown command %s.", s);
                ABORT(mesg);
                break;
        }
    }

    return 0;
}

int sp = 0;
double stack[MAXVAL];

void push(double e) {
    if (sp < MAXVAL) {
        stack[sp++] = e;
    } else {
        sprintf(mesg, "ac: Stack full, cannot push %g.", e);
        ABORT(mesg);
    }
}

double pop() {
    if (sp > 0) {
        return stack[--sp];
    } else {
        ABORT("ac: empty stack.");
        return 0;
    }
}

int getop(char s[]) {
    int i, c;
    static int bufp, buf[BUFSIZE];

    while ((s[0] = c = getch()) == ' ' || c == '\t')
        ;

    s[1] = '\0';

    i = 0;
    
    if (!isdigit(c) && c != '.') {
        if (c == '+' || c == '-' || c == '*' || c == '/' || c ==  '\n') {
            return c;
        } else if (c >= 'a' && c <= 'a' + varsp) {
            return VAR;
        } else if (c == '_') {
            return UNDERSCORE;
        }

        while (isalpha(s[++i] = c = getch()))
            ;

        s[i] = '\0';

        if (c != EOF) {
            ungetch(c);
        }

        return COMMAND;
    } else {
        if (isdigit(c)) {
            while (isdigit(s[++i] = c = getch()))
                ;
        }

        if (c == '.') {
            while (isdigit(s[++i] = c = getch()))
                ;
        }

        s[i] = '\0';

        if (c != EOF) {
            if (bufp >= BUFSIZE) {
                ABORT("ac: ungetch: too many characters.");
            } else {
                buf[bufp++] = c;
            }
        }

        return NUMBER;
    }
}

char buf[BUFSIZE];
int bufp = 0;

int getch() {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp >= BUFSIZE) {
        ABORT("ac: ungetch: too many characters.");
    } else {
        buf[bufp++] = c;
    }
}
