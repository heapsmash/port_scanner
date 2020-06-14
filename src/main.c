#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#include "tlpi_hdr.h"

#define BUF_SIZE 10 // Maximum size of messages exchanged  between client and server

int main(int argc, char **argv)
{
        if (argc < 3 || strcmp(argv[1], "--help") == 0 || strchr(argv[2], ':') == NULL)
                usageErr("Usage: %s host start_port:end_port\n", argv[0]);

        long start_port = 0;
        long end_port = 0;
        char *endptr = NULL;
        char *range = argv[2];

        start_port = strtol(range, &endptr, 10);
        if (*endptr == ':')
        {
                end_port = strtol(endptr + 1, &endptr, 10);
                if (*endptr != '\0')
                        usageErr("Usage: %s host start_port:end_port\n", argv[0]);
        }
        else if (*endptr == '\0')
                end_port = start_port;
        else
                usageErr("Usage: %s host start_port:end_port\n", argv[0]);

        struct sockaddr_in addr;
        if (inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0)
                fatal("inet_pton failed for address '%s'", argv[1]);

        for (int port = start_port; port <= end_port; port++)
        {

                int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (socket_fd == -1)
                        errExit("socket");

                memset(&addr, 0, sizeof(struct sockaddr_in));
                addr.sin_family = AF_INET;
                addr.sin_port = htons(port);

                if (connect(socket_fd, (struct sockaddr *)&addr, sizeof addr) != -1)
                {
                        printf("port %d open\n", port);
                        close(socket_fd);
                }
        }

        return 0;
}
