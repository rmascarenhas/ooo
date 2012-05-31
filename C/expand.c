/* This program expands shorthand notations like a-z or 0-9.
Usage:
    $ expand a-z
    This will produce "abcdefghijklmnopqrstuvxwyz" as output.
*/

#include <stdio.h>
#include <ctype.h>

#define MAX_EXPANDED_SIZE 100
#define EXPANDER '-'

void print_help_message(void);
void expand(char expr[], char expanded[]);
int aredigits(char c1, char c2);

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        print_help_message();
        return 1;
    }

    char expanded[MAX_EXPANDED_SIZE];
    
    expand(argv[1], expanded);
    printf("%s\n", expanded);

    return 0;
}

void print_help_message() {
    printf("Usage:\n\t"
            "$ expand <expr>\n"
            "Expands the given expression to its full representation.\n");
}

void expand(char expr[], char expanded[]) {
    char start, end;
    int current_expr, current_expand;
    char c, before, after;

    current_expr = current_expand = 0;

    while (expr[current_expr] != '\0') {
        before = current_expr > 0 ? expr[current_expr -1] : ' ';
        after = expr[current_expr + 1];

        if (expr[current_expr] == EXPANDER) {
            /* Char before and after the expander must be alpha or digit */
            if (current_expr > 0 && aredigits(before, after)) {
                for (c = before + 1; c < after; ++c) {
                    expanded[current_expand++] = c;
                }
            } else {
                expanded[current_expand++] = EXPANDER;
            }
        } else {
            expanded[current_expand++] = expr[current_expr];
        }

        ++current_expr;
    }

    expanded[current_expand] = '\0';
}

int aredigits(char c1, char c2) {
    return ((isalpha(c1) || isdigit(c1)) && (isalpha(c2) || isdigit(c2)));
}
