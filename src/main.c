/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>



#define MAX_LINE		80 /* 80 chars per line, per command */


/* Reads what the user enters on the command line, and parses the command and input parameters */
void setup(char inputBuffer[], char *args[],int *background);



int main(void)
{

    char inputBuffer[MAX_LINE];
	int background;
    int status;

    
    char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    int should_run = 1;
	
	int i, upper;
		
    while (should_run){   
        printf("COMMAND-> ");
        fflush(stdout);
        
        setup(inputBuffer, args, &background);   /* parse command and its parameters */
        
        /**
         * After reading user input, the steps are:
         * (1) fork a child process
         * (2) the child process will invoke execvp()
         * (3) if command does not include &, parent invokes wait()
         */

        int r = fork();
        fflush(stdout);

        if(r == 0) {
            printf("Running Child Process with PID: %d\n", (int) getpid());
            execvp(args[0],args);
            fprintf(stderr, "Child process could not do execvp.\n");
            exit(1);
        } else if (r > 0) {

            if(background != 1) { //& was not entered
                pid_t c = wait(&status); /* Wait for child to complete. */
            }

        } else {
            fprintf(stderr, "Fork failed.\n"); exit(1);
        }

        
    }
    
	return 0;
}






/* The setup function below will not return any value, but it will just: read
 in the next command line; separate it into distinct arguments (using blanks as
 delimiters), and set the args array entries to point to the beginning of what
 will become null-terminated, C-style strings. */

void setup(char inputBuffer[], char *args[],int *background)
{
    int length, /* # of characters in the command line */
    i,      /* loop index for accessing inputBuffer array */
    start,  /* index where beginning of next command parameter is */
    ct;     /* index of where to place the next parameter into args[] */
    
    ct = 0;
    
    *background = 0;
    
    /* read what the user enters on the command line */
    length = read(STDIN_FILENO,inputBuffer,MAX_LINE);
    
    /* 0 is the system predefined file descriptor for stdin (standard input),
     which is the user's screen in this case. inputBuffer by itself is the
     same as &inputBuffer[0], i.e. the starting address of where to store
     the command that is read, and length holds the number of characters
     read in. inputBuffer is not a null terminated C-string. */
    
    start = -1;
    if (length == 0)
        exit(0);            /* ^d was entered, end of user command stream */
    
    /* the signal interrupted the read system call */
    /* if the process is in the read() system call, read returns -1
     However, if this occurs, errno is set to EINTR. We can check this  value
     and disregard the -1 value */
    if ( (length < 0) && (errno != EINTR) ) {
        perror("error reading the command");
        exit(-1);           /* terminate with error code of -1 */
    }
    
	printf(">>%s<<",inputBuffer);
    for (i=0;i<length;i++){ /* examine every character in the inputBuffer */
        
        switch (inputBuffer[i]){
            case ' ':
            case '\t' :               /* argument separators */
                if(start != -1){
                    args[ct] = &inputBuffer[start];    /* set up pointer */
                    ct++;
                }
                inputBuffer[i] = '\0'; /* add a null char; make a C string */
                start = -1;
                break;
                
            case '\n':                 /* should be the final char examined */
                if (start != -1){
                    args[ct] = &inputBuffer[start];
                    ct++;
                }
                inputBuffer[i] = '\0';
                args[ct] = NULL; /* no more arguments to this command */
                break;
                
            default :             /* some other character */
                if (start == -1)
                    start = i;
                if (inputBuffer[i] == '&'){
                    *background  = 1;
                    inputBuffer[i] = '\0';
                }
        } /* end of switch */
    }    /* end of for */
    args[ct] = NULL; /* just in case the input line was > 80 */
    
	for (i = 0; i <= ct; i++)
		printf("args %d = %s\n",i,args[i]);
    
    printf("background: %d\n",*background);
    
} /* end of setup routine */



