#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "lib.h"

int main(int argc, char *argv[])
{
    size_t size = atoi(argv[1]);
    int data[size];

    srand((unsigned) time(NULL));
    for(size_t i = 0; i < size; i++) {
        data[i] = rand();
    }

    insertion_sort(data, size);
}
