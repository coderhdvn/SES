
// Client side C/C++ program to demonstrate Socket programming 
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
#include <time.h>

#define TRUE 1 
#define FALSE 0
#define SERVER_PORT 8888

struct sockaddr_in address; 
int opt = TRUE;
int master_socket;
int port;

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
		//perror("bind failed");
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
    port = rand_port();
	while (bind_port(port)==0){}

}

int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    //char *hello = "****Hello from client"; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(SERVER_PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    srand (time(NULL));
    int r = rand() % 10000;
    char * b = (char *)malloc(10);
    build_server();
    listen(master_socket, 3);
    sprintf(b,"%d",port);
     
    send(sock , b , strlen(b) , 0 ); 
    //valread = read( sock , buffer, 1024);
    printf("Hello message sent\n"); 
    //valread = read( sock , buffer, 1024); 
     
    //while (1)
    //{
        valread = read( sock , buffer, 1024);
        printf("%s\n",buffer );
    //}
    return 0; 
} 

