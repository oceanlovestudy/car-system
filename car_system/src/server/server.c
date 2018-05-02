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
#include <math.h>

#include "driver_client_struct.h"
#include "driver_client_func.h"
#include "passenger_client_struct.h"
#include "passenger_client_func.h"
#include "indent_struct.h"
#include "package.h"
#include "heart_package.h"
#include "data_struct_list.h"
#include "server.h"
#include "pas_cli.h"
#include "dri_cli.h"

#define SER_PORT 6000		    //服务器端口号
#define SER_IP ("127.0.0.1")	    //服务器IP
#define SER_COUNT 30		    //服务器监听和epoll监听事件的最大个数
#define VERSION ("V1.0")	    //版本信息
#define EPO_CNT 256		    //epoll监听的最大个数
#define COUNT 100		    //记录信息的最大个数

int pas_count = 0;		    //客户个数
int dri_count = 0;		    //驾驶员个数
int in_count = 0;		    //订单个数
struct List* pas_list = NULL;	    //客户个人信息链表
struct List* dri_list = NULL;	    //驾驶员个人信息链表
struct List* in_list = NULL;	    //已经完成的订单信息链表
struct List* temp_in_list = NULL;   //临时订单信息
struct List* in_list_history = NULL;//历史订单信息
struct Online pas_online[10];	    //在线乘客
struct Online dri_online[10];	    //在线司机
int pas_online_count = 0;
int dri_online_count = 0;

struct Heart_package pas_heart_pack[10];//存放心跳包
struct Heart_package dri_heart_pack[10];//存放心跳包
int dri_i = 0;
int pas_i = 0;
//struct Dri_to_Pas dis;		    //存放司机与下单客户的距离

