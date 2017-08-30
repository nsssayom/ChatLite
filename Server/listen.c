
int ListenSocket(char *ip, int port){

    int MainSocket;
    struct sockaddr_in service;
    re_listen:
    service.sin_family = AF_INET; // AF_INET is the Internet address family.
    service.sin_addr.s_addr = inet_addr(ip); // Local IP
    service.sin_port = htons(port);// Local Port

    MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(MainSocket < 0){
        printf("Error: Socket Error - [ Invalid Socket ]\n");
        return 0;
    }else{
        printf("Server: Socket Loaded\n");
    }

    // Bind Socket
    if(bind(MainSocket, (struct sockaddr *)&service, sizeof(service)) < 0){
        printf("Error: Binding Failed at Port [ %d ].\n", port);
        printf("       Check If Port Is Already In Use.\n");
        close(MainSocket);
        port++;
        goto re_listen;
        return 0;
    }else{
        printf("Server: Binded Successfully\n");
        printf("Server: IP [ %s ]\n", ip);
    }

    if(listen(MainSocket, 5) < 0){
       printf("Error: Unable To Listen On Port [ %d ].\n", port);
    }else{
       printf("Server: Listening On Port [ %d ]\n", port);
    }

    printf("\n");
    return MainSocket;

}
