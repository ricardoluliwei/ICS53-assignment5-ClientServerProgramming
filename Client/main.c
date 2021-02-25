//
//  main.c
//  A5-Client-Local
//
//  Created by Tony on 2021/2/22.
//

#include <stdio.h>
#include "main.h"

int clientfd;

void getMaxProfit(char* stock){
    write(clientfd, stock, strlen(stock));
    read(clientfd, stock , MAXLINE);
    fputs(stock, stdout);
}

void getPrice(char* stock_date){
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    char *host, *port, input[MAXLINE], buf2[MAXLINE], *buffer;
    //rio_t rio;
    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = argv[2];
    clientfd = open_clientfd(host, port);
    char* spliter = " \n";
    while (1){ // while loop to get user input
        printf("> ");
        memset(input, 0, 80);
        fgets(input, (sizeof input / sizeof input[0]), stdin);
        if(input[strlen(input)-1] == '\n') input[strlen(input)-1]=0;
        strcpy(buf2, input);
        if(strcmp(input, "quit") == 0){break;}
        buffer = strtok(buf2, spliter);
        if(strcmp(buffer, "MaxProfit") == 0){
            buffer = strtok(NULL, spliter);
            char * stock = buffer;
            getMaxProfit(stock);
            continue;
        }
        if(strcmp(buffer, "Prices") == 0){
            char original_length = strlen(input);
            char buf3[MAXLINE];
            buf3[0] = original_length; // set first byte to size
            strcat(buf3, input);
            printf("%s\n", buf3);
            write(clientfd, buf3, strlen(buf3));
            read(clientfd, input  , MAXLINE);
            fputs(input, stdout);
        }
                   
    }
    /*
    while(fgets(buf, MAXLINE, stdin) != NULL){
        write(clientfd, buf, strlen(buf));
        read(clientfd, buf  , MAXLINE);
        fputs(buf, stdout);
        
    }
     */
    close(clientfd); //line:netp:echoclient:close
    exit(0);
    return 0;
}
