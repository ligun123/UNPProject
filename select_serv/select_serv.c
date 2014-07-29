//
//  main.c
//  select_serv
//
//  Created by HalloWorld on 14-7-29.
//  Copyright (c) 2014年 HalloWorld. All rights reserved.
//

#include "unp.h"

int main(int argc, const char * argv[])
{

    // insert code here...
    printf("select_serv\n");
    int listenfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in servAddr, cliaddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SERV_PORT);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(listenfd, (SA *)&servAddr, sizeof(servAddr));
    Listen(listenfd, LISTENQ);
    
    int maxfd = listenfd;
    int maxi = -1;
    int client[FD_SETSIZE], nReady;
    
    for (int i = 0; i < FD_SETSIZE; i ++) {
        client[i] = -1;
    }
    
    fd_set allset, rset;
    FD_ZERO(&allset);
    FD_ZERO(&rset);
    FD_SET(listenfd, &allset);
    
    socklen_t cliLen;
    int connfd, sockfd, i = 0;
    char buf[MAXLINE];
    ssize_t n = 0;
    while (TRUE) {
        rset = allset;
        nReady = Select(maxfd + 1, &rset, NULL, NULL, NULL);
        
        if (FD_ISSET(listenfd, &rset)) {
            cliLen = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA *)&cliaddr, &cliLen);
            char ipBuf[20];
            printf("connfd %d from %s:%d\n", connfd, Inet_ntop(AF_INET, &cliaddr.sin_addr, ipBuf, sizeof(ipBuf)), ntohs(cliaddr.sin_port));
            for (i = 0; i < FD_SETSIZE; i ++) {
                if (client[i] < 0) {
                    client[i] = connfd;
                    break ;
                }
            }
            if (i == FD_SETSIZE) {
                err_quit("too many clients");
            }
            FD_SET(connfd, &allset);
            
            if (connfd > maxfd) {
                maxfd = connfd;
            }
            
            if (i > maxi) {
                maxi = i;
            }
            
            if (--nReady <= 0) {
                continue ;
            }
        }
        
        for (i = 0; i <= maxi; i ++) {
            if ((sockfd = client[i]) < 0) {
                continue ;
            }
            if (FD_ISSET(sockfd, &rset)) {
                if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
                    //connection closed by client
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    printf("read on %d %s", sockfd, buf);
                    Write(sockfd, buf, n);
                }
                if (-- nReady <= 0) {
                    break ;
                }
            }
        }
    }
    
    return 0;
}

