#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"

int mode = FALSE;
//int add = FALSE;

/**
 * 
 */
int recurseDir(char *path, char *target, char *indent) {
    DIR *dir;
    struct dirent *entry;
    char *fullPath = malloc(MAX_PATH_SIZE);
    char *nextIndent = malloc(MAX_PATH_SIZE);

    dir = opendir(path);

    if (fullPath == NULL || nextIndent == NULL) return 0;
    
    if (dir == NULL) {
        free(fullPath);
        free(nextIndent);
        return 0;
    }

    if (target != NULL) mode = TRUE;

    while ((entry = readdir(dir)) != NULL) {
        char *name = entry->d_name;

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) continue;

        snprintf(fullPath, MAX_PATH_SIZE, "%s/%s", path, name);

        if (isDir(fullPath)) {
            snprintf(nextIndent, MAX_PATH_SIZE, "%s%s", indent, INDENT);
            if (mode == FALSE) {
                printf("%s%s/ (directory)\n", indent, name);
                recurseDir(fullPath, NULL, nextIndent);
            } else {
                if (recurseDir(fullPath, target, nextIndent)) {
                    char *str = malloc(MAX_PATH_SIZE);
                    snprintf(str, MAX_PATH_SIZE, "%s%s/ (directory)", indent, name);
                    push(s, str);
                    free(str);
                    //add = FALSE;
                }
            }
        } else {
            double fileSize = getFileSize(fullPath);
            if (fileSize != -1) {
                if (mode == FALSE) {
                    printf("%s%s (%.0lf bytes)\n", indent, name, fileSize);
                } else {
                    //pushOntoStack(name, indent, target, fileSize);
                    if (strcmp(name, target) == 0) {
                        char *str = malloc(MAX_PATH_SIZE);
                        snprintf(str, MAX_PATH_SIZE, "%s%s (%.0lf bytes)", indent, name, fileSize);
                        push(s, str);
                        free(str);
                        return TRUE;
                    } else {
                        return FALSE;
                    }
                }
            }
        }
    }
    closedir(dir);
    free(fullPath);
    free(nextIndent);
    return 1;
}

/**
 * Helper method to determine if a given path is a directory.
 * @param path The path/name of the directory
 * @return TRUE (1) if true and FALSE (0) if false
 */
int isDir(char *path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return FALSE;
    }
    if (S_ISDIR(st.st_mode)) {
        return TRUE;
    }
    return FALSE;
}

/**
 * Helper method to determine if a given path is a regular file.
 * @param path The path/name of the file
 * @return size of the file if true and -1 if false
 */
int getFileSize(char *path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return -1;
    }
    if (S_ISREG(st.st_mode)) {
        return st.st_size;
    } else {
        return -1;
    }
}

/**
 * Helper method to push a file onto the stack.
 * @param name The path/name of the directory
 * @param indent The current indentation
 * @param file The file to look for
 * @return TRUE (1) if true and FALSE (0) if false
 */
void pushOntoStack(char *name, char *indent, char *file, double fileSize) {
    if (strcmp(name, file) == 0) {
        char *str = malloc(MAX_PATH_SIZE);
        snprintf(str, MAX_PATH_SIZE, "%s%s (%.0lf bytes)", indent, name, fileSize);
        push(s, str);
        free(str);
        //add = TRUE;
    } 
    // else {
    //     return FALSE;
    // }
}