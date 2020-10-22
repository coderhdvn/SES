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
int my_port;
int peers[20];

void init()
{
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		sizeof(opt)) < 0 ) { 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	
}

int bind_port(int _port){
	address.sin_port = htons( _port ); 
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) { 
        return 0; 
	} 
    return 1;
}

int rand_port(){
	return rand()%10000;
}

void build_server(){
    init();
    my_port = rand_port();

    while (bind_port(my_port)==0){
        my_port = rand_port();
    }

    if (listen(master_socket, 4)<0){
        printf("error at listening");
    }

}

int sock = 0, valread; 
struct sockaddr_in serv_addr; 
char buffer[1024] = {0}; 
    

int join()
{
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
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

void connect_peers();

void *server_listen(void * vargp)
{
    puts("my port's listening\n");
    int count = 0;
    while (count < no_peers-1){
        int addrlen = sizeof(address);
        int client_socket = 
        accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        puts("1 process connecting");
        count++;
    }
}

void connect_peers(){
    
    for (int i = 0; i < no_peers; i++){
        if (peers[i] != my_port){
            int tsock = socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in taddress;
            taddress.sin_family = AF_INET;
            taddress.sin_port = htons(peers[i]);
            if (inet_pton(AF_INET, "127.0.0.1", &taddress.sin_addr) <= 0){
                printf("\nInvalid address/ Address not supported \n");
            }
            if (connect(tsock, (struct sockaddr *)&taddress, sizeof(taddress)) < 0){
                printf("\nConnection Failed \n");
            }
            puts("connect to peer\n");
        }
    }
}

int main(int argc, char const *argv[]) { 
    
    if ( join()==-1 ){
        return -1;
    }
    srand (time(NULL));
    char * b = (char *)malloc(10);
    build_server();

    sprintf(b,"%d",my_port);
    send(sock , b , strlen(b) , 0 );
    
    valread = read( sock , buffer, 1024);
    puts("recived info\n");

    get_peer_info(buffer);

    pthread_t thread_id; 
    pthread_create(&thread_id, NULL, server_listen, NULL); 

    connect_peers();

    pthread_join(thread_id, NULL);     
} 

