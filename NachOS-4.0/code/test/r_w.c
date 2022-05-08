#include "syscall.h"
#define Type_ConsoleInput	2
#define Type_ConsoleOutput	3  
#define ReadWrite 0
#define ReadOnly 1

int main()
{
  int size;
  char result[255];

  PrintString("\nNhap noi dung: ");
  // Mo console input de doc vao result
  // Doc toi da 255 ky tu
  size = Read(result,255,Type_ConsoleInput);
  
  PrintString("\nSo ky tu doc duoc: ");
  PrintNum(size);
  PrintChar('\n');
  // In ra console output
  PrintString("\nNoi dung ban vua nhap: ");
  Write(result,size,Type_ConsoleOutput);
    PrintChar('\n');
  Halt();
}