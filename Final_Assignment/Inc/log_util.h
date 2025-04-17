#ifndef LOG_UTIL_H
#define LOG_UTIL_H

#include "types.h"

// Khởi tạo hệ thống log FIFO (truyền đường dẫn FIFO)
int log_init_fifo(const char* fifo_path);

// Ghi log thread-safe thông qua FIFO
void log_event_fifo(const char* format, ...);

// Giải phóng tài nguyên, đóng FIFO, hủy mutex
void log_close_fifo(void);

#endif
