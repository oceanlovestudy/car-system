#ifndef PASSENGER_CLIENT_STRUCT

#define PASSENGER_CLIENT_STRUCT

#include <time.h>

typedef struct Passenger_info
{
    char number[20];	    //帐号
    char password[20];	    //密码
    char name[20];	    //姓名
    char contact_info[12];  //联系方式
    float money;	    //可用余额
    int status;		    //状态1-在线，状态0-离线
    int order_indent;	    //是否下单1-下单 0-未下单
    int indent_cnt;	    //已完成的订单个数
    int indent_number[32];  //已经完成的订单号
}Passenger_info;

typedef struct Passenger_RT
{
    float position_x;	    //客户实时位置x坐标
    float position_y;	    //客户实时位置y坐标
    char pas_time[64];	    //获取本地时间
}Passenger_RT;

#define PSE_INFO_SIZE (sizeof(Passenger_info))
#define PSE_RT_SIZE (sizeof(Passenger_RT))

#endif
