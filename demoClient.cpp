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
#define LENGTH 1024

void checkMail(int sock, std::string& buffer) 
{
    int valread;
    std::cout << "I have checkMail running.\n";
    
    while(true) 
    {
        valread = read(sock, buffer.data(), LENGTH);
        std::cout << "I have a message! The message is: " << buffer << '\n';
    }
}

void sendMail(int sock) 
{
    std::string input;
    std::cout << "I have sendMail running.\n";
    
    while(true)
    {
        std::getline(std::cin, input);
        send(sock, input.c_str(), LENGTH, 0);
    }
}
   
int main(int argc, char* argv[])
{
    int sock = 0, valread = 0;
    struct sockaddr_in serv_addr;
    
    std::string input;
    std::string buffer(LENGTH, 0);
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "\n Socket creation error \n";
        return -1;
    }
       
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
           
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
        printf("\nInvalid address/Address not supported \n");
        return -1;
    }
       
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
        
    std::string start = "Greetings!\n";
    send(sock, start.c_str(), LENGTH, 0);
        
    std::thread check(checkMail, sock, std::ref(buffer));
    std::thread send(sendMail, sock);

    check.join();
    send.join();
    return 0;
}

#endif // #if defined(__unix__)
