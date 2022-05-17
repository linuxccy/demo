#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <pthread.h>
//#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
void *recvmess(){
	printf("client recv ...\n");
	SOCKET client = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN server_add;
	server_add.sin_family = AF_INET;
	server_add.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server_add.sin_port = htons(6000);
	printf("start connect...\n");
	int flag = connect(client,(SOCKADDR*)&server_add,sizeof(server_add));
	if(flag==-1) {printf("recvclient fail\n");return 0;}
	else {printf("recvclient success\n");}
	int error;
	char buff[100];
	while(1){
		error = recv(client,buff,100,0);
		if(error!=0) {
			printf("%s\n",buff);
		}
	}
	closesocket(client);
}
void *sendmess(){
	printf("client send ...\n");
	SOCKET client = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN server_add;
	server_add.sin_family = AF_INET;
	server_add.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server_add.sin_port = htons(5000);
	printf("connect ...\n");
	int flag = connect(client,(SOCKADDR*)&server_add,sizeof(server_add));
	if(flag == -1) {printf("sendclient fail\n");return 0;}
	else printf("sendclient success\n");
	char *buff;
	while(1){
		//printf(":");
		//scanf("%s",buff);
		gets(buff);
		send(client,buff,50,0);
	}
	closesocket(client);
}
int main(){
	//初始化DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	pthread_t t1,t2;
	pthread_create(&t2,NULL,recvmess,NULL);
	pthread_create(&t1,NULL,sendmess,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	//while(1);
	return 0;
}
