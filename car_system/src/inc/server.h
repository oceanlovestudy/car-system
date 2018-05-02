#ifndef SERVER_H
#define SERVER_H

#include "heart_package.h"
//登录结构体
typedef struct Login_test
{
    char num[20];		//账户
    char pass[20];		//密码
}Login_test;

//在线的司机和乘客的信息
typedef struct Online
{
    char number[20];
    int sfd;
    char IP[30];
    int port;
}Online;

/*
//司机与乘客的距离
typedef struct Dri_to_Pas
{
    char number[20];
    float dis;
}Dri_to_Pas;
*/

//服务器主函数
int server_main();

/*登录验证函数*/
int compare(void* num, void* pass, void* number, void* password);

/*客户查询历史订单信息*/
int compare_indent(void* num, void*number);
    

/*查找在线的距离最近的司机，进行发送消息*/
int search_sendto_dri(struct Heart_package* dri_heart_pack, struct Indent* in);

/*查找在线的乘客，进行发送消息*/
int search_pas_send(char* num, struct Online* on);

/*查找在线的司机，进行发送消息*/
int search_dri_send(char* num, struct Online* on);
#endif
