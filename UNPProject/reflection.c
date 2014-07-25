//
//  reflection.c
//  UNPProject
//
//  Created by HalloWorld on 14-7-25.
//  Copyright (c) 2014年 HalloWorld. All rights reserved.
//

#include "unp.h"

int main(int argc, const char * argv[])
{
    printf("reflection.c\n");
    int lsfd = quick_listen(5555);
    
    socklen_t len;
    struct sockaddr_in clientAddr;
    char ipBuffer[16];
    pid_t childPid;
    
    while (TRUE) {
        len = sizeof(clientAddr);
        int clientfd = Accept(lsfd, &clientAddr, &len);
        printf("connection from %s:%d\n", inet_ntop(AF_INET, &clientAddr.sin_addr, ipBuffer, sizeof(ipBuffer)), ntohs(clientAddr.sin_port));
        if ((childPid = Fork()) == 0) {
            Close(lsfd);
            str_echo(clientfd);
            exit(0);
        }
        Close(clientfd);
    }
    Close(lsfd);
}
