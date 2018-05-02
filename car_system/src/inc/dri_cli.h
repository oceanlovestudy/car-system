#ifndef DRI_CLI_H
#define DRI_CLI_H

/*司机端选择界面函数*/
void dri_interface();

/*司机登录后的选择界面函数*/
void dri_login_interface();

/*司机登录后的选择函数的实现*/
int achieve_dri_login_interface(struct Indent* in, struct Driver_info* d_info, struct Driver_RT* d_rt, int sockfd, struct Header_data* head);
    

/*司机端主函数*/
int dri_cli_main();

/*信号接收函数*/
void sighandle(int sig);

/*司机接单后的选择界面函数*/
void dri_appointtment_interface();
#endif
