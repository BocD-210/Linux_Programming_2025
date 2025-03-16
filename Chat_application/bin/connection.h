#ifndef CONNECTION_H
#define CONNECTION_H

//create a list connect
void init_connections();

//add connection to list
int add_connection();

void close_connection(int id);

void list_connections();
#endif 