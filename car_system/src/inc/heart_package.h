#ifndef HEART_PACKAGE_H
#define HEART_PACKAGE_H

//心跳包的结构体
typedef struct Heart_package
{
   char number[20];	//存储帐号
   int cfd;		//存放该帐号的套接字
   float position_x;	//该帐号位置的x坐标
   float position_y;	//该帐号位置的y坐标
//   int heart_status;	//是否接收到心跳包 接收到-1,未收到-0
}Heart_package;


#endif
