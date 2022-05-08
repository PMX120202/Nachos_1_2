// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
#include "filesys.h"
#define SlotSize 10
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:

char *User2System(int virtAddr, int limit)
{
	int i; // index
	int oneChar;
	char *kernelBuf = NULL;

	kernelBuf = new char[limit + 1]; // need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
	memset(kernelBuf, 0, limit + 1);

	// printf("\n Filename u2s:");
	for (i = 0; i < limit; i++)// vong lap doc dung ki tu vo vung nho
	{
		kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		// printf("%c",kernelBuf[i]);
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}

int System2User(int virtAddr, int len, char *buffer)// luu dl ra ben ngoai vung nho 
{
	if (len < 0)
		return -1;
	if (len == 0)
		return len;
	int i = 0;
	int oneChar = 0;
	do
	{//ghi tung ki ra vung nho
		oneChar = (int)buffer[i];
		kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}

// Câu 2, tăng giá trị biến Program Counter
void IncreasePC()
{//4 byte nap cho lenh ke tiep
	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
	kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(NextPCReg));
	kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(NextPCReg) + 4);
}

void Handle_ReadNum()
{
	int readResult = SysReadNum();
	kernel->machine->WriteRegister(2, readResult);
	IncreasePC();
}

void Handle_PrintNum()
{
	int character = kernel->machine->ReadRegister(4);
	SysPrintNum(character);
	IncreasePC();
}

void Handle_ReadChar()
{
	kernel->machine->WriteRegister(2, (int)SysReadChar());
	IncreasePC();
}

void Handle_PrintChar()
{
	char ch = (char)kernel->machine->ReadRegister(4);
	SysPrintChar(ch);
	IncreasePC();
}

void Handle_RandomNum()
{
	SysRandomNum();
	IncreasePC();
}

void Handle_ReadString()
{
	int virtAddr ,length ;
	virtAddr = kernel->machine->ReadRegister(4);
	length = kernel->machine->ReadRegister(5);
	char *buffer ;
	buffer = User2System(virtAddr, length);
	for (int i = 0; i < length; i++)
		buffer[i] = kernel->InputChar();
	System2User(virtAddr, length, buffer);
	IncreasePC();
}

void Handle_PrintString()
{
	int virtAddr ,length = 0;
	virtAddr = kernel->machine->ReadRegister(4);
	char *buffer ;
	buffer = User2System(virtAddr, 255);
	while (buffer[length] != 0)
		length++; //đếm độ dài của chuỗi
	for (int i = 0; i < length; i++)
		kernel->OutputChar(buffer[i]);
	// SysPrintString(buffer);
	delete buffer;
	IncreasePC();
}
//-----------------------------------------------------------------------------------





