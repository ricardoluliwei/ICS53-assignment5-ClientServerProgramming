//
//  main.h
//  Assignment5-Server-Local
//
//  Created by Tony on 2021/2/22.
//

#ifndef main_h
#define main_h
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
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define    MAXLINE     8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */


int open_listenfd(char *port)
{
    struct addrinfo hints, *listp, *p;
    int listenfd, rc, optval=1;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;             /* Accept connections */
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* ... on any IP address */
    hints.ai_flags |= AI_NUMERICSERV;            /* ... using port number */
    if ((rc = getaddrinfo(NULL, port, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo failed (port %s): %s\n", port, gai_strerror(rc));
        return -2;
    }

    /* Walk the list for one that we can bind to */
    for (p = listp; p; p = p->ai_next) {
        /* Create a socket descriptor */
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;  /* Socket failed, try the next */

        /* Eliminates "Address already in use" error from bind */
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,    //line:netp:csapp:setsockopt
                   (const void *)&optval , sizeof(int));

        /* Bind the descriptor to the address */
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break; /* Success */
        if (close(listenfd) < 0) { /* Bind failed, try the next */
            fprintf(stderr, "open_listenfd close failed: %s\n", strerror(errno));
            return -1;
        }
    }


    /* Clean up */
    freeaddrinfo(listp);
    if (!p) /* No address worked */
        return -1;

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0) {
        close(listenfd);
    return -1;
    }
    return listenfd;
}

void doit(int connfd){
    dup2(connfd, STDOUT_FILENO);
    printf("Hello, World!\n");
}

void sendB(int connfd){
    size_t n;
    int i=0;
    char input[MAXLINE];
    char in_wo_sz[MAXLINE];
    FILE* twtr;
    twtr = fopen("TWTR.csv", "r");
    //fread(buf, 1, 10, twtr);
    //printf("%s\n", buf);
    while((n = read(connfd, input, MAXLINE)) != 0){
        printf("server received %d bytes\n", (int)n);
        
        for(i =1; i< strlen(input); i++){
            in_wo_sz[i-1] = input[i];
        }
        char* buffer;
        char* spliter = " \n";
        buffer = strtok(in_wo_sz, spliter);
        if(strcmp(buffer, "Prices")==0){
            buffer = strtok(NULL, spliter);
            printf("Printing Prices of %s\n", buffer);
            write(connfd, input , n);
        }
        if(strcmp(buffer, "Prices")==0){
            buffer = strtok(NULL, spliter);
            printf("Printing MaxProfit of %s\n", buffer);
            write(connfd, input , n);
        } else write(connfd, "Other command", n);
        
    }
}

#endif /* main_h */
