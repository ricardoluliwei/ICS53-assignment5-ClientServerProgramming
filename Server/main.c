//
//  main.c
//  Assignment5-Server-Local
//
//  Created by Tony on 2021/2/22.
//
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

#define MAXLINE  8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */
#define APPL_STR "APPL"
#define TWTR_STR "TWTR"
#define PRICE_STR "Prices"
#define MAXPROFIT_STR "MaxProfit"
#define DATA_NUM 503
#define DATE_LEN 11

struct APPL{
    char date[DATA_NUM][DATE_LEN];
    float close[DATA_NUM];
};

struct TWTR{
    char date[DATA_NUM][DATE_LEN];
    float close[DATA_NUM];
};

struct TWTR twtr_stock;
struct APPL appl_stock;

void read_file(char* file1, char* file2){
    FILE* fd1 = fopen(file1, "r");
    FILE* fd2 = fopen(file2, "r");
    char buf[1000];
    fgets(buf, 100, fd1);
    int i = 0;
    while(fgets(buf, 100, fd1)){
        char *buf2;
        buf2 = strtok(buf, ",");
        strcpy(appl_stock.date[i], buf2);
        buf2 = strtok(NULL, ",");
        buf2 = strtok(NULL, ",");
        buf2 = strtok(NULL, ",");
        buf2 = strtok(NULL, ",");
        appl_stock.close[i] = atof(buf2);
        printf("APPL Date: %s, Closing Price: %.2f\n", appl_stock.date[i], appl_stock.close[i]);
        //printf("%s\n", buf);
        i++;
    }
    fgets(buf, 100, fd2);
    i = 0;
    while(fgets(buf, 100, fd2)){
        char *buf2;
        buf2 = strtok(buf, ",");
        strcpy(twtr_stock.date[i], buf2);
        buf2 = strtok(NULL, ",");
        buf2 = strtok(NULL, ",");
        buf2 = strtok(NULL, ",");
        buf2 = strtok(NULL, ",");
        twtr_stock.close[i] = atof(buf2);
        printf("TWTR Date: %s, Closing Price: %.2f\n", twtr_stock.date[i], twtr_stock.close[i]);
        //printf("%s\n", buf);
        i++;
    }
    
    
}

float getPrice(char* stock, char* date){
    if(strncmp(stock, APPL_STR, 4) == 0){

    } else if(strncmp(stock, TWTR_STR, 4) == 0){

    }
    return -1;
}

float maxProfit(char* stock);

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

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    read_file("AAPL.csv", "TWTR.csv");
    return 0;
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
