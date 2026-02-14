#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"

int mode = FALSE;

/**
 * Recursively searches through the given directory and 
 * lists all directories and files if mode is 1
 * or stores all directories in the path of any matching file if mode is 2
 * 
 * @param path The base directory/file path
 * @param target The file name to search for (NULL if in mode 1)
 * @param indent The current indentation
 * @return 1/TRUE upon success/target found, 0/FALSE otherwise
 */
int recurseDir(char *path, char *target, char *indent) {
    DIR *dir;
    struct dirent *entry;
    char *fullPath = malloc(MAX_PATH_SIZE);
    char *nextIndent = malloc(MAX_PATH_SIZE);

    if (fullPath == NULL || nextIndent == NULL) {
        if (fullPath) free(fullPath);
        if (nextIndent) free(nextIndent);
        return 0;
    }
    
    dir = opendir(path);
    if (dir == NULL) {
        closedir(dir);
        freeMem(fullPath, nextIndent);
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
                }
            }
        } else {
            double fileSize = getFileSize(fullPath);
            if (fileSize != -1) {
                if (mode == FALSE) {
                    printf("%s%s (%.0lf bytes)\n", indent, name, fileSize);
                } else {
                    if (strcmp(name, target) == 0) {
                        char *str = malloc(MAX_PATH_SIZE);
                        snprintf(str, MAX_PATH_SIZE, "%s%s (%.0lf bytes)", indent, name, fileSize);
                        push(s, str);
                        free(str);
                        closedir(dir);
                        freeMem(fullPath, nextIndent);
                        return TRUE;
                    } else {
                        closedir(dir);
                        freeMem(fullPath, nextIndent);
                        return FALSE;
                    }
                }
            }
        }
    }
    closedir(dir);
    freeMem(fullPath, nextIndent);
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

void freeMem(char *fullPath, char *nextIndent) {
    free(fullPath);
    free(nextIndent);
}