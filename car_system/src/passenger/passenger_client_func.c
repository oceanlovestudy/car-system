#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "passenger_client_struct.h"
#include "driver_client_struct.h"
#include "indent_struct.h"

/*
   函数功能：实现客户的注册功能
   函数参数：客户输入的帐号信息
   函数返回值：注册成功返回注册的信息，注册失败返回NULL
   */
void register_passenger(struct Passenger_info* p)
{
    int i = 0;
    printf("注册帐号:"); 
    scanf("%s", p->number);
    printf("帐号密码:");
    scanf("%s", p->password);
    printf("用户姓名:");
    scanf("%s", p->name);
    printf("联系方式:");
    scanf("%s", p->contact_info);
    printf("充值金额:");
    scanf("%f", &p->money);
    p->status = 0;
    p->order_indent = 0;
    p->indent_cnt = 0;
    for(i = 0;i < 32; i++)
    {
	p->indent_number[i] = 0;
    }
    return ;
}

/*函数功能：得到客户的实时位置*/
void get_pas_rt(struct Passenger_RT* p_rt)
{
    printf("乘客实时地址x坐标:\n");
    scanf("%f", &p_rt->position_x);
    printf("乘客实时地址y坐标:\n");
    scanf("%f", &p_rt->position_y);
    get_time(p_rt->pas_time);
    return ;
}

/*
   函数功能：定位客户的打车位置功能
   函数参数：客户的基本信息和存放客户实时信息的参数
   函数返回值：定位成功返回客户的实时信息，定位失败返回NULL
   */
struct Passenger_RT* passenger_start_location(const struct Passenger_info* p_info, struct Passenger_RT* p_rt)
{

}
/*
   函数功能：实现客户的打车功能,生成打车订单信息
   函数参数：客户的基本信息，客户的实时信息，存放订单信息的参数
   函数返回值：成功返回打车订单信息，生成订单失败返回NULL
   */
struct Indent* get_car(const struct Passenger_info* p_info, const struct Passenger_RT* p_rt, struct Indent* in)
{

}

/*
   函数功能：客户的订单被司机接受后，显示司机与客户的距离和司机的基本信息
   函数参数：订单信息，司机的实时信息
   函数返回值：显示成功返回0，显示失败返回-1
   */
int pas_show_indent_info(struct Indent* in, struct Driver_RT* d_rt)
{

}

/*
   函数功能：显示客户的历史订单记录功能
   函数参数：客户的帐号的基本信息
   函数返回值：显示成功返回0，显示失败返回-1
   */
int show_history_passenger(struct Passenger_info* p)
{

}

/*
   函数功能：显示客户基本信息
   函数参数：客户的帐号的基本信息
   函数返回值：返回void
   */
void show_passenger_info(struct Passenger_info* p)
{
    printf("注册帐号:%s\n", p->number);
    printf("帐号密码:%s\n", p->password);
    printf("用户姓名:%s\n", p->name);
    printf("联系方式:%s\n", p->contact_info);
    printf("可用余额:%.2f\n", p->money);
    printf("用户状态:%d\n", p->status);
    printf("下单状态:%d\n", p->order_indent);
    printf("已经完成的订单个数:%d\n", p->indent_cnt);
    int i = 0;
    for(i = 0;i < p->indent_cnt; i++)
    {
	printf("已完成订单号:%d\n", p->indent_number[i]);
    }
    return ;
}

/*
     函数功能：修改客户基本信息
     函数参数:   客户帐号的基本信息
     函数返回值：返回void
     */
void modify_passenger_info(struct Passenger_info* p)
{
    printf("请输入修改的信息的操作命令:\n");
    printf("1-密码 2-姓名 3-联系方式 4-充值金额 5-退出\n");
    int cmd;
    scanf("%d", &cmd);
    float mone;
    switch(cmd)
    {
	case 1:
	    printf("请输入新的密码(6-20位):\n");
	    scanf("%s", p->password);
	    break;
	case 2:
	    printf("请输入新的姓名:\n");
	    scanf("%s", p->name);
	    break;
	case 3:
	    printf("请输入新的联系方式:\n");
	    scanf("%s", p->contact_info);
	    break;
	case 4:
	    printf("充值金额:\n");
	    scanf("%f", &mone);
	    p->money = p->money + mone;
	    break;
	case 5:
	    printf("退出修改!正在退出!\n");
	    return ;
	    break;
	default:
	    break;
    }
    return ;
}

