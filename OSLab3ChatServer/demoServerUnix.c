// OSLab3ChatServer
// demoServerUnix.c
// Downloaded from https://alaska.instructure.com/courses/6364/files/1441843?module_item_id=377680
// Last modified on 2022-02-25 by Justyn Durnford
// Server side C program to demonstrate Socket programming.

#if defined(__unix__)

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int main(int argc, char* argv[])
{
    printf("Now executing demoServerUnix.\n");

    int server_fd, new_socket, valread;

    struct sockaddr_in address;

    int opt = 1;
    int addrlen = sizeof(address);

    char buffer[1024] = {0};
    char* hello = "Hello from server!";
       
    // Create socket file descriptor.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Socket creation successful.\n");
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("setsockopt successful.\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
       
    // Forcefully attach socket to the port 8080.
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("Socket binding failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Socket binding successful.\n");

    // Listen for connections on a socket.
    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Listen successful.\n");

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
    {
        perror("Acceptance error.\n");
        exit(EXIT_FAILURE);
    }

    printf("Acceptance successful.\n");

    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);

    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent.\n");

    return 0;
}

#endif // #if defined(__unix__)