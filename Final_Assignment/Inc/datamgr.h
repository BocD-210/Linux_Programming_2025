#ifndef DATAMGR_H
#define DATAMGR_H

#include "types.h"
#include "sbuffer.h"

#define TOO_HOT_THRESHOLD 35.0
#define TOO_COLD_THRESHOLD 15.0

typedef struct {
    int fd; 
    sbuffer_t* buffer; // Con trỏ đến buffer dùng chung để lưu dữ liệ
    int log_fd;        // File descriptor của file FIFO để ghi log
} datamgr_args_t;

typedef struct sensor_room
{
    int room_id;
    int sensor_id;
    struct sensor_room *next;
} sensor_map_t;

typedef struct sensor_avg_node {
    int sensor_id;
    float temperatures[5];
    int count;         // số lượng giá trị hiện có (tối đa 5)
    int index;         // chỉ số hiện tại để lưu tiếp theo
    struct sensor_avg_node* next;
} sensor_avg_node_t;

sensor_avg_node_t* avg_head = NULL; // danh sách liên kết


int create_sensor_map();

void* datamgr_read_data(void* arg);

sensor_map_t *load_sensor_map(const char *filename);

int get_room_id(sensor_map_t *map, int sensor_id);


float calculate_running_avg(int sensor_id, float temperature);

#endif