int server_main()
{
    struct sockaddr_in ser_in;	//服务端IP和端口号信息
    struct sockaddr_in cli_in;	//客户端IP和端口号信息
    int addrlen;		//地址大小

    int listenfd;		//服务端的监听套接字
    int connfd;			//新的与客户端连接的套接字
    /*已连接客户端套接字,用以触发读事件,接收数据,读sockfd*/
    int sockfd;		
    /*nfds为epoll触发事件的个数,触发的事件将会被放置到events数组中*/
    int nfds;	    
    
    int ret;			//API函数返回值
    int epfd;			//epoll套接字
    struct epoll_event ev;  //epoll监控事件信息(监听的套接字，读/写事件) 
    struct epoll_event events[SER_COUNT];   //触发的事件记录在events数组
    struct Header_data head;	//数据包头
    struct Login_test acc;	//用户(客户，司机)验证登录
    
    char* p = NULL;		//用以指向数据包的指针p
    char buf_type[20];		//存放消息类型 
    char command[30];		//存放命令
    char IP[30];		//存放IP
    int PORT;			//存放PORT
    strcpy(head.version, VERSION);	//版本信息
    
    int i;			//遍历存放信息的数组
    int pos;			//保存登录用户在数组中的位置
    struct Passenger_info pas_temp;	//临时存放客户个人信息
    struct Driver_info dri_temp;	//临时存放驾驶员个人信息
    struct Indent in_temp;		//临时存放订单信息
    struct Passenger_RT pas_temp_rt;	//临时存放客户实时信息
    struct Driver_RT dri_temp_rt;	//临时存放驾驶员实时信息
    struct Passenger_info* p_temp = NULL;
    struct Driver_info* d_temp = NULL;
    struct Indent* i_temp = NULL;
    struct Passenger_RT* p_rt = NULL;
    struct Driver_RT* d_rt = NULL;
    struct Heart_package p_heart;//存放心跳包
    struct Heart_package d_heart;//存放心跳包

    char temp_number[20];		//临时存放帐号
    struct Indent temp_in_history;	//临时存放订单信息
    struct List* temp_l;		//指向临时信息节点
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0)
    {
	perror("创建服务端监听套接字listenfd失败！\n");
	exit(1);
    }

    /* 将套接字与IP地址和端口号进行绑定 */
    ser_in.sin_family = AF_INET;
    ser_in.sin_port = htons(SER_PORT);
    inet_pton(AF_INET, SER_IP, &ser_in.sin_addr.s_addr);
    bzero(&(ser_in.sin_zero), 8);
    /*绑定*/
    ret = bind(listenfd, (struct sockaddr*)&ser_in, sizeof(ser_in));
    if(ret < 0)
    {
	perror("listenfd套接字绑定失败!\n");
	exit(1);
    }

    /*监听模式*/
    ret = listen(listenfd, SER_COUNT);
    if(ret < 0)
    {
	perror("listenfd设置监听模式错误！\n");
	exit(1);
    }
    /*创建epoll套接字*/
    epfd = epoll_create(EPO_CNT);
    /*设置监听套接字的epoll事件为读事件*/
    ev.data.fd = listenfd;
    ev.events = EPOLLIN|EPOLLET;
    /*将监听套接字加入到epoll监控事件*/
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
    addrlen = sizeof(cli_in);
    for(;;)
    {
	/*epoll等待监控事件触发*/
	/*nfds为epoll触发事件的个数,触发的事件将会被放置到events数组中*/
	nfds = epoll_wait(epfd, events, SER_COUNT, -1);
	for(i = 0; i < nfds; i++)
	{
	    if(events[i].data.fd == listenfd)/*监听套接字事件触发，有链接请求*/
	    {
		connfd = accept(listenfd, (struct sockaddr*)&cli_in, &addrlen);	
		/* 建立新的套接字与客户端连接 */
		if(connfd < 0)
		{
		    perror("connfd套接字连接客户端失败!\n");
		    continue;
		}
		inet_ntop(AF_INET, &cli_in.sin_addr.s_addr, IP, sizeof(IP));
		PORT = ntohs(cli_in.sin_port);
		printf("连接进来的IP:PORT[%s:%d]", IP, PORT);

		/*设置新连接的套接字为读事件*/
		ev.data.fd = connfd;
		ev.events = EPOLLIN|EPOLLET;
		/*将新连接的客户端套接字加入到epoll监控事件*/
		epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
	    }
	    else if(events[i].events&EPOLLIN)
		/*已连接客户端套接字读事件触发,接收到数据，读sockfd*/
	    {
		if((sockfd = events[i].data.fd) < 0)
		{
		    continue;
		}
		/*接收客户端发送的数据*/
		ret = read(sockfd, &head, HEADER_SIZE);
		if(ret == 0)
		{
		    /*断开连接，清除套接字*/
		    ev.data.fd = sockfd;
		    ev.events = EPOLLIN|EPOLLET;
		    epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, &ev);
		    continue;
		}
		if(head.type == DRI_CLI_REG)//驾驶员注册信息
		{
		    memset(buf_type, 0, sizeof(buf_type));
		    strcpy(buf_type, "DRI_CLI_REG");
		    //接收数据
		    ret = read(sockfd, &dri_temp, head.length);
		    printf("驾驶员端---->[IP:%s;PORT:%d]\n", IP, PORT); 
		    if(dri_count < COUNT)
		    {
			head.type = DRI_CLI_REG_OK;
			//将成功的注册的驾驶员信息插入驾驶员信息链表中
			dri_list = create_list(&dri_temp, DRI_INFO_SIZE, pas_list);
			printf("驾驶员人数：%d:%s\n", dri_count+1, dri_temp.name);			
			dri_count++;
		    }
		    else
		    {
			head.type = DRI_CLI_REG_FL;
			printf("驾驶员注册人数已经达到上限!\n");
		    }	
		}
		else if(head.type == DRI_CLI_LOG)//驾驶员登录信息
		{
		    memset(buf_type, 0, sizeof(buf_type));
		    strcpy(buf_type, "DRI_CLI_LOG");
		    ret = read(sockfd, &acc, head.length);
		    printf("驾驶员登录信息:%d\n", DRI_CLI_LOG); 
		    temp_l = search_list(acc.num, acc.pass, dri_list, compare);	
		    if(NULL != temp_l)
		    {
			d_temp = (struct Driver_info*)(temp_l->data); 
			dri_temp = *d_temp;
			head.type = DRI_CLI_LOG_OK;
			strcpy(dri_online[dri_online_count].number, dri_temp.number);
			dri_online[dri_online_count].sfd = sockfd;
			strcpy(dri_online[dri_online_count].IP,IP);
			dri_online[dri_online_count].port = PORT;
			dri_online_count++;
		    }
		    else
		    {
			head.type = DRI_CLI_LOG_FL;
		    }
		}
		else if(head.type == PSE_CLI_REG)//乘客注册信息
		{
		    memset(buf_type, 0, sizeof(buf_type));
		    strcpy(buf_type, "PSE_CLI_REG");
		    //接收数据
		    ret = read(sockfd, &pas_temp, head.length);
		    printf("乘客端---->[IP:%s;PORT:%d]\n", IP, PORT);
		    
		    if(pas_count < COUNT)
		    {
			head.type = PSE_CLI_REG_OK;
			//将成功的注册的乘客信息插入客户信息链表中
			pas_list = create_list(&pas_temp, PSE_INFO_SIZE, pas_list);
			printf("乘客人数：%d\n", pas_count+1);
			pas_count++;
		    }
		    else
		    {
			head.type = PSE_CLI_REG_FL;
			printf("乘客注册人数已经达到上限!\n");
		    }	
		}
		else if(head.type == PSE_CLI_LOG)//乘客登录信息
		{
		    memset(buf_type, 0, sizeof(buf_type));
		    strcpy(buf_type, "PSE_CLI_LOG");
		    ret = read(sockfd, &acc, head.length);
		  
		    printf("乘客登录信息:%d\n", PSE_CLI_LOG); 
		    temp_l = search_list(acc.num, acc.pass, pas_list, compare);	
		    if(NULL != temp_l)
		    {
			p_temp = (struct Passenger_info*)(temp_l->data); 
			pas_temp = *p_temp;
			head.type = PSE_CLI_LOG_OK;
			strcpy(pas_online[pas_online_count].number,pas_temp.number);
			pas_online[pas_online_count].sfd = sockfd;
			strcpy(pas_online[pas_online_count].IP,IP);
			pas_online[pas_online_count].port = PORT;
			pas_online_count++;
		    }
		    else
		    {
			head.type = PSE_CLI_LOG_FL;
		    }
		}
		else if(head.type == PSE_PLACE_ORDER)//客户生成订单
		{
		    memset(buf_type, 0, sizeof(buf_type));
		    strcpy(buf_type, "PSE_PLACE_ORDER");
		    printf("**********客户生成订单消息!发送给老司机****\n");
		    ret = read(sockfd, &in_temp, sizeof(in_temp));
		    head.type = DRI_ACCEPT_ORDER;
		    //直接发送给接单司机
		    show_indent_signal(&in_temp);
		    sockfd = search_sendto_dri(dri_heart_pack, &in_temp);
		}
		else if(head.type == DRI_CLOSE_ORDER)
		{
		    memset(buf_type, 0, sizeof(buf_type));
		    strcpy(buf_type, "DRI_CLOSE_ORDER");
		    printf("消息类型:%d-订单完成消息!\n", head.type);
		    ret = read(sockfd, &in_temp, sizeof(in_temp));  
		    printf("司机发送过来的信息\n");
		    show_indent_signal(&in_temp);
		    //直接发送给下单客户
		    sockfd = search_pas_send(in_temp.passenger_number,pas_online);
		    head.type = PSE_END_ORDER;
		}
		else if(head.type == PSE_SELECT_HISTORY)//查询历史消息
		{ 
		    printf("消息类型:%d-乘客查询历史消息!\n", head.type);
		    ret = read(sockfd, &temp_number, sizeof(temp_number));  
		    in_list_history = search_list_all(temp_number, \
			    in_list_history, in_list, compare_indent);
		    //sockfd = search_pas_send(temp_number,pas_online);
		    if(in_list_history != NULL)
		    {
			head.type = PSE_SELECT_HISTORY_OK;
		    }
		    else
		    {
			head.type = PSE_SELECT_HISTORY_FL;
		    }
		}
		else if(head.type == DRI_HEART_PACKAGE)//司机的心跳包
		{
		    ret = read(sockfd, &d_heart, sizeof(Heart_package));
		    for(i = 0; i < dri_i; i++)
		    {
			if(d_heart.cfd == sockfd)
			{
			    dri_heart_pack[i] = d_heart;
			    dri_heart_pack[i].cfd = sockfd;
			}
		    }
		    if(i == dri_i)
		    {
			dri_heart_pack[dri_i] = d_heart;
			dri_heart_pack[i].cfd = sockfd;
			dri_i++;
		    }

		}
		else if(head.type == PSE_HEART_PACKAGE)//乘客的心跳包
		{
		    ret = read(sockfd, &p_heart, sizeof(Heart_package));
		    for(i = 0; i < pas_i; i++)
		    {
			if(p_heart.cfd == sockfd)
			{
			    pas_heart_pack[i] = p_heart;
			    pas_heart_pack[i].cfd = sockfd;
			}
		    }
		    if(i == pas_i)
		    {
			pas_heart_pack[pas_i] = p_heart;
			pas_heart_pack[i].cfd = sockfd;
			pas_i++;
		    }
		}
		/*读取并处理完数据后，服务器需要给客户端回馈数据，
		  此时采用异步方式，采取修改与该客户端套接字为写事件，
		 待处理完所有的本次读事件后，epoll等待事件将重新触发，
		 其中就至少包括该套接字的写事件*/
		ev.data.fd = sockfd;
		ev.events = (EPOLLOUT|EPOLLET);
		epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
	    }
	    else if(events[i].events&EPOLLOUT)
		/*与客户端连接的写事件触发,有数据待发送，写sockfd*/
	    {	
		sockfd = events[i].data.fd;	
		printf("反馈客户端_消息类型:%d!_____________\n", head.type);
		/*反馈给驾驶员端数据*/
		if(head.type == DRI_CLI_REG_OK)//驾驶员注册成功
		{		    
		    printf("消息类型:%d-驾驶员注册成功!\n", head.type);
		    p = package(head.version, head.type, 0, NULL);
		    ret = write(sockfd, p, HEADER_SIZE);//只发送报文头
		    free(p);
		}
		else if(head.type == DRI_CLI_REG_FL)//驾驶员注册失败
		{
		    printf("消息类型:%d-驾驶员注册失败!\n", head.type);
		    p = package(head.version, head.type, 0, NULL);
		    ret = write(sockfd, p, HEADER_SIZE);//只发送报文头
		    free(p);
		}
		else if(head.type == DRI_CLI_LOG_OK)//驾驶员登录成功
		{		    
		    printf("消息类型:%d-驾驶员登录成功!\n", head.type);
		    p = package(head.version, head.type, \
			    sizeof(struct Driver_info), &dri_temp);
		    ret = write(sockfd, p, HEADER_SIZE \
			    +sizeof(struct Driver_info));//头和数据
		    free(p);
		}
		else if(head.type == DRI_CLI_LOG_FL)//驾驶员登录失败
		{
		    printf("消息类型:%d-驾驶员登录失败!\n", head.type);
		    p = package(head.version, head.type, 0, NULL);
		    ret = write(sockfd, p, HEADER_SIZE);//只发送报文头
		    free(p);
		}
		/*反馈给乘客端数据*/
		else if(head.type == PSE_CLI_REG_OK)//乘客注册成功
		{		    
		    printf("消息类型:%d-乘客注册成功!\n", head.type);
		    p = package(head.version, head.type, 0, NULL);
		    ret = write(sockfd, p, HEADER_SIZE);//只发送报文头
		    free(p);
		}
		else if(head.type == PSE_CLI_REG_FL)//乘客注册失败
		{
		    printf("消息类型:%d-乘客注册失败!\n", head.type);
		    p = package(head.version, head.type, 0, NULL);
		    ret = write(sockfd, p, HEADER_SIZE);//只发送报文头
		    free(p);
		}
		else if(head.type == PSE_CLI_LOG_OK)//乘客登录成功
		{		    
		    printf("消息类型:%d-乘客登录成功!\n", head.type);
		    p = package(head.version, head.type, \
			    sizeof(struct Passenger_info), &pas_temp);
		    ret = write(sockfd, p, HEADER_SIZE \
			    +sizeof(struct Passenger_info));//头和数据
		    if(ret < 0)
		    {
			printf("服务端数据发送失败！\n");
			exit(1);
		    }
		    free(p);
		}
		else if(head.type == PSE_CLI_LOG_FL)//乘客登录失败
		{
		    printf("消息类型:%d-乘客登录失败!\n", head.type);
		    p = package(head.version, head.type, 0, NULL);
		    ret = write(sockfd, p, HEADER_SIZE);//只发送报文头
		    free(p);
		}
	
		//发送给司机接单的信号
		else if(head.type == DRI_ACCEPT_ORDER)
		{
		    printf("消息类型:%d-发送给接单司机!\n", head.type);
		    p = package(head.version, head.type, sizeof(in_temp), \
			    &in_temp);
		     
		    ret = write(sockfd, p, HEADER_SIZE+sizeof(in_temp));
		    free(p);
		}
		//发送给乘客订单完成信号
		else if(head.type == PSE_END_ORDER)
		{
		    printf("消息类型:%d-乘客订单完成信息!\n", head.type);
		    show_indent_signal(&in_temp);
		    in_list = create_list(&in_temp, INDENT_SIZE, in_list);
		    p = package(head.version, head.type, sizeof(in_temp), \
			    &in_temp);
		    ret = write(sockfd, p, HEADER_SIZE+sizeof(in_temp));
		    free(p);
		}
		//发送给乘客的历史订单信号
		else if(head.type == PSE_SELECT_HISTORY_OK)
		{
		    struct List* t = in_list_history;
		    printf("消息类型:%d-乘客的历史订单!\n", head.type);
		    while(t != NULL)
		    {
			memcpy(&temp_in_history, t->data, \
				sizeof(temp_in_history));
//			show_indent_signal(&temp_in_history);
			p = package(head.version, head.type, \
			    sizeof(temp_in_history), &temp_in_history);
			ret = write(sockfd, p, \
				HEADER_SIZE+sizeof(temp_in_history));
			free(p);
			t = t->next;
		    }
		    head.type == PSE_SELECT_HISTORY_FL;
		    printf("消息类型:%d-乘客的历史订单!\n", head.type);
		    p = package(head.version, head.type, 0, NULL);
		    ret = write(sockfd, p, HEADER_SIZE);//只发送报文头
		    free(p);
		}
		if(ret < 0)
		{
		    printf("服务器反馈给客户端的数据发送失败!\n");
		    exit(1);
		}
		/*重新设置套接字为读事件*/
		ev.data.fd = sockfd;
		ev.events = EPOLLIN|EPOLLET;
		/*重新修改套接字的epoll事件为读事件*/
		epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
	    }
	    else
	    {
		printf("程序运行出错！\n");
	    }
	}
    }
    if(in_list != NULL){
	free(in_list);
    }
    if(pas_list != NULL){
	free(pas_list);
    }
    if(dri_list != NULL){
	free(dri_list);
    }
    if(temp_in_list != NULL){
	free(temp_in_list);
    }
    close(epfd);
    return 0;
}

