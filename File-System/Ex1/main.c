#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int main(void){
    int fd; // file descriptor
    int num_read, num_write;
    char chuoi1[12] = "hello world\n"; // CHUOI1 LA 1 BUFFER CHUA DU LIEU

    fd = open("hello.txt", O_RDWR);

    if(fd == -1){
        printf("open() hello.txt failed\n");
    }

    num_write = write(fd, chuoi1, strlen(chuoi1)); //STRLEN TINH VA TRA VE SO BYTE DUOC GHI CUA CHUOI1
    printf("Write %d bytes to hello.txt\n", num_write); 

    lseek(fd, 6, SEEK_SET);
    write(fd, "LINUX PROGRAMMING\n", strlen("LINUX PROGRAMMING\n"));

    // sau khi su dung cau lenh lseek va ghi du lieu thi du lieu se ghi de len tu byte thu 7 ke tu vi tri dau tien khi ta su dung SEEK_SET

    close(fd);
}