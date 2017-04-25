#include "river.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if((argc > 2)||(argc < 2))
    {
        fprintf(stderr,"Insufficient input commands\n");
        return EXIT_FAILURE;
    }
    int length = leastplankRotations(argv[1]);
    printf("%d\n", length);
    return EXIT_SUCCESS;
}