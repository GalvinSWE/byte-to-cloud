// ============================================================
// F3 · Phần A: Server C dùng blocking I/O. Một khách chậm làm treo tất cả.
// Biên dịch: cc -Wall app/03-server.c -o app/server
// Chạy:      ./app/server
// ============================================================

#include <stdio.h>       // printf, fflush
#include <string.h>      // strlen
#include <unistd.h>      // unistd = UNIX standard: read, write, close
#include <arpa/inet.h>   // htons, htonl: đổi số sang thứ tự byte của network
#include <sys/socket.h>  // socket, bind, listen, accept

int main(void) {
  // socket: xin kernel một "ổ cắm" mạng TCP. Kernel trả về một con số: fd (file descriptor).
  int server = socket(AF_INET, SOCK_STREAM, 0);   // AF_INET = IPv4, SOCK_STREAM = TCP
  int yes = 1;
  setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);  // cho dùng lại cổng ngay khi restart

  struct sockaddr_in addr = {0};                  // địa chỉ để lắng nghe
  addr.sin_family = AF_INET;
  addr.sin_port = htons(4001);                    // cổng 4001. htons = host to network short
  addr.sin_addr.s_addr = htonl(INADDR_ANY);       // nghe trên mọi địa chỉ của máy
  bind(server, (struct sockaddr *)&addr, sizeof addr);  // bind = gắn ổ cắm vào cổng 4001
  listen(server, 16);                             // listen = bắt đầu nhận khách, tối đa 16 khách xếp hàng
  printf("Đang nghe ở http://localhost:4001\n");
  fflush(stdout);

  for (;;) {                                      // lặp mãi: phục vụ từng khách một
    int client = accept(server, NULL, NULL);      // CHẶN: đứng chờ tới khi có khách mới
    printf("Khách mới (fd = %d). Đang chờ khách nói...\n", client);
    fflush(stdout);

    char buf[4096];
    ssize_t n = read(client, buf, sizeof buf);    // CHẶN: đứng chờ tới khi khách gửi byte
    if (n <= 0) { close(client); continue; }      // khách bỏ đi mà không nói gì
    printf("Nhận %zd byte\n", n);

    const char *body = "Xin chào từ C!\n";
    char res[512];
    int len = snprintf(res, sizeof res,           // snprintf = ghi chữ vào một mảng, có giới hạn độ dài
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain; charset=utf-8\r\n"
      "Content-Length: %zu\r\n"                   // strlen đếm BYTE
      "Connection: close\r\n"
      "\r\n"
      "%s", strlen(body), body);
    write(client, res, len);                      // gửi câu trả lời
    close(client);                                // đóng kết nối, trả fd lại cho kernel
    fflush(stdout);
  }
}
