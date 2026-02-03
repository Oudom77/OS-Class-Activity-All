#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include "common.h"

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE + 1];

    mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    if (mq_getattr(mq, &attr) == -1) {
        perror("mq_getattr");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes = mq_receive(mq, buffer, attr.mq_msgsize, NULL);
    if (bytes == -1) {
        perror("mq_receive");
        exit(EXIT_FAILURE);
    }

    buffer[bytes] = '\0';
    printf("Receiver: Message received: %s\n", buffer);

    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}
