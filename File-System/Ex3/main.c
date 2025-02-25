#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// void create_file(const char *filename) {
//     int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
//     if(fd == -1){
//         printf("Error creating file");
//     } 
//     else{
//         printf("File created successfully: %s\n", filename);
//         //close(fd);
//     }
// }

int main(int argc, char *argv[]){
    if(argc > 4){
        printf("Number of argument are out !");
        return 0;
    }
    else{
        //char *filename = argv[1];
        char filename[255];
        strcpy(filename, argv[1]); 
        int fd = open(filename, O_CREAT| O_RDWR | O_APPEND, 0644); // Replace O_APPEND by O_TRUNC if you want to delete old data in file created yet
        if(fd == -1){
            printf("Error creating file");
            close(fd);
        } 
        else{
            printf("File created successfully: %s\n", filename);
        }
        //number of bite want to read/write to file
        int num_bytes = (unsigned int)atoi(argv[2]);
        if(num_bytes <= 0){
            printf("Error: Number of bytes invalid -> STOP");
            return 0;
        }
        else{
            printf("num_byte to r/w: %d\n", num_bytes);
        }

        if(strcmp(argv[3], "w") == 0 ||strcmp(argv[3], "W") == 0){
        // int fd = write(fd, argv[4], num_bytes));
        char *content = argv[4]; // noi dung duoc Write vao 
            if(write(fd, content, num_bytes) == -1){
                printf("Error: Cant Write\n");
                close(fd);
            }
            else{
                printf("Wrote into '%s' is successful \n", filename);
            }
        }

        else if(strcmp(argv[3], "r") == 0 ||strcmp(argv[3], "R") == 0){
            char buffer[num_bytes];
            if(read(fd, buffer, num_bytes) == -1){
                printf("Error: Cant read data in '%s'\n", filename);
            }
            else{
                printf("Data read in '%s': %s", filename, buffer);
            }
        }

        else{
            printf("Command Line Argument Fail !!! Try Again \n");
            close(fd);
        }

        close(fd);

    }

}