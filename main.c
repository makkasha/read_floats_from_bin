#include <stdio.h>
#include <stdlib.h>

FILE *open_file(char *filename, char *mode) {//Function to open files, with error handling
    FILE *pfile = fopen(filename, mode);
    if (pfile == NULL) {
        perror("Failed to open file");
        return NULL;
    } else {
        printf("File '%s' opened successfully.\n", filename);
        return pfile;
    }
}

int close_file(char *filename, FILE *pfile) {//Function to close files, with error handling
    if (!fclose(pfile)) {
        printf("File '%s' closed successfully.\n", filename);
        return 0;
    } else {
        perror("File was not closed successfully");
        return -1;
    }
}

unsigned get_number_of_values(FILE *pfile) {
    fseek(pfile, 0, SEEK_END);//Place stream position indicator at EOF
    unsigned bytes = ftell(pfile);//Read number of bytes in file
    if (bytes % sizeof(float)) {//Check if file does contain floats
        rewind(pfile);
        return 0;
    } else {//File does contain floats
        unsigned count = bytes / sizeof(float);
        rewind(pfile);//Place stream position indicator at start of file
        return count;
    }
}

int main(int argc, char *argv[]) {
    char *filename;

    //Allow for filename to be entered as a program argument
    if (argc > 1) {
        filename = argv[1];
    } else {
        filename = "messung.bin";
    }

    FILE *pfile = open_file(filename, "r+b"); //Read binary file

    if (pfile != NULL) {//Error handling - file could not be opened if argument is false
        unsigned fcount = get_number_of_values(pfile); //Read number of floats in binary file
        if (fcount) {//Error handling - fcount is 0 if no floats could be read
            float *fptr = (float *) calloc(fcount, sizeof(float)); //Allocate memory for read floats
            if (fptr == NULL) {//Error handling - memory could not be reserved
                printf("Memory allocation error! Exiting!\n");
                return -1;
            } else {
                float sum = 0;
                unsigned count = fread(fptr, sizeof(float), fcount, pfile);//Read fcount of floats from file
                if (count != fcount) {//Error handling
                    printf("Read error! No. of read elements != No. of elements in file!\n");
                    return -1;
                }
                else{
                    printf("%u elements were read:\n", fcount);
                    for (int i = 0; i < fcount; i++) {//Calculate sum of floats in file and print the floats to console
                        printf("%f\n", *(fptr + i));
                        sum += *(fptr + i);
                    }
                    printf("Mean = %.10f\n", sum / (float) fcount);//Calculate and print average to console
                    close_file(filename, pfile);
                    return 0;
                }
            }
        }
        else{
            printf("Read error! No floats in binary form! \n");
            close_file(filename, pfile);
            return -1;
        }
    }
    else{
        return -1;
    }
}
