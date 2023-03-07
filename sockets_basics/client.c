#include <stdio.h>
#include <strings.h>

// socket specific
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 80
#define MAXLINE 4096

int	main(int argc, char **argv)
{
	int		n;
	int		socket_fd;
	int		sendbytes;
	char	sendline[MAXLINE];
	char	recvline[MAXLINE];
	struct sockaddr_in servaddr;

	// arg error check
	if (argc != 2)
		printf("argument needed\n");

	// create socket
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
		printf("error creating the socket\n");

	// zero out the adress struct
	bzero(&servaddr, sizeof(servaddr));
	// specify adress family: in this case internet address (AF_INET)
	servaddr.sin_family = AF_INET;
	// specify server port
	// use htons() to convert from local machine byte order to network standard byte order.
	// insures that if 2 computers are communicating using different
	// byte order they can still communicate through the network standar byte order.
	servaddr.sin_port = htons(SERVER_PORT);
}