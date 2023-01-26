#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdio_ext.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#define PORT 7001

union second_menu
{
	unsigned int depo;
	unsigned int withdraw;
	unsigned int bal;
}req;
struct communication
{
	int option;
	int status;
};
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
};
int main()
{
	int sockfd,ret,success=1;
	struct sockaddr_in serv;
	int opt;
	struct login validate;
	struct registration newuser;
	struct communication call;
	sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sockfd==-1)
	{
		printf("ERROR:failed to create socket\n");
		exit(1);
	}
	bzero(&serv,sizeof(serv));
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv.sin_port=htons(PORT);
	serv.sin_family=AF_INET;
	ret=connect(sockfd,(struct sockaddr *)&serv,sizeof(serv));
	if(ret==-1)
	{
		printf("failed to connect\n");
	}
	else
	{
		printf("server is up\n");
	}
	while(1)
	{
L1:
		printf("select the option from menu\n");
		printf("1.LOGIN\n2.REGISTER\n3.EXIT\n");
		scanf("%d",&opt);
		__fpurge(stdin);
		switch(opt)
		{
			case 1:
				printf("selected LOGIN\n");
				printf("Enter the user id\n:");
				scanf("%s",validate.user_id);
				__fpurge(stdin);
				printf("Enter the password\n:");
				scanf("%s",validate.password);
				__fpurge(stdin);
				call.option=opt;
				ret=send(sockfd,&call,sizeof(call),0);
				if(send<0)
				{
					printf("failed to send communication\n");
					exit(1);
				}

				ret=send(sockfd,&validate,sizeof(validate),0);
				if(send<0)
				{
					printf("failed to send\n");
					exit(1);
				}
				ret=recv(sockfd,&call,sizeof(call),0);
				if(ret==-1)
				{
					printf("failed to recieve\n");
					exit(2);
				}
				if(call.status==0)
				{
					printf("logged in successfully\n");
					success=0;
					goto R2;
				}
				else
				{
					printf("user not found\n");
				}
				break;


			case 2:
				call.option=opt;
				ret=send(sockfd,&call,sizeof(call),0);
				if(send<0)
				{
					printf("failed to send communication\n");
					exit(1);
				}
				printf("selected REGISTER\n");
				printf("Enter the username\n:");
				scanf("%s",newuser.username);
				__fpurge(stdin);
				printf("\nEnter the user id\n:");
				scanf("%s",newuser.user_id);
				__fpurge(stdin);
				send(sockfd,&newuser,sizeof(newuser),0);
				recv(sockfd,&call,sizeof(call),0);
				printf("call :%d\n",call.status);
				if(call.status==0)
				{
					printf("USER ALREADY EXISTS \n\n");
					goto L1;
				}
				printf("\nEnter the password\n:");
				scanf("%s",newuser.password);
				__fpurge(stdin);
				printf("\nEnter the aadhar number\n:");
				scanf("%lu",&newuser.aadhar_no);
				__fpurge(stdin);
				printf("\nEnter the mobile number\n:");
				scanf("%lu",&newuser.mobile_no);
				__fpurge(stdin);
				printf("\nEnter the dateofbirth in dd/mm/yy format\n");
				scanf("%d/%d/%d",&newuser.user.day,&newuser.user.month,&newuser.user.year);
				__fpurge(stdin);
				printf("\nEnter the address\n Houseno:");
				scanf("%[^\n]s",newuser.user_ad.house_no);
				__fpurge(stdin);
				printf("\n area:");
				scanf("%[^\n]s",newuser.user_ad.area);
				__fpurge(stdin);
				printf("\n District:");
				scanf("%[^\n]s",newuser.user_ad.district);
				__fpurge(stdin);
				printf("\n pincode:");
				scanf("%[^\n]s",newuser.user_ad.pincode);
				__fpurge(stdin);
				printf("enter nominee details\nnominee name:");
				scanf("%[^\n]s",newuser.user_n.nominee_name);
				__fpurge(stdin);
				printf("\n no minee mobile no:");
				scanf("%lu",&newuser.user_n.nominee_mobileno);
				__fpurge(stdin);
				newuser.balance=0;
				newuser.account_no=newuser.aadhar_no+newuser.mobile_no;
				/*ret=send(sockfd,&call,sizeof(call),0);
				if(send<0)
				{
					printf("failed to send communication\n");
					exit(1);
				}*/
				ret=send(sockfd,&newuser,sizeof(newuser),0);
				if(send<0)
				{
					printf("failed to send\n");
					exit(1);
				}
				break;

			case 3:
				printf("selected EXIT\n");
				exit(0);
				break;
			default:
				printf("selected invalid option\n");
				break;
		}
	}
	if(success==0)
	{
R2:
		printf("\n\n\noperations MENU\n");
		printf("1.DEPOSIT\n2.WITHDRAW\n3.BALANCE\n4.TRANSACTION HISTORY\n5.PROFILE INFORMATION\n6.LOGOUT\n");
		int opt2;
		printf("selectoption from menu\n");
		scanf("%d",&opt2);
		switch(opt2)
		{
			case 1:
				printf(" entered DEPOSIT\n");
				printf("enter the amount to deposit\n");
				scanf("%d",&req.bal);
				call.option=opt2;
				send(sockfd,&call,sizeof(call),0);
				send(sockfd,&req,sizeof(req),0);
				recv(sockfd,&call,sizeof(call),0);
				if(call.status==0)
				{
					printf("deposited successfully\n");
				}
				else
				{
					printf("enter the amount greater than 0\n");
				}

				break;
			case 2:
				printf(" entered WITHDRAW\n");
				call.option=opt2;
				send(sockfd,&call,sizeof(call),0);
				recv(sockfd,&req,sizeof(req),0);
				break;
			case 3:
				printf(" entered BALANCE\n");
				call.option=opt2;
				send(sockfd,&call,sizeof(call),0);
				recv(sockfd,&req,sizeof(req),0);
				break;
			case 4:
				printf(" entered TRANSACTION HISTORY\n");
				call.option=opt2;
				send(sockfd,&call,sizeof(call),0);
				//recv(sockfd,&call,sizeof(call),0);
				break;
			case 5:
				printf(" entered PROFILE INFO\n");
				call.option=opt2;
				send(sockfd,&call,sizeof(call),0);
				//recv(sockfd,&call,sizeof(call),0);
				break;
			case 6:
				printf(" entered LOGOUT\n");
				goto L1;
				break;
			default:
				printf(" select valid option\n");
				break;

		}
	}
}
