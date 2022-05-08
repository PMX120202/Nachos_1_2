

#include "syscall.h"

int main()
{
	int Id_Nguon;
	int Id_Dich;
	//Do dai file
	int fileSize;
	int i;
	char c;
	char nameNguon[32];
	char nameDich[32];
    int lenNguon;
    int lenDich;
	


	PrintString("Nhập độ dài của tên file nguồn :  ");
    lenNguon = ReadNum();
    PrintString("Nhập tên file nguồn : ");
    ReadString(nameNguon, lenNguon);
    ReadChar();
	PrintString("Nhập độ dài của tên file đích :  ");
    lenDich = ReadNum();
    PrintString("Nhập tên file đích : ");
    ReadString(nameDich, lenDich);
	Id_Nguon = Open(nameNguon); // mở file nguồn
	
	if (Id_Nguon != -1) // kiểm tra mở file thành công chưa
	{
		//Tạo file đích
		Id_Dich = Create(nameDich);
		Close(Id_Dich);
		
		Id_Dich = Open(nameDich); // mở file đích
		if (Id_Dich != -1) // kiểm tra mở file đích thành công chưa
		{
			//Seek đến cuối file nguồn để lấy độ nội dung file nguồn
			fileSize = Seek(-1, Id_Nguon);

			//Chuan bi sao chep
			Seek(0, Id_Nguon); // Seek đến đầu file nguồn để tiến hành đọc
			Seek(0, Id_Dich); // Seek đến đầu file đích để tiền hành ghi
			for (i = 0; i < fileSize; i++) 
			{
				Read(&c, 1, Id_Nguon); //Đọc ký tự file nguồn
				Write(&c, 1, Id_Dich); //Ghi ký tự vào file đích
			}
			
		PrintString("\n-----------ĐÃ COPY NỘI DUNG CỦA FILE-------------\n");
			Close(Id_Dich);
		}
		else
			PrintString("Không tạo được file đích!!!\n\n");
		Close(Id_Nguon);
	}
	else
		PrintString("Không mở được file nguồn");
	Halt();
	return 0;
}
