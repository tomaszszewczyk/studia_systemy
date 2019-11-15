#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    void (*generate)(const char * file, size_t count, size_t size);
    void (*sort)(const char * file, size_t count, size_t size);
    void (*copy)(const char * file1, const char * file2, size_t count, size_t size);
} implementation_t;

void sys_generate(const char * file, size_t count, size_t size);
void sys_sort(const char * file, size_t count, size_t size);
void sys_copy(const char * file1, const char * file2, size_t count, size_t size);

void lib_generate(const char * file, size_t count, size_t size);
void lib_sort(const char * file, size_t count, size_t size);
void lib_copy(const char * file1, const char * file2, size_t count, size_t size);

int main(int argc, char *argv[])
{
    if(argc < 3) {
        printf("Wrong number of arguments\n");
        return -1;
    }

    const char * command = argv[1];
    const char * mode = argv[2];
    implementation_t implementation;

    if(strcmp(mode, "sys") == 0) {
        printf("Using sys implementation\n");

        implementation = (implementation_t) {
            .generate = sys_generate,
            .sort = sys_sort,
            .copy = sys_copy,
        };
    } else if (strcmp(mode, "lib") == 0) {
        printf("Using lib implementation\n");

        implementation = (implementation_t) {
            .generate = lib_generate,
            .sort = lib_sort,
            .copy = lib_copy,
        };
    } else {
        printf("Invalid mode\n");
        return -2;
    }

    if(strcmp(command, "generate") == 0) {
        printf("Command generate\n");

        if(argc < 4) {
            printf("Wrong number of arguments\n");
            return -1;
        }

        implementation.generate(argv[3], atoi(argv[4]), atoi(argv[5]));
    } else if (strcmp(command, "sort") == 0) {
        printf("Command sort\n");

        if(argc < 4) {
            printf("Wrong number of arguments\n");
            return -1;
        }

        implementation.sort(argv[3], atoi(argv[4]), atoi(argv[5]));
    } else if (strcmp(command, "copy") == 0) {
        printf("Command copy\n");

        if(argc < 7) {
            printf("Wrong number of arguments\n");
            return -1;
        }

        implementation.copy(argv[3], argv[4], atoi(argv[5]), atoi(argv[6]));
    } else {
        printf("Invalid command\n");
        return -3;
    }

    printf("thanks\n");
    return 0;
}

void sys_read(int fd, char * tmp, size_t size) {
    for(int r = 0; r < size;) {
        int output = read(fd, tmp, size - r);

        if(output < 0) {
            printf("Error while reading\n");
            exit(-1);
        }

        r += output;
    }
}

void sys_write(int fd, char * tmp, size_t size) {
    for(int w = 0; w < size;) {
        int output = write(fd, tmp, size - w);

        if(output < 0) {
            printf("Error while writing\n");
            exit(-1);
        }

        w += output;
    }
}

void sys_generate(const char * file, size_t count, size_t size) {
    int fd_file = open(file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    int fd_rand = open("/dev/urandom", O_RDONLY);

    if(fd_file < 0) {
        printf("Error while opening file\n");
        exit(-1);
    }

    if(fd_rand < 0) {
        printf("Error while opening random\n");
        exit(-1);
    }

    for(size_t i = 0; i < count; i++) {
        char tmp[size];
        sys_read(fd_rand, tmp, size);
        sys_write(fd_file, tmp, size);
    }

    close(fd_file);
    close(fd_rand);
}

void sys_sort(const char * file, size_t count, size_t size) {
    int fd_file = open(file, O_RDWR);

    if(fd_file < 0) {
        printf("Error while opening file\n");
        exit(-1);
    }

    bool something_been_changed;

    do {
        something_been_changed = false;
        lseek(fd_file, 0, SEEK_SET);

        uint8_t a[size];
        uint8_t b[size];

        for(size_t i = 0; i < count - 1; i++) {
            sys_read(fd_file, a, size);
            sys_read(fd_file, b, size);

            if(a[0] > b[0]) {
                lseek(fd_file, size * i, SEEK_SET);

                sys_write(fd_file, b, size);
                sys_write(fd_file, a, size);

                something_been_changed = true;
            }

            lseek(fd_file, size * (i + 1), SEEK_SET);
        }

    } while(something_been_changed);

    close(fd_file);
}

void sys_copy(const char * file1, const char * file2, size_t count, size_t size) {
    int fd_file1 = open(file1, O_RDONLY);
    int fd_file2 = open(file2, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);

    if(fd_file1 < 0) {
        printf("Error while opening file1\n");
        exit(-1);
    }

    if(fd_file2 < 0) {
        printf("Error while opening file2\n");
        exit(-1);
    }

    for(size_t i = 0; i < count; i++) {
        char tmp[size];
        sys_read(fd_file1, tmp, size);
        sys_write(fd_file2, tmp, size);
    }

    close(fd_file1);
    close(fd_file2);
}

void lib_read(FILE * fd, char * tmp, size_t size) {
    size_t output = fread(tmp, 1, size, fd);

    if(output != size) {
        printf("Error while reading\n");
        exit(-1);
    }
}

void lib_write(FILE * fd, char * tmp, size_t size) {
    size_t output = fwrite(tmp, 1, size, fd);

    if(output != size) {
        printf("Error while writing\n");
        exit(-1);
    }
}

void lib_generate(const char * file, size_t count, size_t size) {
    FILE * fd_file = fopen(file, "w");
    FILE * fd_rand = fopen("/dev/urandom", "r");

    if(fd_file == NULL) {
        printf("Error while opening file\n");
        exit(-1);
    }

    if(fd_rand == NULL) {
        printf("Error while opening random\n");
        exit(-1);
    }

    for(size_t i = 0; i < count; i++) {
        char tmp[size];
        lib_read(fd_rand, tmp, size);
        lib_write(fd_file, tmp, size);
    }

    fclose(fd_file);
    fclose(fd_rand);
}

void lib_sort(const char * file, size_t count, size_t size) {
    FILE * fd_file = fopen(file, "r+");

    if(fd_file == NULL) {
        printf("Error while opening file\n");
        exit(-1);
    }

    bool something_been_changed;

    do {
        something_been_changed = false;
        fseek(fd_file, 0, SEEK_SET);

        uint8_t a[size];
        uint8_t b[size];

        for(size_t i = 0; i < count - 1; i++) {
            lib_read(fd_file, a, size);
            lib_read(fd_file, b, size);

            if(a[0] > b[0]) {
                fseek(fd_file, size * i, SEEK_SET);

                lib_write(fd_file, b, size);
                lib_write(fd_file, a, size);

                something_been_changed = true;
            }

            fseek(fd_file, size * (i + 1), SEEK_SET);
        }

    } while(something_been_changed);

    fclose(fd_file);
}

void lib_copy(const char * file1, const char * file2, size_t count, size_t size) {
    FILE * fd_file1 = fopen(file1, "r");
    FILE * fd_file2 = fopen(file2, "w");

    if(fd_file1 == NULL) {
        printf("Error while opening file1\n");
        exit(-1);
    }

    if(fd_file2 == NULL) {
        printf("Error while opening file2\n");
        exit(-1);
    }

    for(size_t i = 0; i < count; i++) {
        char tmp[size];
        lib_read(fd_file1, tmp, size);
        lib_write(fd_file2, tmp, size);
    }

    fclose(fd_file1);
    fclose(fd_file2);
}
