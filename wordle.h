#ifndef WORDLE_H
#define WORDLE_H

#define WORD_LENGTH 5
#define MAX_WORDS 15000

extern char wordList[MAX_WORDS][WORD_LENGTH + 1];

void checkword(const char guess[WORD_LENGTH], const char solution[WORD_LENGTH], char result[WORD_LENGTH + 1]);
void loadWordList(const char *filename);

#endif
