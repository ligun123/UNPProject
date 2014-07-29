//
//  quick_listen.c
//  UNPProject
//
//  Created by HalloWorld on 14-7-25.
//  Copyright (c) 2014年 HalloWorld. All rights reserved.
//

#include "unp.h"

int quick_listen(int port)
{
    int listenfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    struct sockaddr_in svaddr;
    memset(&svaddr, 0, sizeof(svaddr));
    svaddr.sin_family = AF_INET;
    svaddr.sin_port = htons(port);
    svaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(listenfd, (struct sockaddr *)&svaddr, sizeof(svaddr));
    
    Listen(listenfd, LISTENQ);
    return listenfd;
}
