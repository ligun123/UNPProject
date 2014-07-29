//
//  main.c
//  client
//
//  Created by HalloWorld on 14-7-28.
//  Copyright (c) 2014年 HalloWorld. All rights reserved.
//

#include "unp.h"

int main(int argc, const char * argv[])
{
    // insert code here...
    printf("client.c!\n");
    struct sockaddr_in serverAddr;
    if (argc != 2) {
        err_quit("usage: tcpcli <IPAddress>");
    }
    
    int sockfd[5];
    for (int i = 0; i < 5; i ++) {
        sockfd[i] = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(SERV_PORT);
        Inet_pton(AF_INET, argv[1], &serverAddr.sin_addr);
        Connect(sockfd[i], (struct sockaddr *)&serverAddr, sizeof(serverAddr));
        printf("sockfd[%d] == %d\n", i, sockfd[i]);
    }
    
    str_cli(stdin, sockfd[0]);
    return 0;
}

