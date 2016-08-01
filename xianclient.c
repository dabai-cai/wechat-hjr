#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<pthread.h>
#include<sys/socket.h>

#define BUF_SIZE 1024
#define NAME_SIZE 20
typedef struct message
{
     char name[20];
     char file[BUF_SIZE];
}MSG;

void *send_msg(void *arg);
void *recv_msg(void *arg);

char name[NAME_SIZE];
MSG msg;

int main(int argc,char *argv[])
{
   int sock;strcpy(name,argv[3]);
   struct sockaddr_in serv_addr;
   pthread_t snd_thread,rcv_thread;
   void *thread_return;
   if(argc!=4){
       printf("Usage :%s <IP> <PORT> <name>\n",argv[0]);
        exit(1);
   }
   
  sprintf(name,"[%s]: ",argv[3]);
  sock=socket(PF_INET,SOCK_STREAM,0);
  if(sock==-1){
    printf("fail to build socket!\n");
    exit(1);
  }
  else
     printf("build the socket success!\n");
  
  
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
  serv_addr.sin_port=htons(atoi(argv[2]));

  if((connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)))==-1)
    printf("fail to connect the server!\n");
  else printf("connect the server!\n");
  
 pthread_create(&snd_thread,NULL,send_msg,(void*)&sock);
 pthread_create(&rcv_thread,NULL,recv_msg,(void*)&sock);
 pthread_join(snd_thread,&thread_return);
 pthread_join(rcv_thread,&thread_return);
 
 close(sock);
 return 1;

}

void *send_msg(void *arg)
{
  int sock=*((int *)arg);
  MSG msg;strcpy(msg.name,name);
  while(1)
 {
   fgets(msg.file,BUF_SIZE,stdin);
   write(sock,&msg,sizeof(MSG));
 }
 return NULL;
}

void *recv_msg(void *arg)
{
  MSG msg;
  int sock=*((int *)arg);
  int str_len=0;
  char temp[BUF_SIZE];
  while(1)
 {
    str_len=read(sock,&msg,sizeof(MSG));
    if(strcmp(temp,msg.file)==0)continue;
    else strcpy(temp,msg.file);
    if(str_len==-1)
     return (void*)-1;
   msg.file[str_len]=0;
  fputs(msg.name,stdout);fputs(msg.file,stdout);
 }
 return NULL;
}






























