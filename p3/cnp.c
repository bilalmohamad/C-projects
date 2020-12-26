/**
 * @file cnp.c
 * @author Bilal Mohamad (bmohama)
 *
 * This program will take command-line arguments modify an input file and output the changes
 * to an output file. The actions that can be performed are cut, copy, and paste.
 * The numbers that follow each command are necessary for performing each action.
 * If a hyphen is in place of the input and output file name,
 * then standard input and output is used for writing instead.
 *
 */

#include "buffer.h"
#include "document.h"

/** String constant used for the comparison for cut */
#define CUT "cut"
/** String constant used for the comparison for copy */
#define COPY "copy"
/** String constant used for the comparison for paste */
#define PASTE "paste"

/** Global variable used for the buffer when performing any of the actions from buffer.c */
extern char buffer[MAX_LENGTH + 1];

/** Print a usage message, then exit unsuccessfully. */
static void usage()
{
    fprintf(stderr, "Invalid command\n");
    fprintf(stderr, "usage: ((cut s n)|(copy s n)|(paste s))* (infile|-) (outfile|-)\n");
    exit(EXIT_FAILURE);
}

/**
 * Starting point for the program
 *
 * @param argc    the number of arguments during execution
 * @param argv    an array of argument strings during execution
 *
 * @return the exit status
 */
int main(int argc, char *argv[])
{
    if (argc <= 2){
        usage();
        return EXIT_FAILURE;
    }

    //For input
    FILE *inputFp;
    if (strcmp(argv[argc - 2], "-") == 0){
        inputFp = stdin;
    }
    else {
        inputFp = fopen(argv[argc - 2], "r");
    }

    if (inputFp == NULL) {
        fprintf(stderr, "Can't open file: %s\n", argv[argc - 2]);
        exit(EXIT_FAILURE);
    }

    //Reads in the document
    char doc[ MAX_LINES ][ MAX_LENGTH + 1 ];
    int lines = readDocument(inputFp, doc);

    if (lines > MAX_LINES){
        fprintf(stderr, "Too many lines\n");
        exit(EXIT_FAILURE);
    }

//    printf("%d\n", lines);

    //Main loop for arguments
    for (int row = 0; row < lines; row++){

        int lineLength = strlen(doc[row]);

        if (lineLength > MAX_LENGTH){
              fprintf(stderr, "Line too long\n");
              exit(EXIT_FAILURE);
        }

        for (int i = 1; i < argc - 2; i++){

            char *command = argv[i];

            if (strcmp(command, CUT) == 0){
                int start;
                int number;
                bool flag;

                if (sscanf(argv[i + 1], "%d", &start) != EOF
                        && sscanf(argv[i + 2], "%d", &number) != EOF){
                    flag = cut(doc[row], start, number);

                    if (!flag){
                        usage();
                        return EXIT_FAILURE;
                    }
                }
                else{
                    usage();
                    return EXIT_FAILURE;
                }

                //Skips over two arguments
                i += 2;
            }
            else if (strcmp(command, COPY) == 0){

                int start;
                int number;
                bool flag;

                if (sscanf(argv[i + 1], "%d", &start) != EOF
                        && sscanf(argv[i + 2], "%d", &number) != EOF){
                    flag = copy(doc[row], start, number);

                    if (!flag){
                        usage();
                        return EXIT_FAILURE;
                    }
                }
                else{
                    usage();
                    return EXIT_FAILURE;
                }

                //Skips over two arguments
                i += 2;
            }

            else if (strcmp(command, PASTE) == 0){

                int start;
                bool flag;

                if (sscanf(argv[i + 1], "%d", &start) != EOF){
                    flag = paste(doc[row], start);

                    if (!flag){
                        usage();
                        return EXIT_FAILURE;
                    }
                }
                else{
                    usage();
                    return EXIT_FAILURE;
                }

                //Skips over one argument
                i++;
            }
            else{
                usage();
                return EXIT_FAILURE;
            }
        }
    }

    //Writes to output
    FILE *outputFp;
    if (strcmp(argv[argc - 1], "-") == 0){
        outputFp = stdout;
    }
    else {
        outputFp = fopen(argv[argc - 1], "w");
    }
    printDocument(outputFp, doc, lines);

    fclose(inputFp);
    fclose(outputFp);

    return EXIT_SUCCESS;
}
