#include "../Inc/log_util.h"
#include "../Inc/sbuffer.h"
#include "../Inc/types.h"
#include "../Inc/connmgr.h"


static int fifo_fd = -1;
static pthread_mutex_t log_mutex;
static int log_initialized = 0;

int log_init_fifo(const char* fifo_path) {
    if (log_initialized) return 0;  // tránh init nhiều lần

    // Tạo FIFO nếu chưa có
    if (access(fifo_path, F_OK) == -1) {
        if (mkfifo(fifo_path, 0666) != 0) {
            perror("Không thể tạo FIFO log");
            return -1;
        }
    }

    fifo_fd = open(fifo_path, O_WRONLY | O_NONBLOCK);
    if (fifo_fd < 0) {
        perror("Không thể mở FIFO để ghi log");
        return -1;
    }

    if (pthread_mutex_init(&log_mutex, NULL) != 0) {
        perror("Không thể tạo mutex log");
        close(fifo_fd);
        fifo_fd = -1;
        return -1;
    }

    log_initialized = 1;
    return 0;
}

void log_event_fifo(const char* format, ...) {
    if (!log_initialized || fifo_fd < 0) return;

    pthread_mutex_lock(&log_mutex);

    char message[256];
    char final_msg[300];

    // Format phần nội dung log
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    // Thêm timestamp
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

    snprintf(final_msg, sizeof(final_msg), "[%s] %s\n", time_str, message);

    // Ghi vào FIFO
    write(fifo_fd, final_msg, strlen(final_msg));

    pthread_mutex_unlock(&log_mutex);
}

void log_close_fifo(void) {
    if (!log_initialized) return;

    pthread_mutex_lock(&log_mutex);

    if (fifo_fd >= 0) {
        close(fifo_fd);
        fifo_fd = -1;
    }

    pthread_mutex_unlock(&log_mutex);
    pthread_mutex_destroy(&log_mutex);

    log_initialized = 0;
}