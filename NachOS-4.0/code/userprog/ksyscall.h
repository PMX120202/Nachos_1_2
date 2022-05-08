/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include <stdlib.h>
#include "synchconsole.h"
#include"machine.h"

#define MAX_OPEN_FILE 10

void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}


int SysReadNum()
{
  // int từ -2,147,483,648 đến 2,147,483,647 => max length là 11 + 1 dấu - nếu số âm
  const int maxLength = 12;
  char numStr[maxLength + 1] = {0};
  long res = 0;
  for (int i = 0; i < maxLength; i++)
  {
    char ch = kernel->InputChar();
    if (ch >= '0' && ch <= '9')
    {
      numStr[i] = ch;
    }
    else if (i == 0 && ch == '-')
      numStr[i] = ch;
    else if (ch != '\n' && ch != '\0' && ch != 0)
      return 0;
    else
    {
      DEBUG('a', "\n The integer number is not valid");
      numStr[i] = 0;
      break;
    }
  }
  int i = (numStr[0] == '-') ? 1 : 0;
  // số dương sẽ thì việc thêm vào res thì vị trí 0, số âm thì từ vị trí 1 (vị trí 0 là dấu âm)
  while (i < maxLength + 1 && numStr[i] >= '0' && numStr[i] <= '9')
  {
    res = res * 10 + numStr[i++] - '0';
  }
  res = (numStr[0] == '-') ? (-res) : res; // thêm dấu
  if (res <= -2147483647 || res > 2147483647) // gioi han 4 byte
    return 0;
  return (int)res;
}

void SysPrintNum(int num)
{
  if (num == 0)
  {
    kernel->OutputChar('0');
    return;
  }
  // chuyển số thành chuỗi để in ra màn hình
  int startIndex, numTemp;
  char *numStr = new char[13];
  if (num < 0)
  {
    startIndex = 1; // nếu số âm chuỗi xuất phát từ vị trí 1
    numStr[0] = '-';
    numTemp = num * -1; // gán biến số tạm là số dương
  }
  else
  {
    startIndex = 0;
    numTemp = num;
  }
  while (numTemp > 0)
  {
    numStr[startIndex++] = (char)((numTemp % 10) + 48);
    numTemp /= 10;
  }
  int strLen = startIndex;
  numStr[strLen] = 0;
  int stop = (num < 0) ? 1 : 0;
  if (stop == 1)
    kernel->OutputChar('-');
  for (int i = strLen; i >= stop; i--)
  {
    kernel->OutputChar(numStr[i]);
  }
}

char SysReadChar()
{
  return kernel->InputChar();
}

void SysPrintChar(char character)
{
  kernel->OutputChar(character);
}

int SysRandomNum()
{
      int result;
      RandomInit(time(NULL));
      result = RandomNumber();
      kernel->machine->WriteRegister(2, result);
      return result;
}

void SysReadString(char *buffer, int length)
{
    int idx;
    char ch;
    for (idx = 0; idx < length; ++idx)
        buffer[idx] = 0;
    for (idx = 0; idx < length;)
    {
        do
        { // bo qua cac ki tu EOF
            ch = kernel->synchConsoleIn->GetChar();
        } while (ch == EOF);
        if (ch == '\001' || ch == '\n') // enter -> ket thuc nhap
            break;
        buffer[idx++] = ch;
    }
}


void SysPrintString(char *buffer)
{
  int length = 0;
  while (buffer[length] != 0)
    length++; //đếm độ dài của chuỗi
  for (int i = 0; i < length; i++)
    kernel->OutputChar(buffer[i]);
}


// void SysPrintAscii()
// {
//   for (int i = 32; i < 127; i++)
//   {
//     SysPrintNum(i);
//     kernel->OutputChar(' ');
//     kernel->OutputChar((char)i);
//     kernel->OutputChar('\t');
//     if (i % 10 == 0)
//       kernel->OutputChar('\n');
//   }
//   kernel->OutputChar('\n');
// }


//-----------------------------------------------------------------------------






#endif /* ! __USERPROG_KSYSCALL_H__ */
