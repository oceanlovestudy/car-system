#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <signal.h>

#include "driver_client_struct.h"
#include "driver_client_func.h"
#include "passenger_client_struct.h"
#include "passenger_client_func.h"
#include "indent_struct.h"
#include "heart_package.h"
#include "package.h"
#include "server.h"
#include "pas_cli.h"

#define SER_PORT 6000
#define SER_IP ("127.0.0.1")
#define SER_COUNT 30
#define VERSION ("V1.0")

int sockfd;
struct Heart_package pas_heart;	    //乘客的心跳包
struct Indent pas_in_history[10];   //下单的历史信息
struct Header_data head;	    //数据包头
int pas_cli_main()
{
    struct sockaddr_in ser_in;
    int ret;
    int cnt;
    int fd;			    //管道文件描述符
    int operate;		    //操作命令
    struct Passenger_info p_info;   //客户个人信息
    struct Passenger_RT p_rt;	    //客户实时信息
    struct Login_test acc;	    //客户登录信息
    struct Indent pas_in;	    //下单信息
    pas_in.indent_number = 0;
    char* p;
    char command[30];		    //命令

    fd_set set, rset;
    FD_ZERO(&set);
    FD_SET(0, &set);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
	perror("客户端socket创建失败!\n");
	exit(1);
    }

    /*填充服务端IP和PORT信息*/
    ser_in.sin_family = AF_INET;
    ser_in.sin_port = htons(SER_PORT);
    inet_pton(AF_INET, SER_IP, &ser_in.sin_addr.s_addr);
    ret = connect(sockfd, (struct sockaddr*)&ser_in, sizeof(ser_in));
    if(ret < 0)
    {
	perror("客户端connect连接失败!\n");
	exit(1);
    }
    /*把套接字插入集合中*/
    FD_SET(sockfd, &set);
    for(;;)
    {
	rset = set;
	ret = select(sockfd+1, &rset, NULL, NULL, NULL);
	if(FD_ISSET(0, &rset))
	{
	    printf("请输入是否执行：exit(qiut)或getdata\n");
	    memset(command, 0, sizeof(command));
	    //接收数据
	    ret = read(0, command, sizeof(command)-1);
	    if(ret < 0)
	    {
		printf("客户端接收命令失败!\n");
		continue;
	    }
	    else if(ret == 0)
	    {
		printf("客户端无法接收数据,与服务器断开连接!\n");
		continue;
	    }
	    command[ret-1] = '\0';
	    if(strcmp(command, "exit")==0 ||
		    strcmp(command, "quit")==0)
	    {
		close(sockfd);
		return 0;
	    }
	    if(strcmp(command, "getdata")==0)
	    {
		printf("等待客户端接入...客户端发送数据...\n");
		FD_ISSET(sockfd, &rset);
		goto Data;
	    }
	    system(command);
	    continue;
	}
	else if(FD_ISSET(sockfd, &rset))
	{
	    //发送数据
Data:	    /*客户端界面选择函数*/
	    pas_interface();
	    scanf("%d", &operate);
	    if(operate == 1)
	    {
		register_passenger(&p_info);	//注册信息	
		p = package(VERSION, PSE_CLI_REG, sizeof(p_info), &p_info);
		ret = write(sockfd, p, HEADER_SIZE+sizeof(p_info));
		if(ret < 0)
		{
		    printf("客户端注册信息发送失败!\n");
		    exit(1);
		}
		free(p);
	    }
	    else if(operate == 2)
	    {
		printf("请输入登录帐号：\n");
		scanf("%s", acc.num);
		printf("请输入登录密码：\n");
		scanf("%s", acc.pass);
		printf("************************\n");
		p = package(VERSION, PSE_CLI_LOG, sizeof(acc), &acc);
		ret = write(sockfd, p, HEADER_SIZE+sizeof(acc));
		if(ret < 0)
		{
		    printf("客户端登录信息发送失败!\n");
		    exit(1);
		}
		free(p);
    	    }
	    
	    //接收数据	       
	    ret = read(sockfd, &head, HEADER_SIZE);
	    if(ret < 0)
	    {
		printf("客户端接收信息失败！\n");
		exit(1);
	    }
	    else if(ret == 0)
	    {
		printf("客户端与服务器断开连接!\n");
		exit(1);
	    }
	    printf("从服务端发来的消息类型:%d\n", head.type);
	    if(operate == 1)
	    {
		if(head.type == PSE_CLI_REG_OK)
		{
		    printf("恭喜您注册成功!\n");
		}
		else if(head.type == PSE_CLI_REG_FL)
		{
		    printf("您注册失败!\n");
		}
	    }
	    else if(operate == 2)
	    {
		if(head.type == PSE_CLI_LOG_OK)
		{
		    //重新接收数据，接收后面的数据
		    ret = read(sockfd, &(p_info), sizeof(p_info));
		    printf("恭喜您登录成功!\n");

		    //登录状态1-在线 0-离线
		    p_info.status = 1;
		    //下单状态
		    p_info.order_indent = 0;
		    printf("登录成功用户的信息:帐号:%s姓名:%s\n", p_info.number, p_info.name);
		    get_pas_rt(&p_rt);
		    strcpy(pas_heart.number, p_info.number);
		    pas_heart.cfd = sockfd;
		    pas_heart.position_x = p_rt.position_x;
		    pas_heart.position_y = p_rt.position_y;
		    signal(SIGALRM, sighandle);
		    alarm(5);
		    system("clear");
		    while(1)
		    {
Login_pas:
			pas_login_interface();
			cnt = achieve_pas_login_interface(&p_info, &p_rt, &pas_in);	
			if(cnt == 1)
			{
			    printf("您的订单已经发出，等待司机接单.....\n");
			    head.type = PSE_PLACE_ORDER; 
			    p = package(VERSION, PSE_PLACE_ORDER, \
				    sizeof(pas_in), &pas_in);
			    printf("从乘客端发送出去的订单信息:\n");
			    show_indent_signal(&pas_in);
			    ret = write(sockfd, p, HEADER_SIZE+sizeof(pas_in));
			    if(ret < 0)
			    {
				printf("乘客订单信息发送失败!\n");
				exit(1);
			    }
			    free(p);

			    //接收数据
			    printf("正在接收订单完成的消息...\n");
			    ret = read(sockfd, &head, HEADER_SIZE);
			    if(ret < 0)
			    {
				printf("乘客接收订单完成信息失败!\n");
				exit(1);
			    }
			    else if(ret == 0)
			    {
				printf("乘客与服务器断开连接!\n");
				exit(1);
			    }
			    printf("订单完成的消息%d\n", head.type);
			    if(head.type == PSE_END_ORDER)
			    {
				printf("订单完成！\n");
				ret = read(sockfd, &pas_in, sizeof(pas_in));
				show_indent_signal(&pas_in);
				goto Login_pas;
			    }
			}
			else if(cnt == 6)
			{
			    break;
			}
		    }
		}
		else if(head.type == PSE_CLI_LOG_FL)
		{
		    printf("您登录失败!\n");
		}
	    }
	}
    }
    close(sockfd);
    return 0;
}

