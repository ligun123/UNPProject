#include	"unp.h"

void
str_cli(FILE *fp, int sockfd)
{
    int maxfdp1, stdineof = 0;
    fd_set rset;
    char buf[MAXLINE];
    ssize_t n;
    
    while (TRUE) {
        FD_ZERO(&rset);
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        Select(maxfdp1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(sockfd, &rset)) {
            if ((n = Read(sockfd, buf, MAXLINE)) == 0) {
                if (stdineof == 1) {
                    return ;
                } else {
                    err_quit("str_cli: server terminated prematurely");
                }
            }
            Write(fileno(stdout), buf, n);
        }
        
        if (FD_ISSET(fileno(fp), &rset)) {
            if ((n = Read(fileno(fp), buf, MAXLINE)) == 0) {
                stdineof = 1;
                Shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(fp), &rset);
                continue ;
            }
            Write(sockfd, buf, n);
        }
    }
}


/*  非i/o复用的客户端
void
str_cli(FILE *fp, int sockfd)
{
    char	sendline[MAXLINE], recvline[MAXLINE];
    
    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        
        Writen(sockfd, sendline, strlen(sendline));
        
        if (Readline(sockfd, recvline, MAXLINE) == 0)
            err_quit("str_cli: server terminated prematurely");
        Fputs(recvline, stdout);
    }
}
*/