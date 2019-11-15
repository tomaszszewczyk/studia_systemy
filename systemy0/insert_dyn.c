#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <dlfcn.h>

#include "lib.h"

int main(int argc, char *argv[])
{
    size_t size = atoi(argv[1]);
    int data[size];

    srand((unsigned) time(NULL));
    for(size_t i = 0; i < size; i++) {
        data[i] = rand();
    }

    void (*f)(int arr[], int n) = dlsym(dlopen("liblib.so",  RTLD_LAZY | RTLD_GLOBAL), "insertion_sort");
    f(data, size);
}
