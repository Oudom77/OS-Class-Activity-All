#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>   // shm_open, mmap
#include <sys/stat.h>   // mode constants
#include <unistd.h>     // ftruncate

int main()
{
    /* the size (in bytes) of shared memory object */
    const int SIZE = 4096;

    /* name of the shared memory object */
    const char *name = "OS-oudom";

    /* strings written to shared memory */
    const char *message_0 = "Hello";
    const char *message_1 = "Oudom!";

    /* shared memory file descriptor */
    int shm_fd;

    /* pointer to shared memory object */
    void *ptr;

    /* create the shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    /* configure the size of the shared memory object */
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    /* memory map the shared memory object */
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    /* write to the shared memory object */
    sprintf((char *)ptr, "%s", message_0);
    ptr += strlen(message_0);
    sprintf((char *)ptr, "%s", message_1);

    return 0;
}
