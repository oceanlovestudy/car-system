#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
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
#include "data_struct_list.h"
#include "indent_struct.h"
#include "package.h"
#include "heart_package.h"
#include "server.h"
#include "dri_cli.h"

#define SER_PORT 6000
#define SER_IP ("127.0.0.1")
#define SER_COUNT 30
#define VERSION ("V1.0")
 
int sockfd;
struct Heart_package dri_heart;   //司机的心跳包
/*驾驶员端主函数*/
int dri_cli_main()
{
    struct sockaddr_in ser_in;
    int ret;
    int operate;		    //操作命令
    struct Header_data head;	    //数据包头
    struct Driver_info d_info;	    //司机个人信息
    struct Driver_RT d_rt;	    //司机实时信息
    struct Login_test acc;	    //司机登录信息
    struct Indent dri_in;	    //接单信息
    char* p;
    char command[30];		    //命令

    fd_set set, rset;		    //套接字集合
    FD_ZERO(&set);
    FD_SET(0, &set);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);	//tcp连接
    if(sockfd < 0)
    {
	perror("驾驶员端socket创建失败!\n");
	exit(1);
    }

    /*填充服务端IP和PORT信息*/
    ser_in.sin_family = AF_INET;
    ser_in.sin_port = htons(SER_PORT);
    inet_pton(AF_INET, SER_IP, &ser_in.sin_addr.s_addr);
    ret = connect(sockfd, (struct sockaddr*)&ser_in, sizeof(ser_in));
    if(ret < 0)
    {
	perror("驾驶员端connect连接失败!\n");
	exit(1);
    }
    /*把套接字插入集合中*/
    FD_SET(sockfd, &set);
    for(;;)
    {
	rset = set;
	ret = select(sockfd+1, &rset, NULL, NULL, NULL);
	printf("请输入是否执行：exit(qiut)或getdata\n");
	if(FD_ISSET(0, &rset))
	{
	    memset(command, 0, sizeof(command));
	    //接收数据
	    ret = read(0, command, sizeof(command));
	    if(ret < 0)
	    {
		printf("驾驶员端接收命令失败!\n");
		continue;
	    }
	    else if(ret == 0)
	    {
		printf("驾驶员端无法接收数据,与服务器断开连接!\n");
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
		printf("等待驾驶员端接入...驾驶员端发送数据...\n");
		FD_ISSET(sockfd, &rset);
		goto Data;
	    }
	    system(command);
	    continue;
	}
	else if(FD_ISSET(sockfd, &rset))
	{
	    //发送数据
Data:	    /*驾驶员端界面选择函数*/
	    dri_interface();
	    scanf("%d", &operate);
	    if(operate == 1)
	    {
		register_driver(&d_info);	//注册信息	
		p = package(VERSION, DRI_CLI_REG, sizeof(d_info), &d_info);
		ret = write(sockfd, p, HEADER_SIZE+sizeof(d_info));
		if(ret < 0)
		{
		    printf("驾驶员端注册信息发送失败!\n");
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
		printf("******************************\n");
		p = package(VERSION, DRI_CLI_LOG, sizeof(acc), &acc);
		ret = write(sockfd, p, HEADER_SIZE+sizeof(acc));
		if(ret < 0)
		{
		    printf("驾驶员端登录信息发送失败!\n");
		    exit(1);
		}
		free(p);
    	    }
    
	    //接收数据
	    ret = read(sockfd, &head, HEADER_SIZE);
	    if(ret < 0)
	    {
		printf("驾驶员端接收信息失败！\n");
		exit(1);
	    }
	    else if(ret == 0)
	    {
		printf("驾驶员端与服务器断开连接!\n");
		exit(1);
	    }
	    printf("从服务端发来消息类型:%d\n", head.type);
	    if(operate == 1)
	    {
		if(head.type == DRI_CLI_REG_OK)
		{
		    printf("恭喜您注册成功!\n");
		}
		else if(head.type == DRI_CLI_REG_FL)
		{
		    printf("您注册失败!\n");
		}
	    }
	    else if(operate == 2)
	    {
		if(head.type == DRI_CLI_LOG_OK)
		{
		    //重新接收数据，接收后面的数据
		    ret = read(sockfd, &(d_info), sizeof(d_info));
		    printf("恭喜您登录成功!\n");
		    //登录状态1-在线 0-离线
		    d_info.status = 1;
		    printf("登录成功驾驶员的信息:帐号:%s姓名:%s\n", d_info.number, d_info.name); 
		    get_dri_rt(&d_rt);
		    strcpy(dri_heart.number,d_info.number);
		    dri_heart.cfd = sockfd;
		    dri_heart.position_x = d_rt.position_x;
		    dri_heart.position_y = d_rt.position_y;
		    signal(SIGALRM, sighandle);
		    alarm(5);
		    dri_login_interface();
		    while(1)
		    {
			/*司机登录后的选择函数的实现*/
			achieve_dri_login_interface(&dri_in,&d_info,&d_rt,sockfd, &head);			     
			dri_login_interface();
		    }
		}
		else if(head.type == DRI_CLI_LOG_FL)
		{
		    printf("您登录失败!\n");
		}
	    }
	}
    }
    close(sockfd);
    return 0;
}

