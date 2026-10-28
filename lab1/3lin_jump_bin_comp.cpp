
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

const int SIZE = 1000;
const int TRIALS = 5;
const int REPEATS = 10000;

int linearSearch(int arr[], int size, int value)
{
    for (int i = 0; i < size; i++)
        if (arr[i] == value)
            return i;

    return -1;
}

int binarySearch(int arr[], int size, int value)
{
    int left = 0, right = size - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (arr[mid] == value)
            return mid;
        else if (arr[mid] < value)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;
}

int jumpSearch(int arr[], int size, int value)
{
    int blockSize = sqrt(size);
    int start = 0;
    int end = blockSize;

    while (start < size && arr[(end < size ? end : size) - 1] < value)
    {
        start = end;
        end += blockSize;

        if (start >= size)
            return -1;
    }

    for (int i = start; i < (end < size ? end : size); i++)
        if (arr[i] == value)
            return i;

    return -1;
}

int main()
{
    srand(time(NULL));

    double linearTotal = 0;
    double binaryTotal = 0;
    double jumpTotal = 0;

    for (int trial = 0; trial < TRIALS; trial++)
    {
        int arr[SIZE], sortedArr[SIZE];

        for (int i = 0; i < SIZE; i++)
        {
            arr[i] = rand() % 1000;
            sortedArr[i] = arr[i];
        }

        // Sort using Bubble Sort
        for (int i = 0; i < SIZE - 1; i++)
        {
            for (int j = 0; j < SIZE - i - 1; j++)
            {
                if (sortedArr[j] > sortedArr[j + 1])
                {
                    int temp = sortedArr[j];
                    sortedArr[j] = sortedArr[j + 1];
                    sortedArr[j + 1] = temp;
                }
            }
        }

        int target = arr[rand() % SIZE];
        volatile int result = -1;

        clock_t start = clock();

        for (int i = 0; i < REPEATS; i++)
            result = linearSearch(arr, SIZE, target);

        linearTotal += (double)(clock() - start) / CLOCKS_PER_SEC
                       / REPEATS;

        start = clock();

        for (int i = 0; i < REPEATS; i++)
            result = binarySearch(sortedArr, SIZE, target);

        binaryTotal += (double)(clock() - start) / CLOCKS_PER_SEC
                       / REPEATS;

        start = clock();

        for (int i = 0; i < REPEATS; i++)
            result = jumpSearch(sortedArr, SIZE, target);

        jumpTotal += (double)(clock() - start) / CLOCKS_PER_SEC
                     / REPEATS;
    }

    cout << "Average search time over 5 trials:\n";
    cout << "Linear Search: " << linearTotal / TRIALS << " seconds\n";
    cout << "Binary Search: " << binaryTotal / TRIALS << " seconds\n";
    cout << "Jump Search: " << jumpTotal / TRIALS << " seconds\n";

    return 0;
}
