#ifndef PACKAGE_H
#define PACKAGE_H

#include "driver_client_struct.h"
#include "passenger_client_struct.h"
#include "indent_struct.h"
#include "passenger_client_func.h"

#define DRI_CLI_REG 10000	    //司机注册
#define DRI_CLI_REG_OK 11	    //司机注册成功
#define DRI_CLI_REG_FL 10	    //司机注册失败
#define DRI_CLI_REG_AS -11	    //司机注册帐号重复
#define DRI_CLI_LOG 10001	    //司机登录
#define DRI_CLI_LOG_OK 111	    //司机登录成功
#define DRI_CLI_LOG_FL 100	    //司机登录失败

#define DRI_GPS_INFO 10002	    //上报司机GPS定位信息
#define DRI_ACCEPT_ORDER 10003	    //司机接单
#define DRI_CLOSE_ORDER 10004	    //完成订单后，司机关闭订单服务
#define DRI_DRIVER_EXIT 10005	    //司机退出司机客户端 

#define DRI_HEART_PACKAGE 11111	    //司机心跳包
#define DRI_HEART_PACKAGE_OK 1111   //司机心跳包发送成功
#define DRI_HEART_PACKAGE_FL 1110   //司机心跳包发送失败

#define PSE_CLI_REG 20000	    //客户注册
#define PSE_CLI_REG_OK 21	    //客户注册成功
#define PSE_CLI_REG_FL 20	    //客户注册失败
#define PSE_CLI_REG_AS -21	    //客户注册帐号重复
#define PSE_CLI_LOG 20001	    //客户登录
#define PSE_CLI_LOG_OK 222	    //客户登录成功
#define PSE_CLI_LOG_FL 200	    //客户登录失败

#define PSE_PLACE_ORDER 20002	    //客户下单
#define PSE_CANCEL_ORDER 20003	    //客户取消订单
#define PSE_END_ORDER 20004	    //客户完成订单
#define PSE_SELECT_HISTORY 20005    //客户查阅订单历史记录
#define PSE_SELECT_HISTORY_OK 200051    //成功查阅订单历史记录
#define PSE_SELECT_HISTORY_FL 200050    //查阅订单历史记录失败
#define PSE_PASSENGER_EXIT 20006    //客户退出客户端

#define PSE_HEART_PACKAGE 22222	    //客户心跳包
#define PSE_HEART_PACKAGE_OK 2222   //客户心跳包发送成功
#define PSE_HEART_PACKAGE_FL 2220   //客户心跳包发送失败

//package数据包格式
typedef struct Header_data
{
    char version[20];	//软件版本
    int type;		//消息类型
    int length;		//数据长度
    char data[0];		
}Header_data;

#define HEADER_SIZE (sizeof(struct Header_data))
/*
   函数功能：把头部和数据打包成数据包
   函数参数：数据结构Header的成员变量，即头部;data,即需要，传输的数据;
   函数返回值：数据包
    */
void* package(char* version, int type, int length, void* data);

#endif
