// OSLab3ChatServer
// demoClient.cpp
// Downloaded from https://alaska.instructure.com/courses/6364/files/1441843?module_item_id=377680
// Last modified on 2022-03-01 by Justyn Durnford and Dawson Hampton
// Client side C++ program to demonstrate Socket programming.

#if defined(__unix__)

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
using std::cin;
using std::cout;

#include <string>
using std::string;
using std::getline;

#define PORT 8080
   
int main(int argc, char* argv[])
{
    sleep(1);
    cout << "Now executing demoClient.\n";

    int sock = 0, valread = 0;
    struct sockaddr_in serv_addr;

    string buffer(1024, 0);
    string input("Hello from client!");

    // Create socket file descriptor.
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "Socket creation failed.\n";
        exit(EXIT_FAILURE);
    }
    
    cout << "Socket creation successful.\n";
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form.
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
        cout << "Invalid address/Address not supported.\n";
        exit(EXIT_FAILURE);
    }
    
    cout << "Address conversion successful.\n";
   
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "Connection Failed.\n";
        exit(EXIT_FAILURE);
    }
    
    cout << "Connection successful.\n";
    
    while (true)
    {
        cout << "Enter a message.\n";
        getline(cin, input);
        
        send(sock, input.c_str(), input.size(), 0);
        cout << "Hello message sent.\n";

        valread = read(sock, &buffer[0], 1024);
        cout << buffer << '\n';
    }

    return 0;
}

#endif // #if defined(__unix__)
