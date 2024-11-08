#include "chat.h"

int main(){
    int listener;
    int newfd;
    struct sockaddr_storage user_addr;
    socklen_t addrlen;

    char buff[100];
    char userIP[INET6_ADDRSTRLEN];
    
    void init();

    // initialise with 5 users
    int fd_count=0;
    int fd_size=5;
    struct pollfd *pollfds = malloc(sizeof(*pollfds)*fd_size);

    listener=get_listner_socket();
    if(listener==-LISTENER_ERROR){
        exit(1);
    }

    // add a user
    pollfds[0].fd=listener;
    pollfds[0].events=POLLIN;
    fd_count=1;

    for(;;){
        int poll_count = poll(pollfds, fd_count, -1);   // -1 to block untill event
        if(poll_count==-1){
            perror("poll");
            exit(1);
        }

        for(int i=0;i<fd_count;i++){
            // check if any is ready to read
            if(pollfds[i].revents & POLLIN){    // ready to read
                // if listener is ready, handle the connection
                if(pollfds[i].fd==listener){
                    addrlen = sizeof(user_addr);
                    newfd=accept(listener,(struct sockaddr*)&user_addr,&addrlen);
                    if(newfd==-1){
                        perror("accept");
                        continue;
                    }
                    else{
                        add_fd(&pollfds,newfd,&fd_count,&fd_size);
                        printf("New connection from user at : %s on socket %d\n",inet_ntop(user_addr.ss_family,get_ip_address((struct sockaddr*)&user_addr),userIP,INET6_ADDRSTRLEN),newfd);
                        snprintf(logmsg,sizeof(logmsg),"New connection from user at : %s on socket %d\n",inet_ntop(user_addr.ss_family,get_ip_address((struct sockaddr*)&user_addr),userIP,INET6_ADDRSTRLEN),newfd);
                        log_ip_usr(logmsg);
                        memset(logmsg,0,sizeof(logmsg));                        
                    }
                }
                else{
                    // if not listener then a regular client
                    int sender_fd=pollfds[i].fd;
                    int nbytes=recv(sender_fd,buff,sizeof(buff),0);
                    if(nbytes<=0){
                        // error or connection closed(==0)
                        if(nbytes==0){
                            printf("server : socket %d left\n",sender_fd);
                        }
                        else{
                            perror("recv");
                        }

                        close(sender_fd);  // close the sender fd
                        del_fd(pollfds, i,&fd_count);
                    }
                    else{
                        // got data from user
                        if(spam_check(buff)==-SPAMSAID){
                            clients[i].spam_msg_count+=1;
                            if(clients[i].spam_msg_count>=KICKOUTTHRESHOLD){
                                del_fd(pollfds,i,&fd_count);
                                close(pollfds[i].fd);   
                            }
                        }
                        broadcast_message(pollfds,i,buff,strlen(buff),fd_count,listener,sender_fd);
                    }
                }
            }
        }
    }
    return 0;
}