#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

const int BUFFLEN = 1024;
const char* PORT = "8080";

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(iResult != 0)
    {
        printf("fail to startup");
        return 1;
    }

    addrinfo* result = NULL;
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_socktype = SOCK_STREAM;
    iResult = getaddrinfo(NULL, PORT, &hints, &result);
    if(iResult != 0)
    {
        printf("fail to get addr info");
        WSACleanup();
        return 1;
    }

    
    return 0;
}