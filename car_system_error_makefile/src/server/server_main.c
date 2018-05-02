#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#include "passenger_client_struct.h"
#include "passenger_client_func.h"
#include "driver_client_struct.h"
#include "indent_struct.h"
#include "server.h"

int main()
{
    server_main(); 

    return 0;
}
	
