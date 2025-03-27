#include "../bin/types.h"
#include "../bin/command.h"
#include "../bin/connection.h"
#include "../bin/server.h"
#include "../bin/socket.h"

// Define global variable
int listen_port;
int listen_socket;
pthread_t listen_thread;

// Function listens for connections
void* listen_for_connections(void* arg) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    while (1) {
        // Accept connection
        int client_socket = accept(listen_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }
        // Add new connection
        int id = add_connection(client_socket, client_addr, 0) - 1;
        if (id < 0) {
            close(client_socket);
            continue;
        }

        //check: have this connected yet !
        int retValue = 0;
        for(int i = 0; i < MAX_CONNECTIONS; i++){
            if(connections[i].is_active && connections[i].socket == client_socket
                && connections[i].addr.sin_addr.s_addr == client_addr.sin_addr.s_addr  // can't use "==" to compare 2 struct => need pointer to s_addr in struct in_addr
                && connections[i].addr.sin_port == client_addr.sin_port)
            {
                retValue = -1;
                break;
            }
        }

        if(retValue != 0){
            printf("New connections: %s : %d connected yet ! Not create new connection!\n", 
                    inet_ntoa(client_addr.sin_addr), 
                    htons(client_addr.sin_port));
        }
        
        printf("\nReceived a new connection %s : %d (ID: %d)\n", 
               inet_ntoa(client_addr.sin_addr), 
               htons(client_addr.sin_port), 
               id);
        printf(">> "); // Show prompt again
        fflush(stdout);
        
        // Create new thread to handle connection
        int* conn_id = malloc(sizeof(int));
        *conn_id = id;
        
        if (pthread_create(&connections[id].thread, NULL, handle_peer_connection, conn_id) != 0) {
            perror("pthread_create");
            free(conn_id);
            remove_connection(id);
        }
    }
    
    return NULL;
}

// Listening socket 
void init_listener(int port) {
    struct sockaddr_in server_addr;
    
    // Create socket
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket < 0) {
        error("Can't create socket");
    }
    
    // Set up socket address 
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    // Bind port to socket
    if (bind(listen_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        error("Can't Bind addr to socket");
    }
    
    // Listening for connection
    if (listen(listen_socket, 5) < 0) {
        error("Can's listen connect");
    }
    
    // Save listening port
    listen_port = port;
    
    // Create thread to handle incoming connections
    if (pthread_create(&listen_thread, NULL, listen_for_connections, NULL) != 0) {
        error("Can't create thread listen");
    }
    
    printf("Listening for connections on port %d...\n", port);
}

// Function to establish a connection to another peer
void connect_to_peer(const char* destination, int port) { // check khi gọi hàm connect
    struct sockaddr_in peer_addr;
    
    // Check IP
    if (!inet_aton(destination, &peer_addr.sin_addr)) {
        printf("IP addred invalid.\n");
        return;
    }

    // Set peer address information
    memset(&peer_addr, 0, sizeof(peer_addr));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_addr.s_addr = inet_addr(destination);
    peer_addr.sin_port = htons(port);
    
    // create TCP socket
    int peer_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (peer_socket < 0) {
        perror("socket");
        return;
    }
    
    // connect to peer
    if (connect(peer_socket, (struct sockaddr*)&peer_addr, sizeof(peer_addr)) < 0) {
        perror("connect");
        close(peer_socket);
        return;
    }
    
    // add new connection
    int id = add_connection(peer_socket, peer_addr, 1) - 1;
    if (id < 0) {
        close(peer_socket);
        return;
    }
    
    printf("Connected to %s:%d (ID: %d).\n", destination, port, id);
    
    // Create new thread to handle connection
    int* conn_id = malloc(sizeof(int));
    *conn_id = id;
    
    if (pthread_create(&connections[id].thread, NULL, handle_peer_connection, conn_id) != 0) {
        perror("pthread_create");
        free(conn_id);
        remove_connection(id);
    }
}