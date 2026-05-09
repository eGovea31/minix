#include <stdio.h>      // printf, perror, snprintf
#include <stdlib.h>     // exit
#include <string.h>     // strcmp
#include <dirent.h>     // DIR, opendir, readdir, closedir
#include <sys/stat.h>   // struct stat, lstat, S_ISDIR, S_ISLNK
#include <unistd.h>     // miscellaneous (good practice)

void tree(const char *path, int depth) {
    DIR *dir = opendir(path);
    struct dirent *entry;

    if (!dir) {
        perror("opendir");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        struct stat st;
        char fullpath[1024];

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        lstat(fullpath, &st);

        // Indentación según profundidad
        for (int i = 0; i < depth; i++)
            printf("    ");

        printf("%s\n", entry->d_name);

        // Si es directorio y no symlink, recursión
        if (S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode)) {
            if (strcmp(entry->d_name, ".") != 0 &&
                strcmp(entry->d_name, "..") != 0) {
                tree(fullpath, depth + 1);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *path;

    // If no argument → use current directory
    if (argc < 2) {
        path = ".";
    } else {
        path = argv[1];
    }

    printf("%s\n", path);
    tree(path, 1);

    return 0;
}