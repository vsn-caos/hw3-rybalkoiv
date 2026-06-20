#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    char expr[4096];
    if (fgets(expr, sizeof(expr), stdin) == NULL)
        return 1;

    size_t len = strlen(expr);
    if (len > 0 && expr[len - 1] == '\n')
        expr[--len] = '\0';

    char cmd[4096 + 16];
    snprintf(cmd, sizeof(cmd), "print(%s)", expr);

    execlp("python3", "python3", "-c", cmd, NULL);
    perror("execlp");
    return 1;
}
