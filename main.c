#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define NUM_THREADS 20

void *scanner(void *argv)
{
}

int main(int argc, char **argv)
{
        if (argc < 3 || strcmp(argv[1], "--help") == 0)
        {
                fprintf(stderr, "Usage: %s host start_port:end_port\n", argv[0]);
                return 1;
        }

        long start_port = 0;
        long end_port = 0;
        char *endptr = NULL;
        char *range = argv[2];

        start_port = strtol(range, &endptr, 10);
        if (*endptr == ':')
        {
                end_port = strtol(endptr + 1, &endptr, 10);
                if (*endptr != '\0')
                {
                        fprintf(stderr, "Usage: %s host start_port:end_port\n", argv[0]);
                        return 1;
                }
        }
        else if (*endptr == '\0')
                end_port = start_port;
        else
        {
                fprintf(stderr, "Usage: %s host start_port:end_port\n", argv[0]);
                return 1;
        }

        pthread_t newthread[NUM_THREADS];

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(struct sockaddr_in));
        addr.sin_family = AF_INET;
        if (inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0)
        {
                fprintf(stderr, "inet_pton failed for address '%s'", argv[1]);
                return 1;
        }

        struct timeval timeout;
        for (int port = start_port; port <= end_port; port++)
        {

                timeout.tv_sec = 1;
                timeout.tv_usec = 0;

                int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (socket_fd == -1)
                {
                        perror("socket");
                        return 1;
                }

                if (setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&timeout, sizeof(struct timeval)) < 0)
                {
                        fprintf(stderr, "setsockopt failed for address '%s'", argv[1]);
                        close(socket_fd);
                        return 1;
                }

                addr.sin_port = htons(port);
                if (connect(socket_fd, (struct sockaddr *)&addr, sizeof addr) != -1)
                {
                        printf("port %d open\n", port);
                        close(socket_fd);
                }
        }

        return 0;
}
