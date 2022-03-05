// OSLab3ChatServer
// demoClient.cpp
// Downloaded from https://alaska.instructure.com/courses/6364/files/1441843?module_item_id=377680
// Last modified on 2022-03-04 by Dawson Hampton and Justyn Durnford
// Client side C++ program to demonstrate Socket programming.

#if defined(__unix__)

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <functional>
#include <iostream>
#include <string>
#include <thread>

#define PORT 8080
   
int main(int argc, char* argv[])
{
    int sock = 0, valread = 0;
    struct sockaddr_in serv_addr;
    
    std::string input;
    std::string buffer(1024, 0);
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "\n Socket creation error \n";
        exit(EXIT_FAILURE);
    }
       
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
           
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
        printf("\nInvalid address/Address not supported \n");
        exit(EXIT_FAILURE);
    }
       
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        exit(EXIT_FAILURE);
    }
    
    while(true)
    {
        std::cout << "Enter a message.\n";
        std::getline(std::cin, input);
        send(sock, input.c_str(), input.size(), 0);
        std::cout << "Sent a message.\n";
        valread = read(sock, buffer.data(), buffer.size());
        std::cout << buffer << '\n';
    }
    
    return 0;
}

#endif // #if defined(__unix__)
