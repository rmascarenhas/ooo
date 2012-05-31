/* 
 * 	fist - Fast Integer SorTer
 *
 * Author: Renato Mascarenhas Costa (renatomc at grad.icmc.usp.br)
 * 
 *  fist is an integer file sorter. It sorts files with any number of
 *  integers and can also generate a test file with a specified number
 *  of integers defined. The user can set whether the file may have 
 *  repeated numbers or not. When generating a file, the user also
 *  have to tell the offset and delta of the interval.
 *
 *  fist uses a bitmap-like representation of the numbers, mapping
 *  all the numbers of the file in a string where the position i
 *  counts the number of occurrences of the number i in the file.
 *  Thus, fist uses n bytes of memory, where n is the delta of the 
 *  interval of the numbers of the file.
 *
 *  fist works pretty good when the input file have integers of the
 *  same lenght. This causes fist to be faster and memory-eficient.
 *  Aimed for situations such as phone numbers files, identifiers, etc.
 *
 *  fist is licensed under the GPL v3.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <errno.h>

#define WRONG_TASK		-1
#define SORT_FILE		0
#define GENERATE_FILE	1

#define NO_REPETITIONS	0
#define REPETITIONS		1

long int offset, delta, number;
int task; /* tells what fist have to do: generate the random file or sort an existing one */
char *filename;
char repetitions;

char SLASHES[4] = { '|', '/', '-', '\\' };


void
print_help_message() { 
	printf( "fist - Fast Integer SorTer\n" \
		"Usage:\n" \
		"\t\tfist [options] [file] \n" \
		"OPTIONS\n" \
		"\t -g, --generate-file\n" \
		"\t\tGenerates a new file with random integers. If not specified values " \
		"for offset, delta and amount of numbers, default values of 0, 1000 and 100 will "\
		"be taken\n" \
		"\t-o, --offset (offset)\n" \
		"\t\t Specifies the offset of the generated file\n" \
		"\t-d, --delta (delta)\n" \
		"\t\tSpecifies delta value for the generated file\n" \
		"\t-n, --number (number)\n" \
		"\t\tSpecifies the number of integers to be generated\n" \
		"\t-r, --no-repetitions\n"\
		"\t\tIf this option is set, the generated file will contain only different integers. " \
		"Of course, you should not set number of generated integers bigger than delta.\n" \
		"\t-s, --sort (filename)\n" \
		"\t\t Sorts the file specified. Writes the result in the file fist_output.txt\n");

}

void
parse_command(int argc, char *argv[]) { 
	offset = 0;
	delta = 1000;
	number = 100;
	task = WRONG_TASK;
	repetitions = REPETITIONS;
	int i = 1;

	while (i < argc) {
		if (!strcmp(argv[i], "-g") || !strcmp(argv[i], "--generate-file")) {
			task = GENERATE_FILE;
		} else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--sort")) {
			task = SORT_FILE;
			if (++i >= argc) {
				printf("ERROR: no file specified. Please enter fist -h for help.\n");
				exit(1);
			} else {
				strncpy(filename, argv[i], sizeof(filename));
			}
		} else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--offset")) {
			if (++i >= argc) {
				printf("ERROR: no offset specified. Please enter fist -h for help.\n");
				exit(1);
			} else {
				if ((offset = strtol(argv[i], NULL, 10)) == 0) {
					printf("ERROR: invalid offset given: %s\n", argv[i]);
					exit(1);
				}
			}
		} else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--delta")) {
			if (++i >= argc) {
				printf("ERROR: no delta specified. Please enter fist -h for help.\n");
				exit(1);
			} else {
				if ((delta = strtol(argv[i], NULL, 10)) ==0) {
					printf("ERROR: invalid delta given: %s\n", argv[i]);
					exit(1);
				}
			}
		} else if (!strcmp(argv[i], "-n") || !strcmp(argv[i], "--number")) {
			if (++i >= argc) {
				printf("ERROR: no number specified. Please enter fist -h for help.\n");
				exit(1);
			} else {
				if ((number = strtol(argv[i], NULL, 10)) == 0) {
					printf("ERROR: invalid number given: %s\n", argv[i]);
					exit(1);
				}
			}
		} else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--no-repetitions")) {
			repetitions = NO_REPETITIONS;
		}

		i++;
	}

	if (task == WRONG_TASK) {
		print_help_message();
		exit(1);
	}

	if (repetitions == NO_REPETITIONS && number > delta) {
		printf("ERROR: cannot generate integers with no repetitions when number > delta. Type fist -h for help.\n");
		exit(1);
	}
}

