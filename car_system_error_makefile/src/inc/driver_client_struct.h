#ifndef DRIVER_CLIENT_STRUCT

#define DRIVER_CLIENT_STRUCT

#include <time.h>

typedef struct Driver_info
{
    char number[20];	    //帐号
    char password[20];	    //密码
    char name[20];	    //姓名
    char contact_info[12];  //联系方式
    char license_tag[20];   //车牌号 车牌以#开头，例如:#A0361
    int status;		    //状态1-在线，状态0-离线
    int money;		    //赚的钱
}Driver_info;

typedef struct Driver_RT
{
    float position_x;	    //司机实时位置x坐标
    float position_y;	    //司机实时位置y坐标
    char dri_time[64];	    //获取本地时间
    int car_speed;	    //车速(km/h)
    int task_status;	    //任务状态1-执行接单任务,0-未接单
    float distance_to_passenger;//和客户的距离
}Driver_RT;

#define DRI_INFO_SIZE (sizeof(Driver_info))
#define DRI_RT_SIZE (sizeof(Driver_RT))

#endif
