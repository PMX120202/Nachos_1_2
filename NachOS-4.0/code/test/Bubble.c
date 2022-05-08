#include "syscall.h"
#include <stdbool.h>

bool asc(int a, int b)
{
    return a > b;
}

bool decs(int a, int b)
{
    return a < b;
}

void swap(int *a, int *b)
{
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

void BubbleSort(int arr[], int length, bool (*comparison)(int, int))
{
    int i, j;
    bool swapped;
    for (i = 0; i < length - 1; i++)
    {
        swapped = false;
        for (j = 0; j < length - i - 1; j++)
        {
            if (comparison(arr[j],arr[j + 1]))
            {
                swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }
        if (swapped == false)
            break;
    }
}

int InputLength()
{
    int res;
    do
    {
        PrintString("Nhập vào số phần tử của mảng (0 < n <= 100): ");
        res = ReadNum();
    } while (res <= 0 || res > 100);
    return res;
}

int Comparison()// lua chon
{
    int res;
    do
    {
        PrintString("Chọn chiều muốn sắp xếp : 1.Tăng 2.Giảm : ");
        res = ReadNum();
    } while (res != 1 && res != 2);
    return res;
}

int main()
{
    int i, compa;
    int n = InputLength();
    int arr[n];
    for (i = 0; i < n; i++)
    {
        PrintString("Arr[");
        PrintNum(i);
        PrintString("] = ");
        arr[i] = ReadNum();
    }
    compa = Comparison();
    PrintString("Mang truoc khi sap xep : ");
    for (i = 0; i < n; i++)
    {
        PrintNum(arr[i]);
        PrintChar(' ');
    }
    (compa == 1) ? BubbleSort(arr, n, asc) : BubbleSort(arr, n, decs);
    PrintString("\nMang sau khi sap xep : ");
    for (i = 0; i < n; i++)
    {
        PrintNum(arr[i]);
        PrintChar(' ');
    }
    PrintChar('\n');
    Halt();
}