/*
This is the Server side of a chatting application 'ChatLite' written in C.
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

#include "lib/sds/sds.c"

int TotalClient = 0;
int *ClientArray;
int *CloseSignal;
char MessageQueue[1024];

int client_list_init(int **list);
int client_list_add(int **list, int id);
int client_list_remove(int **list, int id);


#include "listen.c"
#include "handler.c"

int main(){
    int i, MainSocket = -1, AcceptSocket = -1;
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    pthread_t child_thread;

    ClientArray = malloc(sizeof(int) * 100);
    CloseSignal = malloc(sizeof(int) * 100);
    client_list_init(&ClientArray);
    client_list_init(&CloseSignal);

    MainSocket = ListenSocket("0.0.0.0", 1258);

    for(i = 0; i < 100; i++){
        ClientArray[i] = -1;
        CloseSignal[i] = -1;
    }

    while(1){
        AcceptSocket = -1;

        while(AcceptSocket == -1){
            // Accept Connection Request
            AcceptSocket = accept(MainSocket, (struct sockaddr*)&cli_addr, &clilen);

            // Check if Socket is Connected
            if(AcceptSocket != -1){
                // Create a Thread
                TotalClient++;
                if(pthread_create(&child_thread, NULL, handle_connection, (void *)AcceptSocket)){
                    printf("\nError: Creating Thread");
                    return -1;
                }
                AcceptSocket = -1;
            }

        }
    }

    return 0;
}

int client_list_init(int **list){
    int i;
    for(i = 0; i < 100; i++){
        (*list)[i] = -1;
    }
    return 0;
}

int client_list_add(int **list, int id){
    int i;

    // Check if already exists
    for(i = 0; i < 100; i++){
        if((*list)[i] == id){
            return 1;
        }
    }

    // Add in a blank slot
    for(i = 0; i < 100; i++){
        if((*list)[i] == -1){
            (*list)[i] = id;
            return 1;
        }
    }
    return 0;
}

int client_list_remove(int **list, int id){
    int i;

    // Find and delete
    for(i = 0; i < 100; i++){
        if((*list)[i] == id){
            (*list)[i] = -1;
            return 1;
        }
    }
    return 0;
}
