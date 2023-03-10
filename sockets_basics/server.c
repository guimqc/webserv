#include "socket.h"
#include <stdio.h>
#include <string.h>

// **bind error**

void	error(char *msg)
{
	printf("%s", msg);
	exit(1);
}

int	main(int argc, char **argv)
{
	int		listen_fd, conn_fd, n;
	struct	sockaddr_in servaddr;
	uint8_t buff[MAXLINE + 1];
	uint8_t recvline[MAXLINE + 1];

	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0) < 0))
		error("socket error\n");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	// basically saying we listen to anything that's an internet address
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);
	if ((bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0)
		error("bind error\n");
	if ((listen(listen_fd, 10)) < 0)
		error("listen error\n");

	while (1)
	{
		struct sockaddr addr;
		socklen_t		addr_len;

		printf("waiting for connection on port %i\n", SERVER_PORT);
		fflush(stdout);
		// accept() blocks until an incoming connection arrives and returns
		// an fd for the connection to write in
		conn_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
		memset(recvline, 0, MAXLINE);
		while ((n = read(conn_fd, recvline, MAXLINE + 1)) > 0)
		{
			printf("%s\n", recvline);
			if (recvline[n - 1] == '\n')
				break;
			memset(recvline, 0, MAXLINE);
			if (n < 0)
				error("read error\n");
			// write response message to connection into buff
			snprintf((char *)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello");
			// write response message into connection socket (conn_fd)
			write(conn_fd, buff, strlen((char *)buff));
			close(conn_fd);
		}
	}
}
