#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{

    int fd[2];//file descriptors for the pipe, fd[0] for read, fd[1] for write
    
    //create pipe
    if (pipe(fd) == -1) {
        fprintf(2, "Error: pipe(fd) error.\n");
        exit(1);
    }
    
    //create child process
    int pid = fork();
    if (pid == -1) {
        fprintf(2, "Error: fork() error.\n");
        exit(1);
    }
    
    //child process
    if (pid == 0) {
        char buffer[1];
        close(fd[1]);//close the write end
        //read data
        if (read(fd[0], buffer, 1) == -1) {
            fprintf(2, "Error: read() error in child.\n");
            exit(1);
        }
        close(fd[0]);//close the read end
        printf("%d: received ping\n", getpid());
        //create a new pipe to write
        if (pipe(fd) == -1) {
            fprintf(2, "Error: pipe(fd) error.\n");
            exit(1);
        }
        //write data back
        if (write(fd[1], buffer, 1) == -1) {
            fprintf(2, "Error: write() error in child.\n");
            exit(1);
        }
        close(fd[1]);
    } 
    
    //parent process
    else {
        char buffer[1];
        buffer[0] = 'a';//prepare data to be sent
        close(fd[0]);
        //write data
        if (write(fd[1], buffer, 1) == -1) {
            fprintf(2, "Error: write() error in parent.\n");
            exit(1);
        }
        close(fd[1]); 
        wait(0);//wait for the child process to complete
        // create a new pipe to read
        if (pipe(fd) == -1) {
            fprintf(2, "Error: pipe(fd) error.\n");
            exit(1);
        }
        close(fd[1]);
        //read data
        if (read(fd[0], buffer, 1) == -1) {
            fprintf(2, "Error: read() error in parent.\n");
            exit(1);
        }
        close(fd[0]);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}

