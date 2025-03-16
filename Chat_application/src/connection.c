#include "../bin/connection.h"
#include "../bin/define.h"
#include "../bin/command.h"
#include "../bin/socket.h"

Connection connections[MAX_CONNECTIONS];
pthread_mutex_t connections_mutex = PTHREAD_MUTEX_INITIALIZER;

// crate list connection
void init_connections() {
    pthread_mutex_lock(&connections_mutex);
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        connections[i].id = i;
        connections[i].socket = -1;
        connections[i].is_active = 0;
        // connections[i].initiated_by_me = 0;
    }
    pthread_mutex_unlock(&connections_mutex);
}

// add connection to list
int add_connection(int socket, struct sockaddr_in addr, int initiated_by_me) {
    pthread_mutex_lock(&connections_mutex);
    
    static int index = 0;  // location of connection

    connections[index].socket = socket;
    connections[index].addr = addr;
    connections[index].is_active = 1;
    //connections[index].initiated_by_me = initiated_by_me;

    printf("Add connection to %d (%s:%d).\n", index, 
           inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    // Increase index, if exceed MAX_CONNECTIONS then return to 0
    index = (index) % MAX_CONNECTIONS;

    pthread_mutex_unlock(&connections_mutex);
    return index;
}

void close_connection(int id) {
    pthread_mutex_lock(&connections_mutex);
    
    if (id >= 0 && id < MAX_CONNECTIONS && connections[id].is_active) {
        close(connections[id].socket);
        connections[id].socket = -1;
        connections[id].is_active = 0;
        
        if (connections[id].thread) {
            pthread_cancel(connections[id].thread);
            connections[id].thread = 0;
        }
        
        printf("Closed connect %d (%s:%d).\n", id, 
               inet_ntoa(connections[id].addr.sin_addr), 
               ntohs(connections[id].addr.sin_port));
    }
    
    pthread_mutex_unlock(&connections_mutex);
}

// show list connection
void list_connections() {
    printf("\nList connection:\n");
    printf("ID\t IP address\t\tPort\n");
    printf("----------------------------------------------------------\n");
    
    pthread_mutex_lock(&connections_mutex);
    
    int active_count = 0;
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (connections[i].is_active) {
            active_count++;
            printf("%d\t%s\t\t%d\n", 
                   i, 
                   inet_ntoa(connections[i].addr.sin_addr), 
                   ntohs(connections[i].addr.sin_port));
        }
    }
    
    pthread_mutex_unlock(&connections_mutex);
    
    if (active_count == 0) {
        printf("No connection.\n");
    }
}

