
#include "syscall.h"

int main() 
{
    char fileName[256];
    int length;
    PrintString("Nhap chieu dai ten file: ");
    length = ReadNum();
    PrintString("Nhap ten file: ");
    ReadString(fileName, length);
    Remove(fileName);// goi ham xoa 
    PrintString("\nXOA FILE THANH CONG !!!!!\n");
}