/*客户端界面选择函数*/
void pas_interface()
{
    system("clear");
    printf("欢迎您来到嘟嘟打车客户端\n");
    printf("请输入您想要进行的操作:\n"); 
    printf("1-注册 2-登录\n");

    return ;
}

/*客户登录后的界面选择函数*/
void pas_login_interface()
{
    printf("欢迎您登录嘟嘟打车客户端\n");
    printf("请输入您想要的服务:\n"); 
    printf("1-约车 2-查看历史约车记录 3-查看个人信息\n");
    printf("4-修改个人信息 5-查看版本信息 6-退出嘟嘟打车客户端\n");
    
    return ;
}

/*信号接收函数*/
void sighandle(int sig)
{
     int ret;
     char* p;
     p = package(VERSION, PSE_HEART_PACKAGE, sizeof(Heart_package), &pas_heart);
     ret = write(sockfd, p, HEADER_SIZE+sizeof(pas_heart));
     if(ret < 0)
     {
        printf("乘客端登录信息发送失败!\n");
	exit(1);
     }
     free(p);
     alarm(5);
     return ;
}

/*实现客户登录后的界面选择函数*/
int achieve_pas_login_interface(struct Passenger_info* p_info, struct Passenger_RT* p_rt, struct Indent* in)
{
    int i = 0;
    int count = 0;
    int time = 0;
    char temp_num[20];
    struct Indent temp_in;
    strcpy(temp_num, p_info->number);

    int ret;
    char* p;
    int operate;
    printf("请输入服务操作:\n");
    scanf("%d", &operate);
    switch(operate)
    {
	case 1:
	    //约车 
	    cli_add_indent_info(in, p_info);
	    if(in->indent_number != 0)
	    {
		return 1;
	    }
	    else
	    {
		break;
	    }
	case 2:
	    //查看历史订单信息
	    printf("乘客%s的历史订单信息\n", temp_num);
	    p = package(VERSION, PSE_SELECT_HISTORY, \
		    sizeof(temp_num), &temp_num);
	    ret = write(sockfd, p, HEADER_SIZE+sizeof(temp_num));
	    free(p);
	    for(i = 0; i<10; i++){pas_in_history[i].indent_number = 0;}
	    printf("正在接收历史订单完成的消息...\n");
	    while(1)
	    {
		ret = read(sockfd, &head, HEADER_SIZE);
		printf("消息类型:%d-乘客的历史订单消息!\n", head.type);
		if(ret == 0 || ret < 0)
		{
		    printf("read fail!\n");
		    break;
		}
		if(head.type == PSE_SELECT_HISTORY_FL){break;}
		else if(head.type == PSE_SELECT_HISTORY_OK)
		{
		    printf("first->count:%d\n", count);
		    ret = read(sockfd, &temp_in, \
			    sizeof(struct Indent));	
		    memcpy(&pas_in_history[count], &temp_in, \
			    sizeof(struct Indent));
		    count++;
		    printf("after->count:%d\n", count);
		    if(ret == 0 || ret < 0 || count==1)
		    {
			break;
		    }
		}
	    }
	    printf("历史订单信息如下：\n");
	    for(i = 0; i < count; i++)
	    {
		show_indent_signal(&pas_in_history[i]);
	    }
	    break;
	case 3:
	    //查看个人信息
	    show_passenger_info(p_info);
	    break;
	case 4:
	    //修改个人信息
	    modify_passenger_info(p_info);
	    break;
	case 5:
	    //查看版本信息
	    printf("版本信息:%s\n", VERSION);
	    printf("版权所有:ocean\n");
	    break;
	case 6:
	    //退出嘟嘟打车客户端
	    printf("您将退出嘟嘟打车客户端！正在退出...\n");
	    return 6;
	    break;
	default:
	    printf("输入操作错误,请重新输入您的操作命令！\n");
	    break;
    }
    return 0;
}

/*客户约车的选择界面函数*/
void pas_appointment_interface()
{
    system("clear");
    printf("欢迎您预约嘟嘟打车客户端\n");
    printf("请输入您想要服务:\n"); 
    printf("1-下单(服务器选择离客户最近的未接单的司机接单)\n");
    printf("2-查看接单司机的基本信息\n");
    printf("3-查看接单司机与自己的距离信息(实时更新)\n");
    printf("4-与接单司机进行文字通话\n");
    printf("5-查看全部司机的基本信息\n");
    printf("6-退出约车(订单已经完成或者退出约车界面返回上一级界面菜单)\n");
    
    return ;
}
