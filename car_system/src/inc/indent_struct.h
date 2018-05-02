#ifndef INDENT_STRUCT

#define INDENT_STRUCT

#include <time.h>
#include "passenger_client_struct.h"
#include "driver_client_struct.h"
//订单状态
enum IndentStatus{cancel_indent=-1,waiting_indent,being_indent,end_indent};

typedef struct Indent
{
    int indent_number;		//订单号
    char passenger_number[20];	//客户帐号
    char passenger_time[64];	//客户下单时间
    float passenger_start_x;	//客户起始位置x坐标 
    float passenger_start_y;	//客户起始位置y坐标
    float passenger_end_x;	//客户目的地位置x坐标 
    float passenger_end_y;	//客户目的地位置y坐标
    float start_to_end;		//行车距离

    char driver_number[20];	//司机帐号
    char driver_contact_info[12];//司机联系方式
    char driver_name[20];	//司机姓名
    char driver_license_tag[20];//司机车牌号
    
    char driver_accept_time[64];//司机接受订单时间
    char driver_arrive_time[64];//司机到达客户起始位置的时间
    char end_time[64];		//完成订单时间
    int passenger_wait_time;	//乘客等待时间(minute)
    int car_run_time;		//行车时间(minute)
    int end_indent_time;	//完成订单时间(minute)
    int spend_money;		//完成订单所需花费
    enum IndentStatus indent_status;		//订单状态
}Indent;

#define INDENT_SIZE (sizeof(Indent))
#define PER_PRICE 3
#define EPSILON 0.01

#define X_DIFFER ((passenger_end_x)-(passenger_start_x))
#define X_SQUARE ((X_DIFFER)*(X_DIFFER))    
#define Y_DIFFER ((passenger_end_y)-(passenger_start_y))
#define Y_SQUARE ((Y_DIFFER)*(Y_DIFFER)) 
#define DISTANCE (sqrt((X_SQUARE)+(Y_SQUARE))) 

//乘客增加订单信息
struct Indent* cli_add_indent_info(struct Indent* in, struct Passenger_info* p_info);
//司机增加订单信息
struct Indent* dri_add_indent_info(struct Indent* in, struct Driver_info* d_info, struct Driver_RT* d_rt);
//增加成完成的订单信息
struct Indent* end_add_indent_info(struct Indent* in);
//打印订单的信息
void show_indent_signal(struct Indent* in);


//生成当前时间
char* get_time(char* time);
//计算分钟数
int work_out_minute(char* a_time, char* b_time);
//计算距离
float work_out_distance(float x1,float y1, float x2, float y2);
#endif
