#include "../Inc/sbuffer.h"
#include "../Inc/types.h"

sbuffer_t *sbuffer_init(void)
{
    sbuffer_t buffer =  malloc(sizeof(sbuffer_t));
    if (buffer == NULL)
    {
        return NULL;
    }

    buffer->head = NULL;
    buffer->tail = NULL;
    buffer->size = 0;

    // Khởi tạo mutex
    if (pthread_mutex_init(&(buffer->mutex), NULL) != 0)
    {
        free(buffer);
        return NULL;
    }

    return buffer;
}

void sbuffer_free(sbuffer_t **buffer)
{
    if (buffer == NULL || *buffer == NULL)
    {
        return SBUFFER_FAILURE;
    }

    pthread_mutex_lock(&((*buffer)->mutex));

    // Giải phóng tất cả các node
    sbuffer_node_t *current = (*buffer)->head;
    while (current != NULL)
    {
        sbuffer_node_t *tmp = current;
        current = current->next;
        free(tmp);
    }

    pthread_mutex_unlock(&((*buffer)->mutex));

    // Destroy mutex
    pthread_mutex_destroy(&((*buffer)->mutex));

    // Giải phóng buffer
    free(*buffer);
    *buffer = NULL;

    return SBUFFER_SUCCESS;
}

int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data)
{
    if (buffer == NULL || data == NULL)
    {
        return SBUFFER_FAILURE;
    }

    // Tạo node mới
    sbuffer_node_t *new_node = malloc(sizeof(sbuffer_node_t));
    if (new_node == NULL)
    {
        return SBUFFER_FAILURE;
    }

    // Sao chép dữ liệu
    new_node->data = *data;
    new_node->next = NULL;

    // Khóa mutex để đảm bảo truy cập an toàn
    pthread_mutex_lock(&(buffer->mutex));

    // Thêm node vào cuối buffer
    if (buffer->tail == NULL)
    { // Buffer rỗng
        buffer->head = new_node;
        buffer->tail = new_node;
    }
    else
    {
        buffer->tail->next = new_node;
        buffer->tail = new_node;
    }

    buffer->size++;

    // Mở khóa mutex
    pthread_mutex_unlock(&(buffer->mutex));

    return SBUFFER_SUCCESS;
}

int sbuffer_remove(sbuffer_t *buffer, sensor_data_t *data)
{
    if (buffer == NULL || data == NULL)
    {
        return SBUFFER_NO_DATA;
    }

    // Khóa mutex để đảm bảo truy cập an toàn
    pthread_mutex_lock(&(buffer->mutex));

    // Kiểm tra buffer có rỗng không
    if (buffer->head == NULL)
    {
        pthread_mutex_unlock(&(buffer->mutex));
        return SBUFFER_FAILURE; // Buffer rỗng
    }

    // Lấy node đầu tiên
    sbuffer_node_t *node = buffer->head;

    // Sao chép dữ liệu
    *data = node->data;

    // Cập nhật head
    buffer->head = node->next;

    // Nếu buffer trở thành rỗng, cập nhật tail
    if (buffer->head == NULL)
    {
        buffer->tail = NULL;
    }

    buffer->size--;

    // Giải phóng node
    free(node);

    // Mở khóa mutex
    pthread_mutex_unlock(&(buffer->mutex));

    return 0;
}

bool sbuffer_is_empty(sbuffer_t *buffer)
{
    if (buffer == NULL)
    {
        return true;
    }

    bool is_empty;

    pthread_mutex_lock(&(buffer->mutex));
    is_empty = (buffer->head == NULL);
    pthread_mutex_unlock(&(buffer->mutex));

    return is_empty;
}

int sbuffer_size(sbuffer_t *buffer)
{
    if (buffer == NULL)
    {
        return -1;
    }

    int size;

    pthread_mutex_lock(&(buffer->mutex));
    size = buffer->size;
    pthread_mutex_unlock(&(buffer->mutex));

    return size;
}