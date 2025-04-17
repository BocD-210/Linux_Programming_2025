#include "../Inc/log_util.h"
#include "../Inc/sbuffer.h"
#include "../Inc/types.h"
#include "../Inc/datamgr.h"

int create_sensor_map()
{
    int fd = open("sensor_map.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("Không thể tạo file sensor_map.txt");
        return -1;
    }

    // Nội dung dữ liệu
    const char *data =
        "2110 1\n"
        "0608 2\n"
        "3004 3\n"
        "1975 4\n";

    ssize_t data_written = write(fd, data, strlen(data));
    if (data_written == -1)
    {
        perror("Lỗi khi ghi dữ liệu vào file");
        close(fd);
        return -1;
    }

    // close(fd);
    printf("Đã tạo file sensor_map.txt\n");
    return fd;
}

void *datamgr_read_data(void *args)
{
    datamgr_args_t *data_args = (datamgr_args_t *)args;
    sbuffer_t *buffer = data_args->buffer;
    int fd = data_args->fd;

    int room_id, sensor_id;

    sensor_map_t *head = NULL;
    sensor_map_t *tail = NULL;

    sensor_map_t *map = load_sensor_map("sensor_map.txt");

    while (1)
    {
        sensor_data_t data_none;
        if (sbuffer_remove(buffer, &data_none) == 0)
        {
            int room_id = get_room_id(map, data_none.sensor_id);
            float avg_temp = calculate_running_avg(data_none.sensor_id, data_none.temp);

            log_event_fifo("Sensor %d (Room %d): Nhiệt độ hiện tại %.2f | Trung bình: %.2f | Thời gian: %ld\n",
                   data_none.sensor_id, room_id, data_none.temp, avg_temp, data_none.timestamp);
        }
        else
        {
            // Buffer rỗng hoặc lỗi
            printf("No data available or failed to read from buffer\n");
        }
    }
}

sensor_map_t *load_sensor_map(const char *filename)
{
    static FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Không thể mở file sensor_map.txt");
        return NULL;
    }

    sensor_map_t *head = NULL, *tail = NULL;
    int room_id, sensor_id;

    while (fscanf(file, "%d %d", &room_id, &sensor_id) == 2)
    {
        sensor_map_t *new_node = malloc(sizeof(sensor_map_t));
        if (!new_node)
        {
            perror("Malloc thất bại");
            fclose(file);
            return NULL;
        }

        new_node->room_id = room_id;
        new_node->sensor_id = sensor_id;
        new_node->next = NULL;

        if (head == NULL)
            head = tail = new_node;
        else
        {
            tail->next = new_node;
            tail = new_node;
        }
    }

    fclose(file);
    return head;
}

int get_room_id(sensor_map_t *map, int sensor_id)
{
    sensor_map_t *current = map;
    while (current != NULL)
    {
        if (current->sensor_id == sensor_id)
            return current->room_id;
        current = current->next;
    }
    return -1; // Không tìm thấy
}

sensor_avg_node_t *get_or_create_avg_node(int sensor_id)
{
    sensor_avg_node_t *current = avg_head;
    while (current != NULL)
    {
        if (current->sensor_id == sensor_id)
            return current;
        current = current->next;
    }

    // Nếu không có → tạo mới
    sensor_avg_node_t *new_node = malloc(sizeof(sensor_avg_node_t));
    new_node->sensor_id = sensor_id;
    new_node->count = 0;
    new_node->index = 0;
    memset(new_node->temperatures, 0, sizeof(new_node->temperatures));
    new_node->next = avg_head;
    avg_head = new_node;
    return new_node;
}

float calculate_running_avg(int sensor_id, float new_temp)
{
    sensor_avg_node_t *node = get_or_create_avg_node(sensor_id);

    node->temperatures[node->index] = new_temp;
    node->index = (node->index + 1) % 5;

    if (node->count < 5)    node->count++;

    float sum = 0.0;
    for (int i = 0; i < node->count; ++i)
    {
        sum += node->temperatures[i];
    }

    float avg = sum / node->count;

    if(node->count == 5)
        evaluate_temperature_status(avg, sensor_id);
    return avg;
}

void evaluate_temperature_status(float avg, int sensor_id)
{
    if (avg <= TOO_COLD_THRESHOLD)
    {
        log_event_fifo("The sensor node with ID %d reports it’s too cold (running avg temperature = %.2f)", sensor_id, avg);
    }
    else if (avg >= TOO_HOT_THRESHOLD)
    {
        log_event_fifo("The sensor node with ID %d reports it’s too hot (running avg temperature = %.2f)", sensor_id, avg);
    }
    else
    {
        log_event_fifo("The sensor node with ID %d reports normal temperature (avg = %.2f)", sensor_id, avg);
    }
}
