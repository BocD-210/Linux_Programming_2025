#include "../bin/define.h"
#include "../bin/command.h"
#include "../bin/connection.h"
#include "../bin/server.h"
#include "../bin/socket.h"

// Định nghĩa biến toàn cục
int listen_port;
int listen_socket;
pthread_t listen_thread;

// Hàm server lắng nghe kết nối
void* listen_for_connections(void* arg) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    while (1) {
        // Chấp nhận kết nối
        int client_socket = accept(listen_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }
        
        // Kiểm tra kết nối trùng lặp
        // if (connection_exists(client_addr)) {
        //     printf("Đã từ chối kết nối trùng lặp từ %s:%d\n", 
        //            inet_ntoa(client_addr.sin_addr), 
        //            ntohs(client_addr.sin_port));
        //     close(client_socket);
        //     continue;
        // }
        
        // Thêm kết nối mới
        int id = add_connection(client_socket, client_addr, 0);
        if (id < 0) {
            close(client_socket);
            continue;
        }
        
        printf("\nReceived a new connection %s:%d (ID: %d)\n", 
               inet_ntoa(client_addr.sin_addr), 
               ntohs(client_addr.sin_port), 
               id);
        printf("Command : >> "); // Hiển thị lại prompt
        fflush(stdout);
        
        // Tạo thread mới để xử lý kết nối
        int* conn_id = malloc(sizeof(int));
        *conn_id = id;
        
        if (pthread_create(&connections[id].thread, NULL, handle_peer_connection, conn_id) != 0) {
            perror("pthread_create");
            free(conn_id);
            close_connection(id);
        }
    }
    
    return NULL;
}

// Hàm khởi tạo socket lắng nghe
void init_listener(int port) {
    struct sockaddr_in server_addr;
    
    // Tạo socket
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket < 0) {
        error("Can't create socket");
    }
    
    // Thiết lập thông tin địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    // Gắn socket với port
    if (bind(listen_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        error("Can't Bind addr to socket");
    }
    
    // Lắng nghe kết nối
    if (listen(listen_socket, 5) < 0) {
        error("Can's listen connect");
    }
    
    // Lưu port đang lắng nghe
    listen_port = port;
    
    // Tạo thread để xử lý các kết nối đến
    if (pthread_create(&listen_thread, NULL, listen_for_connections, NULL) != 0) {
        error("Can't create thread listen");
    }
    
    printf("Listening for connections on port %d...\n", port);
}

// Hàm thiết lập kết nối đến peer khác
void connect_to_peer(const char* destination, int port) { // check khi gọi hàm connect
    struct sockaddr_in peer_addr;
    
    // Kiểm tra địa chỉ IP
    if (!inet_aton(destination, &peer_addr.sin_addr)) {
        printf("IP addred invalid.\n");
        return;
    }
    
    // Kiểm tra self-connection
    // char* my_ip = get_my_ip();
    // if (strcmp(destination, my_ip) == 0 && port == listen_port) {
    //     printf("Không thể kết nối đến chính máy này.\n");
    //     return;
    // }
    
    // Thiết lập thông tin địa chỉ peer
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
    int id = add_connection(peer_socket, peer_addr, 1);
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
        close_connection(id);
    }
}