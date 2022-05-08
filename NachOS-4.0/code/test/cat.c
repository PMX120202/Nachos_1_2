#include "syscall.h"

#define MAX_FILE_LENGTH 32
#define MAX_CONTENT_LENGTH 512

void readUserInput(char *content)
{
    char c;
    int i;
    i = 0;
    do
    {
        c = ReadChar();
        if (c == '\n')
        {
            content[i] = '\0';
            break;
        }
        content[i] = c;
        ++i;
    } while (c != '\n');
}

int main()
{
    // Init
    int fileID, contentLength;
    char content[MAX_CONTENT_LENGTH], fileName[MAX_FILE_LENGTH];
    char c;

    // ten file da co
    PrintString("Nhap ten file: ");
    readUserInput(fileName);

    // mo file
    fileID = Open(fileName);

    if (fileID == -1)
        PrintString("Mo file that bai!\n");
    else
    {
        PrintString("Mo file thanh cong!\n");

        //chieu dai muon doc
        PrintString("\nNhap do dai noi dung muon doc: ");
        contentLength = ReadNum();
        PrintChar('\n');

        // doc file
        if (Read(content, contentLength, fileID) == -1)
            PrintString("Doc file that bai!\n");
        else
        {
            PrintString("\nNoi dung file la: ");
            PrintString(content);
            PrintChar('\n');
        }

        // dong file vua doc
        if (Close(fileID) == -1)
            PrintString("Dong file that bai!\n");
        else
            PrintString("\nChuong trinh thuc hien thanh cong!\n\n");
    }

    Halt();
}