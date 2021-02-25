//
//  main.c
//  Assignment5-Server-Local
//
//  Created by Tony on 2021/2/22.
//
#include "main.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char buf[MAXBUF];
int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
    }

    listenfd = open_listenfd(argv[1]);
    while (1) {
    clientlen = sizeof(clientaddr);
    connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen); //line:netp:tiny:accept
        getnameinfo((struct sockaddr *) &clientaddr, clientlen, hostname, MAXLINE,
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
    //doit(connfd);                                             //line:netp:tiny:doit
        sendB(connfd);
        close(connfd);                                            //line:netp:tiny:close
    }
    return 0;
}
