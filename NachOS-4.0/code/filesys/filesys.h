//-----------------------------------------------------------------------------

// filesys.h
//	Data structures to represent the Nachos file system.
//
//	A file system is a set of files stored on disk, organized
//	into directories.  Operations on the file system have to
//	do with "naming" -- creating, opening, and deleting files,
//	given a textual file name.  Operations on an individual
//	"open" file (read, write, close) are to be found in the OpenFile
//	class (openfile.h).
//
//	We define two separate implementations of the file system.
//	The "STUB" version just re-defines the Nachos file system
//	operations as operations on the native UNIX file system on the machine
//	running the Nachos simulation.
//
//	The other version is a "real" file system, built on top of
//	a disk simulator.  The disk is simulated using the native UNIX
//	file system (in a file named "DISK").
//
//	In the "real" implementation, there are two key data structures used
//	in the file system.  There is a single "root" directory, listing
//	all of the files in the file system; unlike UNIX, the baseline
//	system does not provide a hierarchical directory structure.
//	In addition, there is a bitmap for allocating
//	disk sectors.  Both the root directory and the bitmap are themselves
//	stored as files in the Nachos file system -- this causes an interesting
//	bootstrap problem when the simulated disk is initialized.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef FS_H
#define FS_H
#define MAX_FILE 10
#include "copyright.h"
#include "sysdep.h"
#include "openfile.h"

#ifdef FILESYS_STUB // Temporarily implement file system calls as
					// calls to UNIX, until the real file system
					// implementation is available
class FileSystem
{
public:
	OpenFile **ListFile; // Thành ra 1 cái mảng chứa địa chỉ của những tên file hiện có ( chekc  file co dag mo ko)
	// Dinh nghia lai ham khoi tao cua FileSystem
	FileSystem()
	{
		ListFile = new OpenFile *[15];
		for (int i = 0; i < 15; ++i)
		{
			ListFile[i] = NULL; // gán tất cả 15 ô vào giá trị null
		}
		this->Create("stdin", 0);
		this->Create("stdout", 0);
		ListFile[0] = this->Open("stdin");
		ListFile[1] = this->Open("stdout");
	}

	bool Create(char *name, int initialSize)
	{ // luc dau khong co tham so int initialisize
		int fileDescriptor = OpenForWrite(name);

		if (fileDescriptor == -1)
			return FALSE;
		Close(fileDescriptor);
		return TRUE;
	}

	OpenFile *Open(char *name)
	{
		int fileDescriptor = OpenForReadWrite(name, FALSE);

		if (fileDescriptor == -1)
			return NULL;
		return new OpenFile(fileDescriptor);
	}

	OpenFile *Open(char *name, int type)
	{
		int fileDescriptor = OpenForReadWrite(name, FALSE);

		if (fileDescriptor == -1)
			return NULL;
		// index++;
		return new OpenFile(fileDescriptor, type);
	}

	bool Remove(char *name) { return Unlink(name) == 0; }
	int FindFreeSlot()
	{
		for (int i = 2; i < 15; i++)
		{
			if (ListFile[i] == NULL)
				return i;
		}
		return -1;
	}

	// Ham huy doi tuong FileSystem
	~FileSystem()
	{
		for (int i = 0; i < 15; ++i)
		{
			if (ListFile[i] != NULL)
				delete ListFile[i];
		}
		delete[] ListFile;
	}
};

#else // FILESYS

class FileSystem
{
public:
	OpenFile **ListFile; // de luu gia tri mang file

	FileSystem(bool format); // Initialize the file system.
							 // Must be called *after* "synchDisk"
							 // has been initialized.
							 // If "format", there is nothing on
							 // the disk, so initialize the directory
							 // and the bitmap of free blocks.

	bool Create(char *name, int initialSize);
	// Create a file (UNIX creat)

	OpenFile *Open(char *name); // Open a file (UNIX open)

	bool Remove(char *name); // Delete a file (UNIX unlink)

	void List(); // List all the files in the file system

	void Print(); // List all the files and their contents
	
	int FindFreeSlot();
	OpenFile* Open(char *name, int type); //Mo file voi tham so type
	~FileSystem();

private:
	OpenFile *freeMapFile;	 // Bit map of free disk blocks,
							 // represented as a file
	OpenFile *directoryFile; // "Root" directory -- list of
							 // file names, represented as a file
};

#endif // FILESYS

#endif // FS_H