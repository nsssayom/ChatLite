
void *handle_connection(void *socket_id){
    int i, bytes_received, buffer_size = 1024;
    char buffer_data[1025], new_buffer[1025];
    int AcceptSocket = (int)socket_id;

    client_list_add(&ClientArray, AcceptSocket);
    printf("\nConnected [%d]", AcceptSocket);
    fflush(stdout);

    sprintf(buffer_data, "Welcome to ChatLite!");
    if(send(AcceptSocket, buffer_data, strlen(buffer_data), MSG_NOSIGNAL) == -1){
        client_list_add(&CloseSignal, AcceptSocket);
    }

    memset(buffer_data, 0, strlen(buffer_data));
    memset(new_buffer, 0, strlen(new_buffer));

    while(1){
        bytes_received = recv(AcceptSocket, buffer_data, buffer_size, MSG_NOSIGNAL);
        if(bytes_received > 0){
            // Data Received
            printf("\nReceived @ User-[%d]: %s", AcceptSocket, buffer_data);

            // Forward Message to Everyone
            printf(" :: Sending To [ ");
            for(i = 0; i < 100; i++){
                if(ClientArray[i] != -1){

                    //sprintf(new_buffer, "User-[%d]: %s", AcceptSocket, buffer_data);
                    sprintf(new_buffer, "%s", buffer_data);

                    if(ClientArray[i] != AcceptSocket){ // Don't Send To Sender
                        if(send(ClientArray[i], new_buffer, strlen(new_buffer), MSG_NOSIGNAL) == -1){
                            client_list_add(&CloseSignal, ClientArray[i]);
                        }else{
                            printf(" %d", ClientArray[i]);
                        }
                    }
                }
            }
            printf(" ]");

            fflush(stdout);
            memset(buffer_data, 0, strlen(buffer_data));
            memset(new_buffer, 0, strlen(new_buffer));
        }else if(bytes_received == -1){
            client_list_add(&CloseSignal, AcceptSocket);
        }

        // Check if received closed signal
        for(i = 0; i < 100; i++){
            if(CloseSignal[i] == AcceptSocket){
                client_list_remove(&ClientArray, AcceptSocket);
                client_list_remove(&CloseSignal, AcceptSocket);
                TotalClient--;
                printf("\nConnection Closed [%d]", AcceptSocket);
                close(AcceptSocket);
                fflush(stdout);
                pthread_exit(0);
                return NULL;
            }
        }
    }

    return NULL;
}
