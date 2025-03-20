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

// General constants
#define MAX_CONNECTIONS 10
#define MAX_BUFFER_SIZE 1024
#define MAX_MESSAGE_SIZE 100
#define MAX_COMMAND_SIZE 256

// Connection information structure
typedef struct {
    int id;                 // ID of peer
    int socket;             // Socket descriptor
    struct sockaddr_in addr; // address
    pthread_t thread;       // Thread handler connect
    int is_active;          // Trạng thái kết nối
} Connection;

// Global variable extern - defined and allocated in connection.c
extern Connection connections[MAX_CONNECTIONS];
extern int listen_port;
extern int listen_socket;
extern pthread_t listen_thread;
extern pthread_mutex_t connections_mutex;

#endif /* COMMON_H */