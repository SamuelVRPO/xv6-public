#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define ITERATIONS 500
#define CHILDREN 2

int main(void) {
    int fd;
    char buf = 'A';
    int i;

    // Create a file for race condition demonstration
    fd = open("testfile", O_CREAT | O_WRONLY, 0666);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    // Write something initial
    if (write(fd, "START\n", 6) != 6) {
        perror("write");
        exit(1);
    }

    // Fork multiple children to cause a race condition writing to the same file
    for (i = 0; i < CHILDREN; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            // Child process: write a bunch of letters to testfile
            // Race condition: multiple processes writing at once
            for (int j = 0; j < ITERATIONS; j++) {
                if (write(fd, &buf, 1) != 1) {
                    perror("write in child");
                    _exit(1);
                }
                // Change character to differentiate children
                buf++;
                if (buf > 'Z') buf = 'A';
                // Introduce delay to force scheduling and increase race unpredictability
                if ((j % 100) == 0) {
                    sleep(1); // brief delay
                }
            }
            // Done writing
            close(fd);
            _exit(0);
        }
    }

    // Parent waits for all children
    for (i = 0; i < CHILDREN; i++) {
        waitpid(-1, NULL, 0);
    }

    // Write an ending marker
    if (write(fd, "\nEND\n", 5) != 5) {
        perror("write end");
        exit(1);
    }
    close(fd);

    // Memory leak: allocate memory and never free it
    for (i = 0; i < 5; i++) {
        void *mem = malloc(4096);
        if (!mem) {
            perror("malloc");
            exit(1);
        }
        // Not freeing mem, causing a memory leak
    }
    
    // Sleep again for good measure
    sleep(2);


    // Should never reach here
    return 0;
}