//----------------------------------------------------------------
void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);
	// Câu 1, sửa file exception.cc để xử lý tất cả các exceptions được liệt kê trong machine/machine.h
	DEBUG(dbgSys, "Received Exception " << which << " type : " << type << "\n");
	switch (which)
	{
	case NoException: // Everything ok!
	{
		// trả lại quyền điều khiển cho hệ điều hành
		DEBUG(dbgSys, "\nSwitch to system mode\n");

		kernel->interrupt->setStatus(SystemMode);
		return;
	}
	case SyscallException: // A program executed a system call.
	{
		switch (type)
		{
		case SC_Halt:
		{
			DEBUG(dbgSys, "\nShutdown: initiated by user program.\n");
			SysHalt();
			ASSERTNOTREACHED();
			break;
		}
		case SC_ReadNum:
		{
			Handle_ReadNum();
			break;
		}
		case SC_PrintNum:
		{
			Handle_PrintNum();
			break;
		}
		case SC_ReadChar:
		{
			Handle_ReadChar();
			break;
		}
		case SC_PrintChar:
		{
			Handle_PrintChar();
			break;
		}
		case SC_RandomNum:
		{
			Handle_RandomNum();
			break;
		}
		case SC_ReadString:
		{
			Handle_ReadString();
			break;
		}
		case SC_PrintString:
		{
			Handle_PrintString();
			break;
		}

		case SC_Create: 
		{ 
			// input: arg1 as the file name address
			// output: successful: 0, failed: -1
			// function: create a empty file with given file name from the user.
			// get string address from r4
			int strAddr = (int)kernel->machine->ReadRegister(4);
			// get the string from User memory space to System memory space
			char *kernelBuffer = User2System(strAddr, 33);

			if (strlen(kernelBuffer) == 0)
			{
				DEBUG(dbgSys, "Ten file khong hop le!\n");
				kernel->machine->WriteRegister(2, -1);
			}
			else
			{
				if (kernelBuffer != NULL)
				{
					if (!kernel->fileSystem->Create(kernelBuffer,0)) //tra ve true neu tao thanh cong
					{
						DEBUG(dbgSys, "tao file that bai!\n");
						kernel->machine->WriteRegister(2, -1);
					}
					else
						kernel->machine->WriteRegister(2, 0);// thanh cong
				}
				else
				{
					DEBUG(dbgSys, "Loi bo nho day.(Khong du bo nho!\n");
					kernel->machine->WriteRegister(2, -1);
				}
			}
			delete kernelBuffer;
			IncreasePC();
			return;
			ASSERTNOTREACHED();
		
		break;
		}
		case SC_Read:
		{
			int virtAddr = kernel->machine->ReadRegister(4);//lay dia chi buff tu thanh ghi so 5
			int charcount = kernel->machine->ReadRegister(5);
			int id = kernel->machine->ReadRegister(6);// lay id cua file tu thanh ghi so 6
			int OldPos;
			int NewPos;
			char *buf;
			//neu trung vs cac dk o duoi day thi tra ve -1
			if (id < 0 || id > MAX_FILE || kernel->fileSystem->ListFile[id] == NULL || kernel->fileSystem->ListFile[id]->type == 3) 
			{
				DEBUG(dbgSys, "Error\n");
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
				break;
			}
			OldPos = kernel->fileSystem->ListFile[id]->GetCurrentPos();
			buf = User2System(virtAddr, charcount); 
			if (kernel->fileSystem->ListFile[id]->type == 2)
			{				
				int size = 0;
				for (int i = 0; i < charcount; i++)
				{
					if (kernel->synchConsoleIn->GetChar() != '\0')
						size++;
					else
						break;
				}
				System2User(virtAddr, size, buf); //copy chuoi tu sys space sang user space vs bo dem buff va so file thuc
				kernel->machine->WriteRegister(2, size);//tra ve so byte doc dc
				delete buf;
				IncreasePC();
				return;
			}
			if ((kernel->fileSystem->ListFile[id]->Read(buf, charcount)) > 0)
			{
				NewPos = kernel->fileSystem->ListFile[id]->GetCurrentPos();
				System2User(virtAddr, NewPos - OldPos, buf); 
				kernel->machine->WriteRegister(2, NewPos - OldPos);//ghi ket qua vao thanh ghi thu 2
			}
			else
			{
				kernel->machine->WriteRegister(2, -2);//file rong 
			}
			delete buf;
			IncreasePC();
			return;
			break;
		}
		case SC_Close:
		{
			int fid = kernel->machine->ReadRegister(4);
			if (fid >= 2 && fid <= MAX_FILE)// chi xu li trong 
			{
				if (kernel->fileSystem->ListFile[fid])// mo file thanh cong 
				{
					delete kernel->fileSystem->ListFile[fid];//xoa vung nho luu tru file
					kernel->fileSystem->ListFile[fid] = NULL;// gan bnag null
					kernel->machine->WriteRegister(2, 0);//thanh cong 
					IncreasePC();
					return;
					break;
				}
			}
			kernel->machine->WriteRegister(2, -1);
			IncreasePC();
			return;
			break;
		}
		case SC_Open:
		{
			int virtual_add = kernel->machine->ReadRegister(4);	   // đọc thanh ghi số 4 để lấy ra địa chỉ , xong từ địa chỉ ->biết chuối
			char *namefile = User2System(virtual_add, 32); // hàm copy chuỗi từ user sang kernel
			// Bản chất của việc file : hệ thống sẽ cũng 1 bảng chứa những file mở (bảng file)
			//  Mục tiêu code 1 bảng chứa toàn bộ những file mình muốn mở .
			//  Mình muốn mở file mình sẽ thư viện hỗ trợ của hệ điều hành Linux
			//  bảng chứa file , mình tìm ra vị trí còn trống để nó có thể mở file
			int freeSlot = kernel->fileSystem->FindFreeSlot();
			if (freeSlot == -1)
			{
				DEBUG(dbgSys, "\nBo nho day ");
				cout << "Bo nho day " << endl;
				kernel->machine->WriteRegister(2, -1);
				delete[] namefile;
				IncreasePC();
				break;
			}
			kernel->fileSystem->ListFile[freeSlot] = kernel->fileSystem->Open(namefile);
			if (!kernel->fileSystem->ListFile[freeSlot])
			{
				DEBUG(dbgSys, "\nKhong ton tai file");
				cout << "Khong ton tai file" << endl;
				kernel->machine->WriteRegister(2, -1);
			}
			else
			{
				DEBUG(dbgSys, "\nMo file thanh cong");
				//cout << "Mo file thanh cong " << endl;
				kernel->machine->WriteRegister(2, freeSlot);
			}
			delete[] namefile;
			IncreasePC();
			return;
		}
		case SC_Write:
		{
			int virtAddr = kernel->machine->ReadRegister(4);
			int charcount = kernel->machine->ReadRegister(5);
			int id = kernel->machine->ReadRegister(6);
			int OldPos, NewPos;
			char *buf;
			//check dk 
			if (id < 0 || id > MAX_FILE || kernel->fileSystem->ListFile[id] == NULL || kernel->fileSystem->ListFile[id]->type == 1 || kernel->fileSystem->ListFile[id]->type == 2)
			{
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			OldPos = kernel->fileSystem->ListFile[id]->GetCurrentPos(); //check thanh cong thi lay vitri OldP
			buf = User2System(virtAddr, charcount);
			if (kernel->fileSystem->ListFile[id]->type == 0)
			{
				if ((kernel->fileSystem->ListFile[id]->Write(buf, charcount)) > 0)
				{
					NewPos = kernel->fileSystem->ListFile[id]->GetCurrentPos();
					kernel->machine->WriteRegister(2, NewPos - OldPos);
					delete buf;
					IncreasePC();
					return;
				}
			}
			if (kernel->fileSystem->ListFile[id]->type == 3)
			{
				int i = 0;
				while (buf[i] != 0 && buf[i] != '\n')//lap den khi nao gap ki tu
				{
					kernel->synchConsoleOut->PutChar(buf[i]);
					i++;
				}
				buf[i] = '\n';
				kernel->synchConsoleOut->PutChar(buf[i]);//ghi ki tu
				kernel->machine->WriteRegister(2, i - 1);// tra so byte ve thannh ghi so 2
				delete buf;
				IncreasePC();
				return;
			}
			IncreasePC();
			return;
			break;
			
		}
		case SC_Remove:
		{
			int virtAddr = kernel->machine->ReadRegister(4);
			char *filename = User2System(virtAddr, MaxFileLength);
			if (kernel->fileSystem->Remove(filename) == -1)// ko ron tai file
			{
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			else
			{
				kernel->machine->WriteRegister(2, 0);
				IncreasePC();
				return;
			}
			delete filename;
			break;	
		}		
		case SC_Seek:
		{
			int pos = kernel->machine->ReadRegister(4);
			int id = kernel->machine->ReadRegister(5);
			if (id < 0 || id > MAX_FILE || kernel->fileSystem->ListFile[id] == NULL)//dk ngoai khoang xet
			{
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			pos = (pos == -1) ? kernel->fileSystem->ListFile[id]->Length() : pos;
			if (pos > kernel->fileSystem->ListFile[id]->Length() || pos < 0) //pos > chieu dai file or pos < 0 thi sai
			{
				kernel->machine->WriteRegister(2, -1);
			}
			else
			{
				kernel->fileSystem->ListFile[id]->Seek(pos);
				kernel->machine->WriteRegister(2, pos);//ghi vi tri vao thanh ghi so 2 
			}
			IncreasePC();
			return;
			break;
		}


		//---------------------------
		default:
		{
			SysHalt();
			break;
		}
		}
		break;
	}
	case PageFaultException: // No valid translation found
	{
		DEBUG('a', "\nNo valid translation found");
		printf("\nNo valid translation found");
		SysHalt();
		break;
	}
	case ReadOnlyException: // Write attempted to page marked "read-only"
	{
		DEBUG('a', "\nWrite attempted to page marked read-only");
		printf("\nWrite attempted to page marked read-only");
		SysHalt();
	}
	case BusErrorException: // Translation resulted in an invalid physical address
	{
		DEBUG('a', "\nTranslation resulted in an invalid physical address");
		printf("\nTranslation resulted in an invalid physical address");
		SysHalt();
		break;
	}
	case AddressErrorException: // Unaligned reference or one that was beyond the end of the address space
	{
		DEBUG('a', "Unaligned reference or one that was beyond the end of the address space");
		printf("\nUnaligned reference or one that was beyond the end of the address space");
		SysHalt();
		break;
	}
	case OverflowException: // Integer overflow in add or sub.
	{
		DEBUG('a', "\nInteger overflow in add or sub.");
		printf("\nInteger overflow in add or sub.");
		SysHalt();
		break;
	}
	case IllegalInstrException: // Unimplemented or reserved instr.
	{
		DEBUG('a', "Unimplemented or reserved instr.");
		printf("\nUnimplemented or reserved instr.");
		SysHalt();
		break;
	}
	case NumExceptionTypes:
	{
		DEBUG('a', "Number exception types");
		printf("\nNumber exception types");
		SysHalt();
		break;
	}
	}
}