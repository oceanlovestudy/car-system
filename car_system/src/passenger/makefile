OBJ_PAS_CLI:=pas_cli_main.o passenger_client_func.o passenger_client.o package.o indent_struct.o -lm 

OPTION:=-c -I ../inc -g

#乘客端main	
pas_cli_main:$(OBJ_PAS_CLI)
	gcc $(OBJ_PAS_CLI) -o pas_cli_main
#乘客端main
pas_cli_main.o:pas_cli_main.c
	gcc $(OPTION) pas_cli_main.c -o pas_cli_main.o -g
passenger_client_func.o:passenger_client_func.c
	gcc $(OPTION) passenger_client_func.c -o passenger_client_func.o -g
passenger_client.o:passenger_client.c
	gcc $(OPTION) passenger_client.c -o passenger_client.o -g
driver_client_func.o:driver_client_func.c
	gcc $(OPTION) driver_client_func.c -o driver_client_func.o -g
driver_client.o:driver_client.c
	gcc $(OPTION) driver_client.c -o driver_client.o -g
package.o:package.c
	gcc $(OPTION) package.c -o package.o -g
indent_struct.o:indent_struct.c
	gcc $(OPTION) indent_struct.c -o indent_struct.o -g

clear_o:
	rm -rf *.o 
