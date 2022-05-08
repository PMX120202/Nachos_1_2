#include "syscall.h"

int main()
{
    PrintString("Số nguyên ngẫu nhiên (int) : ");
    PrintNum(RandomNum());
    PrintChar('\n');
    Halt();
}
