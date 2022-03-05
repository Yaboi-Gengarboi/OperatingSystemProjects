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

std::string one2two;
std::string two2one;
bool mail[2];

void sig_handler(int signo)
{
    if (signo == SIGINT)
        std::cout << "Received SIGINT\n";
}

void getMail(int new_socket, int position) 
{
    int valread;
    std::string buffer(1024, 0);
    
    while (true) 
    {  
        valread = read(new_socket, buffer.data(), 1024);
        
        if (position == 0) 
        {
            std::cout << "Client1 sent a message: " << buffer << '\n';
            one2two = buffer; 
            sleep(1); 
            mail[1] = true;
        }
        else 
        {
            std::cout << "Client2 sent a message: " << buffer << '\n';
            two2one = buffer; 
            sleep(1); 
            mail[0] = true;
        }
    }
}

void sendMail(int new_socket, int position) 
{
    while (true) 
    {
        if (mail[0])
        {
            std::cout << "Sending a message to Client1: " << two2one << '\n';
            send(new_socket, two2one.c_str(), two2one.size(), 0);
            mail[0] = false;
        }
        
        if (mail[1])
        {
            std::cout << "Sending a message to Client2: " << one2two << '\n';
            send(new_socket, one2two.c_str(), one2two.size(), 0);
            mail[1] = false;
        }
    }
}

void ServerFunc(int position) 
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
    
    std::thread firstUser(ServerFunc, 0);
    std::thread secondUser(ServerFunc, 1);
    
    firstUser.join();
    secondUser.join();
    return 0;
}

#endif // #if defined(__unix__)
