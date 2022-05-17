#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_SIZE 100
int sockid_1 = 0,sockid_2 = 0;
SOCKET client_ret_1[MAX_SIZE];//服务端recv套接字accept return 
SOCKET client_ret_2[MAX_SIZE];//服务端send套接字accept return 
SOCKADDR_IN client_ret_add_1[MAX_SIZE];//与之对应的client SOCKADDR
SOCKADDR_IN client_ret_add_2[MAX_SIZE];
//命令
char zl[10][10] = {"sup","sin","add","scr","del","cha","show"};
//学生信息结构体
typedef struct STU{
	char Name[20];
	char ID[20];
	int a;
	int b;
	int c;
}st;
//客户端注册信息
typedef struct User{
	char username[50];
	char userpswd[50];
}user;
typedef struct Sockmess{
	SOCKET sock;
	int id;
}sk;
st stu[MAX_SIZE];
int Stu_add(st student){
	FILE *fp = fopen("./stu.txt","a+");
	fprintf(fp,"%s %s %d %d %d\n",student.Name,student.ID,student.a,student.b,student.c);
	fclose(fp);
	FILE *f = fopen("./count.txt","r");
	int cnt = 0;
	fscanf(f,"%d",&cnt);
	fclose(f);
	cnt = cnt + 1;
	f = fopen("./count.txt","w");
	fprintf(f,"%d",cnt);
	fclose(f);
	return 1;
}
char* Stu_sreach(st student){
	FILE *fp = fopen("./stu.txt","r");
	char stuname[20],stuid[20];
	int cnt = 0,flag = 0;
	strcpy(stuname,student.Name);
	strcpy(stuid,student.ID);
	FILE *f = fopen("./count.txt","r");
	fscanf(f,"%d",&cnt);
	fclose(f);
	st tmp;
	int i;
	for(i=0;i<cnt;i++){
		fscanf(fp,"%s %s %d %d %d\n",tmp.Name,tmp.ID,&tmp.a,&tmp.b,&tmp.c);
		if(strcmp(tmp.Name,student.Name)==0) {flag=1;break;} 
	}
	char *stumess;
	if(flag) {sprintf(stumess,"%s %s %d %d %d",tmp.Name,tmp.ID,tmp.a,tmp.b,tmp.c);}
	else {sprintf(stumess,"%s","error");}
	fclose(fp);
	return stumess;
}
int Stu_delete(st student){
	FILE *fp = fopen("./stu.txt","r");
	char stuname[20],stuid[20];
	int cnt = 0,flag = 0;
	strcpy(stuname,student.Name);
	strcpy(stuid,student.ID);
	FILE *f = fopen("./count.txt","r");
	fscanf(f,"%d",&cnt);
	fclose(f);
	st tmp;
	int i;
	for(i=0;i<cnt;i++){
		fscanf(fp,"%s %s %d %d %d\n",tmp.Name,tmp.ID,&tmp.a,&tmp.b,&tmp.c);
		if(strcmp(tmp.Name,student.Name)==0) {flag=1;break;} 
	}
	if(flag){
		int j,ans = 0;
		for(j=0;j<cnt;j++){
			if(j!=i) {
				fscanf(fp,"%s %s %d %d %d\n",stu[ans].Name,stu[ans].ID,&stu[ans].a,&stu[ans].b,&stu[ans].c);
				ans = ans + 1;
			}	
		}
		fclose(fp);
		fp = fopen("./stu.txt","w+");
		for(i=0;i<ans;i++){
			fprintf(fp,"%s %s %d %d %d\n",stu[i].Name,stu[i].ID,stu[i].a,stu[i].b,stu[i].c);
		}
		fclose(fp);
		f = fopen("./count.txt","w+");
		fprintf(f,"%d",ans);
		fclose(f);
		return 1;
	}
	else{
		return 0;
	}
}
int Stu_change(st student,st stutmp){
	FILE *fp = fopen("./stu.txt","r");
	char stuname[20],stuid[20];
	int cnt = 0,flag = 0;
	strcpy(stuname,student.Name);
	strcpy(stuid,student.ID);
	FILE *f = fopen("./count.txt","r");
	fscanf(f,"%d",&cnt);
	fclose(f);
	st tmp;
	int i;
	for(i=0;i<cnt;i++){
		fscanf(fp,"%s %s %d %d %d\n",tmp.Name,tmp.ID,&tmp.a,&tmp.b,&tmp.c);
		if(strcmp(tmp.Name,student.Name)==0) {flag=1;break;} 
	}
	if(flag){
		int j,ans = 0;
		for(j=0;j<cnt;j++){
			if(j!=i) {
				fscanf(fp,"%s %s %d %d %d\n",stu[ans].Name,stu[ans].ID,&stu[ans].a,&stu[ans].b,&stu[ans].c);
				ans = ans + 1;
			}	
			else{
				stu[ans] = stutmp;
				ans++;
			}
		}
		fclose(fp);
		fp = fopen("./stu.txt","w+");
		for(i=0;i<ans;i++){
			fprintf(fp,"%s %s %d %d %d\n",stu[i].Name,stu[i].ID,stu[i].a,stu[i].b,stu[i].c);
		}
		fclose(fp);
		return 1;
	}
	else{
		return 0;
	}
}
user user_screah(user u){
	FILE *f = fopen("./uscnt.txt","r");
	int cnt = 0,flag = 0;
	fscanf(f,"%d",&cnt);
	fclose(f);
	FILE *fp = fopen("./user.txt","r");
	user tmp = {"1","1"};
	//printf("cnt: %d\n",cnt);
	if(cnt){
		for(int i=0;i<cnt;i++){
			fscanf(fp,"%s %s\n",tmp.username,tmp.userpswd);
			if(strcmp(tmp.username,u.username)==0) {flag=1;break;}
		}
	}
	if(flag==0) {strcpy(tmp.username,"error");strcpy(tmp.userpswd,"error");}
	fclose(fp);
	return tmp;
}
int singin(user u){
	user tmp = user_screah(u);
	if(strcmp(tmp.username,"error")!=0) return 0;
	FILE *fp = fopen("./user.txt","a+");
	fprintf(fp,"%s %s\n",u.username,u.userpswd);
	fclose(fp);
	int cnt = 0;
	FILE *f = fopen("./uscnt.txt","r");
	fscanf(f,"%d",&cnt);
	fclose(f);
	f = fopen("./uscnt.txt","w+");
	cnt = cnt + 1;
	fprintf(f,"%d",cnt);
	fclose(f);
	return 1;
}
int singup(user u){
	user tmp = user_screah(u);
	if(strcmp(u.username,tmp.username)==0&&strcmp(u.userpswd,tmp.userpswd)==0) return 1;
	else return 0;
}
void sendmess(int id,char *mess){
	SOCKET client = client_ret_2[id];
	send(client,mess,100,0);
}
void *recvmess(int *id1){
	int id = *id1;
	//printf("%d %d\n",client_ret_add_1[id].sin_port,client_ret_add_2[id].sin_port);
	SOCKET client = client_ret_1[id];
	int zt = 0;
	char buff[1024];
	int error;
	char username[50],userpswd[50];
	while(1){
		//printf("g\n");
		sendmess(id,"注册:sin 登录:sup 增查删改:add,scr,del,cha,打印:show");
		error = recv(client,buff,1024,0);
		if(error==-1) {
			closesocket(client);
			return 0;
		}
		//printf("error: %d\n",error);
		if(error!=0){
			if(strcmp(zl[0],buff)==0){
				if(zt==1) {
					sendmess(id,"您已经处于登陆状态了");
					continue;
				}
				sendmess(id,"user: ");
				int gin = recv(client,username,50,0);
				if(gin==-1) {
					closesocket(client);
					return 0;
				}
				sendmess(id,"pswd: ");
				gin = recv(client,userpswd,50,0);
				if(error==-1) {
					closesocket(client);
					return 0;
				}
				//sendmess(id,tmp_1);
				user tr;
				strcpy(tr.username,username);
				strcpy(tr.userpswd,userpswd);
				user us = user_screah(tr);
				if(strcmp(tr.username,us.username)==0&&strcmp(us.userpswd,tr.userpswd)==0) {sendmess(id,"登录成功");zt = 1;}
				else sendmess(id,"账号或密码错误");
			}
			else if(strcmp(zl[1],buff)==0){
				sendmess(id,"user: ");
				int gin = recv(client,username,50,0);
				if(gin==-1) {
					closesocket(client);
					return 0;
				}
				sendmess(id,"pswd: ");
				gin = recv(client,userpswd,50,0);
				if(error==-1) {
					closesocket(client);
					return 0;
				}
				/*
				if(gin>0){
					printf("name: %s\n",username);
					//sendmess(id,"pwsd: ");
					recv(client,userpswd,50,0);
				}
				printf("pswd: %s\n",userpswd);
				*/
				user u;
				strcpy(u.username,username);
				strcpy(u.userpswd,userpswd);
				int fl = singin(u);
				if(fl==1){
					sendmess(id,"注册成功");
				} 
				else sendmess(id,"注册失败");
			}
			else if(strcmp(zl[2],buff)==0){
				if(zt==0){
					sendmess(id,"您需要先登陆");
					continue;
				}
				char stumess[100];
				sendmess(id,"请输入学生信息: name id a b c");
				int gin = recv(client,stumess,50,0);
				if(gin==-1){
					closesocket(client);
					return 0;
				}
				FILE *tmp = fopen("./tmp.txt","a+");
				fclose(tmp);
				tmp = fopen("./tmp.txt","w");
				fputs(stumess,tmp);
				fclose(tmp);
				tmp = fopen("./tmp.txt","r");
				st Stumess;
				fscanf(tmp,"%s %s %d %d %d",Stumess.Name,Stumess.ID,&Stumess.a,&Stumess.b,&Stumess.c);
				int fl  = Stu_add(Stumess);
				if(fl) sendmess(id,"添加成功");
			}
			else if(strcmp(zl[3],buff)==0){
				if(zt==0){
					sendmess(id,"您需要先登陆");
					continue;
				}
				sendmess(id,"查询人信息:name");
				st Stumess;
				int gin = recv(client,Stumess.Name,50,0);
				if(gin==-1){
					closesocket(client);
					return 0;
				}
				sendmess(id,"查询人信息:id");
				gin = recv(client,Stumess.ID,50,0);
				if(gin==-1){
					closesocket(client);
					return 0;
				}
				char *getmess = Stu_sreach(Stumess);
				sendmess(id,getmess);
			}
			else if(strcmp(zl[4],buff)==0){
				if(zt==0){
					sendmess(id,"您需要先登陆");
					continue;
				}
				sendmess(id,"删除人信息:name");
				st Stumess;
				int gin = recv(client,Stumess.Name,50,0);
				if(gin==-1){
					closesocket(client);
					return 0;
				}
				sendmess(id,"删除人信息:id");
				gin = recv(client,Stumess.ID,50,0);
				if(gin==-1){
					closesocket(client);
					return 0;
				}
				int fl = Stu_delete(Stumess);
				if(fl) sendmess(id,"删除成功");
				else sendmess(id,"删除失败");
			}
			else if(strcmp(zl[5],buff)==0){
				if(zt==0){
					sendmess(id,"您需要先登陆");
					continue;
				}
				sendmess(id,"更改人信息:name");
				st Stumess;
				int gin = recv(client,Stumess.Name,50,0);
				if(gin==-1){
					closesocket(client);
					return 0;
				}
				sendmess(id,"更改人信息:id");
				gin = recv(client,Stumess.ID,50,0);
				if(gin==-1){
					closesocket(client);
					return 0;
				}
				char stumess[100];
				sendmess(id,"请输入更新学生信息: name id a b c");
				gin = recv(client,stumess,50,0);
				if(gin==-1){
					closesocket(client);
					return 0;
				}
				FILE *tmp = fopen("./tmp.txt","a+");
				fclose(tmp);
				tmp = fopen("./tmp.txt","w");
				fputs(stumess,tmp);
				fclose(tmp);
				tmp = fopen("./tmp.txt","r");
				st Stutmp;
				fscanf(tmp,"%s %s %d %d %d",Stutmp.Name,Stutmp.ID,&Stutmp.a,&Stutmp.b,&Stutmp.c);
				int fl = Stu_change(Stumess,Stutmp);
				if(fl) sendmess(id,"修改成功");
				else sendmess(id,"修改失败");
			}
			else if(strcmp(zl[6],buff)==0){
				if(zt==0){
					sendmess(id,"您需要先登陆");
					continue;
				}
				FILE *fp = fopen("./stu.txt","r");
				FILE *f = fopen("./count.txt","r");
				int cnt = 0;
				fscanf(f,"%d",&cnt);
				fclose(f);
				int i;
				st stmp;
				for(i=0;i<cnt;i++){
					fscanf(fp,"%s %s %d %d %d\n",stmp.Name,stmp.ID,&stmp.a,&stmp.b,&stmp.c);
					char tmp[50];
					sprintf(tmp,"%s %s %d %d %d",stmp.Name,stmp.ID,stmp.a,stmp.b,stmp.c);
					sendmess(id,tmp);
				}
				fclose(fp);
			}
		}
	}
}
void *recvsock(){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET server = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN server_add;
	server_add.sin_family = AF_INET;
	server_add.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
	server_add.sin_port = htons(5000);
	bind(server,(SOCKADDR*)&server_add,sizeof(SOCKADDR));
	listen(server,20);
	while(1){
		sockid_1 = sockid_1 + 1;
		//printf("s1: %d\n",sockid_1);
		int len = sizeof(client_ret_add_1[sockid_1]);
		client_ret_1[sockid_1] = accept(server,(SOCKADDR*)&client_ret_add_1[sockid_1],&len);
		pthread_t t;
		//sk clientst = {client_ret_1[sockid_1],sockid_1};
		pthread_create(&t,NULL,recvmess,&sockid_1);
		pthread_join(t,NULL);
	}
}
void *sendsock(){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET server = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN server_add;
	server_add.sin_family = AF_INET;
	server_add.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
	server_add.sin_port = htons(6000);
	bind(server,(SOCKADDR*)&server_add,sizeof(SOCKADDR));
	listen(server,20);
	while(1){
		sockid_2 = sockid_2 + 1;
		//printf("s2: %d\n",sockid_2);
		int len = sizeof(client_ret_add_2[sockid_2]);
		client_ret_2[sockid_2] = accept(server,(SOCKADDR*)&client_ret_add_2[sockid_2],&len);
	}
}
int main(){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	pthread_t t,v;
	pthread_create(&t,NULL,recvsock,NULL);
	pthread_create(&v,NULL,sendsock,NULL);
	pthread_join(t,NULL);
	pthread_join(v,NULL);
}
