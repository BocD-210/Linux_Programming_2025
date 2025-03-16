#ifndef COMMAND_H
#define COMMAND_H

void signal_handler(int sig);

// Hàm dọn dẹp tài nguyên trước khi thoát
void cleanup();

//show command line interface in chat app
void show_help();

int command_handler(char* command);

void signal_handler(int sig);

void error(const char *msg);

#endif /* COMMANDS_H */
