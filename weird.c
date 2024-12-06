#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define ITERATIONS 500
#define CHILDREN 2

int
main(void)
{
    int fd;
    char buf = 'A';
    int i;

    // Create a file for race condition demonstration
    fd = open("testfile", O_CREATE | O_WRONLY);
    if(fd < 0){
        printf(1, "Failed to open testfile\n");
        exit();
    }

    write(fd, "START\n", 6);

    for(i = 0; i < CHILDREN; i++) {
        int pid = fork();
        if(pid < 0) {
            printf(1, "fork failed\n");
            exit();
        }
        if(pid == 0) {
            for(int j = 0; j < ITERATIONS; j++){
                write(fd, &buf, 1);
                buf++;
                if(buf > 'Z') buf = 'A';
                if((j % 100) == 0) {
                    sleep(5); // brief delay to interleave syscalls
                }
            }
            close(fd);
            exit();
        }
    }

    // Parent waits for all children
    for(i = 0; i < CHILDREN; i++){
        wait();
    }

    // Write an ending marker
    write(fd, "\nEND\n", 5);
    close(fd);

    // Memory leak: allocate memory and never free
    // Using sbrk to grow process size
    for(i = 0; i < 5; i++){
        if(sbrk(4096) == (char*)-1){
            printf(1, "sbrk failed\n");
            exit();
        }
    }

    // // Make a few more syscalls to show variety
    // // For example, read from standard input (even if nothing is typed)
    // char input[10];
    // read(0, input, 10); // probably returns 0 or waits for input
    // // Sleep again
    // sleep(10);

    // // Crash on condition: For demonstration, just force a crash by invalid memory access
    // // This will likely kill the process. Perfect to observe in strace output.
    // // Condition: if ITERATIONS > 0 (which it is), we do a bad memory write
    // if(ITERATIONS > 0) {
    //     // Write to NULL pointer to force a crash
    //     *(char*)0 = 'X'; 
    // }

    // We should never reach here
    exit();
}
