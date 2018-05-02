#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "../inc/indent_struct.h"
//#include "driver_client_struct.h"

//生成当前时间
char* get_time(char* ti)
{
    char tr[64];
    time_t t = time(0);
    strftime(tr, sizeof(tr), "%Y-%m-%d %H:%M:%S", localtime(&t));
//    printf("get_time_tr:%s\n", tr);
    strcpy(ti, tr);
//    printf("get_time_ti:%s\n", ti);
    return ti;
}

//计算分钟数
int work_out_minute(char* a_time, char* b_time)
{
    int minu = 0, i = 9;
    char a_m[3];
    char a_s[3];
    char b_m[3];
    char b_s[3];
    int k = 0;
    for(k = 0, i = 9; i<11; i++, k++)
    {
	a_m[k] = a_time[i];
    }
    a_m[k] = '\0';
    for(k = 0, i = 12; i<14; i++, k++)
    {
	a_s[k] = a_time[i];
    }
    a_s[k] = '\0';
    for(k = 0, i = 9; i<11; i++, k++)
    {
	b_m[k] = b_time[i];
    }
    b_m[k] = '\0';
    for(k = 0, i = 12; i<14; i++, k++)
    {
	b_s[k] = b_time[i];
    }
    b_s[k] = '\0';
    int am = 0, as = 0, bm = 0, bs = 0;
    am = (a_m[0] - '0')*10 + a_m[1] - '0';	
    as = (a_s[0] - '0')*10 + a_s[1] - '0';	
    bm = (b_m[0] - '0')*10 + b_m[1] - '0';	
    bs = (b_s[0] - '0')*10 + b_s[1] - '0';
    minu = (am - bm)*60 + (as-bs);
    return minu;
}
//计算距离
float work_out_distance(float x1,float y1, float x2, float y2)
{
    float d_x, d_y, d;
    d_x = (x1-x2)*(x1-x2);
    d_y = (y1-y2)*(y1-y2);
    d = sqrt(d_x+d_y);
    return d;
}

//乘客增加订单信息
struct Indent* cli_add_indent_info(struct Indent* in, struct Passenger_info* p_info)
{
    //copy乘客帐号
    strcpy(in->passenger_number, p_info->number);
    //生成订单号
    in->indent_number = rand()%1000;
    printf("乘客%s,您好,本次约车生成订单号是：%d\n", in->passenger_number, \
	    in->indent_number);
    //乘客起始位置
    printf("请输入客户起始位置的x坐标：\n");
    scanf("%f", &in->passenger_start_x);
    printf("请输入客户起始位置的y坐标：\n");
    scanf("%f", &in->passenger_start_y);
    //乘客目的位置
    printf("请输入客户目的位置的x坐标：\n");
    scanf("%f", &in->passenger_end_x);
    printf("请输入客户目的位置的y坐标：\n");
    scanf("%f", &in->passenger_end_y);

    in->start_to_end = work_out_distance(in->passenger_start_x, \
	in->passenger_start_y, in->passenger_end_x, in->passenger_end_y);
//    memset(in->passenger_time, 0, sizeof(64));
    get_time(in->passenger_time);
    printf("客户下单时间:%s\n", in->passenger_time);
    //修改订单状态
    in->indent_status = waiting_indent;	//等待被接单
    memset(in->driver_number, 0, sizeof(in->driver_number));
    memset(in->driver_contact_info, 0, sizeof(in->driver_contact_info));
    memset(in->driver_name, 0, sizeof(in->driver_name));
    memset(in->driver_license_tag, 0, sizeof(in->driver_name));
    memset(in->driver_accept_time, 0, sizeof(in->driver_accept_time));
    memset(in->driver_arrive_time, 0, sizeof(in->driver_arrive_time));
    memset(in->end_time, 0, sizeof(in->end_time));
    in->passenger_wait_time = 0;
    in->car_run_time = 0;
    in->end_indent_time = 0;
    in->spend_money = 0;
}

//司机增加订单信息
struct Indent* dri_add_indent_info(struct Indent* in, struct Driver_info* d_info, struct Driver_RT* d_rt)
{
    strcpy(in->driver_number, d_info->number);	//复制司机帐号
    strcpy(in->driver_contact_info, d_info->contact_info);  //复制司机电话
    strcpy(in->driver_name, d_info->name);	//复制司机姓名
    strcpy(in->driver_license_tag, d_info->license_tag);    //复制车牌号
    //修改订单状态
    in->indent_status = being_indent;	//已经接单
    get_time(in->driver_accept_time);
    if(fabs(d_rt->distance_to_passenger) < EPSILON)
    {
	get_time(in->driver_arrive_time);
    }
    else
    {
	memset(in->driver_arrive_time, 0, sizeof(64));
    }
}

//增加成完成的订单信息
struct Indent* end_add_indent_info(struct Indent* in)
{ 
    //修改订单状态
    in->indent_status = end_indent;	//完成订单
    //订单完成时间 
    get_time(in->end_time);
    //乘客等待时间
    in->passenger_wait_time = work_out_minute(in->driver_arrive_time, \
		in->passenger_time);
    //行车时间
    in->car_run_time = work_out_minute(in->end_time, \
		in->driver_arrive_time);
    //完成订单所花时间
    in->car_run_time = work_out_minute(in->end_time,in->passenger_time);
    //完成订单的费用
    in->spend_money = (in->start_to_end) * PER_PRICE;  
}

//打印订单的信息
void show_indent_signal(struct Indent* in)
{
    printf("————————————————————————————订单信息：————————————————————\n");
    printf("\t订单号:%d\n", in->indent_number);
    printf("\t客户帐号:%s\n", in->passenger_number);
    printf("\t客户下单时间:%s\n", in->passenger_time);
    printf("\t客户起始位置(x,y):(%.2f, %.2f)\n", in->passenger_start_x, in->passenger_start_y);
    printf("\t客户目的位置(x,y):(%.2f, %.2f)\n", in->passenger_end_x, in->passenger_end_y);
    
    
    printf("\t行车距离:%.2f\n", in->start_to_end);
    printf("\t司机帐号:%s\n", in->driver_number);
    printf("\t司机联系方式:%s\n", in->driver_contact_info);
    printf("\t司机姓名:%s\n", in->driver_name);
    printf("\t司机车牌号码:%s\n", in->driver_license_tag);
    
    printf("\t司机接受订单时间:%s\n", in->driver_accept_time);
    printf("\t司机起步的时间:%s\n", in->driver_arrive_time);
    printf("\t完成订单的时间:%s\n", in->end_time);
    printf("\t乘客等待时间(minute):%d\n", in->passenger_wait_time);
    printf("\t行车时间(minute):%d\n", in->car_run_time);
    printf("\t完成订单的时间:%d\n", in->end_indent_time);
    
    printf("\t完成订单费用:%d\n", in->spend_money);
    printf("\t订单状态:%d\n", in->indent_status);
    return ;
}
/*
int main()
{
    char t[64];
    get_time(t);
    printf("main_t:%s\n", t);
    return 0;
}
*/
