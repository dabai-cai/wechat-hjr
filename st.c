#include<stdio.h>   
#include<stdlib.h>
#include<sys/types.h>  //数据类型定义
#include<sys/stat.h>
#include<netinet/in.h>  //定义数据结构sockaddr_in
#include<sys/socket.h>  //提供socket函数及数据结构
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/ipc.h>
#include<errno.h>
#include<sys/shm.h>
#include<time.h>
#define PERM S_IRUSR|S_IWUSR 
#define SIZE 1024
int i=0;
key_t shm_create()//创建共享内存函数！！
{
   key_t shmid;
   //shmid = shmget(IPC_PRIVATE,1024,PERM);
   if((shmid = shmget(IPC_PRIVATE,1024,PERM)) == -1)
   {
     fprintf(stderr,"Create Share Memory Error:%s\n\a",strerror(errno));
     exit(1);
   }
   return shmid;
}
int main(int argc,char *argv[])
{
  if(argc<3){
    printf("缺失参数阿傻逼！\n");
    return 1;
    }
  int sevsocket,clisocket;
  pid_t fpid,spid;
  struct sockaddr_in sevaddr,cliaddr;
  key_t smith;
  smith=shm_create();//共享存储的构建,返回文件描述符
  sevsocket=socket(PF_INET,SOCK_STREAM,0);
  if(sevsocket==-1)
     printf("套接字建立失败！\n");
  else
     printf("套接字建立成功！\n");

   sevaddr.sin_family = AF_INET;//IPv4协议族
   sevaddr.sin_port = htons(atoi(argv[1]));//端口转换
   sevaddr.sin_addr.s_addr = INADDR_ANY;
   bzero(&(sevaddr.sin_zero),0);
   if(bind(sevsocket,(struct sockaddr*)&sevaddr,sizeof(struct sockaddr)) == -1)//建立
    printf("套接字和本机连接不成功！\n");
  else
    printf("服务端套接字和本地联系成功！\n");
  
  
  int size=0;
 
char *temp=(char *)malloc(225);


while(1)//大循环
{
    if((listen(sevsocket,5))==-1)//监听
    printf("监听不成功！\n");
  else
    printf("监听中！！！！！！！！！\n");
    clisocket=accept(sevsocket,(struct sockaddr *)&cliaddr,&size);//接收
    if(clisocket==-1)
        printf("连接失败！");
   else
    printf("client %d连接成功!!!!!\n",i++);
   fpid=fork();//分进程

if(fpid==0)//各个子进程分别办事  
{   
  spid=fork();

  while(1)
 {
   if(spid==0)//子进程用来收信息
   {      char file[SIZE];
       memset(file,'\0',SIZE);
       int tt=0;
       tt=recv(clisocket,file,SIZE,0);
        if(tt==0){
         printf("接收不到消息！\n");
         return 1;
          }
       char *cd=shmat(smith,0,0);
       memset(cd,'\0',1024);
       strcpy(cd,file);
       printf("%s\n",file);
   }
   else//父进程用来发消息；
   {
      sleep(5);
        char *ffile=(char *)malloc(225);
        ffile=shmat(smith,0,0);
       // memset(ffile,'\0',225);
         int TT=0;
        char *scd=shmat(smith,0,0);
        if((strcmp(temp,ffile))!=0)
       {
        TT=send(clisocket,ffile,strlen(ffile),0);
        memset(temp,'\0',225);
        strcpy(temp,ffile);
         if(TT==-1)
           {
            printf("消息发不出！\n");
            return 1;
           }
       }
   }
 }
}
}
close(sevsocket);
close(clisocket);
return 1;
}
        





       
       
       
       
  
  











