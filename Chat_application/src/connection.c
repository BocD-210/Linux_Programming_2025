#include "../inc/connection.h"
#include "../inc/types.h"
#include "../inc/command.h"
#include "../inc/socket.h"
#include "../inc/server.h"

Connection connections[MAX_CONNECTIONS];
pthread_mutex_t connections_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t keep_alive = PTHREAD_MUTEX_INITIALIZER;
char resp_keep_alive[10];

// Create handler_keep_alive check connection
void* handler_keep_alive(void *arg)
{   
    // char resp_keep_alive[10];
    int conn_id = (*(int *)arg);
    int retry_cnt = 0;
    free(arg);

    Connection *conn = &connections[conn_id];

    while (conn->is_active)
    {
        sleep(5);

        if(!conn->is_active) break;

        // send "Keep_alive -> peer through socket_fd"
        if(send(conn->socket, CHECK_KEEP_ALIVE, strlen(CHECK_KEEP_ALIVE), 0) < 0){
            perror("Failed to send KEEP_ALIVE");
            break;
        }

        pthread_mutex_lock(&keep_alive);
        if(strcmp(resp_keep_alive, "Alive") == 0){
            memset(resp_keep_alive, 0, 10);
            retry_cnt = 0;
        }

        retry_cnt++;
        if(retry_cnt > 3){
            printf("ID %d not response !!! Remove connection\n", conn_id);
            remove_connection(conn->id);
        }

        // đặt bocking (read or recv with flag 0) to check recceived

    }  
    return NULL;     
}

// crate list connection
void init_connections()
{
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        connections[i].id = i;
        connections[i].socket = -1;
        connections[i].is_active = 0;
        // connections[i].initiated_by_me = 0;
    }
}

// add connection to list
int add_connection(int socket, struct sockaddr_in addr, int initiated_by_me)
{
    int *conn_id = malloc(sizeof(int));

    pthread_t keep_alive;
    pthread_mutex_lock(&connections_mutex);

    static int index = 0; // location of connection

    connections[index].socket = socket;
    connections[index].addr = addr;
    connections[index].is_active = 1;
    // connections[index].initiated_by_me = initiated_by_me;

    printf("Add connection to %d (%s:%d).\n", index,
           inet_ntoa(addr.sin_addr), htons(addr.sin_port));

    // create thread handle msg keep_Alive
    *conn_id = index;
    pthread_create(&keep_alive, NULL, handler_keep_alive, conn_id);
    pthread_detach(keep_alive);

    // Increase index, if exceed MAX_CONNECTIONS then return to 0
    index = index + 1;

    pthread_mutex_unlock(&connections_mutex);
    return index;
}

void remove_connection(int id)
{
    pthread_mutex_lock(&connections_mutex);

    if (id >= 0 && id < MAX_CONNECTIONS && connections[id].is_active)
    {
        close(connections[id].socket);
        connections[id].socket = -1;
        connections[id].is_active = 0;

        if (connections[id].thread)
        {
            pthread_cancel(connections[id].thread);
            connections[id].thread = 0;
        }

        printf("Closed connect %d (%s:%d).\n", id,
               inet_ntoa(connections[id].addr.sin_addr),
               htons(connections[id].addr.sin_port));
    }

    pthread_mutex_unlock(&connections_mutex);
}

// show list connection
void list_connections()
{
    printf("\nList connection:\n");
    printf("ID\t IP address\t\tPort\n");
    printf("----------------------------------------------------------\n");

    pthread_mutex_lock(&connections_mutex);

    int active_count = 0;
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (connections[i].is_active)
        {
            active_count++;
            printf("%d\t%s\t\t%d\n",
                   i,
                   inet_ntoa(connections[i].addr.sin_addr),
                   htons(connections[i].addr.sin_port));
        }
    }

    pthread_mutex_unlock(&connections_mutex);

    if (active_count == 0)
    {
        printf("No connection.\n");
    }
}
