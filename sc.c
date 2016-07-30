#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include <sys/types.h>
#include <sys/shm.h>
#include<time.h>
#define SIZE 1024

int main(int argc,char *argv[])
{
   
   if(argc<4){
     printf("缺失参数阿,白白傻逼！\n");
     return 1;
    }
  int sock;
  if((sock=socket(PF_INET,SOCK_STREAM,0))==-1)//建立套接字
   { printf("套接字建立不成功！,白白\n");
    return 1;
  }

  struct sockaddr_in sockaddr;
   sockaddr.sin_family=AF_INET;
   sockaddr.sin_addr.s_addr=inet_addr(argv[1]);
   sockaddr.sin_port=htons(atoi(argv[2]));
   

  int tt=0;
  tt=connect(sock,(struct sockaddr *)&sockaddr,sizeof(sockaddr));//连接
  if(tt==-1)
     printf("连接不成功！\n");
  else
    printf("连线成功!\n");

  pid_t pid;
  pid=fork();
   while(1)
   {
char message[SIZE];
memset(message,0,SIZE);
strcpy(message,argv[3]);
strcat(message,":");strcat(message,"\n");
       
      if(pid==0)//父进程用来发消息
     {
          char file[225];
          memset(file,'\0',225);
          fgets(file,225,stdin);
          strcat(message,file);
          send(sock,message,strlen(message),0);
     }
      else//子进程用来收消息；
       {memset(message,'\0',SIZE);
           int flag=0;
           flag=recv(sock,message,SIZE,0);
          if(flag==0){
             printf("接收不到消息！\n");
             return 1;
           }
           printf("%s\n",message);
       }
 
   }
close(sock);
return 1;

}
        
  










  

