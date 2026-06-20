#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int current = 1;
    int read_fd = -1;

    while (current < n) {
        int pipefd[2];
        pipe(pipefd);

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }

        if (pid > 0) {
            close(pipefd[0]);

            if (read_fd != -1) {
                char buf[1];
                read(read_fd, buf, 1);
                close(read_fd);
            }

            if (current == 1)
                printf("%d", current);
            else
                printf(" %d", current);
            fflush(stdout);

            write(pipefd[1], "x", 1);
            close(pipefd[1]);
            wait(NULL);
            return 0;
        } else {
            close(pipefd[1]);
            read_fd = pipefd[0];
            current++;
        }
    }

    if (read_fd != -1) {
        char buf[1];
        read(read_fd, buf, 1);
        close(read_fd);
    }

    if (n == 1)
        printf("%d\n", n);
    else
        printf(" %d\n", n);
    fflush(stdout);

    return 0;
}
