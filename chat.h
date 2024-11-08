#ifndef CHAT_H
#define CHAT_H

#define _GNU_SOURCE

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
#include<ctype.h>

#include "badwords.h"
#include "constants.h"



char logmsg[120];
char chatLog[4096];


/*
TODO
X   multiple connections at socket bug
X   log the ips and the usernames
X   log the chats
X   print the past chts upon newusr login
    userlist -> make dynamic ??
    add comments
X   add spam filter user says sensored shii 3 times, kick out
    some emoji type shii (like if they type sth then a certain figure gets sent type?)
    [WAYY FUTURE] passwords??????

*/

void log_ip_usr(char *log){
    FILE *logptr;
    logptr=fopen("log","a");
    if(!logptr){
        fprintf(stderr,"unabe to open log file\n");
        return;
    }
    fprintf(logptr,"%s\n",log);
    fclose(logptr);
}

void log_chat(char* msg){
    sprintf(&chatLog[strlen(chatLog)], "%s\n", msg);
}

int spam_check(char *msg){
    //strcasestr()    // compare and check for a substring in a string and ignore CASE
    for(int i=0;i<sizeof(censor_words)/sizeof(censor_words[0]);i++){
        if(strcasestr(msg,censor_words[i])!=NULL){
            printf("A censored word was said\n");
            return -SPAMSAID;
        }
        return 0;
    }
}

typedef struct{
    int fd;
    char username[MAXUSERNAMELEN];
    int registered;
    int spam_msg_count;
}client_info;

client_info clients[MAXUSERS];



int client_count =0;

// Function to trim trailing whitespace/newline from a string
void trim_trailing_whitespace(char *str) {
    int length = strlen(str);
    while (length > 0 && isspace((unsigned char)str[length - 1])) {
        str[length - 1] = '\0';  // Replace trailing space or newline with null terminator
        length--;
    }
}


void register_username(int client_index, char *msg) {
    trim_trailing_whitespace(msg);
    memset(clients[client_index].username, 0, MAXUSERNAMELEN);
    // Copy up to MAXUSERNAMELEN - 1 characters, and ensure null-termination
    strncpy(clients[client_index].username, msg, MAXUSERNAMELEN - 1);
    trim_trailing_whitespace(clients[client_index].username);
    clients[client_index].registered = 1;
    printf("User registered as: %s\n", clients[client_index].username);
    snprintf(logmsg,sizeof(logmsg),"User registered as: %s\n", clients[client_index].username);
    log_ip_usr(logmsg);
    memset(logmsg,0,sizeof(logmsg));
    
}

void broadcast_message(struct pollfd pollfds[], int i, char *msg, int msg_len, int fd_count, int listener, int sender_fd) {
    // Ensure msg is null-terminated
    msg[msg_len] = '\0';

    // Register username if not already registered
    if (clients[i].registered==0) {
        register_username(i, msg);
        return; // Return to avoid broadcasting the username as a message
    }

    // Define a buffer for the final message
    char message_with_username[MAXUSERNAMELEN + MAXMSGLEN + 4] = {0}; // 3 extra for ": " and null terminator

    // Create the formatted message
    snprintf(message_with_username, sizeof(message_with_username), "%s: %s", clients[i].username, msg);
    log_chat(message_with_username);  
    

    // Broadcast the message to all clients except the sender and listener

    for (int k = 0; k < fd_count; k++) {
        int dest_fd = pollfds[k].fd;
        if (dest_fd != listener && dest_fd != sender_fd) {
            if (send(dest_fd, message_with_username, strlen(message_with_username), 0) == -1) {
                perror("send");
            }
        }
    }
    memset(msg, 0, MAXMSGLEN);                     // Clear msg buffer for future use
    memset(message_with_username, 0, sizeof(message_with_username));
}

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
void init(){
    memset(clients,0,sizeof(clients));
    memset(chatLog,0,sizeof(chatLog));
}
void add_fd(struct pollfd *pollfds[], int newfd, int *fd_count, int *fd_size){
    
    if(*fd_count==*fd_size){
        *fd_size *=2;           // double size

        *pollfds=realloc(*pollfds,sizeof(**pollfds)*(*fd_size));
    }

    (*pollfds)[*fd_count].fd=newfd;
    (*pollfds)[*fd_count].events=POLLIN;    // check ready to read

    (*fd_count)++;
    if(send(newfd,CHATRESTORING,strlen(CHATRESTORING),0)==-1){
        perror("send");
    }
    if(send(newfd,chatLog,strlen(chatLog),0)==-1){
        perror("send");
    }
    if(send(newfd,CHATRESTORED,strlen(CHATRESTORED),0)==-1){
        perror("send");
    }
    if (send(newfd, CONNECTMESSAGE, strlen(CONNECTMESSAGE), 0) == -1) {
        perror("send");
    }
}

void del_fd(struct pollfd pollfds[], int i, int *fd_count){
    pollfds[i] = pollfds[*fd_count-1];      // copy the one from the end over this
    clients[i].registered=0;
    (*fd_count)--;
}


#endif