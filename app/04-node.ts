// ============================================================
// F4 · Phần B: Cùng event loop đó, nhưng là Node.js. Và cách làm nó sập.
// Chạy: node app/04-node.ts
// ============================================================

import http from 'node:http'; // module HTTP có sẵn của Node: bên dưới là libuv + kqueue

const server = http.createServer((req, res) => {
  // Hàm này được event loop gọi mỗi khi có một request ĐÃ ĐỌC XONG.
  // Không có accept(), không có read(), không có vòng for(;;): Node làm hộ hết.

  if (req.url === '/slow') {
    // Cố ý làm việc NẶNG cho CPU trong 3 giây: đứng nấu phở ngay giữa quán.
    const end = Date.now() + 3000;
    while (Date.now() < end) {
      // vòng lặp rỗng: chiếm 100% luồng JavaScript
    }
    res.end('slow: xong sau 3 giây\n');
    return;
  }

  res.end('fast: trả lời ngay\n');
});

server.listen(4003, () => console.log('Node đang nghe ở http://localhost:4003'));
