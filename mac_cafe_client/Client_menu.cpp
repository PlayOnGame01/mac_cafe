#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <Windows.h>
#include <iostream>
using namespace std;

int main()
{
    WSADATA wsadata;

    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (res != NO_ERROR)
    {
        cout << "WSAStartup error " << res << endl;
        return 1;
    }

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET)
    {
        cout << "socket error " << WSAGetLastError() << endl;
        return 2;
    }

    sockaddr_in addrTo;
    addrTo.sin_family = AF_INET;
    addrTo.sin_port = htons(23000);
    inet_pton(AF_INET, "127.0.1.2", &addrTo.sin_addr.s_addr);

    const size_t sendBufSize = 1024;
    cout << "Wait please: " << endl;
    char sendBuf[sendBufSize];
    cin.getline(sendBuf, sendBufSize);

    cout << "Sending data...." << endl;
    int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&addrTo, sizeof(addrTo));
    if (sendResult == SOCKET_ERROR)
    {
        cout << "sendto error " << WSAGetLastError() << endl;
        return 4;
    }

    const size_t receiveBufSize = 1024;
    char receiveBuf[receiveBufSize];

    sockaddr_in addrFrom;
    int addrFromSize = sizeof(addrFrom);

    cout << "Receiving data...." << endl;
    int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&addrFrom, &addrFromSize);
    if (bytesReceived == SOCKET_ERROR)
    {
        cout << "recvfrom error " << WSAGetLastError() << endl;
        return 4;
    }

    receiveBuf[bytesReceived] = '\0';
    cout << "Received from: " << addrFrom.sin_addr.s_host << endl;
    cout << "Wait: " << receiveBuf << endl;

    char receiveBuf2[receiveBufSize];
    int bytesReceived2 = recvfrom(udpSocket, receiveBuf2, receiveBufSize, 0, (SOCKADDR*)&addrFrom, &addrFromSize);
    if (bytesReceived2 == SOCKET_ERROR)
    {
        cout << "recvfrom error " << WSAGetLastError() << endl;
        return 4;
    }
    receiveBuf2[bytesReceived2] = '\0';
    cout << receiveBuf2 << endl;

    closesocket(udpSocket);
    WSACleanup();
}