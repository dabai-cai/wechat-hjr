#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<pthread.h>
#define BUF_SIZE 1024
#define MAX_CLNT 1000
typedef struct message
{
     char name[20];
     char file[BUF_SIZE];
}MSG;

MSG msg;
pthread_mutex_t mutx;
void * control_clnt(void *arg);
void send_msg(MSG msg,int len);

int sum_clnt=0;
int clnt_socks[MAX_CLNT];

int main(int argc,char *argv[])
{
    int serv_sock,clnt_sock=0;
    struct sockaddr_in serv_addr,clnt_addr;
    int clnt_adr_sz;
    pthread_t t_id;
    if(argc!=2){
      printf("usage :%s  <port>\n",argv[0]);
      exit(1);
    }
    
    pthread_mutex_init(&mutx,NULL);
    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1){
      printf("Can't build the server socket!\n");
      exit(1);
    }


    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);

    if((bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)))==-1){
       printf("fail to build socket\n");exit(1);
       }
    else printf("build the server socket!\n");

    if((listen(serv_sock,100))==-1){
       printf("fail to listen!\n");return 1;}
    else
        printf("listening!!!!!!!!!!!!!!!!!!!\n");

    while(1){
    clnt_adr_sz=sizeof(clnt_addr);
    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_adr_sz);
    if(clnt_sock==-1){
    printf("fail to connect the client!\n");
    exit(1);
    } 
    
    pthread_mutex_lock(&mutx);
         clnt_socks[sum_clnt++]=clnt_sock;
    pthread_mutex_unlock(&mutx);

    pthread_create(&t_id,NULL,control_clnt,(void*)&clnt_sock);
    pthread_detach(t_id);
    //if(pthread_detach(t_id)==0)close(clnt_sock);
     printf("connect the client IP :%s \n",inet_ntoa(clnt_addr.sin_addr));
   }
   close(serv_sock);
return 0;
}
void *control_clnt(void *arg)
{
    int clnt_sock=*((int *)arg);
    int str_len=0,i;
    while((str_len=read(clnt_sock,&msg,sizeof(msg)))!=0)
      send_msg(msg,str_len);

    pthread_mutex_lock(&mutx);
    for(i=0;i<sum_clnt;i++)
      if(clnt_sock==clnt_socks[i])
      {
          while(i<sum_clnt-1)
            {clnt_socks[i]=clnt_socks[i+1];i++;}
          break;
      }
    sum_clnt--;
    pthread_mutex_unlock(&mutx);
    close(clnt_sock);printf("关灯!\n");
    return NULL;
}

void send_msg(MSG msg,int len)
{
    int i;
    pthread_mutex_lock(&mutx);
    for(i=0;i<sum_clnt;i++)    
      write(clnt_socks[i],&msg,len);
    pthread_mutex_unlock(&mutx);
}






































