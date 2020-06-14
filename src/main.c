#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include "tlpi_hdr.h"

#define BUF_SIZE 10 // Maximum size of messages exchanged  between client and server

int main(int argc, char **argv)
{
        struct sockaddr_in addr;
        int socket_fd;
        short port;

        if (argc < 3 || strcmp(argv[1], "--help") == 0 || strchr(argv[2], ':') == NULL)
                usageErr("Usage: %s host start_port:end_port\n", argv[0]);

        char *start_port = strtok(argv[2], ":");
        char *end_port = strtok(NULL, ":");

        if (end_port == NULL)
                usageErr("Usage: %s host port_start:port_end\n", argv[0]);

        int n_end_port = getInt(end_port, GN_ANY_BASE, argv[0]);

        for (int current_port = getInt(start_port, GN_ANY_BASE, argv[0]); current_port <= n_end_port; current_port++)
        {
                socket_fd = socket(AF_INET, SOCK_STREAM, 0);
                if (socket_fd == -1)
                        errExit("socket");

                memset(&addr, 0, sizeof(struct sockaddr_in));
                addr.sin_family = AF_INET;
                addr.sin_port = htons(current_port);
                if (inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0)
                        fatal("inet_pton failed for address '%s'", argv[1]);

                if (connect(socket_fd, (struct sockaddr *)&addr, sizeof addr) != -1)
                {
                        printf("port %d open\n", current_port);
                        close(socket_fd);
                }
        }
        return 0;
}
