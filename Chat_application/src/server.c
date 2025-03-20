#include "../bin/connection.h"
#include "../bin/define.h"


// Connection connections[MAX_CONNECTIONS];
// pthread_mutex_t connections_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to handle receiving messages from peers
void* handle_peer_connection(void* arg) {
    int conn_id = *((int*)arg);
    free(arg);
    
    Connection* conn = &connections[conn_id];
    char buffer[MAX_BUFFER_SIZE];
    
    while (1) {
        memset(buffer, 0, MAX_BUFFER_SIZE);
        int bytes_received = recv(conn->socket, buffer, MAX_BUFFER_SIZE-1, 0);
        
        if (bytes_received <= 0) {
            // Connection closed or error
            printf("connect %s:%d closed.\n", 
                   inet_ntoa(conn->addr.sin_addr), 
                   htons(conn->addr.sin_port));
            close_connection(conn_id);
            return NULL;
        }
        
        // Show received messages
        printf("\nMessage from %d : %d is: %s\n", 
               conn_id, 
               ntohs(conn->addr.sin_port), 
               buffer);
        printf("Command >> "); // Show prompt again
        fflush(stdout);
    }
    
    return NULL;
}

void send_message(int id, const char* message) {
    if (id < 0 || id >= MAX_CONNECTIONS || !connections[id].is_active) {
        printf("ID invalid\n");
        return;
    }
    
    if (strlen(message) > MAX_MESSAGE_SIZE) {
        printf("Message too long (max %d character).\n", MAX_MESSAGE_SIZE);
        return;
    }
    
    if (send(connections[id].socket, message, strlen(message), 0) < 0) {
        perror("send");
        printf("Can's send\n");
        close_connection(id);
        return;
    }
    
    printf("Sent message to %d (%s:%d).\n", 
       id, 
       inet_ntoa(connections[id].addr.sin_addr), 
       htons(connections[id].addr.sin_port));
}