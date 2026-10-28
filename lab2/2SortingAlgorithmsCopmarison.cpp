
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int SIZE = 50000;
const int TRIALS = 5;

// 1. Selection Sort
void selectionSort(int arr[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        int minIndex = i;

        for (int j = i + 1; j < size; j++)
        {
            if (arr[j] < arr[minIndex])
                minIndex = j;
        }

        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }
}

// 2. Bubble Sort
void bubbleSort(int arr[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        bool swapped = false;

        for (int j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }
}

// 3. Insertion Sort
void insertionSort(int arr[], int size)
{
    for (int i = 1; i < size; i++)
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

// 4. Merge Sort
void merge(int arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// 5. Quick Sort
int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;

            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pivotIndex = partition(arr, low, high);

        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

// Copy array manually
void copyArray(int source[], int destination[], int size)
{
    for (int i = 0; i < size; i++)
        destination[i] = source[i];
}

int main()
{
    srand(time(NULL));

    double selectionTotal = 0;
    double bubbleTotal = 0;
    double insertionTotal = 0;
    double mergeTotal = 0;
    double quickTotal = 0;

    int* original = new int[SIZE];
    int* arr = new int[SIZE];

    for (int trial = 0; trial < TRIALS; trial++)
    {
        // Generate random input
        for (int i = 0; i < SIZE; i++)
            original[i] = rand() % 100 + 1;

        // Selection Sort
        copyArray(original, arr, SIZE);
        clock_t start = clock();

        selectionSort(arr, SIZE);

        selectionTotal +=
            (double)(clock() - start) / CLOCKS_PER_SEC;

        // Bubble Sort
        copyArray(original, arr, SIZE);
        start = clock();

        bubbleSort(arr, SIZE);

        bubbleTotal +=
            (double)(clock() - start) / CLOCKS_PER_SEC;

        // Insertion Sort
        copyArray(original, arr, SIZE);
        start = clock();

        insertionSort(arr, SIZE);

        insertionTotal +=
            (double)(clock() - start) / CLOCKS_PER_SEC;

        // Merge Sort
        copyArray(original, arr, SIZE);
        start = clock();

        mergeSort(arr, 0, SIZE - 1);

        mergeTotal +=
            (double)(clock() - start) / CLOCKS_PER_SEC;

        // Quick Sort
        copyArray(original, arr, SIZE);
        start = clock();

        quickSort(arr, 0, SIZE - 1);

        quickTotal +=
            (double)(clock() - start) / CLOCKS_PER_SEC;

        cout << "Trial " << trial + 1 << " completed.\n";
    }

    cout << "\n===== Average Execution Time =====\n";
    cout << "Selection Sort: " << selectionTotal / TRIALS
         << " seconds\n";

    cout << "Bubble Sort:    " << bubbleTotal / TRIALS
         << " seconds\n";

    cout << "Insertion Sort: " << insertionTotal / TRIALS
         << " seconds\n";

    cout << "Merge Sort:     " << mergeTotal / TRIALS
         << " seconds\n";

    cout << "Quick Sort:     " << quickTotal / TRIALS
         << " seconds\n";

    delete[] original;
    delete[] arr;

    return 0;
}
