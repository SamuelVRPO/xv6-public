#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
    volatile int i, j;
    for(i = 0; i < 500000000; i++){
        j = i % 10;

        if(j == -1){  // This condition is never true but uses 'j'
            printf(1, "This will never be printed: %d\n", j);
        }
    }
    exit();
}
