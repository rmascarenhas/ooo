#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


char *makePwd(int lenght, int options[])
{
    srand(time(NULL));
    struct timespec *req = (struct timespec *) malloc(sizeof(struct timespec));
    struct timespec *rem = NULL;
    req->tv_sec = 0;
    req->tv_nsec = 100000000; 
    
    int start = 0, finish = 0, i;
    char *pwd = (char *) malloc(lenght*sizeof(char) + 1);
    
    if (options[0])
    {
        start = 48;
        finish = 57;
    }
    else if (options[2])
    {
        start = 97;
        finish = 122;
    }
    else if (options[3])
    {
        start = 65;
        finish = 90;
    }
    printf("Generating random password... this can take some time...\n[");
    for (i = 0; i < lenght; i++)
        printf(" ");
    printf("]\r[");
    for (i = 0; i < lenght; i++)
    {
        nanosleep(req,rem);
        if (start == finish && !options[1])
        {
            int type = rand() % 3;
            switch(type)
            {
                case 0:
                    pwd[i] = rand() % 10 + 48;
                    break;

                case 1:
                    pwd[i] = rand() % 26 + 65;
                    break;

                case 2:
                    pwd[i] = rand() % 26 + 97;
                    break;
            }
        }
        else if (options[1])
        {
            int type = rand() % 2;
            switch(type)
            {
                case 0:
                    pwd[i] = rand() % 26 + 65;
                    break;

                case 1:
                    pwd[i] = rand() % 26 + 97;
                    break;
            }
        }
        else
            pwd[i] = rand() % (finish - start + 1) + start;
        printf("=");
        fflush(NULL);
    }
    pwd[lenght] = '\0';
    return pwd;
}


int main (int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Usage:\n-l\n\tspecifies the lenght of the password to generate\n" \
                "-n, --only-numbers\n\tGenerates a password using only numbers\n" \
                "-c, --only-char\n\tGenerates a password using only characters\n" \
                "-w, --lower-case\n\tGenerates a password using only lower-case characters\n" \
                "-u, --upper-case\n\tGenerates a password using only upper-case characters\n" \
                "-v, --version\n\tPrints the program version and finishes\n"  
              ); 
        return EXIT_SUCCESS;
    }
    
    if (!strcmp(argv[1],"-v") || !strcmp(argv[1],"--version"))
    {
        printf("pwd-maker v0.1\n");
        return EXIT_SUCCESS;
    }
    
    
    if (argc > 2)
    {
        if (!strcmp(argv[1],"-l") && atoi(argv[2]))
        {
            int options[4] = { 0,0,0,0 };
        
            if (argc == 4)
            {
                if (!strcmp(argv[3],"-n") || !strcmp(argv[3],"--only-numbers"))
                    options[0] = 1;
                else if (!strcmp(argv[3],"-c") || !strcmp(argv[3],"--only-char"))
                    options[1] = 1;
                else if (!strcmp(argv[3],"-w") || !strcmp(argv[3],"--lower-case"))
                    options[2] = 1;
                else if (!strcmp(argv[3],"-u") || !strcmp(argv[3],"--upper-case"))
                    options[3] = 1;
                else
                {
                    printf("Unknown option: %s\n",argv[3]);
                    return EXIT_FAILURE;
                }
            }
            else if (argc > 4)
            {
                printf("Unknown option %s for %s\n",argv[4],argv[3]);
                return EXIT_FAILURE;
            }

           printf("\nPassword: %s\n", makePwd(atoi(argv[2]),options));
           return EXIT_SUCCESS;
        }
        else if (!strcmp(argv[1],"-l") && !atoi(argv[2]))
        {
            printf("Unknown lenght: %s\n",argv[2]);
            return EXIT_FAILURE;
        }
    }
    else
        printf("Wrong set of parameters passed.\n");
    return EXIT_FAILURE;        
} 
