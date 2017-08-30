/*
This is the Client side of a chatting application 'ChatLite' written in C.
Project Title:  ChatLite
Developed by:   Sayom, Nazmus Shakib
Institution:    American International University - Bangladesh (AIUB)
ID:             17-33926-1
Department:     Computer Science & Engineering
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <resolv.h>

#include <pthread.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>

#include "lib/sds/sds.c"
#include "getch.c"

int MySocket;
char buffer, input_msg[256], input_msg_s[256], host[128], ip[16], username[20];
pthread_t checker_thread;

void *check_message();
void send_message();
int connect_to_server(char *c_host, int c_port);

int main(){
    int port;

    printf("Enter Host: ");
    scanf("%s", host);
    printf("Enter Port: ");
    scanf("%d", &port);
    printf("Enter Username: ");
    scanf("%s", &username);

    hostname_to_ip(host, ip);

    // Connect To Server
    if((MySocket = connect_to_server(ip, port)) < 0){
        printf("Error: Unable to Connect");
        return -1;
    }else{
        printf("> Connected to %s on Port %d", ip, port);
    }

    // Multi Thread
    if(pthread_create(&checker_thread, NULL, check_message, NULL)){
        printf("\nError: Creating Thread");
        return 1;
    }

    // Send Message
    while(1){
        send_message();
    }

    return 0;
}

void *check_message(){
    int BufferSize;
    char response[1025];
    while(1){
        sleep(2);
        //printf("\n>>>> TRYING!");
        //Receive a reply from the server
        memset(response, 0, strlen(response));
        while((BufferSize = recv(MySocket, response, 1025, 0)) != '\0'){
            response[BufferSize] = '\0';
            if(BufferSize > 0){
                //printf("\r");
                printf("%s", response);
                printf("\n> %s", input_msg);
                fflush(stdout);
            }
        }

    }
    return NULL;
}

void send_message(){
    int i = 0, j;
    char buffer;

    printf("\n> ");
    while(1){
        buffer = getch();
        if(buffer == 127 || buffer == 8){
            if(i > 0){
                printf("\b \b");
                i--;
                input_msg[i+1] = '\0';
            }else{
                printf(" \b");
                input_msg[0] = '\0';
            }

            fflush(stdout);
            continue;
        }
        //printf("%d ", buffer);
        if(buffer == 10 || i > 254){
            break;
        }

        input_msg[i] = buffer;
        input_msg[i+1] = '\0';

        printf("%c", buffer);
        fflush(stdout);
        i++;
    }

    if(i > 0){
        for(j = 0; j < strlen(input_msg) + 2; j++){
            printf("\b \b");
        }
        fflush(stdout);
        //Edit
        sprintf(input_msg_s, "%s: %s", username, input_msg);
        //Edit
        //if(send(MySocket, input_msg, strlen(input_msg), MSG_NOSIGNAL) < 0){
        if(send(MySocket, input_msg_s, strlen(input_msg_s), MSG_NOSIGNAL) < 0){
            printf("\n Connection Lost!");
            return;
        }else{
            printf("> %s", input_msg);
        }
        memset(input_msg, 0, strlen(input_msg));
    }

}

int connect_to_server(char *c_host, int c_port){

    int MySocket;
    struct sockaddr_in server;

    //Create a socket
    if((MySocket = socket(AF_INET , SOCK_STREAM , 0)) < 0){
        printf("\n> Could Not Create Socket");
        return -1;
    }

    server.sin_addr.s_addr = inet_addr(c_host);
    server.sin_family = AF_INET;
    server.sin_port = htons(c_port);

    //Connect to remote server
    if(connect(MySocket, (struct sockaddr *)&server , sizeof(server)) < 0){
        printf("\n>Error: Connection Failed\n");
        printf("        Please Check Internet Connection!\n");
        return -1;
    }

    return MySocket;
}

int hostname_to_ip(char *hostname, char *ip){
	struct hostent *he;
	struct in_addr **addr_list;
	int i;

	if ((he = gethostbyname(hostname )) == NULL){
		// get the host info
		printf("Error: Unable To Resolve Hostname");
		return 1;
	}

	addr_list = (struct in_addr **) he->h_addr_list;

	for(i = 0; addr_list[i] != NULL; i++){
		//Return the first one;
		strcpy(ip , inet_ntoa(*addr_list[i]) );
		return 0;
	}

	return 1;
}