/*司机端选择界面函数*/
void dri_interface()
{
    system("clear");
    printf("欢迎您来到嘟嘟打车驾驶员端\n");
    printf("请输入您想要进行的操作:\n");
    printf("1-注册 2-登录\n");
}

/*司机登录后的选择界面函数*/
void dri_login_interface()
{
    printf("欢迎您登录嘟嘟打车驾驶员端\n");
    printf("请输入您想要的服务:\n");
    printf("1-接单 2-查看司机实时信息 3-查看个人信息\n");
    printf("4-修改个人信息 5-查看版本信息 6-退出嘟嘟打车驾驶员端\n");
}
 

/*信号接收函数*/
void sighandle(int sig)
{
    int ret;
    char* p;
    p = package(VERSION, DRI_HEART_PACKAGE, sizeof(Heart_package), &dri_heart);
    ret = write(sockfd, p, HEADER_SIZE+sizeof(dri_heart));
    if(ret < 0)
    {
	printf("驾驶员端登录信息发送失败!\n");
	exit(1);
    }
    free(p);
    alarm(5);
    return ;
}

/*司机登录后的选择函数的实现*/
int achieve_dri_login_interface(struct Indent* dri_in, struct Driver_info* d_info, struct Driver_RT* d_rt, int sockfd, struct Header_data* head)
{
    int ret;
    char* p;
    int operate;
    printf("请输入服务操作:\n");
    scanf("%d", &operate);
    switch(operate)
    {
	case 1:
	    //接单
	    printf("司机正在接单...\n");
	    ret = read(sockfd, head, HEADER_SIZE);
	    if(head->type == DRI_ACCEPT_ORDER)
	    {
		ret = read(sockfd, dri_in, sizeof(struct Indent));
		//接单司机增加订单信息
		printf("开始完成...\n");
		dri_add_indent_info(dri_in, d_info, d_rt);
		printf("司机行车中的订单信息\n");
		show_indent_signal(dri_in);
		//信息添加完毕
		sleep(100);
		printf("订单完成...\n");
		//添加订单完成信息
		end_add_indent_info(dri_in);	
		printf("司机发送给服务器的消息\n");
		show_indent_signal(dri_in);
		//发送订单信息 
		p = package(VERSION, DRI_CLOSE_ORDER, sizeof(struct Indent), dri_in);
		ret = write(sockfd, p, HEADER_SIZE+sizeof(struct Indent));
		free(p);
	    }
	    break;
	case 2:
	    //查看司机的实时信息
	    show_driver_rt(d_rt);
	    break;
	case 3:
	    //查看个人信息
	    show_driver_info(d_info);
	    break;
	case 4:
	    //修改个人信息
	    modify_driver_info(d_info);
	    break;
	case 5:
	    //查看版本信息
	    printf("版本信息:%s\n", VERSION);
	    printf("版权所有:ocean\n");
	    break;
	case 6:
	    //退出嘟嘟打车驾驶员端
	    printf("您将退出嘟嘟打车驾驶员端！正在退出...\n");
	    break;
	default:
	    printf("输入操作错误,请重新输入您的操作命令!\n");
	    break;
    }
    return 0;
}


/*司机接单后的选择界面函数*/
void dri_appointtment_interface()
{
    system("clear");
    printf("欢迎您查看接单情况-嘟嘟打车驾驶员端\n");
    printf("请输入您想要的服务:\n");
    printf("1-查看下单客户的基本信息\n");
    printf("2-查看自己与客户的距离信息(实时更新)\n");
    printf("3-与客户进行文字通话\n");
    printf("4-关闭订单(订单已经完成,服务器产生费用,并返回上一级菜单)\n");
}
