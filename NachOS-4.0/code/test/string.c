#include "syscall.h"

int InputLength()
{
    int res;
    do
    {
        PrintString("Nhập độ dài của chuỗi (0 < n <= 255) : ");
        res = ReadNum();
    } while (res  < 1 || res > 255);
    return res;
}

int main()
{
    char str[256];
    int length;
    length = InputLength();
    PrintString("Nhập chuỗi : ");
    ReadString(str,length);
    PrintString("Chuỗi vừa nhập : ");
    PrintString(str);
    PrintChar('\n');
    Halt();
}
