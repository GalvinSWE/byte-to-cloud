// ============================================================
// F2 · Tư duy ngược: 6 cách phá hỏng bộ nhớ
// Biên dịch: cc -Wall app/02-break.c -o app/break
// Chạy từng cách phá, ví dụ: ./app/break 1
// ============================================================

#include <stdio.h>   // printf, fflush
#include <stdlib.h>  // malloc (thuê tủ), free (trả tủ), atoi (chữ -> số)
#include <string.h>  // strcpy (chép chuỗi)
#include <stdint.h>  // int32_t

// ---- Cách 1: giữ tờ giấy ghi vị trí trên BÀN sau khi hàm xong ----
int *on_desk(void) {
  int item = 1234;           // đồ đặt trên bàn (stack) của hàm này
  int *note = &item;         // & = "địa chỉ của": ghi lại vị trí
  return note;               // hàm xong -> bàn bị dọn, nhưng tờ giấy vẫn được đưa ra
}
void someone_else(void) {
  int junk = 9999;           // hàm khác ngồi vào đúng cái bàn đó
  int *note = &junk;
  (void)note;
}

// ---- Cách 2: đặt đồ QUÁ TO lên bàn ----
void huge_on_desk(void) {
  int32_t big[10000000];     // 10 triệu x 4 byte = 40 MB, bàn chỉ khoảng 8 MB
  big[0] = 1;
  printf("đặt được %d\n", big[0]);
}

// ---- Cách 5: nhét đồ to hơn tủ, tràn sang tủ bên cạnh ----
struct account {
  char name[4];              // tủ 4 hộp cho tên
  int32_t balance;           // tủ 4 hộp cho số dư, nằm NGAY SAU name
};

int main(int argc, char **argv) {
  // atoi = ASCII to integer: đổi chữ "1" bạn gõ thành số 1
  int how = (argc > 1) ? atoi(argv[1]) : 0;

  if (how == 1) {            // DANGLING POINTER
    int *note = on_desk();
    someone_else();
    printf("Cách 1: đồ trên bàn giờ là %d (lúc đặt là 1234)\n", *note);
  }

  if (how == 2) {            // STACK OVERFLOW
    printf("Cách 2: đặt 40 MB lên bàn...\n");
    fflush(stdout);          // in ngay, không đợi
    huge_on_desk();
  }

  if (how == 3) {            // MEMORY LEAK
    for (int i = 1; i <= 10; i++) {
      int32_t *locker = malloc(40000000);   // thuê một tủ 40 MB
      memset(locker, 1, 40000000);          // memset = memory set: bỏ đồ đầy tủ
      // ... quên free(locker)
    }
    printf("Cách 3: đã thuê 10 tủ x 40 MB, không trả tủ nào\n");
  }

  if (how == 4) {            // USE-AFTER-FREE
    char *ticket_a = malloc(16);
    strcpy(ticket_a, "mat-khau-A");         // strcpy = string copy
    free(ticket_a);                         // trả tủ... nhưng vẫn giữ vé cũ
    char *ticket_b = malloc(16);            // người khác thuê, được đúng tủ đó
    strcpy(ticket_b, "du-lieu-B");
    printf("Cách 4: mở tủ bằng vé cũ thấy: %s\n", ticket_a);
    free(ticket_b);
  }

  if (how == 5) {            // BUFFER OVERFLOW
    struct account acc = {"Ann", 1000};
    const char *longname = "Nguyen";        // 6 chữ, tủ name chỉ có 4 hộp
    for (size_t i = 0; i < strlen(longname); i++) acc.name[i] = longname[i];
    printf("Cách 5: balance từ 1000 thành %d\n", acc.balance);
  }

  if (how == 6) {            // DOUBLE FREE
    char *ticket = malloc(16);
    free(ticket);
    printf("Cách 6: trả tủ lần 2...\n");
    fflush(stdout);
    free(ticket);                           // trả lại đúng tủ đó lần nữa
  }

  if (how == 0) printf("Gõ thêm số 1 tới 6, ví dụ: ./app/break 1\n");
  return 0;
}
