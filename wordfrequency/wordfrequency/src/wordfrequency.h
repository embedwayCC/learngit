/* wordfrequency.h */

#ifndef WORDFREQUENCY_H
#define WORDFREQUENCY_H

#define WORD_LENGTH 200

typedef struct wordfrequency{
	char word[WORD_LENGTH];
	int time;
}Frequency;

//extern List* list;

extern void HandleWord(char* word, bool flag);

extern int SearchWord(char* word, bool* flag);

extern int CountWordByPosition(char* word);

extern int PrintResult();

extern int FPrintResult();

extern int PositionToDictionary();

extern int PositionToTimes();

extern void InitList();

extern void DestroyList();

extern void PrintQuickSort(char* type);

#endif



