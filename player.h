#ifndef PLAYER_H
#define PLAYER_H

#define WORD_LENGTH 5
#define MAX_WORDS 15000

void loadAnswerList(const char *filename);

char *player_AI(const char lastResult[WORD_LENGTH + 1]);

#endif
