#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <search_string>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    struct stat st;
    fstat(fd, &st);
    size_t size = st.st_size;

    if (size == 0) {
        close(fd);
        return 0;
    }

    char *data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    if (data == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    size_t slen = strlen(argv[2]);
    if (slen > 0) {
        for (size_t i = 0; i + slen <= size; i++) {
            if (memcmp(data + i, argv[2], slen) == 0)
                printf("%zu\n", i);
        }
    }

    munmap(data, size);
    return 0;
}
