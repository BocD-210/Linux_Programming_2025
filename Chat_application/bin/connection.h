#ifndef CONNECTION_H
#define CONNECTION_H

void* handler_keep_alive(void *arg);

//create a list connect
void init_connections();

//add connection to list
int add_connection();

void remove_connection(int id);

void list_connections();

#endif 