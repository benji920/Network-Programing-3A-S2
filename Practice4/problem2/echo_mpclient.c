#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char *argv[])
{
    int sock;
    pid_t pid;
    char buf[BUF_SIZE];
    char receive[BUF_SIZE];
    struct sockaddr_in serv_adr;
    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");

    pid = fork();
    if (pid == 0)
        write_routine(sock, buf);
    else
        read_routine(sock, buf);

    close(sock);
    return 0;
}

void read_routine(int sock, char *receive)
{
    while (1)
    {
        int str_len = read(sock, receive, BUF_SIZE);
        if (str_len == 0)
            return;

        receive[str_len] = 0;
        
        //read(sock, receive, BUF_SIZE);
        printf("Message from server: %s\n", receive);
        fflush(stdout);
    }
}
void write_routine(int sock, char *buf)
{
    while (1)
    {
        scanf("%s",buf);
        if (!strcmp(buf, "q") || !strcmp(buf, "Q"))
        {
            shutdown(sock, SHUT_WR); //向服务器端传递 EOF,因为fork函数复制了文件描述度，所以通过1次close调用不够
            return;
        }
        write(sock, buf, strlen(buf)+1);
        strcmp(buf,"");
    }
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
