#ifndef CHAT_H
#define CHAT_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<poll.h>

#define PORT "9034"
#define BACKLOGS 10
#define LISTENER_ERROR 100

void *get_ip_address(struct sockaddr *sa){
    if(sa->sa_family==AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int get_listner_socket(){
    int listener;
    int yes=1;
    int rv;

    struct addrinfo hints, *res, *p;

    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;

    if((rv=getaddrinfo(NULL,PORT,&hints,&res))!=0){
        perror("getaddrinfo");
        exit(1);
    }

    for(p=res;p!=NULL;p=p->ai_next){
        if((listener = socket(p->ai_family, p->ai_socktype,p->ai_protocol))<0){
            perror("socket");
            continue;
        }

        // reuse the socket
        setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));

        if(bind(listener,p->ai_addr,p->ai_addrlen)<0){
            close(listener);
            continue;
        }
        break;
    }
    freeaddrinfo(res);
    if(p==NULL){
        fprintf(stderr,"client : unable to get socket\n");
        return -LISTENER_ERROR;
    }
    if(listen(listener,10)==-1){
        perror("listen");
        exit(1);
    }
    return listener;
}

void add_fd(struct pollfd *pollfds[], int newfd, int *fd_count, int *fd_size){
    if(*fd_count==*fd_size){
        *fd_size *=2;           // double size

        *pollfds=realloc(*pollfds,sizeof(**pollfds)*(*fd_size));
    }

    (*pollfds)[*fd_count].fd=newfd;
    (*pollfds)[*fd_count].events=POLLIN;    // check ready to read

    (*fd_count)++;
}

void del_fd(struct pollfd pollfds[], int i, int *fd_count){
    pollfds[i] = pollfds[*fd_count-1];      // copy the one from the end over this
    (*fd_count)--;
}


#endif