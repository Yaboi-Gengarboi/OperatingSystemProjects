// OSLab3ChatServer
// demoServer.cpp
// Downloaded from https://alaska.instructure.com/courses/6364/files/1441843?module_item_id=377680
// Last modified on 2022-03-01 by Justyn Durnford and Dawson Hampton
// Server side C++ program to demonstrate Socket programming.

#if defined(__unix__)

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
using std::cout;

#include <functional>
using std::ref;

#include <string>
using std::string;

#include <thread>
using std::thread;

#define PORT 8080

bool hasClient1Sent, hasClient2Sent;
string client1Str, client2Str;

void serverFunc(string& my_str, string& their_str)
{
    int server_fd, new_socket, valread;

    struct sockaddr_in address;

    int opt = 1;
    int addrlen = sizeof(address);

    string buffer(1024, 0);
    string reply;
       
    // Create socket file descriptor.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        cout << "Socket creation failed.\n";
        exit(EXIT_FAILURE);
    }
    
    cout << "Socket creation successful.\n";
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        cout << "setsockopt failed.\n";
        exit(EXIT_FAILURE);
    }
    
    cout << "setsockopt successful.\n";

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
       
    // Forcefully attach socket to the port 8080.
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        cout << "Socket binding failed.\n";
        exit(EXIT_FAILURE);
    }
    
    cout << "Socket binding successful.\n";

    // Listen for connections on a socket.
    if (listen(server_fd, 3) < 0)
    {
        cout << "Listen failed.\n";
        exit(EXIT_FAILURE);
    }
    
    cout << "Listen successful.\n";

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
    {
        cout << "Acceptance error.\n";
        exit(EXIT_FAILURE);
    }
    
    cout << "Acceptance successful.\n";
    
    while (true)
    {
        sleep(1);
        
        valread = read(new_socket, &buffer[0], 1024);
        cout << buffer << '\n';
        
        reply = string("The message you sent was: ") + buffer;
        send(new_socket, reply.c_str(), reply.size(), 0);
        cout << "Reply sent!\n";
    }
}

int main(int argc, char* argv[])
{
    thread firstUser(serverFunc, ref(client1Str), ref(client2Str));
    thread secondUser(serverFunc, ref(client2Str), ref(client1Str));

    firstUser.join();
    return 0;
}

#endif // #if defined(__unix__)
