#include "../bin/command.h"
#include "../bin/types.h"
#include "../bin/connection.h"
#include "../bin/server.h"
#include "../bin/socket.h"

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// showhelp
void show_help() {
    printf("\nCommand line interface:\n");
    printf("  help                                      - Show user manual\n");
    printf("  myport                                    - Show listening ports\n");
    printf("  connect <destination> <port>              - Connect to peer\n");
    printf("  list                                      - Show list connection\n");
    printf("  terminate <connection id>                 - Terminated peer\n");
    printf("  send <connection id> <message>            - send message to peer\n");
    printf("  exit                                      - exit chat application\n");
}

// Clean all before exit
void cleanup() {
    // Close all connect
    pthread_mutex_lock(&connections_mutex);
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (connections[i].is_active) {
            close(connections[i].socket);
            connections[i].is_active = 0;
            
            if (connections[i].thread) {
                pthread_cancel(connections[i].thread);
            }
        }
    }
    pthread_mutex_unlock(&connections_mutex);
    
    // close socket
    if (listen_socket >= 0) {
        close(listen_socket);
    }
    
    // close thread
    if (listen_thread) {
        pthread_cancel(listen_thread);
        pthread_join(listen_thread, NULL);
    }
    
    // close mutex
    pthread_mutex_destroy(&connections_mutex);
    
    printf("Closed all connection and exited\n");
}


void signal_handler(int sig) {
    if (sig == SIGINT) {
        printf("\nExiting\n");
        cleanup();
        exit(EXIT_SUCCESS);
    }
}


// fundtion process command

int command_handler(char* command) {
    char cmd_copy[MAX_COMMAND_SIZE];
    strncpy(cmd_copy, command, MAX_COMMAND_SIZE);
    
    char *cmd, *arg1, *arg2;
    
    // Delete "\n"
    cmd_copy[strcspn(cmd_copy, "\n")] = 0;
    
    // check command
    cmd = strtok(cmd_copy, " ");
    if (cmd == NULL) {
        return 1; 
    }
    
    if (strcmp(cmd, "help") == 0) {
        show_help();
    } 
    else if (strcmp(cmd, "myport") == 0) {
        printf("Listening for port: %d\n", listen_port);
    } 
    else if (strcmp(cmd, "connect") == 0) {
        arg1 = strtok(NULL, " ");
        arg2 = strtok(NULL, " ");
        
        if (arg1 && arg2) {
            connect_to_peer(arg1, atoi(arg2));
        } else {
            printf("Use: connect <destination> <port>\n");
        }
    } 
    else if (strcmp(cmd, "list") == 0) {
        list_connections();
    } 
    else if (strcmp(cmd, "terminate") == 0) {
        arg1 = strtok(NULL, " ");
        
        if (arg1) {
            remove_connection(atoi(arg1));
        } else {
            printf("Use: terminate <connection id>\n");
        }
    } 
    else if (strcmp(cmd, "send") == 0) {
        arg1 = strtok(NULL, " ");
        if (!arg1) {
            printf("Use: send <connection id> <message>\n");
            return 1;
        }
        
        int id = atoi(arg1);
        
        // Get the rest of the string as the message
        arg2 = strtok(NULL, "");
        if (arg2) {
            send_message(id, arg2);
        } else {
            printf("Use: send <connection id> <message>\n");
        }
    } 
    else if (strcmp(cmd, "exit") == 0) {
        printf("Exiting Chat App\n");
        return 0; // out loop
    } 
    else {
        printf("Invalid command. Type 'help' for instructions.\n");
    }
    
    return 1; // continue loop
}