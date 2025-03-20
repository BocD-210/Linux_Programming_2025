#ifndef SERVER_H
#define SERVER_H

// Function to handle receiving messages from peer
void* handle_peer_connection(void* arg);

void send_message(int id, const char* message);


#endif 
