#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
int main(int argc,char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd=socket(PF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(5001);
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
   
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(struct sockaddr_in));

    char sendline[1024],get[1024];
   while(1)
  {
   gets(sendline);
   send(sockfd,sendline,strlen(sendline),0);
   recv(sockfd,get,1024,0);
   printf("黄zx\n");
   puts(get);
   //write(sockfd,sendline,strlen(sendline));
  }
    //write(sockfd,sendline,strlen(sendline));
    close(sockfd);
}
