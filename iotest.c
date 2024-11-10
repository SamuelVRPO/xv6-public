#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
    int i;
    for(i = 0; i < 1000; i++){
        printf(1, "I/O operation %d\n", i);
        sleep(10);  // Sleep to simulate I/O delay
    }
    exit();
}
