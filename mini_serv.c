#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void	fatal_error()
{
	write(2, "Fatal error\n", 12);
	exit(1);
}

int	ids[4096];
char	*users_buf[4096];
int	count = 0;
char	write_buf[4096];

fd_set	rfds, wfds, afds;

int	max_fd;

char	read_buffer[4096];

void	register_new_client(int fd)
{
	max_fd = fd > max_fd ? fd : max_fd;
	ids[fd] = count++;
	FD_SET(fd, &afds);
	printf("accepted new user\n");
}

void	send_to_all(int senderfd, char *msg)
{
	for (int fd = 0; fd <= max_fd; fd++)
	{
		if (FD_ISSET(fd, &wfds) && fd != senderfd)
			send(fd, msg, strlen(msg), 0);
	}
}

int main(int ac, char **av)
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli; 

	if (ac != 2)
	{
		write(2, "Wrong number of arguments\n", 26);
		return (1);
	}

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd < 0)
		fatal_error();

	max_fd = sockfd;
	FD_SET(max_fd, &afds);

	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1])); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		fatal_error();
	else
		printf("Socket successfully binded..\n");

	if (listen(sockfd, 10) != 0)
		fatal_error();

	while (1)
	{
		rfds = afds;
		wfds = afds;

		if (select(max_fd + 1, &rfds, &wfds, NULL, NULL) < 0)
			fatal_error();

		for (int fd = 0; fd <= max_fd; fd++)
		{
			if (!FD_ISSET(fd, &rfds))
				continue;

			if (fd == sockfd)
			{
				printf("new client connected");
				socklen_t addr_len = sizeof(servaddr);
				int client_fd = accept(sockfd, (struct sockaddr *)&servaddr, &addr_len);
				if (client_fd >= 0)
				{
					sprintf(write_buf, "added user %d\n", count);
					register_new_client(client_fd);
					send_to_all(fd, write_buf);
					break ;
				}
			}
			else
			{
				int read_bytes = recv(fd, read_buffer, 1000, 0);
				if (read_bytes <= 0)
				{
					FD_CLR(fd, &afds);
					close(fd);
					printf("Disconnecting that bozo\n");
				}
				else
				{
					read_buffer[read_bytes] = 0;
					users_buf[fd] = str_join(users_buf[fd], read_buffer);
					if (users_buf[fd][strlen(users_buf[fd]) - 1] == '\n')
					{
						printf("From %d sending to all: %s", ids[fd], users_buf[fd]);
						send_to_all(fd, users_buf[fd]);
						free(users_buf[fd]);
						users_buf[fd] = NULL;
					}
				}
			}
		}
	}
	return 0;
}

// len = sizeof(cli);
// connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
// if (connfd < 0) { 
//     printf("server acccept failed...\n"); 
//     exit(0); 
// } 
// else
//     printf("server acccept the client...\n");