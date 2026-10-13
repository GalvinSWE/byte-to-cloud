// ============================================================
// F2 · Bước 1: Địa chỉ và pointer
// Biên dịch: cc -Wall app/02-pointer.c -o app/pointer
// Chạy:      ./app/pointer
// Mỗi hộp có một số nhà (địa chỉ). Pointer là một tờ giấy ghi số nhà, và bản thân nó cũng chỉ là 8 hộp chứa số. &x = hỏi số nhà của x. *p = đi tới căn nhà ghi trên tờ giấy p.
// ============================================================

#include <stdio.h> // stdio = standard input/output: có printf

int main(void)
{
    int x = 1000; // xin 4 hộp, bỏ số 1000 vào

    printf("x chứa:        %d\n", x);          // %d = decimal: in giá trị (đồ trong nhà)
    printf("x ở địa chỉ:   %p\n", (void *)&x); // & = "địa chỉ của". %p = pointer: in số nhà

    int *p = &x; // p = tờ giấy ghi số nhà của x

    printf("p đang giữ:    %p\n", (void *)p);      // trên tờ giấy ghi số nào?
    printf("p chiếm:       %zu byte\n", sizeof p); // tờ giấy to bao nhiêu hộp?

    *p = 5;                           // * = "đi tới nhà ở địa chỉ p", bỏ số 5 vào đó
    printf("x bây giờ:     %d\n", x); // không đụng vào x... x có đổi không?

    return 0; // 0 = báo hệ điều hành: chạy thành công
}