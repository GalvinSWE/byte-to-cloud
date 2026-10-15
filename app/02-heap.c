// ============================================================
// F2 · Bước 4: Heap. Hộp phải tự thuê, tự trả, nhưng sống bao lâu tuỳ bạn.
// Biên dịch: cc -Wall app/02-heap.c -o app/heap
// Chạy:      ./app/heap          (thí nghiệm 1: heap sống lâu hơn hàm, chứa được dữ liệu lớn)
//            ./app/heap stack    (thí nghiệm 2: thử đặt cùng dữ liệu đó trên stack)
//            ./app/heap leak     (thí nghiệm 3: thuê mà quên trả)
// ============================================================

#include <stdio.h>  // stdlib = standard library: malloc, free
#include <stdlib.h> // stdlib = standard library: malloc, free
#include <stdint.h> // stdint = standard integer: int32_t
#include <string.h> // string: strcmp (string compare = so sánh hai chuỗi)

#define N 10000000 // #define = đặt tên cho một hằng số. 10 triệu booking id

// Tạo danh sách n booking id trên HEAP, rồi trả về ĐỊA CHỈ của danh sách.
int32_t *make_ids(size_t n)
{
    // malloc = memory allocation: thuê n × 4 hộp trên heap.
    int32_t *ids = malloc(n * sizeof(int32_t));

    if (ids == NULL)
        return NULL; // NULL = "không có địa chỉ nào": kho hết chỗ

    for (size_t i; i < n; i++)
    {
        ids[i] = (int32_t)(i + 1); // ghi id 1, 2, 3, ... vào từng ô
    }

    return ids; // trả tờ giấy ghi số nhà. Vùng heap KHÔNG bị nhấc đi khi hàm xong.
}

// Thử đặt cùng 10 triệu id đó trên STACK (trong khay của hàm).
void on_stack(void)
{
    int32_t big[N]; // 10 triệu × 4 byte = 40 MB trong MỘT khay
    big[0] = 1;
    big[N - 1] = 2;
    printf("", big[0], big[N - 1]);
}

int main(int argc, char **argv)
{
    // argv[1] = chữ thứ nhất bạn gõ sau tên chương trình (ví dụ "stack" hoặc "leak").
    if (argc > 1 && strcmp(argv[1], "stack") == 0)
    {
        printf("Thí nghiệm 2: đặt 40 MB lên stack...\n");
        fflush(stdout);
        on_stack();
        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "leak" == 0))
    {
        printf("Thí nghiệm 3: thuê mà quên trả\n");
        for (int round = 0; round <= 10; round++)
        {
            int32_t *tmp = make_ids(N); // thuê 40 MB
            (void)tmp;                  // dùng xong... nhưng QUÊN free(tmp)
            printf("  vòng %2d: đang giữ khoảng %3d MB chưa trả\n", round, round * 40);
        }
        return 0;
    }

    printf("Thí nghiệm 1: 10 triệu id trên heap\n");
    int32_t *ids = make_ids(N); // hàm make_ids đã xong, khay của nó đã bị nhấc đi

    printf("ids ở %p\n", (void *)ids);
    printf("ids[0] = %d, ids[N-1] = %d\n", ids[0], ids[N - 1]); // dữ liệu còn nguyên không?

    free(ids);  // free = trả 40 MB lại cho kho
    ids = NULL; // xé tờ giấy cũ, để không lỡ dùng lại (tránh use-after-free)
    return 0;
}