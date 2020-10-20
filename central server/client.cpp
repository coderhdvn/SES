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
#include <pthread.h> 

#define TRUE 1 
#define FALSE 0
#define SERVER_PORT 8888

struct sockaddr_in address; 
int opt = TRUE;
int master_socket;
int port;
int peers[20];

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
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) { 
        return 0; 
	} 
    return 1;
}

int rand_port(){
	return rand()%10000;
}

void build_server(){
    port = rand_port();
	while (bind_port(port)==0){}

}

int sock = 0, valread; 
struct sockaddr_in serv_addr; 
char buffer[1024] = {0}; 
    

int join()
{
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(SERVER_PORT); 
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
}

int no_peers = 0;
void get_peer_info(char * buffer){

    printf("str=%s \n",buffer);
    int formmer = 0;
    for ( int i = 0; i < strlen( buffer ) ; i++){
        if (buffer[i]=='#'){
            
            int num =0;
            for (int j = formmer; j < i; j++){
                num = num*10 + buffer[j]-'0';
            }
            formmer= i + 1;
            peers[no_peers++] = num;
        }
    }
}

void *server_listen(void * vargp)
{
    printf("running");
    while (1)
    {
        int addrlen = sizeof(address);
        int client_socket = accept(master_socket, 
        (struct sockaddr *)&address, (socklen_t*)&addrlen);
        printf("1 process connecting");
    }
}

int main(int argc, char const *argv[]) { 
    
    if ( join()==-1 )
    {
        return -1;
    }
    srand (time(NULL));
    char * b = (char *)malloc(10);
    build_server();
    
    listen(master_socket, 20);
    
    pthread_t thread_id; 
    pthread_create(&thread_id, NULL, server_listen, NULL); 
    
    printf("server is listening");

    sprintf(b,"%d",port);
    send(sock , b , strlen(b) , 0 );

    printf("Hello message sent\n"); 
    valread = read( sock , buffer, 1024);
    get_peer_info(buffer);
    
    pthread_join(thread_id, NULL); 
    return 0; 
} 

