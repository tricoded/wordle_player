#include "wordle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char wordList[MAX_WORDS][WORD_LENGTH + 1];

void loadWordList(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Failed to open wordList file");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (fscanf(file, "%s", wordList[i]) != EOF)
    {
        i++;
    }
    wordList[i][0] = '\0'; 
    fclose(file);
}

void checkword(const char guess[WORD_LENGTH], const char solution[WORD_LENGTH], char result[WORD_LENGTH + 1])
{
    memset(result, 'B', WORD_LENGTH);
    result[WORD_LENGTH] = '\0';

    int check[WORD_LENGTH] = {0}; 

    for (int i = 0; i < WORD_LENGTH; i++)
    {
        if (guess[i] == solution[i])
        {
            result[i] = 'G';
            check[i] = 1;
        }
    }

    for (int i = 0; i < WORD_LENGTH; i++)
    {
        if (result[i] == 'G')
            continue;

        for (int j = 0; j < WORD_LENGTH; j++)
        {
            if (check[j] == 0 && guess[i] == solution[j])
            {
                result[i] = 'Y';
                check[j] = 1;
                break;
            }
        }
    }
}
