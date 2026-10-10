// ============================================================
// Ngày 1: Mọi thứ là byte.
// Biên dịch: cc -Wall app/01-bytes.c -o app/bytes
//   cc    = C compiler (trình biên dịch C)
//   -Wall = Warnings: all (bật mọi cảnh báo)
//   -o    = output (tên file chương trình tạo ra)
// Chạy:      ./app/bytes
// ============================================================

// --- PHẦN 1: Mượn code có sẵn --------------------------------
// #include : include = "kèm vào". Dán nội dung file .h vào đây trước khi biên dịch.
//            Dấu # = lệnh cho bộ tiền xử lý (preprocessor), chạy trước compiler.
// .h       : header = phần đầu. File chỉ chứa KHAI BÁO: tên hàm, nhận gì, trả gì.
#include <stdio.h>  // stdio  = standard input/output (nhập/xuất chuẩn): có printf
#include <string.h> // string = chuỗi ký tự: có strlen, memcpy
#include <stdint.h> // stdint = standard integer (số nguyên chuẩn): có uint8_t, int32_t

// --- PHẦN 2: Tự viết một hàm tên là dump ---------------------
// dump = "đổ ra" (memory dump): in toàn bộ byte của một vùng nhớ.
// Việc của hàm: nhận ĐỊA CHỈ của một vùng nhớ + SỐ BYTE, rồi in từng byte dạng hex.
//
// void   : "trống rỗng". Hàm này không trả về giá trị nào.
// const  : constant (hằng). Không được sửa thứ này bên trong hàm.
// char   : character (ký tự), 1 byte.
// *      : "là địa chỉ của". const char *label = label là địa chỉ của một chuỗi chữ.
// label  : nhãn để in ra.
// p      : pointer (con trỏ) = một địa chỉ. void * = địa chỉ chưa biết trỏ tới kiểu gì.
// size_t : size type (kiểu dùng cho kích thước). _t luôn nghĩa là "type".
// n      : number, số byte cần in.
void dump(const char *label, const void *p, size_t n)
{
    // unsigned = không dấu (giá trị 0..255 cho 1 byte).
    // b = bytes: nhìn địa chỉ p như địa chỉ của một dãy byte. Không byte nào bị thay đổi.
    const unsigned char *b = p;

    // printf = print formatted (in theo khuôn mẫu).
    //   %-14s : s = string. In chuỗi label, dấu - là căn trái, chiếm 14 cột.
    //   %2zu  : z = dành cho size_t, u = unsigned. In số n, chiếm 2 cột.
    printf("%-14s %2zu byte", label, n);

    // for = "với mỗi giá trị của i".
    //   size_t i = 0 : i = index (chỉ số), bắt đầu từ 0.
    //   i < n        : còn lặp khi i nhỏ hơn n.
    //   i++          : sau mỗi vòng, tăng i thêm 1.
    for (size_t i = 0; i < n; i++)
    {
        // b[i] = byte thứ i tính từ địa chỉ b.
        // %02X : X = hexadecimal (hệ 16) viết hoa; 2 = rộng 2 chữ số; 0 = thêm số 0 ở đầu (8 -> 08).
        printf(" %02X", b[i]);
        // printf(" %3u", b[i]);
    }

    printf("\n"); // \n = new line (xuống dòng), là byte 0A.
};

// --- PHẦN 3: main, nơi chương trình bắt đầu chạy -------------
// main : "chính". Hệ điều hành luôn gọi hàm main đầu tiên.
// int  : integer (số nguyên). main trả về một số cho hệ điều hành (exit code).
// (void) : main không nhận tham số nào.
int main(void)
{
    // Mỗi dòng: KIỂU TÊN = GIÁ TRỊ;
    // Kiểu quyết định: xin bao nhiêu byte, và đọc byte đó theo quy ước nào.

    // uint8_t = unsigned integer, 8 bit, type. Xin 1 byte, số không dấu (0..255).
    uint8_t small = 200;

    // int32_t = integer, 32 bit, type. Xin 4 byte, số có dấu.
    int32_t num = 1000;
    int32_t neg = -1; // 4 byte, số âm.

    // char = character. Nháy ĐƠN 'A' = một ký tự, 1 byte.
    char letter = 'A';

    // double = double precision (độ chính xác gấp đôi float). Xin 8 byte, số thực.
    double money = 0.1;

    // Nháy KÉP "..." = chuỗi. name là ĐỊA CHỈ của chữ 'N' đầu tiên.
    const char *name = "Nguyễn";

    // &small        : & = "địa chỉ của". Biến small nằm ở đâu trong RAM.
    // sizeof small  : size of = "kích thước của". small chiếm bao nhiêu byte (compiler tính sẵn).
    dump("uint8_t 200", &small, sizeof small);
    dump("int32_t 1000", &num, sizeof num);
    dump("int32_t -1", &neg, sizeof neg);
    dump("char 'A'", &letter, sizeof letter);
    dump("double 0.1", &money, sizeof money);

    // name đã là địa chỉ rồi, nên không cần dấu &.
    // strlen = string length (độ dài chuỗi): đếm số BYTE cho tới khi gặp byte 00 ở cuối chuỗi.
    dump("Nguyen (UTF-8)", name, strlen(name));

    // --- Thí nghiệm: cùng 4 byte, hai cách đọc ---
    // raw = thô (byte chưa được gán nghĩa). [4] = mảng 4 byte liền nhau.
    // 0x41 : x = hexadecimal. 0x41 là số 65 viết theo hệ 16.
    unsigned char raw[4] = {0x41, 0x42, 0x43, 0x44};

    int32_t as_int; // Xin 4 byte cho một số nguyên, chưa gán giá trị.
    // memcpy = memory copy (chép bộ nhớ): chép NGUYÊN 4 byte từ raw sang as_int, không đổi gì.
    memcpy(&as_int, raw, 4);

    // %d   : d = decimal (hệ 10). Đọc 4 byte đó thành số nguyên có dấu.
    // %.4s : s = string. Đọc 4 byte đó thành chữ, .4 = chỉ lấy 4 byte.
    // (char *)raw : cast (ép kiểu). "Coi địa chỉ raw là địa chỉ của chữ".
    printf("\n41 42 43 44 đọc thành số: %d\n", as_int);
    printf("41 42 43 44 đọc thành chữ: %.4s\n", (char *)raw);

    // --- Thí nghiệm: tràn số (overflow) ---
    // 200 + 100 = 300, nhưng small chỉ có 1 byte (0..255).
    small = small + 100;
    printf("\nuint8_t: 200 + 100 = %u\n", small); // %u : u = unsigned (không dấu).

    // return = trả về. 0 = báo hệ điều hành: chạy thành công.
    return 0;
}