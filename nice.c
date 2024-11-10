#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[]) 
{
    int pid, value;
    int old_value;

    if (argc == 2) {
        pid = 0;
        value = atoi(argv[1]);
    } else if (argc == 3) {
        pid = atoi(argv[1]);
        value = atoi(argv[2]);
    } else {
        printf(2, "Usage: nice <pid> <value> or nice <value>\n");
        exit();
    }

    old_value = nice(pid, value);

    if (old_value == -1) {
        printf(2, "Failed to change nice value\n");
    } else {
        int target_pid = (pid == 0) ? getpid() : pid;
        printf(1, "Changed nice value of pid %d from %d to %d\n", target_pid, old_value, value);
    }

    exit();
}