void 
sanitize(char *str) { 
	char *nl;
	nl = strrchr(str, '\n');

	if (nl) {
		*nl = '\0';
	}
}


/* 4 Step algorithm: reads the input file twice. See loops comments
 * for further descriptions of each step */
void 
sort_file(char *filename) { 
	FILE *fd;

	if ((fd = fopen(filename, "r")) == NULL) {
		printf("ERROR: file not found: %s\n", filename);
		exit(1);
	} else {
		printf("Sorting file %s... ", filename);
		
		number = 0;
		long int max, row;
		offset = LONG_MAX;
		max = LONG_MIN;
		char *line = (char *)malloc(12*sizeof(char));
		char *end;

		/* Step 1: scans the file to get number of lines, offset and delta */
		while (!feof(fd)) {
			number++;
			fgets(line, 12, fd);
			sanitize(line); /* removes trailing newline */
			row = strtol(line, &end, 10);

			if (*end != '\0') {
				printf("\nERROR: not recognized integer: \"%s\" in line %ld\n", line, number);
				exit(1);
			} else if (errno == ERANGE) {
				printf("\nERROR: integer out of range: %s in line %ld\n", line, number);
				exit(1);
			}

			if (row < offset) {
				offset = row;
			}

			if (row > max) {
				max = row;
			}
			printf("\b%c", SLASHES[number % 4]);
			fflush(stdout);
		}

		delta = max - offset + 1;
		char *bitmap = (char *)malloc(delta*sizeof(char));

		/* Step 2: initializes bitmap */
		int i;
		for (i = 0; i < delta; i++) {
			bitmap[i] = 0;
			printf("\b%c", SLASHES[i % 4]);
			fflush(stdout);
		}

		fseek(fd, 0, SEEK_SET);
		number = 0;

		/* Step 3: populates bitmap */
		while (!feof(fd)) {
			number++;
			fgets(line, 12, fd);
			sanitize(line);
			row = strtol(line, &end, 10);
			bitmap[row - offset]++;

			printf("\b%c", SLASHES[number % 4]);	
			fflush(stdout);
		}

		FILE *fist = fopen("fist_output.txt", "w");
		int j;

		/* Step 4: writes the output to file fist_output.txt */
		for (i = 0; i < delta; i++) {
			for (j = 0; j < bitmap[i]; j++) {
				fprintf(fist, "%li\n", i + offset);
			}
			printf("\b%c", SLASHES[i % 4]);
			fflush(stdout);
		}
	}
	printf("\bDone!\n");
}


void
generate_random_file(long int offset, long int delta, long int number, char repetitions) { 
	FILE *fd = fopen("fist.txt", "w");
	int i, n;

	printf("Generating random file... ");

	srand(time(NULL));
	//struct timespec *req = (struct timespec*)malloc(sizeof(struct timespec));
	//struct timespec *rem = NULL;
	//req->tv_sec = 0;
	//req->tv_nsec = 1000000;

	if (repetitions == REPETITIONS) {
		for (i = 0; i < number; i++) {
			printf("\b%c", SLASHES[i % 4]);
			//nanosleep(req, rem);
			n = rand() % delta + offset;
			fprintf(fd, "%d\n", n);
			fflush(stdout);
		}	
	}
	printf("\bDone!\n");
}


int
main(int argc, char *argv[])
{
	if (argc <= 1) {  
		printf( "fist - Fast Integer SorTer\n" \
				"Renato Mascarenhas Costa (renatomc at grad.icmc.usp.br)\n" \
				"fist is an integer file sorter. You may sort your file with fist " \
				"or generate a randomly created file of integers.\n" \
				"For help, please use -h or --help\n");
	} else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {		
		print_help_message();
	} else {
		filename = (char *)malloc(256*sizeof(char));
		parse_command(argc, argv);	
		if (task == GENERATE_FILE) {
			generate_random_file(offset, delta, number, repetitions);
		} else {
			sort_file(filename);	
		}
	}
				
	return 0;
}
