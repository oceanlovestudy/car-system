#ifndef DRIVER_CLIENT_FUNC
#define DRIVER_CLIENT_FUNC

#include "driver_client_struct.h"
#include "indent_struct.h"
#include "driver_client_struct.h"

/*
   函数功能：实现驾驶员的注册功能
   函数参数：驾驶员输入的帐号信息
   函数返回值：返回void
   */
void register_driver(struct Driver_info* d);

/*
   函数功能：得到司机的实时信息
   函数参数：存放驾驶员实时信息的参数
   函数返回值：返回void*/
void get_dri_rt(struct Driver_RT* d_rt);

/*
   函数功能：定位驾驶员的位置功能函数
   函数参数：驾驶员的基本信息和存放驾驶员实时信息的参数
   函数返回值：定位成功返回驾驶员的实时信息，定位失败返回NULL
   */
struct Driver_RT* driver_location(const struct Driver_info* d_info, struct Driver_RT* d_rt, struct Indent* in); 
/*
   函数功能：驾驶员接单后，显示客户与驾驶员的订单信息
   函数参数：订单信息，司机的实时信息
   函数返回值：显示成功返回0，显示失败返回-1
   */
int dri_show_indent_info(struct Indent* in, struct Driver_RT* d_rt);

/*
    函数功能：显示驾驶员基本信息
    函数参数：驾驶员的帐号的基本信息
    函数返回值：返回void
    */
void show_driver_info(struct Driver_info* d);

/*
    函数功能：显示驾驶员实时信息
    函数参数：驾驶员的实时信息
    函数返回值：返回void
    */
void show_driver_rt(struct Driver_RT* d_rt);

/*
    函数功能：修改驾驶员基本信息(无法修改帐号)
    函数参数：驾驶员的帐号的基本信息
    函数返回值：返回修改后的驾驶员基本信息
    */
struct Driver_info* modify_driver_info(struct Driver_info* d);
#endif

