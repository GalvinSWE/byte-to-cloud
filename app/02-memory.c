// ============================================================
// F2 · Phần 1: Memory. Các hộp nằm ở đâu, ai cấp, sống bao lâu?
// Biên dịch: cc -Wall app/02-memory.c -o app/memory
// Chạy:      ./app/memory
// ============================================================

#include <stdio.h>  // stdio = standard input/output: printf
#include <stdlib.h> // stdlib = standard library: malloc(malloc = memory allocation, nghĩa là cấp phát bộ nhớ (xin cấp một vùng nhớ).), free
#include <string.h> // string: strlen, strcpy
#include <stdint.h> // stdint = standard integer: int32_t

// Biến toàn cục (global): khai báo NGOÀI mọi hàm.
// Sống suốt từ lúc chương trình chạy tới lúc kết thúc. Nằm ở vùng DATA.
int global_count = 7;

// Một hàm nhỏ để xem biến cục bộ (local) của nó nằm ở đâu.
// call = lần gọi thứ mấy.
void inner(int call)
{
    int local_inner = call; // biến cục bộ: chỉ sống trong lúc hàm inner đang chạy

    // %p : p = pointer. In một địa chỉ (dạng hex).
    // (void *) : cast (ép kiểu) sang "địa chỉ chung", vì %p yêu cầu đúng kiểu này.
    printf("STACK  local_inner (lần gọi %d) %p\n", call, (void *)&local_inner);
}

// struct = structure (cấu trúc): gom nhiều trường thành MỘT khối byte liền nhau.
// Ở đây: 4 byte cho name, rồi ngay sau đó 4 byte cho balance (số dư).
struct account
{
    char name[4];    // 4 hộp cho tên
    int32_t balance; // 4 hộp cho số dư, nằm NGAY SAU name
};

int main(void)
{
    // // ---------- Thí nghiệm 1: mỗi loại biến nằm ở vùng nào ----------
    // int local_main = 1;               // biến cục bộ của main: nằm trên STACK
    // const char *literal = "Xin chào"; // chuỗi viết sẵn trong code: vùng chỉ đọc, cạnh CODE

    // // malloc = memory allocate (xin bộ nhớ). Xin sizeof(int) = 4 byte trên HEAP.
    // // malloc trả về ĐỊA CHỈ của vùng vừa xin. int * = "địa chỉ của một số nguyên".
    // int *heap_a = malloc(sizeof(int));
    // *heap_a = 42; // *heap_a = "cái hộp ở địa chỉ heap_a". Ghi số 42 vào đó.

    // printf("CODE   hàm main                 %p\n", (void *)main); // chính hàm main cũng có địa chỉ
    // printf("CODE   chuỗi \"Xin chào\"         %p\n", (void *)literal);
    // printf("DATA   global_count             %p\n", (void *)&global_count);
    // printf("HEAP   heap_a                   %p\n", (void *)heap_a);
    // printf("STACK  local_main               %p\n", (void *)&local_main);

    // ---------- Thí nghiệm 2: stack được dùng lại ----------
    inner(1);
    inner(2);
};
