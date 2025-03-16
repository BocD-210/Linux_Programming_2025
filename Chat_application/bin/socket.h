#ifndef SOCKET_H
#define SOCKET_H

void* listen_for_connections(void* arg);

void init_listener(int port);

void connect_to_peer(const char* destination, int port);
#endif