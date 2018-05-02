OBJ_DRI_CLI:=dri_cli_main.o package.o driver_client_func.o driver_client.o indent_struct.o -lm

OPTION:=-c -I ../inc -g
#驾驶员端main	
dri_cli_main:$(OBJ_DRI_CLI)
	gcc $(OBJ_DRI_CLI) -o dri_cli_main
#驾驶员端main
dri_cli_main.o:dri_cli_main.c
	gcc $(OPTION) dri_cli_main.c -o dri_cli_main.o -g
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
