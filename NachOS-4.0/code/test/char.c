#include "syscall.h"

int main()
{
    int i;
    char ch;
    PrintString("Mời nhập vào một ký tự : ");
    ch = ReadChar();
    PrintString("Ký tự vừa nhập : ");
    PrintChar(ch);
    PrintChar('\n');
    Halt();
}
