#ifndef SBUFFER_H
#define SBUFFER_H

#include "types.h"

// Các mã lỗi cho sbuffer
#define SBUFFER_SUCCESS 0
#define SBUFFER_FAILURE -1
#define SBUFFER_NO_DATA -2

typedef struct {
    int sensor_id;      // ID của cảm biến
    double temp; // Giá trị nhiệt độ đo được
    time_t timestamp;   // Thời gian đo
} sensor_data_t;    

typedef struct sbuffer_node {
    sensor_data_t data;        // Dữ liệu cảm biến
    struct sbuffer_node* next; // Con trỏ đến node tiếp theo
} sbuffer_node_t;

typedef struct {
    sbuffer_node_t *head;
    sbuffer_node_t *tail;
    pthread_mutex_t mutex;   // Mutex để bảo vệ truy cập đồng thời
    pthread_cond_t cond_not_empty;
    int size;                // Kích thước hiện tại của buffer
} sbuffer_t;

sbuffer_t *sbuffer_init(void);

/**
 * Giải phóng sbuffer
 * Precondition: sbuffer phải đã được khởi tạo
 */
void sbuffer_free(sbuffer_t **buffer);
/**
 * Thêm một item vào sbuffer
 * Precondition: sbuffer phải đã được khởi tạo
 * Returns: 0 khi thành công, -1 khi lỗi
 */
int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data);

/**
 * Lấy một item từ sbuffer
 * Precondition: sbuffer phải đã được khởi tạo
 * Returns: 0 khi thành công, -1 khi buffer rỗng, -2 khi lỗi
 */
int sbuffer_remove(sbuffer_t *buffer, sensor_data_t *data);

/**
 * Kiểm tra xem sbuffer có rỗng không
 * Precondition: sbuffer phải đã được khởi tạo
 * Returns: true nếu buffer rỗng, false nếu không
 */
bool sbuffer_is_empty(sbuffer_t *buffer);

/**
 * Trả về kích thước hiện tại của sbuffer
 * Precondition: sbuffer phải đã được khởi tạo
 * Returns: số lượng phần tử trong buffer
 */
int sbuffer_size(sbuffer_t *buffer);

#endif  /* _SBUFFER_H_ */