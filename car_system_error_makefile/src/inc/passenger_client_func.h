#ifndef PASSENGER_CLIENT_FUNC
#define PASSENGER_CLIENT_FUNC

#include "passenger_client_struct.h"
#include "indent_struct.h"
#include "driver_client_struct.h"

/*
   函数功能：实现客户的注册功能
   函数参数：客户输入的帐号信息
   函数返回值：注册成功返回注册的信息，注册失败返回NULL
   */
void register_passenger(struct Passenger_info* p);

/*函数功能：得到客户的实时位置*/
void get_pas_rt(struct Passenger_RT* p_rt);

/*
   函数功能：定位客户的打车位置功能
   函数参数：客户的基本信息和存放客户实时信息的参数
   函数返回值：定位成功返回客户的实时信息，定位失败返回NULL
   */
struct Passenger_RT* passenger_start_location(const struct Passenger_info* p_info, struct Passenger_RT* p_rt); 
/*
   函数功能：实现客户的打车功能,生成打车订单信息
   函数参数：客户的基本信息，客户的实时信息，存放订单信息的参数
   函数返回值：成功返回打车订单信息，生成订单失败返回NULL
   */
struct Indent* get_car(const struct Passenger_info* p_info, const struct Passenger_RT* p_rt, struct Indent* in);

/*
   函数功能：客户的订单被司机接受后，显示司机与客户的距离和司机的基本信息
   函数参数：订单信息，司机的实时信息
   函数返回值：显示成功返回0，显示失败返回-1
   */
int pas_show_indent_info(struct Indent* in, struct Driver_RT* d_rt);

/*
   函数功能：显示客户的历史订单记录功能
   函数参数：客户的帐号的基本信息
   函数返回值：显示成功返回0，显示失败返回-1
   */
int show_history_passenger(struct Passenger_info* p);

/*
    函数功能：显示客户基本信息
    函数参数：客户的帐号的基本信息
    函数返回值：返回void
    */
void show_passenger_info(struct Passenger_info* p);

/*
    函数功能：修改客户基本信息
    函数参数:	客户帐号的基本信息
    函数返回值：返回void
    */
void modify_passenger_info(struct Passenger_info* p);

#endif

