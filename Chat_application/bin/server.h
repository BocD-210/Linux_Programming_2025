#ifndef SERVER_H
#define SERVER_H

// Hàm xử lý nhận tin nhắn từ peer
void* handle_peer_connection(void* arg);

void send_message(int id, const char* message);


#endif 
