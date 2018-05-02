#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver_client_struct.h"
#include "driver_client_func.h"
#include "passenger_client_struct.h"
#include "indent_struct.h"

/*
   函数功能：实现驾驶员的注册功能
   函数参数：驾驶员输入的帐号信息
   
   函数返回值：void
   */
void register_driver(struct Driver_info* d)
{
    printf("注册帐号:"); 
    scanf("%s", d->number);
    printf("帐号密码:");
    scanf("%s", d->password);
    printf("用户姓名:");
    scanf("%s", d->name);
    printf("联系方式:");
    scanf("%s", d->contact_info);
    printf("车牌号码:");
    scanf("%s", d->license_tag);//车牌号以#号开头,例如:#A0361
    d->status = 0;		//状态1-在线，状态0-离线
    d->money = 0;		//驾驶员赚的钱
    return ;
}
/*司机的实时信息*/
void get_dri_rt(struct Driver_RT* d_rt)
{ 
    printf("驾驶员实时地址x坐标:\n");
    scanf("%f", &d_rt->position_x);
    printf("驾驶员实时地址y坐标:\n");
    scanf("%f", &d_rt->position_y);
    printf("驾驶员实时车速(km/h):\n");
    scanf("%d", &d_rt->car_speed);
    get_time(d_rt->dri_time);
    d_rt->task_status = 0;
    d_rt->distance_to_passenger = 0.0;
    return ;
}

/*
   函数功能：定位驾驶员的打车位置功能
   函数参数：驾驶员的基本信息和存放驾驶员实时信息的参数
   函数返回值：定位成功返回驾驶员的实时信息，定位失败返回NULL
   */
struct Driver_RT* driver_location(const struct Driver_info* d_info, struct Driver_RT* d_rt, struct Indent* in)
{
    printf("驾驶员实时地址x坐标:\n");
    scanf("%f", &d_rt->position_x);
    printf("驾驶员实时地址y坐标:\n");
    scanf("%f", &d_rt->position_y);
    printf("驾驶员实时车速(km/h):\n");
    scanf("%d", &d_rt->car_speed);
    get_time(d_rt->dri_time);
    d_rt->task_status = 0;
    if((d_rt->task_status == 1) && ((in->indent_status == waiting_indent) 
		||(in->indent_status == being_indent)))
    {    
	printf("接单驾驶员与约车客户的距离:\n");
	d_rt->distance_to_passenger = work_out_distance(d_rt->position_x, \
		d_rt->position_y, in->passenger_start_x, \
		in->passenger_start_y);
    }
}

/*
   函数功能：驾驶员接单后，显示客户与驾驶员的订单信息
   函数参数：订单信息，司机的实时信息
   函数返回值：显示成功返回0，显示失败返回-1
   */
int dri_show_indent_info(struct Indent* in, struct Driver_RT* d_rt)
{
    printf("×××××订单信息×××××\n");
    show_indent_signal(in);
    printf("×××司机实时信息×××\n");
    show_driver_rt(d_rt);
}

/*
   函数功能：显示驾驶员基本信息
   函数参数：驾驶员的帐号的基本信息
   函数返回值：返回void
   */
void show_driver_info(struct Driver_info* d)
{
    printf("司机帐号:%s\n", d->number);
    printf("帐号密码:%s\n", d->password);
    printf("用户姓名:%s\n", d->name);
    printf("联系方式:%s\n", d->contact_info);
    printf("车牌号码:%s\n", d->license_tag);
    printf("用户状态:%d\n", d->status);
    printf("已赚的钱:%d\n", d->money);
    return ;
}
/*
    函数功能：显示驾驶员实时信息
    函数参数：驾驶员的实时信息
    函数返回值：返回void
    */
void show_driver_rt(struct Driver_RT* d_rt)
{
    printf("驾驶员实时地址x坐标:%.2f\n", d_rt->position_x);
    printf("驾驶员实时地址y坐标:%.2f\n", d_rt->position_y);
    printf("驾驶员本地时间:%s\n", d_rt->dri_time);
    printf("驾驶员实时车速(km/h):%d\n", d_rt->car_speed);
    printf("驾驶员的任务状态:1-执行接单 0-未接单:%d\n", d_rt->task_status);
    printf("接单驾驶员与约车客户的距离:%.2f\n", d_rt->distance_to_passenger);
}


/*
     函数功能：修改驾驶员基本信息(无法修改帐号)
     函数参数：驾驶员的帐号的基本信息
     函数返回值：返回修改后的驾驶员基本信息
     */
struct Driver_info* modify_driver_info(struct Driver_info* d)
{
    printf("请输入修改的信息的操作命令:\n");
    printf("1-密码 2-姓名 3-联系方式 4-车牌号码 5-退出\n");
    int cmd;
    scanf("%d", &cmd);
    char tag[20];
    switch(cmd)
    {
	case 1:
	    printf("请输入新的密码(6-20位):\n");
	    scanf("%s", d->password);
	    break;
	case 2:
	    printf("请输入新的姓名:\n");
	    scanf("%s", d->name);
	    break;
	case 3:
	    printf("请输入新的联系方式:\n");
	    scanf("%s", d->contact_info);
	    break;
	case 4:
	    printf("请输入新的车牌号码:\n");
	    scanf("%s", tag);
	    memset(d->license_tag, 0, sizeof(d->license_tag));
	    strcpy(d->license_tag, tag);
	    break;
	case 5:
	    printf("退出修改!正在退出!\n");
	    return d;
	    break;
	default:
	    break;
    }
    return d;
}

