// ============================================================
// F2 · Bước 3: Stack. Hộp tự có khi gọi hàm, tự mất khi hàm xong.
// Biên dịch: cc -Wall app/02-stack.c -o app/stack
// Chạy:      ./app/stack          (thí nghiệm 1 và 2)
//            ./app/stack dao      (thêm thí nghiệm 3: đào stack tới khi sập)

// Stack là chồng khay: gọi hàm thì đặt khay, hàm xong thì nhấc khay. Nhanh, vì không cần dọn dẹp gì. Nhưng nhỏ (khoảng 8 MB) và ngắn hạn: biến cục bộ chết ngay khi hàm xong, nên không bao giờ được giữ địa chỉ của nó sau đó.
// ============================================================

#include <stdio.h> // stdio = standard input/output: printf, fflush

// Thí nghiệm 1 khay trồng lên nhau
void level2(void)
{
    int b = 2;                                  // biến cục bộ của level2
    printf("    level2: b ở %p\n", (void *)&b); // %p = pointer: in địa chỉ
}

void level1(void)
{
    int a = 1; // biến cục bộ của level1
    printf("  level1: a ở %p\n", (void *)&a);
    level2(); // gọi tiếp: đặt thêm một khay
}

// Thí nghiệm 2: giữ địa chỉ của khay đã bị nhấc đi
// int * = hàm này trả về một địa chỉ (tờ giấy ghi số nhà).
int *bad(void)
{
    int temp = 1234;   // temp nằm trong khay của bad
    int *addr = &temp; // ghi số nhà của temp ra tờ giấy
    return addr;       // trả tờ giấy về... nhưng khay của bad sắp bị nhấc đi!
}

void other(void)
{
    int junk = 9999; // một hàm khác, có biến cục bộ ở đúng vị trí tương tự
    int *addr = &junk;
    (void)addr; // (void) = "cố ý không dùng" để compiler không cảnh báo
}

// thí nghiệm 3: chồng khay quá cao
int depth = 0; // biến toàn cục: đếm đã chồng bao nhiêu khay

void dig(void)
{
    char tray[1024]; // mỗi khay chứa 1024 hộp (1 KB)
    tray[0] = 1;
    depth++;

    if (depth % 1000 == 0)
    {
        printf("  đã chồng %d khay\n", depth);
        fflush(stdout); // fflush = flush (đẩy ra ngay): in liền, không đợi
    }

    dig(); // hàm tự gọi lại chính nó (recursion) không bao giờ dừng
    tray[1] = tray[0];
}

// argc = argument count: có bao nhiêu chữ trên dòng lệnh
// argv = argument vector: danh sách các chữ đó (hôm nay chỉ cần biết: gõ thêm chữ thì argc > 1)
int main(int argc, char **argv)
{
    (void)argc;
    int m = 0;
    printf("Thí nghiệm 1\nmain: m ở %p\n", (void *)&m);
    level1();

    printf("\nThí nghiệm 2\n");
    int *p = bad();  // p giữ số nhà của temp, trong khi khay ĐÃ BỊ NHẤC ĐI
    int before = *p; // đọc ngay
    other();         // gọi 1 hàm khác: đặt khay mới lên đúng chỗ cũ
    int after = *p;  // đọc lại cùng số nhà đó
    printf("before = %d, after = %d\n", before, after);

    if (argc > 1)
    {
        printf("\nThí nghiệm 3: đào stack...\n");
        dig();
    }

    return 0;
}
