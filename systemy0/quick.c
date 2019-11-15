#include <stddef.h>
#include <stdlib.h>
#include <time.h>

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition (int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quick_sort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

int main(int argc, char *argv[])
{
    size_t size = atoi(argv[1]);
    int data[size];

    srand((unsigned) time(NULL));
    for(size_t i = 0; i < size; i++) {
        data[i] = rand();
    }

    quick_sort(data, 0, size);
}
