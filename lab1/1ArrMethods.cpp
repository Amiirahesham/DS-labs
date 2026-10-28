#include<iostream>
using namespace std;
class Array_methods
{
    int* arr;
    int size = 0;

public:
    Array_methods()
    {
        arr = new int[size]{};
    }
    /////////////////////append////////////////
    void append(int value)
    {
        int* temp = new int[size + 1];

        for (int i = 0; i < size; i++)
        {
            temp[i] = arr[i];
        }

        temp[size] = value;

        delete[] arr;
        arr = temp;
        size++;

        cout << "New array after appending " << value << "===> ";

        for (int i = 0; i < size; i++)
        {
            cout << arr[i] << " ";
        }

        cout << "\nsize of array = "<<size<<endl;
    }
    ///////////////////insert////////////////

    void insert(int index, int value){
        int* temp = new int[size + 1];
        for(int i=0;i<size ;i++)
        {
            if(index >i){
                temp[i] = arr[i];
            }
            else if(index==i){
                temp[index] = value;
                temp[i + 1] = arr[i];
            }
            else
            temp[i+1]=arr[i];
        }

        delete[] arr;
        arr = temp;
        size++;
        cout << "New array after inserting " << value << ": in index : "<<index <<" ===> ";

        for (int i = 0; i < size; i++)
        {
            cout << arr[i] << " ";
        }

        cout << "\nsize of array = "<<size<<endl;
    }
    ///////////////////remove////////////////

void remove(int value)
{
    int index = -1;

    // Find the first occurrence of value
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == value)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "Value not found!" << endl;
        return;
    }

    int* temp = new int[size - 1];

    for (int i = 0; i < index; i++)
    {
        temp[i] = arr[i];
    }

    for (int i = index + 1; i < size; i++)
    {
        temp[i - 1] = arr[i];
    }

    delete[] arr;
    arr = temp;
    size--;

    cout << "New array after removing " << value << " ==> ";

    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }

    cout << "\nSize of array = " << size << endl;
}
    ///////////////////pop////////////////

    void pop(int index = -1)
{
    if (size == 0)
    {
        cout << "Array is empty!" << endl;
        return;
    }

    // If no index is provided, remove the last element
    if (index == -1)
        index = size - 1;

    if (index < 0 || index >= size)
    {
        cout << "Invalid index!" << endl;
        return;
    }

    int* temp = new int[size - 1];

    for (int i = 0; i < index; i++)
    {
        temp[i] = arr[i];
    }

    for (int i = index + 1; i < size; i++)
    {
        temp[i - 1] = arr[i];
    }

    delete[] arr;
    arr = temp;
    size--;

    cout << "Array after pop ==> ";

    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }

    cout << "\nSize = " << size << endl;
}
    ///////////////////clear////////////////

    void clear(){

    }
    ///////////////////index////////////////

    void index(int value){
        int index = -1;

        for (int i = 0; i < size; i++)
        {
            if (arr[i] == value)
            {
                index = i;
                cout<<"value found at index :"<<i<<endl;
                break;
            }
        }

    if (index == -1)
    {
        cout << "Value not found!" << endl;
        return;
    }
        cout << "\nSize of array = " << size << endl;

    }
    ///////////////////count////////////////
void count(int value)
{
    int counter = 0;

    for (int i = 0; i < size; i++)
    {
        if (arr[i] == value)
        {
            counter++;
        }
    }

    cout << value << " appears " << counter << " times." << endl;
}
    ///////////////////reverse////////////////

    void reverse()
{
    int* temp = new int[size];

    for (int i = 0; i < size; i++)
    {
        temp[i] = arr[size - 1 - i];
    }

    delete[] arr;
    arr = temp;

    cout << "Array after reversing: ";

    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }

    cout << endl;
}


};
int main()
{
    Array_methods ob1;
    ob1.append(10);
    ob1.append(15);
    ob1.append(19);
    ob1.append(20);
    ob1.append(35);
    ob1.append(19);
    ob1.insert(2,50);
    ob1.remove(50);
    ob1.index(19);
    ob1.pop();
    ob1.pop(0);
    ob1.count(19);
    ob1.reverse();


    return 0;
}

