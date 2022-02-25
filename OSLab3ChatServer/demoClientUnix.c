// OSLab3ChatServer
// demoClientUnix.c
// Downloaded from https://alaska.instructure.com/courses/6364/files/1441843?module_item_id=377680
// Last modified on 2022-02-25 by Justyn Durnford
// Client side C program to demonstrate Socket programming.

#if defined(__unix__)

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
   
int main(int argc, char* argv[])
{
    printf("Now executing demoClientUnix\n");

    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    char buffer[1024] = { 0 };
    char* hello = "Hello from client!";

    // Create socket file descriptor.
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Socket creation successful.\n");
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form.
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
        perror("Invalid address/Address not supported.\n");
        exit(EXIT_FAILURE);
    }

    printf("Address conversion successful.\n");
   
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connection Failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Connection successful.\n");

    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent.\n");

    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);

    return 0;
}

#endif // #if defined(__unix__)