#ifndef COMMAND_H
#define COMMAND_H

void signal_handler(int sig);

void cleanup();

//show command line interface in chat app
void show_help();

int command_handler(char* command);

void error(const char *msg);

#endif /* COMMANDS_H */
