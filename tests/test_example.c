#include <stdio.h>
#include <stdlib.h>

static int add(int a, int b)
{
    return a + b;
}

int main(void)
{
    if (add(2, 3) != 5) {
        fprintf(stderr, "FAILED: add(2, 3) != 5\n");
        return EXIT_FAILURE;
    }

    puts("PASS: Example unit test succeeded.");
    return EXIT_SUCCESS;
}
