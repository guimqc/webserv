#include "socket.h"

void	error(char *msg)
{
	printf("%s", msg);
	exit(1);
}

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
		error("argument needed\n");

	// create socket (addr family: internet, type, protocol (0 = TCP) )
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
		error("error creating the socket\n");

	// zero out the address struct
	bzero(&servaddr, sizeof(servaddr));
	// specify adress family: in this case internet address (AF_INET)
	servaddr.sin_family = AF_INET;
	// specify server port
	// use htons() to convert from local machine byte order to network standard byte order.
	// insures that if 2 computers are communicating using different
	// byte order they can still communicate through the network standard byte order.
	servaddr.sin_port = htons(SERVER_PORT);
	// inet_pton() converts a string representation of an address into a binary representation
	// in this case we want to put it into servaddr.sin_addr
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		error("inet_pton() error\n");
	// after setting up the address we can now try to connect to it with connect()
	if (connect(socket_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
		error("connect() failed\n");
	// prepare request to send to server
	sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
	sendbytes = strlen(sendline);
	// send request by writing into the socket_fd
	if (write(socket_fd, sendline, sendbytes) != sendbytes)
		error("write error\n");
	// read the server answer from socket_fd into recvline
	memset(recvline, 0, MAXLINE);
	while ((n = read(socket_fd, recvline, MAXLINE - 1)) > 0)
	{
		printf("%s\n", recvline);
		memset(recvline, 0, MAXLINE);
	}
	if (n < 0)
		error("read error\n");
	exit(0);
}
