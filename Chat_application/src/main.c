#include "../bin/command.h"
#include "../bin/connection.h"
#include "../bin/define.h"
#include "../bin/server.h"
#include "../bin/socket.h"

int main(int argc, char *argv[]) {
    // Check ARGC + ARGV
    if (argc != 2) {
        printf("USE: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // Get port from argv1
    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        printf("Port invalid !! from 0 -> 65535\n");
        return EXIT_FAILURE;
    }
    
    // Register signal handler function
    signal(SIGINT, signal_handler);
    
    // Initialize connection list
    init_connections();
    
    // initialize socket listen
    init_listener(port);
    
    // Show help when run chatapp 
    show_help();
    
    // LOOP CLI 
    char command[MAX_COMMAND_SIZE];
    int running = 1;
    
    while (running) {
        printf(">> ");
        fflush(stdout);
        
        if (fgets(command, MAX_COMMAND_SIZE, stdin) == NULL) {
            break;
        }
        
        running = command_handler(command);
    }
    
    cleanup();
    return EXIT_SUCCESS;
}