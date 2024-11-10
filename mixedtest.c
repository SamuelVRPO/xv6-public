#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
    int i;
    volatile int j;
    for(i = 0; i < 1000; i++){
        printf(1, "Mixed operation %d\n", i);
        for(j = 0; j < 1000000; j++){
            j = j % 10;  // CPU work
        }
        sleep(5);  // Sleep to simulate I/O
    }
    exit();
}
