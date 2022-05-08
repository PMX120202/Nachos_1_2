#include "syscall.h"

int main()
{
    int num;
    PrintString("Mời nhập số nguyên (int) : ");
    num = ReadNum();
    PrintString("Số nguyên vừa nhập : ");
    PrintNum(num);
    PrintChar('\n');
    Halt();
}
