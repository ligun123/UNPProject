//
//  main.c
//  UNPProject
//
//  Created by HalloWorld on 14-7-25.
//  Copyright (c) 2014年 HalloWorld. All rights reserved.
//

#include "unp.h"

#include <time.h>

int main(int argc, const char * argv[])
{
    // insert code here...
    printf("time.c\n");
    
    int listenfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    struct sockaddr_in svaddr;
    memset(&svaddr, 0, sizeof(svaddr));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = htons(SERV_PORT);
    svaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(listenfd, (struct sockaddr *)&svaddr, sizeof(svaddr));
    
    Listen(listenfd, LISTENQ);
    
    struct sockaddr_in clientAddr;
    socklen_t len;
    int connfd = 0;
    char buffer[MAXLINE];
    while (TRUE) {
        len = sizeof(clientAddr);
        connfd = Accept(listenfd, (struct sockaddr *)&clientAddr, &len);
        printf("connection from %s:%d\n", inet_ntop(AF_INET, &(clientAddr.sin_addr), buffer, sizeof(buffer)), ntohs(clientAddr.sin_port));
        time_t ticks = time(NULL);
        snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buffer, strlen(buffer));
        Close(connfd);
    }
    Close(listenfd);
    return 0;
}

