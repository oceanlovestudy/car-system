#ifndef PAS_CLI_H
#define PAS_CLI_H


/*客户端选择界面函数*/
void pas_interface();

/*客户登录后的选择界面函数*/
void pas_login_interface();

/*客户约车的选择界面函数*/
void pas_appointment_interface();

/*实现客户登录后的界面选择函数*/
int achieve_pas_login_interface(struct Passenger_info* p_info, \
	struct Passenger_RT* p_rt, struct Indent* in);


/*客户约车的选择界面函数实现*/
//void pas_achieve_appointment_interface();

/*客户端主函数*/
int pas_cli_main(); 

/*信号接收函数*/
void sighandle(int sig);

#endif
