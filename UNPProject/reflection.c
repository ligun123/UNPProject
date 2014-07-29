//
//  reflection.c
//  UNPProject
//
//  Created by HalloWorld on 14-7-25.
//  Copyright (c) 2014年 HalloWorld. All rights reserved.
//

#include "unp.h"

void sig_child(int signo)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %d terminated\n", pid);
    }
    return ;
}

int main(int argc, const char * argv[])
{
    printf("reflection.c\n");
    int lsfd = quick_listen(SERV_PORT);
    Signal(SIGCHLD, sig_child);
    
    socklen_t len;
    struct sockaddr_in clientAddr;
    char ipBuffer[16];
    pid_t childPid;
    
    while (TRUE) {
        len = sizeof(clientAddr);
        int clientfd = Accept(lsfd, (struct sockaddr *)&clientAddr, &len);
        if (clientfd < 0) {
            if (errno == EINTR) {
                continue ;
            } else {
                err_sys("accept error");
            }
        }
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
