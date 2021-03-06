// #include "syscall.h"

// #define MAX_FILE_LENGTH 32
// #define MAX_CONTENT_LENGTH 512

// void readUserInput(char *content)
// {
//     char c;
//     int i;
//     i = 0;
//     do
//     {
//         c = ReadChar();
//         if (c == '\n')
//         {
//             content[i] = '\0';
//             break;
//         }
//         content[i] = c;
//         ++i;
//     } while (c != '\n');
// }

// int main()
// {
//     int fileID1, fileID2, newFileID;
//     char fileName1[MAX_FILE_LENGTH], fileName2[MAX_FILE_LENGTH], newFileName[MAX_FILE_LENGTH];
//     char content1[MAX_CONTENT_LENGTH], content2[MAX_CONTENT_LENGTH];

//     PrintString("Please enter the name of the 1st file: ");
//     readUserInput(fileName1);

//     PrintString("Please enter the name of the 2nd file: ");
//     readUserInput(fileName2);

//     fileID1 = Open(fileName1);
//     fileID2 = Open(fileName2);

//     if (fileID1 != -1 && fileID2 != -1)
//     {
//         PrintString("\nSuccessfully read two files.\n\n");

//         Read(content1, MAX_CONTENT_LENGTH, fileID1);
//         Read(content2, MAX_CONTENT_LENGTH, fileID2);
//         Seek(-1, fileID1);
//         Seek(-1, fileID2);

//         PrintString("Please enter the name of the new file: ");
//         readUserInput(newFileName);

//         if (Create(newFileName) == -1)
//             PrintString("\nCreate new file failed.\n");
//         else
//         {
//             newFileID = Open(newFileName);

//             if (newFileID == -1)
//                 PrintString("ERROR: Cannot open new file!\n\n");
//             else
//             {
//                 Write(content1, MAX_CONTENT_LENGTH, newFileID);
//                 Seek(MAX_CONTENT_LENGTH, newFileID);
//                 Write(content2, MAX_CONTENT_LENGTH, newFileID);
//                 PrintString("Successfully concatenated two files. Result is put into the new file.");

//                 Close(newFileID);
//             }
//         }
//     }
//     else
//     {
//         PrintString("Either of the file is unavailable, please check if the file name is correctly spelled.");
//     }

//     Close(fileID1);

//     Close(fileID2);

//     Halt();
// }

#include "syscall.h"

int main()
{
    int Id1;
    int Id2;
    char c;
    int size;
    int i;
    char name1[32];
    char name2[32];
    int len1;
    int len2;
    int eof;
    PrintString("Nh???p ????? d??i c???a t??n file 1 :  ");
    len1 = ReadNum();
    PrintString("Nh???p t??n file 1 : ");
    ReadString(name1, len1);
    ReadChar();
    PrintString("Nh???p ????? d??i c???a t??n file 2 :  ");
    len2 = ReadNum();
    PrintString("Nh???p t??n file 2 : ");
    ReadString(name2, len2);
    Id1 = Open(name1); // m??? file 1 ????? ghi
    if (Id1 != -1) // ki???m tra file 1 m??? th??nh c??ng ch??a
    {
        Id2 = Open(name2); // m??? file 2
        if (Id2 != -1) // ki???m tra file 2 m??? th??nh c??ng ch??a
        {
            size = Seek(-1, Id2); // l???y ????? d??i file c???n ghi
            Seek(0, Id2); // Seek ?????n ?????u file 2 ????? ti???n h??nh ?????c
            Seek(-1,Id1); // Seek ?????n cu???i file 1 ????? ti???n h??nh ghi n???i file 2 v??o file 1
            for (i = 0; i < size; i++)
            {
                Read(&c, 1, Id2); //?????c k?? t??? file 1
                Write(&c, 1, Id1); //ghi k?? t??? v??o file 2
            }
            PrintString("\nTHANH CONG !!!!!\n");
            Close(Id2);
        }
        else
            PrintString("\nKh??ng m??? ???????c file 2\n");
        Close(Id1);
    }
    else
        PrintString("\nKh??ng m??? ???????c file 1\n");
    Halt();
    return 0;
}