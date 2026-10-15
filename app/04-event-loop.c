// ============================================================
// F4 · Phần A: Event loop bằng C trên kqueue.
// Một người phục vụ, nhưng KHÔNG BAO GIỜ đứng chờ ở một bàn.
// Biên dịch: cc -Wall app/04-event-loop.c -o app/event-loop
// Chạy:      ./app/event-loop
// ============================================================

#include <stdio.h>       // printf, fflush
#include <string.h>      // strlen
#include <unistd.h>      // unistd = UNIX standard: read, write, close
#include <fcntl.h>       // fcntl = file control: bật chế độ non-blocking
#include <arpa/inet.h>   // htons, htonl
#include <sys/socket.h>  // socket, bind, listen, accept
#include <sys/event.h>   // kqueue, kevent: kernel báo "fd nào đang có việc"

// Đăng ký với kqueue: "báo cho tôi khi fd này có dữ liệu để đọc".
void watch(int kq, int fd) {
  struct kevent ev;                                      // kevent = kernel event: một yêu cầu theo dõi
  EV_SET(&ev, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);      // EVFILT_READ = theo dõi việc ĐỌC, EV_ADD = thêm vào danh sách
  kevent(kq, &ev, 1, NULL, 0, NULL);                     // gửi yêu cầu cho kernel
}

int main(void) {
  int server = socket(AF_INET, SOCK_STREAM, 0);
  int yes = 1;
  setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(4002);                           // cổng 4002
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  bind(server, (struct sockaddr *)&addr, sizeof addr);
  listen(server, 128);

  // kqueue = kernel queue: tạo một "bảng theo dõi". Kernel sẽ ghi vào đây những bàn đang giơ tay.
  int kq = kqueue();
  watch(kq, server);                                     // theo dõi: có khách mới vào quán chưa?
  printf("Event loop đang nghe ở http://localhost:4002\n");
  fflush(stdout);

  for (;;) {                                             // ĐÂY LÀ EVENT LOOP
    struct kevent ready[64];                             // danh sách tối đa 64 bàn đang giơ tay
    // CHỖ DUY NHẤT người phục vụ đứng chờ: "có bàn nào giơ tay chưa?"
    // Kernel trả về n = số bàn đang có việc. Không bàn nào có việc thì người phục vụ ngủ, không tốn CPU.
    int n = kevent(kq, NULL, 0, ready, 64, NULL);

    for (int i = 0; i < n; i++) {
      int fd = (int)ready[i].ident;                      // ident = fd của bàn đang giơ tay

      if (fd == server) {                                // khách mới vào quán
        int client = accept(server, NULL, NULL);         // chắc chắn có khách, nên accept không phải chờ
        fcntl(client, F_SETFL, O_NONBLOCK);              // O_NONBLOCK: read với khách này không bao giờ đứng chờ
        watch(kq, client);                               // ghi tên bàn mới vào bảng theo dõi
        printf("Khách mới fd = %d. Không đứng chờ, quay lại vòng lặp.\n", client);
      } else {                                           // một khách cũ vừa nói
        char buf[4096];
        ssize_t r = read(fd, buf, sizeof buf);           // chắc chắn có byte, nên read không phải chờ
        if (r <= 0) {                                    // khách bỏ đi
          printf("fd = %d rời đi.\n", fd);
          close(fd);                                     // close cũng tự xoá fd khỏi kqueue
          continue;
        }
        printf("fd = %d nói %zd byte, trả lời ngay.\n", fd, r);
        const char *body = "Xin chào từ event loop!\n";
        char res[512];
        int len = snprintf(res, sizeof res,
          "HTTP/1.1 200 OK\r\nContent-Type: text/plain; charset=utf-8\r\n"
          "Content-Length: %zu\r\nConnection: close\r\n\r\n%s", strlen(body), body);
        write(fd, res, len);
        close(fd);
      }
      fflush(stdout);
    }
  }
}
