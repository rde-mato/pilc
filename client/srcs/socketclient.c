#include "socketclient.h"

int     initsocket(char *av)
{
    int                 sock;
    struct sockaddr_in  server;

    if ((sock = socket(AF_INET , SOCK_STREAM , 0)) == -1)
    {
        write(2, "Could not create socket", 24);
        exit (1);
    }
    write(1, "Socket created\n", 16);
    server.sin_addr.s_addr = inet_addr(av);
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        write(2, "Connect failed. Error\n", 22);
        exit (1);
    }
    write(1, "Connected\n", 11);
    return(sock);
}

void    authentification(int sock)
{
    char    message[2000];
    char    server_reply[2000];
    char    retry = 2;

    bzero(message, 2000);
    while(1)
    {
        printf("Enter password : ");
        scanf("%s" , message);
        //Send some data
        if (send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            continue ;
        }
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        if (!(strcmp(server_reply, "ok")))
        {
            puts("pass accepted");
            break ;
        }
        else if (retry)
        {
            puts(server_reply);
            retry--;
        }
        else
        {
            puts("Server said ... YOU FAILED !");
            bzero(server_reply, 2000);
            exit (0);
        }
        bzero(server_reply, 2000);
    }
}
