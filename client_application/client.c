/**
 *  @file	aesdsocket.c
 *  @author	Ruchit Naik
 *  @date	04/10/2022
 *	@brief	The file contains code to create IPv4 socket over port 3457. It receives string
 *			and write to a temporary file. The socket then reads the file and sends it back
 *			to the client. The socket handles SIGINT and SIGTERM to terminate the process
 *			smoothly and delete the temporary file used to write the string. The code also
 *			daemonizes the socket to run in background.
 *	@ref	https://www.delftstack.com/howto/c/getaddrinfo-in-c/
 *	        https://beej.us/guide/bgnet/html/#a-simple-stream-server
**/
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define PORT				"3457"
#define MAX_CONNECTIONS		10								//Number of pending requests being queued
#define MAXDATALEN			1024							//Maximum permissible data to be received over socket

#define SUCCESSFUL_READ     0
#define OVERFLOW            -2
#define FAIL_READ           -3

static int fd_socket; 

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char **argv){
	int ret;
	struct addrinfo hints, *p, *res;
	char s[INET_ADDRSTRLEN], buf[MAXDATALEN];
	
	memset(buf, 0, sizeof(buf));
	memset(&hints, 0, sizeof(hints));					    //Clear the hints datastructure

    hints.ai_flags = AI_PASSIVE;						    //Set the flag to make the socket suitable for bind

    //Setup syslog logging for the utility using LOG_USER
	openlog(NULL, 0, LOG_USER);

    if(argc != 2){
        //Execute further only if 2 arguments are passed
        printf("Enter only hostname as argument\n");
        syslog(LOG_ERR, "Invalid argument");
        closelog();
        exit(1);
    }

	ret = getaddrinfo(argv[1], PORT, &hints, &res);		    //The returned socket would be suitable for conneting
	if(ret != 0){
		syslog(LOG_ERR, "getaddrinfo: %s", gai_strerror(ret));
		closelog();
		return -1;
	}
	
    for(p=res; p!=NULL; p=p->ai_next){
        //Creating an socket endpoint for communication using IPv4 by socket streaming
        fd_socket = socket(PF_INET, SOCK_STREAM, 0);
        if(fd_socket == -1){
            syslog(LOG_ERR, "Error opening socket: %d", errno);
            closelog();
            continue;
        }

        ret = connect(fd_socket, res->ai_addr, res->ai_addrlen);
        if(ret == -1){
            syslog(LOG_ERR, "Error connect socket: %d", errno);
            closelog();
            continue;
        }
        break;                                              //Breaks when not able to connect any IP
    }

    if(p == NULL){
        //Unable to connect to any IP on the socket
        syslog(LOG_ERR, "Unable to connect to any IP");
        return -1;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof(s));
    printf("client: connecting to %s\n", s);

    //Clear when done with this structure
    freeaddrinfo(res);

    char *ptr = buf;
    int rc = 0, recv_cnt = 0;
    //forever while
    while(1){
        ptr = buf;
        recv_cnt = 0;
        while(1){
            ret = recv(fd_socket, ptr, 1, 0);
            if(ret == -1){
                syslog(LOG_ERR, "Error client recv: %d", errno);
                closelog();
                close(fd_socket);
                rc = FAIL_READ;
                exit(-1);
            }

            if(*ptr == '\0'){
                //End of reading string
                rc = SUCCESSFUL_READ;
                break;
            }
            else if(recv_cnt >=(MAXDATALEN-1)){
                syslog(LOG_ERR, "Client reciver buffer size exceeded");
                printf("Buffer size exceeded\n");
                rc = OVERFLOW;
                break;
            }   
            else{
                //Increment count if received a valid char successfully
                recv_cnt++;                                     //Maintain the receiver count to keep check
                ptr++;
            }
        }
        printf("Received string from Server: %s", buf);
        char test_buf[] = "response from client\n";
        ptr = test_buf;
        if(rc == SUCCESSFUL_READ){
            while(1){
                ret = send(fd_socket, ptr, 1, 0);
                if(ret == -1){
                    syslog(LOG_ERR, "Error client send: %d", errno);
                    closelog();
                    close(fd_socket);
                    exit(-1);
                }

                if(*ptr == '\0'){
                    //Complete buffer transmitted
                    break;
                }
                else{
                    //Increment pointer until '\0' received
                    ptr++;
                }
            }
        }
    }

    //This section of the code will never be executed
    closelog();
    close(fd_socket);
    return 0;
}