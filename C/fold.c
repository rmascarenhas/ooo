/* This program "folds" its input to fit approximately n columns. The output 
   might be a little shorter or longer; the program will always choose the best
   place to insert a line break so that the line length is as close as possible to n.

Usage:
    $ fold 80 file.txt > fold.txt
    All the lines from file.txt will be folded to approximately 80 columns.

    This program is useful for files with really long lines.

Obs: this program will NOT take into account your tab stop. A tab character found in
a file is counted as a single character. Please have this in mind when using this program.

*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_WORD_SIZE 100

void print_help_message(void);
int readword(char word[]);

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        print_help_message();
        return 1;
    }

    int column_length = atoi(argv[1]);
    int current_line_length = 0;
    int word_length;
    int last_blank_position;
    char current_word[MAX_WORD_SIZE];

    while ((word_length = readword(current_word)) > 0) {
        last_blank_position = current_line_length;
        current_line_length += word_length;

        //printf("Word is %s, length %d\n", current_word, word_length);

        if (current_line_length < column_length) {
            printf("%s", current_word);
        } else  {
            if (current_line_length - column_length < column_length - last_blank_position) {
                printf("%s\n", current_word);
            } else {
                printf("\n%s", current_word);
            }
            current_line_length = 0;
        }
    }
}

void print_help_message() {
    printf("Usage:\n \
            fold <column_length> <file> \n \
The output will be written in the standard output, so it is recommended \
to redirect the output to a file.\n");
}

int readword(char word[]) {
    int current_char;
    int word_length = 0;

    while ((current_char = getchar()) != ' ' && current_char != '\t' && current_char != '\n' && current_char != EOF) {
        word[word_length++] = current_char;
    }

    if (current_char == EOF) {
        word[word_length] = '\0';
        return word_length;
    } else {
        word[word_length++] = current_char;
    }

    word[word_length] = '\0';

    return word_length;
}
