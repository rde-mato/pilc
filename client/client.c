/*
    C ECHO client example using sockets
*/
#include <stdlib.h>
#include<stdio.h> //printf
#include <unistd.h>
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[2000] , server_reply[2000];
    char authmode;
    char retry;

    authmode = 1;
    retry = 2;
    //Create socket
    if ((sock = socket(AF_INET , SOCK_STREAM , 0)) < 0)
        printf("Could not create socket");
    puts("Socket created");
    server.sin_addr.s_addr = inet_addr("192.168.42.1"/*"127.0.0.1"*/);
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");
    //keep communicating with server
    bzero(message, 2000);
    while(1)
    {
        if (authmode)
            printf("Enter password : ");
        else
            printf("Enter message : ");
        scanf("%s" , message);
        //Send some data
        if (send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        if (!(strcmp(server_reply, "ok")) && authmode)
        {
            puts("pass accepted");
            //authmode = 0;
            break ;
        }
        else if (authmode && retry)
        {
            puts(server_reply);
            retry--;
        }
        else if (authmode)
        {
            puts("Server said ... YOU FAILED !");
            break ;
        }
        else
        {
            //puts("Server reply :");
            //puts(server_reply);
            bzero(server_reply, 2000);
            break ;
        }
        bzero(server_reply, 2000);
    }
    int pid;
    if ((pid = fork()) < 0)
    {
        puts("fils de fork !");
        exit (0);
    }
    else if (!pid)
    {
        system("gst-launch-1.0 udpsrc port=5000 ! gdpdepay ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink sync=false");
        exit (0);
    }
    while (1)
    {
        printf("Enter message : ");
        scanf("%s" , message);
        //Send some data
        if (send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        puts("Server reply :");
        puts(server_reply);
        bzero(server_reply, 2000);
    }
    close(sock);
    return 0;
}
