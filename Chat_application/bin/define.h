#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>

// Các hằng số chung
#define MAX_CONNECTIONS 10
#define MAX_BUFFER_SIZE 1024
#define MAX_MESSAGE_SIZE 100
#define MAX_COMMAND_SIZE 256

// Cấu trúc thông tin kết nối
typedef struct {
    int id;                 // ID kết nối
    int socket;             // Socket descriptor
    struct sockaddr_in addr; // Thông tin địa chỉ
    pthread_t thread;       // Thread xử lý kết nối
    int is_active;          // Trạng thái kết nối
    //int initiated_by_me;    // Kết nối được tạo bởi chương trình này
} Connection;

// Biến toàn cục extern - được định nghĩa và cấp phát trong connection.c
extern Connection connections[MAX_CONNECTIONS];
extern int listen_port;
extern int listen_socket;
extern pthread_t listen_thread;
extern pthread_mutex_t connections_mutex;

#endif /* COMMON_H */