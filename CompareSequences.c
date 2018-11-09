#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_AMOUNT_OF_SEQUENCES 100
#define MAX_LINE_LENGTH 100
#define AMOUNT_OF_ARGC 5

#define FILE_NAME_ARGUMENT 1
#define MATCH_WEIGHT_ARGUMENT 2
#define MISMATCH_WEIGHT_ARGUMENT 3
#define GAP_WEIGHT_ARGUMENT 4

#define FILE_OPEN_ERROR "Error in opening file: %s"

void handleFile(char *fileName, int m, int s, int g);

bool isHeaderLine(char *line);

int main(int argc, char **argv)
{
    if (argc < AMOUNT_OF_ARGC)
    {
        // todo
        exit(EXIT_FAILURE);
    }

    // todo strtol instead of atoi
    int m = atoi(argv[MATCH_WEIGHT_ARGUMENT]);
    int s = atoi(argv[MISMATCH_WEIGHT_ARGUMENT]);
    int g = atoi(argv[GAP_WEIGHT_ARGUMENT]);

    handleFile(argv[FILE_NAME_ARGUMENT], m, s, g);

    return 0;
}


void handleFile(char *fileName, int m, int s, int g)
{
    FILE *fileHandle;
    char line[MAX_LINE_LENGTH];

    // try open file
    fileHandle = fopen(fileName, "r");
    if (fileHandle == NULL)
    {
        printf(FILE_OPEN_ERROR, fileName);
        exit(EXIT_FAILURE);
    }

    char *data[MAX_AMOUNT_OF_SEQUENCES];
    int seqNumber = -1;
    char *currentSeq = NULL;
    size_t currentSeqLength = 0;
    size_t currentSeqMemory = 0;
    size_t currentLineLength;
    // read file line by line into 'line' array
    while (fgets(line, MAX_LINE_LENGTH, fileHandle) != NULL)
    {
        if (isHeaderLine(line))
        {
            if (seqNumber >= 0)
            {
                data[seqNumber] = currentSeq;
            }
            seqNumber++;

            // todo check allocation
            // todo free memory
            currentSeq = (char *) malloc(MAX_LINE_LENGTH * sizeof(char));
            currentSeqLength = 0;
            currentSeqMemory = MAX_LINE_LENGTH;
        } else
        {
            //todo check whitespace line
            currentLineLength = strlen(line);

            // remove new line char from line
            if (line[currentLineLength - 1] == '\n')
            {
                line[currentLineLength - 1] = '\0';
                currentLineLength--;
            }
            if (line[currentLineLength - 1] == '\r')
            {
                line[currentLineLength - 1] = '\0';
                currentLineLength--;
            }

            if (currentSeqLength + currentLineLength < currentSeqMemory)
            {
                if (currentSeqLength == 0)
                {
                    currentSeqLength++; // for null terminator
                }
            } else
            {
                // todo check NULL
                currentSeqMemory = currentSeqMemory + MAX_LINE_LENGTH * sizeof(char);
                currentSeq = (char *) realloc(currentSeq, currentSeqMemory);
            }

            // todo not initialized error
            strcat(currentSeq, line);
            currentSeqLength += currentLineLength;
        }
    }
    data[seqNumber] = currentSeq;
    fclose(fileHandle);
}

bool isHeaderLine(char *line)
{
    return *line == '>';
}