/*客户登录验证函数*/
int compare(void* num, void* pass, void* number, void* password)
{ 
    char* tmp_num = (char*)num;
    char* tmp_pass = (char*)pass;
    char* tmp_number = (char*)number;
    char* tmp_password = (char*)password;
    
    if(strcmp(tmp_number, tmp_num)==0 &&
	strcmp(tmp_password, tmp_pass)==0)
    {
	return 1;
    }
    return 0;
}

/*客户查询历史订单信息*/
int compare_indent(void* num, void*number)
{
    char* tmp_num = (char*)num;
    char* tmp_number = (char*)number;

    if(strcmp(tmp_number, tmp_num)==0)
    {
	return 1;
    }
    return 0;
}

/*查找在线的乘客，进行发送消息*/
int search_pas_send(char* num, struct Online* on)
{
    int i = 0;
    for(i = 0; i < 10; i++)
    {
	if(strcmp(num, on[i].number)==0)
	{
	    return on[i].sfd;
	}
    }
    return 0;
}
/*查找在线的司机，进行发送消息*/
int search_dri_send(char* num, struct Online* on)
{
    int i = 0;
    for(i = 0; i < 10; i++)
    {
	if(strcmp(num, on[i].number)==0)
	{
	    return on[i].sfd;
	}
    }
    return 0;
}

/*查找在线的距离最近的司机，进行发送消息*/
int search_sendto_dri(struct Heart_package* dri_heart_pack, struct Indent* in)
{
    int i = 0, j = 0;
    float min = 9999;
    float temp;

    for(i = 0; i < dri_i; i++)
    {
	temp = work_out_distance(dri_heart_pack[i].position_x, \
		dri_heart_pack[i].position_y, in->passenger_start_x, \
		in->passenger_start_y);
	if(fabs(temp-min)>EPSILON)
	{
	    min = temp;
	    j = i;
	}
    }
    return dri_heart_pack[j].cfd; 
}

