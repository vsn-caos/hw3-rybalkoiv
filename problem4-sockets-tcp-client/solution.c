#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ipv4_addr> <port>\n", argv[0]);
        return 1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)atoi(argv[2]));
    if (inet_aton(argv[1], &addr.sin_addr) == 0) {
        fprintf(stderr, "Invalid address\n");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return 1;
    }

    char line[64];
    while (fgets(line, sizeof(line), stdin) != NULL) {
        int32_t num = (int32_t)atoi(line);
        if (send(sockfd, &num, sizeof(num), 0) != sizeof(num))
            break;

        int32_t resp;
        char *buf = (char *)&resp;
        ssize_t got = 0;
        while (got < (ssize_t)sizeof(resp)) {
            ssize_t n = recv(sockfd, buf + got, sizeof(resp) - got, 0);
            if (n <= 0) {
                close(sockfd);
                return 0;
            }
            got += n;
        }
        printf("%d\n", resp);
        fflush(stdout);
    }

    close(sockfd);
    return 0;
}
