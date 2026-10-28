
#include <iostream>
#include <cmath>

using namespace std;

void jumpSearch(int arr[], int size, int value)
{
    int blockSize = sqrt(size);
    int start = 0;
    int end = blockSize;

    while (start < size && arr[min(end, size) - 1] < value)
    {
        start = end;
        end += blockSize;

        if (start >= size)
        {
            cout << "Value not found!" << endl;
            return;
        }
    }

    for (int i = start; i < min(end, size); i++)
    {
        if (arr[i] == value)
        {
            cout << "Value found at index " << i << endl;
            return;
        }
    }

    cout << "Value not found!" << endl;
}

int main()
{
    int arr[8] = {1, 5, 5, 5, 7, 8, 10, 12};
    int size = sizeof(arr) / sizeof(arr[0]);

    jumpSearch(arr, size, 1);

    return 0;
}
