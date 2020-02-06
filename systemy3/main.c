#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

void analyze(char * path) {
    pid_t pid = fork();

    if(pid != 0) {
        return;
    }

    struct dirent * p_dirent;
    DIR * p_dir;

    p_dir = opendir (path);
    if (p_dir == NULL) {
        printf ("Cannot open directory '%s'\n", path);
        exit(1);
    }

    while ((p_dirent = readdir(p_dir)) != NULL) {
        if(p_dirent->d_type == DT_DIR && strcmp(p_dirent->d_name, ".") != 0 && strcmp(p_dirent->d_name, "..") != 0) {
            char new_path[strlen(path) + strlen(p_dirent->d_name) + 1];
            strcpy(new_path, path);
            strcpy(new_path + strlen(new_path), "/");
            strcpy(new_path + strlen(new_path), p_dirent->d_name);
            analyze(new_path);
        } else if(p_dirent->d_type == DT_REG) {
            struct stat sb;
            if (stat(path, &sb) == -1) {
                printf("Error while stat\n");
                exit(1);
            }

            char permissions[10];
            permissions[0] = (sb.st_mode & S_IRUSR) ? 'r' : '-';
            permissions[1] = (sb.st_mode & S_IWUSR) ? 'w' : '-';
            permissions[2] = (sb.st_mode & S_IXUSR) ? 'x' : '-';
            permissions[3] = (sb.st_mode & S_IRGRP) ? 'r' : '-';
            permissions[4] = (sb.st_mode & S_IWGRP) ? 'w' : '-';
            permissions[5] = (sb.st_mode & S_IXGRP) ? 'x' : '-';
            permissions[6] = (sb.st_mode & S_IROTH) ? 'r' : '-';
            permissions[7] = (sb.st_mode & S_IWOTH) ? 'w' : '-';
            permissions[8] = (sb.st_mode & S_IXOTH) ? 'x' : '-';
            permissions[9] = '\0';

            printf("%s/%s\t%ld\t%s\t%s", path, p_dirent->d_name, sb.st_size, permissions, ctime(&sb.st_mtim));
        }
    }

    closedir (p_dir);
    exit(1);
}

int main(int argc, char *argv[])
{
    analyze("/home/tomek/crypto");
}
