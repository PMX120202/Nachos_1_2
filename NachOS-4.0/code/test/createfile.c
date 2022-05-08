#include "syscall.h"

int main()
{
    char fileName[32];
    int length;
    PrintString("Nhập độ dài của tên file :  ");
    length = ReadNum();
    PrintString("Nhập tên file : ");
    ReadString(fileName, length);
    if (Create(fileName) == 0)
    {
        PrintString("File ");
        PrintString(fileName);
        PrintString(" đã được tạo thành công !!\n");
    }
    else
        PrintString("\nChưa tạo được file\n");
    Halt();
}