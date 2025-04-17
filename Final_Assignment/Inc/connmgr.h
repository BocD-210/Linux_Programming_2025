#ifndef CONNMGR_H
#define CONNMGR_H

#include "types.h"
#include "sbuffer.h"

typedef struct {
    int port_number;   // Cổng mà server lắng nghe kết nối từ cảm biến
    sbuffer_t* buffer; // Con trỏ đến buffer dùng chung để lưu dữ liệ
    int log_fd;        // File descriptor của file FIFO để ghi log
} connmgr_args_t;

typedef struct {
    int client_fd;
    sbuffer_t *buffer;
    int log_fd;
} sensor_thread_args_t;

void* connmgr_listen (void* arg);
// /**
//  * Hàm chính của connection manager thread
//  * @param arg pointer to thread_args_t structure
//  * @return NULL on completion
//  */
// void *connection_manager(void *arg);

// /**
//  * Xử lý một kết nối mới từ sensor node
//  * @param client_fd socket file descriptor for the client connection
//  * @param buffer pointer to the shared buffer
//  * @return 0 on success, -1 on failure
//  */
void* handle_sensor_data(void* args);



#endif