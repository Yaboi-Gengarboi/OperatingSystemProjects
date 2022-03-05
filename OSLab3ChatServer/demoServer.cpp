// OSLab3ChatServer
// demoServer.cpp
// Downloaded from https://alaska.instructure.com/courses/6364/files/1441843?module_item_id=377680
// Last modified on 2022-03-04 by Dawson Hampton and Justyn Durnford
// Server side C++ program to demonstrate Socket programming.

#if defined(__unix__)

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

#define PORT 8080

std::string One_to_Two;
std::string Two_to_One;
bool mail[2] = {0};

void sig_handler(int signo)
{
    if (signo == SIGINT)
        std::cout << "Received SIGINT\n";
}

void sendMail(int new_socket, int position) 
{
    std::string myMessage;
    std::string preface;
    
    std::cout << position << " has sendMail running!\n";
    
    if (position == 0) 
        std::string preface = "The message Client 0 sent was: ";
    if (position == 1) 
        std::string preface = "The message Client 1 sent was: ";
        
    std::string reply;
    
    while (true) 
    {
        if (position == 0) 
            myMessage = One_to_Two;
        else 
            myMessage = Two_to_One;
        
        if (mail[position] == true) 
        {
            mail[position] = false;
            reply = preface + myMessage;
            send(new_socket, reply.c_str(), reply.size(), 0);
            std::cout << "Message sent!\n\n";
        }
    }
}

void getMail(int new_socket, int position) 
{
    std::cout << position << " has getMail running!\n";
    int valread;
    std::string theirMessage;
    std::string returnMessage = "Got the message!\n";
    std::string buffer(1024, 0);
    
    while (true) 
    {  
        if (position == 1) 
            theirMessage = One_to_Two;
        else 
            theirMessage = Two_to_One;
            
        for (int i = 0; i < 1024; ++i) 
            buffer[i] = 0;
        
        valread = read(new_socket, buffer.data(), 1024);
        
        if (position == 1) 
        {
            One_to_Two = theirMessage; 
            sleep(1); 
            mail[0] = true;
        }
        else 
        {
            Two_to_One = theirMessage; 
            sleep(1); 
            mail[1] = true;
        }
        
        std::cout << "Their message is: " << theirMessage << '\n';
        send(new_socket, returnMessage.c_str(), returnMessage.size(), 0);
    }
}

void ServerFunc(std::string myMessage, std::string theirMessage, int position, bool mail[2]) 
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    std::string buffer(1024, 0);
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("%i is starting the threads!\n", position);
    
    std::thread get(getMail, new_socket, position);
    std::thread send(sendMail, new_socket, position);
    
    get.join();
    send.join();
}
    
int main(int argc, char* argv[])
{
    if (signal(SIGINT, sig_handler) == SIG_ERR) 
        std::cout << "\ncan't catch SIGINT\n";
    
    std::cout << "Starting the threads.\n";
    std::thread firstUser(ServerFunc, One_to_Two, Two_to_One, 0, mail);
    std::thread secondUser(ServerFunc, Two_to_One, One_to_Two, 1, mail);
    
    firstUser.join();
    secondUser.join();
    return 0;
}

#endif // #if defined(__unix__)
