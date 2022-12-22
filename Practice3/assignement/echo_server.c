#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[]){
	int serv_sock;
	int clnt_sock;
	struct hostent *host;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;
	char message[BUF_SIZE];
	char *message2;
	int str_len, i;
	/*
	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	*/
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi("9111"));
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	clnt_addr_size = sizeof(clnt_addr);
	for(i = 0; i < 5; i++){
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if(clnt_sock == -1)
			error_handling("accept() error!");
		else
			printf("Connected client %d \n", i+1);
		//while(str_len = read(clnt_sock, message, sizeof(message)) != 0){
while(1){
	strcpy(message, "");
	read(clnt_sock, message, BUF_SIZE);
		
		if(!strcmp(message, "") || !strcmp(message, "")){
			printf("Client Disconnected\n");
		fflush(stdout);
			exit(1);}
			
		host = gethostbyname(message);
		
if(!host){
write(clnt_sock,"gethost...error",strlen("gethost...error"));}
else{



			write(clnt_sock,inet_ntoa(*(struct in_addr*)host->h_addr_list[0]),strlen(inet_ntoa(*(struct in_addr*)host->h_addr_list[0]))+1);
}
			}
			
		close(clnt_sock);
	}
	
	close(serv_sock);
	fflush(stdout);
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
}
