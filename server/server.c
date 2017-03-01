/*
    C socket server example
*/

#include <stdlib.h>
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];

    //Create socket
    if ((socket_desc = socket(AF_INET , SOCK_STREAM , 0)) < 0)
        printf("Could not create socket");
    puts("Socket created");
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    //Listen
    listen(socket_desc , 3);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
    //Receive pass
    bzero(client_message, 2000);
    while( (read_size = recv(client_sock , client_message , 2000, 0)) > 0 )
    {
        //Send the message back to client
        if (!(strcmp(client_message, "testpass")))
        {
            write(client_sock , "ok", 2);
            puts(client_message);
            bzero(client_message, 2000);
            break ;
        }
        else
        {
            write(client_sock , "try again", 9);
            puts(client_message);
            bzero(client_message, 2000);
        }
    }
    //fork le gstreamer de la mort
    int pid;
    if ((pid = fork()) < 0)
    {
        puts("fils de fork !");
        exit (0);
    }
    else if (!pid)
    {
        sleep(3);
        system("gst-launch-1.0 rpicamsrc bitrate=1000000 ! 'video/x-h264,width=512,height=288' ! h264parse ! queue ! rtph264pay config-interval=1 pt=96 ! gdppay ! udpsink host=169.254.199.68 port=5000");
        exit (0);
    }
    //Receive a message from client
    while ((read_size = recv(client_sock , client_message , 2000, 0)) > 0)
    {
        write(client_sock , client_message, strlen(client_message));
        puts(client_message);
        bzero(client_message, 2000);
    }
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
        perror("recv failed");
    return 0;
}
