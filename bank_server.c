#include<stdio.h>
#include<dirent.h>
#include<fcntl.h>
#include<stdlib.h>
#include <sys/stat.h>
#include<string.h>
#include<dirent.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#define PORT 7001
union second_menu
{
        unsigned int depo;
        unsigned int withdraw;
        unsigned int bal;
}service;

struct communication
{
	int option;
	int status;
}call;

struct login
{
	char user_id[100];
	char password[10];
};
struct address
{
	char house_no[10];
	char area[20];
	char district[15];
	char pincode[7];
};
struct nominee
{
	char nominee_name[32];
	unsigned long int nominee_mobileno;
};
struct dob
{
	int day;
	int month;
	int year;
};
struct registration
{
	char username[32];
	char user_id[100];
	char password[10];
	unsigned long int account_no;
	unsigned long int aadhar_no;
	unsigned long int mobile_no;
	struct dob user;
	struct address user_ad;
	struct nominee user_n;
	unsigned int balance;
}var;
void directory_check();
void deposit();
void withdraw();
void balance();
void transaction();
void profileinfo();
int profilefd,transactfd;
int main(void)
{
	int sockfd,newsockfd,client_size,ret,opt;
	struct sockaddr_in serv,client;
	struct login validate;
	DIR *dir;
	dir=opendir("//home/surya/Desktop/Bankdatabase/");
	if(dir==NULL)
	{
	    mkdir("/home/surya/Desktop/Bankdatabase/",0777);
	}
	 
	sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sockfd==-1)
	{
		printf("failed to create socket\n");
		exit(1);
	}
	bzero(&serv,sizeof(struct sockaddr_in));
	serv.sin_addr.s_addr=INADDR_ANY;
	serv.sin_port=htons(PORT);
	serv.sin_family=AF_INET;
	ret=bind(sockfd,(struct sockaddr *)&serv,sizeof(serv));
	if(ret==-1)
	{
		printf("failed to bind\n");
		exit(2);
	}
	listen(sockfd,1);
	client_size=sizeof(client);
	newsockfd=accept(sockfd,(struct sockaddr *)&client,&client_size);
	if(newsockfd==-1)
	{
		printf("failed to accept\n");
		exit(1);
	}
l1:
	ret=recv(newsockfd,&call,sizeof(call),0);
	if(ret==-1)
	{
		printf("failed to recieve\n");
		exit(2);
	}
	opt=call.option;
	//printf("opt:%d",opt);
	switch(opt)
	{
		case 1:
			printf("hi\n");
			recv(newsockfd,&validate,sizeof(validate),0);
			directory_check(validate.user_id);
			if(send(newsockfd,&call,sizeof(call),0)==-1)
				printf("failed to send\n");
			goto l1;
			break;


		case 2:
			{
				printf("registration\n");
				recv(newsockfd,&var,sizeof(var),0);
				directory_check(var.user_id);
				//printf("call:%d",call.status);
				if(call.status==0)
				{
					send(newsockfd,&call,sizeof(call),0);
					break;
				}
				else
				{
					send(newsockfd,&call,sizeof(call),0);
				}
				recv(newsockfd,&var,sizeof(var),0);
				chdir("/home/engineer/Desktop/Bankdatabase/");
				mkdir(var.user_id,0777);
				chdir(var.user_id);
				profilefd=open("profile.txt",O_RDWR | O_CREAT,0777 );
				transactfd=open("transct.txt",O_RDWR | O_CREAT,0777 );
				write(profilefd,&var,sizeof(var));

			}
	}
	ret=recv(newsockfd,&call,sizeof(call),0);
	if(ret==-1)
	{
		printf("failed to recieve\n");
		exit(2);
	}
	opt=call.option;
	switch(opt)
	{
		case 1:
			printf(" entered DEPOSIT\n");
			recv(newsockfd,&service,sizeof(service),0);
			if(service.depo<0)
			{
				call.status=1;
				if(send(newsockfd,&call,sizeof(call),0)==-1)
					printf("failed to send\n");
			}
			else
			{
			deposit(service.depo);
				call.status=0;
				if(send(newsockfd,&call,sizeof(call),0)==-1)
				printf("failed to send\n");

			}


			break;
		case 2:
			printf(" entered WITHDRAW\n");
			recv(newsockfd,&var,sizeof(var),0);
			break;
		case 3:
			printf(" entered BALANCE\n");
			recv(newsockfd,&var,sizeof(var),0);
			break;
		case 4:
			printf(" entered TRANSACTION HISTORY\n");
			recv(newsockfd,&var,sizeof(var),0);
			break;
		case 5:
			printf(" entered PROFILE INFO\n");
			recv(newsockfd,&var,sizeof(var),0);
			break;
	}
	close(newsockfd);
}
void directory_check(char *str)
{
	DIR *d;
	struct dirent *dir;
	d=opendir("/home/surya/Desktop/Bankdatabase");
	if(d)
	{
		while((dir=readdir(d))!=NULL)
		{
			if((strcmp(str,dir->d_name))==0)
			{
				//printf("Found\n");
				call.status=0;
				break;
			}
			else
			{	
				//printf("Not found\n");
				call.status=1;
			}
		}
		closedir(d);
	}
}
void deposit(int num)
{
	chdir("/home/surya/Desktop/Bankdatabase/");
	chdir(var.user_id);
	profilefd=open("profile.txt",O_RDWR | O_CREAT,0777 );
	transactfd=open("transct.txt",O_RDWR | O_CREAT,0777 );
	read(profilefd,&var,sizeof(var));
	var.balance=var.balance+num;
	write(profilefd,&var,sizeof(var));
	return;
}





