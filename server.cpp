#include <cstdio>
#include <bitset>
#include <winsock2.h>

#define byte char

using namespace std;

const byte flag = (char)126;

// compilar com -lws2_32

int main() {
    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN serverAddr, clientAddr;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);

    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server, 0);

    printf("Listening for incoming connections...\n");

    byte buffer[1024];
    int clientAddrSize = sizeof(clientAddr);

    client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize);

    if(client != INVALID_SOCKET) {
        printf("Client connected!\n");

        while(recv(client, buffer, sizeof(buffer), 0) != 0) {
          printf("Client says: %s\n", buffer);
          memset(buffer, 0, sizeof(buffer));
        }

        closesocket(client);
        printf("Client disconnected.\n");
    }

    return 0;
}
