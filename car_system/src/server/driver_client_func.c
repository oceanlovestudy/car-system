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

/*
   函数功能：定位驾驶员的打车位置功能
   函数参数：驾驶员的基本信息和存放驾驶员实时信息的参数
   函数返回值：定位成功返回驾驶员的实时信息，定位失败返回NULL
   */
struct Driver_RT* driver_location(const struct Driver_info* d_info, struct Driver_RT* d_rt, struct Indent* in)
{

}
/*
   函数功能：实现驾驶员的接单功能,生成新的打车订单信息
   函数参数：驾驶员的基本信息，驾驶员的实时信息，存放订单信息的参数
   函数返回值：成功返回打车订单信息，生成订单失败返回NULL
   */
struct Indent* get_intent(const struct Driver_info* d_info, const struct Driver_RT* d_rt, struct Indent* in)
{

}

/*
   函数功能：驾驶员接单后，显示客户与驾驶员的订单信息
   函数参数：订单信息，司机的实时信息
   函数返回值：显示成功返回0，显示失败返回-1
   */
int dri_show_indent_info(struct Indent* in, struct Driver_RT* d_rt)
{

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
     函数功能：修改驾驶员基本信息(无法修改帐号)
     函数参数：驾驶员的帐号的基本信息
     函数返回值：返回修改后的驾驶员基本信息
     */
struct Driver_info* modify_driver_info(struct Driver_info* d);

