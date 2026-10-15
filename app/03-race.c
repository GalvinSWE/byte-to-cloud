// ============================================================
// F3 · Phần B: Race condition. Hai luồng cùng cộng tiền vào một tài khoản.
// Biên dịch: cc -Wall app/03-race.c -o app/race
// Chạy:      ./app/race          (không khoá)
//            ./app/race lock     (có khoá: mutex)
// ============================================================

#include <stdio.h>    // printf
#include <string.h>   // strcmp (string compare)
#include <pthread.h>  // pthread = POSIX threads: tạo luồng, mutex
#include <time.h>     // clock_gettime: đo thời gian

int balance = 0;      // tài khoản DÙNG CHUNG: biến toàn cục, mọi luồng đều thấy
int use_lock = 0;     // 1 = có khoá, 0 = không khoá

// mutex = mutual exclusion (loại trừ lẫn nhau): chiếc chìa khoá duy nhất của sổ quỹ
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Việc của MỘT luồng: nạp 1 đồng, 1 triệu lần.
// void *f(void *arg) = kiểu hàm mà pthread yêu cầu. Hôm nay không dùng arg.
void *deposit(void *arg) {
  (void)arg;
  for (int i = 0; i < 1000000; i++) {
    if (use_lock) pthread_mutex_lock(&lock);    // lấy chìa khoá (ai đang cầm thì phải đợi)
    balance = balance + 1;                      // ĐỌC balance, CỘNG 1, GHI lại: 3 bước
    if (use_lock) pthread_mutex_unlock(&lock);  // trả chìa khoá
  }
  return NULL;
}

int main(int argc, char **argv) {
  use_lock = (argc > 1 && strcmp(argv[1], "lock") == 0);

  struct timespec t0, t1;                       // timespec = mốc thời gian (giây + nano giây)
  clock_gettime(CLOCK_MONOTONIC, &t0);

  pthread_t a, b;                               // pthread_t = "mã số" của một luồng
  pthread_create(&a, NULL, deposit, NULL);      // tạo luồng A, cho nó chạy deposit
  pthread_create(&b, NULL, deposit, NULL);      // tạo luồng B, cũng chạy deposit
  pthread_join(a, NULL);                        // join = đợi luồng A làm xong
  pthread_join(b, NULL);                        // đợi luồng B làm xong

  clock_gettime(CLOCK_MONOTONIC, &t1);
  double ms = (t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_nsec - t0.tv_nsec) / 1e6;

  printf("%s: balance = %d (đúng phải là 2000000), mất %.0f ms\n",
         use_lock ? "Có khoá  " : "Không khoá", balance, ms);
  return 0;
}
