#include "../Inc/log_util.h"
#include "../Inc/sbuffer.h"
#include "../Inc/types.h"
#include "../Inc/connmgr.h"

int sensor_id = 0;

// set up TCP socket
void *connmgr_listen(void *args)
{
    connmgr_args_t *conn_args = (connmgr_args_t *)args;
    int port_number = conn_args->port_number;
    sbuffer_t *buffer = conn_args->buffer;
    int log_fd = conn_args->log_fd;

    // sensor_data_t *sensor_data;
    // sensor_data->temp = conn_args->temp;
    // sensor_data->sensor_id = sensor_id;
    // sensor_data->timestamp = time(NULL);

    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // 1️⃣ Tạo socket TCP
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket creation failed");
        return NULL;
    }

    // Set up address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Lắng nghe trên tất cả địa chỉ
    server_addr.sin_port = htons(port_number); // Chuyển đổi port về dạng big-endian

    // 3️⃣ Bind address socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        return NULL;
    }

    if (listen(server_fd, BACKLOG) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        return NULL;
    }

    printf("Listening on port %d...\n", port_number);
    while(1){
    // 5️⃣ Chấp nhận kết nối từ cảm biến
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd < 0)
        {
            perror("Accept failed");
            close(server_fd);
            return NULL;
        }

        printf("New connection from %s:%d\n",
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        sensor_thread_args_t *thread_data_s = malloc(sizeof(sensor_thread_args_t));
        if (!thread_data_s) {
            perror("Failed to allocate thread_data");
            close(client_fd);
            continue;
        }
        
        thread_data_s->client_fd = client_fd;
        thread_data_s->buffer = buffer;
        thread_data_s->log_fd = log_fd;

        pthread_t sensor_thread;
        if(pthread_create(&sensor_thread, NULL, handle_sensor_data, (void *)thread_data_s) != 0){
            perror("Failed to create sensor thread");
                close(client_fd);
                free(thread_data_s);
                continue;
        }

        pthread_detach(sensor_thread);
    }

    close(server_fd); // Đóng socket server khi không cần nữa

    return NULL;
}

void *handle_sensor_data(void *args)
{
    sensor_thread_args_t *sensor_args = (sensor_thread_args_t *)args;
    sensor_data_t *sensor_data = malloc(sizeof(sensor_data));

    log_event_fifo("A sensor node with ID %d has opened a new connection", sensor_id++);

    while (1)
    {
        sensor_data->temp = (rand() % (45 - 7 + 1)) + 7;
        sensor_data->sensor_id = sensor_id;
        sensor_data->timestamp = time(NULL);

        send(sensor_args->client_fd, &sensor_data->temp, sizeof(int), 0);

        pthread_mutex_lock(&(sensor_args->buffer->mutex));
        if(sbuffer_insert(sensor_args->buffer, sensor_data)){
            fprintf(stderr, "Failed to insert data into buffer\n");
        }
        pthread_mutex_unlock(&(sensor_args->buffer->mutex));

        sleep(2);
    }

    log_event_fifo("The sensor node with ID %d has closed the connection", sensor_id);

    close(sensor_args->client_fd);
    free(sensor_args);
    free(sensor_data);
    return NULL;
}
