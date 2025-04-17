
#include "../Inc/log_util.h"
#include "../Inc/sbuffer.h"
#include "../Inc/types.h"
#include "../Inc/connmgr.h"
#include "../Inc/datamgr.h"

// Global variables for cleanup
sbuffer_t *buffer = NULL;
pthread_t connmgr_thread, datamgr_thread, storagemgr_thread;
int log_fd = -1;
pid_t log_pid = -1;

// Signal handler for graceful shutdown
void handle_signal(int sig)
{
    printf("Received signal %d, shutting down...\n", sig);

    // Cancel threads
    pthread_cancel(connmgr_thread);
    pthread_cancel(datamgr_thread);
    pthread_cancel(storagemgr_thread);

    // Wait for threads to complete
    pthread_join(connmgr_thread, NULL);
    pthread_join(datamgr_thread, NULL);
    pthread_join(storagemgr_thread, NULL);

    // Close log FIFO and terminate log process
    if (log_fd >= 0)
    {
        close(log_fd);
    }

    if (log_pid > 0)
    {
        kill(log_pid, SIGTERM);
        waitpid(log_pid, NULL, 0);
    }

    // Free shared buffer
    if (buffer != NULL)
    {
        sbuffer_free(&buffer);
    }

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    // Check command line arguments
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535)
    {
        fprintf(stderr, "Invalid port number\n");
        exit(EXIT_FAILURE);
    }

    // Set up signal handlers
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    int fd = int create_sensor_map();
    if(fd == -1){
        perror("Không thể tạo file sensor_map.txt");
        return;
    }

    // Initialize shared buffer
    buffer = sbuffer_init();
    if (buffer == NULL)
    {
        fprintf(stderr, "Failed to initialize shared buffer\n");
        exit(EXIT_FAILURE);
    }

    // Create log FIFO
    if (log_init_fifo("/tmp/log_fifo") != 0)
    {
        printf("Failed to create log FIFO\n");
        sbuffer_free(&buffer);
        exit(EXIT_FAILURE);
    }

    // Start log process
    log_pid = fork();
    if (log_pid < 0)
    {
        perror("Failed to fork log process");
        sbuffer_free(&buffer);
        exit(EXIT_FAILURE);
    }
    else if (log_pid == 0)
    {
        // Child process (log process)
        log_process();
        exit(EXIT_SUCCESS);
    }

    // Open log FIFO for writing
    log_fd = open(LOG_FIFO, O_WRONLY);
    if (log_fd < 0)
    {
        perror("Failed to open log FIFO");
        kill(log_pid, SIGTERM);
        waitpid(log_pid, NULL, 0);
        sbuffer_free(&buffer);
        exit(EXIT_FAILURE);
    }
    // Create thread arguments
    connmgr_args_t connmgr_args = {port, buffer, log_fd};
    datamgr_args_t datamgr_args = {fd, buffer, log_fd};
    storagemgr_args_t storagemgr_args = {buffer, log_fd};

    // Create threads
    if (pthread_create(&connmgr_thread, NULL, connmgr_listen, &connmgr_args) != 0)
    {
        perror("Failed to create connection manager thread");
        handle_signal(SIGTERM);
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&datamgr_thread, NULL, datamgr_read_data, &datamgr_args) != 0)
    {
        perror("Failed to create data manager thread");
        handle_signal(SIGTERM);
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&storagemgr_thread, NULL, storagemgr_store, &storagemgr_args) != 0)
    {
        perror("Failed to create storage manager thread");
        handle_signal(SIGTERM);
        exit(EXIT_FAILURE);
    }

    // Wait for threads to complete (should not happen unless signaled)
    pthread_join(connmgr_thread, NULL);
    pthread_join(datamgr_thread, NULL);
    pthread_join(storagemgr_thread, NULL);

    // Cleanup
    handle_signal(SIGTERM);

    return 0;
}