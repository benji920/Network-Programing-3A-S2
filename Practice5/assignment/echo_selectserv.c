#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set reads, cpy_reads;

char value;
char envoie[50]= "client    :";

	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i, j;
	char buf[BUF_SIZE];
	

	char new_connection[BUF_SIZE];
	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) //分配IP地址和端口号
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	// on observe le file descriptor serv_sock avec fd_set reads
	fd_max = serv_sock;

	while (1)
	{
		cpy_reads = reads; // select est detrutive alors on garde une copie de notre set
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;
		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
			break;
		if (fd_num == 0)
			continue;
		for (i = 0; i < fd_max + 1; i++) // on parcour tout le set
		{
			if (FD_ISSET(i, &cpy_reads)) // on regarde si ca peut etre lu
			{
				if (i == serv_sock)
				{ // c'est une nouvelle connection

					adr_sz = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);
					FD_SET(clnt_sock, &reads);
					if (fd_max < clnt_sock)
						fd_max = clnt_sock;
					printf("connected client: %d \n", clnt_sock);
					char mess_connec[BUF_SIZE]="Number of Client is     \n";
					char value3=(fd_max-3)+'0';
					mess_connec[20]=value3;
					// for(int k=0;k<sizeof(mess_connec);k++)
					// printf("%c",mess_connec[k]);
					write(clnt_sock,"Welcome\n",strlen("Welcome\n"));
					
					write(clnt_sock,mess_connec,strlen(mess_connec));

					for (j = 0; j < fd_max + 1; j++) // on parcours tout le set
					{
						if (FD_ISSET(j, &reads))
						{
							if (j != clnt_sock && j != i)
							{
								char connec[BUF_SIZE] = "Client   has joined this chat\n";
								int connection = clnt_sock;
								char value = connection + '0';

								connec[7] = value;

								write(j, connec, strlen(connec));
							}
						}
						// if(write(j,buf,BUF_SIZE)==-1)
						//	error_handling("erreur du write");
					}
				}

				else
				{
					// on communique
					str_len = read(i, buf, BUF_SIZE);
					if (str_len == 0)
					{
						FD_CLR(i, &reads);
						close(i);
						printf("closed client : %d \n", i);


					for (j = 0; j < fd_max + 1; j++) // on parcours tout le set
					{
						if (FD_ISSET(j, &reads))
						{
							if (j != serv_sock && j != i)
							{
								char connec2[BUF_SIZE] = "Client   has left this chat\n";
								
								char value2 = i + '0';

								connec2[7] = value2;

								write(j, connec2, strlen(connec2)+1);
							}
						}
						// if(write(j,buf,BUF_SIZE)==-1)
						//	error_handling("erreur du write");
					}
						
					}
					else
					{
						for (j = 0; j < fd_max + 1; j++) // on parcours tout le set
						{
							if (FD_ISSET(j, &reads))
							{
								if (j != serv_sock && j != i)
								{
									
								
									write(j, buf, str_len);
									
								}
								
							}
						}
					}
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
