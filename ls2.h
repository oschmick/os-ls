#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "stack.h"

// function declarations, globals, constants here
#define TRUE 1
#define FALSE 0
#define INDENT "    "
#define MAX_PATH_SIZE 500

int recurseDir(char *path, char *file, char *indent);
int isDir(char *path);
int getFileSize(char *path);
void freeMem(char *fullPath, char *nextIndent);

extern stack_t *s;  // pointer to the stack to hold the listings

#endif
