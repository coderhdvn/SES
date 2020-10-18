#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
	
#define TRUE 1 
#define FALSE 0 
#define PORT 8888 

int opt = TRUE; 
	int master_socket , addrlen , new_socket , client_socket[30] , 
		max_clients = 30 , activity, i , valread , sd; 
	int max_sd; 
	struct sockaddr_in address; 
		
	char buffer[1025]; //data buffer of 1K 

	int no_connected_sock =0;
	
	int port_table[100];	

	//set of socket descriptors 
	fd_set readfds; 
		
	//a message 
	char *message = "ECHO Daemon v1.0 \r\n"; 

void init()
{
		
	//create a master socket 
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		sizeof(opt)) < 0 ) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	
	//type of socket created 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	
}

int bind_port(int port)
{
	init();
	
	address.sin_port = htons( port ); 
		
	//bind the socket to localhost port 8888 
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed");
        return 0; 
		//exit(EXIT_FAILURE); 
	} 
	//printf("Listener on port %d \n", PORT); 
    return 1;
}

int rand_port()
{
	return rand()%10000;
}

void build_server()
{
	int random_port = rand_port();
	while (bind(random_port)==0){}

}

int main()
{

    printf("port %d is %d available\n",8888, build_server(8888));
    
}