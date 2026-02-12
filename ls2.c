#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"


int recurseDir(char *path, char *indent) {
    DIR *dir;
    struct dirent *entry;
    char fullPath[500];
    char nextIndent[500];

    dir = opendir(path);

    if (dir == NULL) {
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        char *name = entry->d_name;

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
            continue;
        }

        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, name);

        if (isDir(fullPath)) {
            printf("%s%s/ (directory)\n", indent, name);
            snprintf(nextIndent, sizeof(nextIndent), "%s%s", indent, INDENT);
            recurseDir(fullPath, nextIndent);
        } else {
            double size = getFileSize(fullPath);
            if (size != -1) {
                printf("%s%s (%.0lf bytes)\n", indent, name, size);
            }
        }
    }
    closedir(dir);
    return 1;
}

int isDir(char *path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return 0;
    }
    if (S_ISDIR(st.st_mode)) {
        return 1;
    }
    return 0;
}

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
