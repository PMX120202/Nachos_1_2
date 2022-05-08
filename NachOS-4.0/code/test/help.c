#include "syscall.h"

int main()
{
    PrintString("Thông tin nhóm : \n20127395 - Phan Minh Xuan\n20127448 - Nguyen Thai Bao\n20127444 - Bui Duy Bao\n");
    PrintString("ASCII : chương trình sẽ in ra bảng mã ASCII (chỉ những ký tự đọc được (từ 32 đến 126) )\n");
    PrintString("SORT : \t- Nhập n là kích thước của mảng (0 < n <= 100) (nhập sai sẽ nhập lại)\n\t- Nhập n phần tử của mảng\n\t- Chọn sắp xếp tăng hoặc giảm (1-tăng 2-giảm)\n\t- In ra mảng đã được sắp xếp\n");
    Halt();
}