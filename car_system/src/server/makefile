OBJ_SERVER:=server_main.o passenger_client_func.o driver_client_func.o server.o package.o data_struct_list.o indent_struct.o -lm 

OPTION:=-c -I ../inc -g

#服务器main
server_main:$(OBJ_SERVER)
	gcc $(OBJ_SERVER) -o server_main
#服务器main

server_main.o:server_main.c
	gcc $(OPTION) server_main.c -o server_main.o -g
passenger_client_func.o:passenger_client_func.c
	gcc $(OPTION) passenger_client_func.c -o passenger_client_func.o -g
passenger_client.o:passenger_client.c
	gcc $(OPTION) passenger_client.c -o passenger_client.o -g
driver_client_func.o:driver_client_func.c
	gcc $(OPTION) driver_client_func.c -o driver_client_func.o -g
driver_client.o:driver_client.c
	gcc $(OPTION) driver_client.c -o driver_client.o -g
server.o:server.c
	gcc $(OPTION) server.c -o server.o -g
package.o:package.c
	gcc $(OPTION) package.c -o package.o -g
data_struct_list.o:data_struct_list.c
	gcc $(OPTION) data_struct_list.c -o data_struct_list.o -g
indent_struct.o:indent_struct.c
	gcc $(OPTION) indent_struct.c -o indent_struct.o -g


clear_o:
	rm -rf *.o 
