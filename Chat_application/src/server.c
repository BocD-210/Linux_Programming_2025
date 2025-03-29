#include "../inc/connection.h"
#include "../inc/types.h"

// Function to handle receiving messages from peers
void* handle_peer_connection(void* arg) {
    int conn_id = *((int*)arg);
    free(arg);
    
    Connection* conn = &connections[conn_id];
    char buffer[MAX_BUFFER_SIZE];
    char recv_keep_alive[10];
    
    while (1) {
        memset(buffer, 0, MAX_BUFFER_SIZE);
        
        //Flag 0 in recv : Blocking until recv data
        int bytes_received = recv(conn->socket, buffer, MAX_BUFFER_SIZE-1, 0);
        
        if (bytes_received <= 0) {
            // Connection closed or error
            printf("connect %s:%d closed.\n", 
                   inet_ntoa(conn->addr.sin_addr), 
                   htons(conn->addr.sin_port));
            remove_connection(conn_id);
            return NULL;
        }

        int ret_value = recv(conn->socket, recv_keep_alive, 10, 0);

        if(ret_value < 0){
            printf("Connection %d closed.\n", conn_id);
            remove_connection(conn_id);
            return NULL;
        }

        if(strcmp(recv_keep_alive, "Keep_alive") == 0){ // reeceived data check 
            pthread_mutex_lock(&keep_alive);
            strcpy(resp_keep_alive,"Alive");
            pthread_mutex_unlock(&keep_alive);
        }
        
        // Show received messages
        printf("\nMessage from %d : %d is: %s\n", 
               conn_id, 
               ntohs(conn->addr.sin_port), 
               buffer);
        printf(">> "); // Show prompt again
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
        remove_connection(id);
        return;
    }
    
    printf("Sent message to %d (%s:%d).\n", 
       id, 
       inet_ntoa(connections[id].addr.sin_addr), 
       htons(connections[id].addr.sin_port));
}
