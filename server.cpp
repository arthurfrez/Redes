#include <cstdio>
#include <winsock2.h>
#include <unistd.h>

using namespace std;

// compilar com -lws2_32

bool startsWith(const char *pre, const char *str) {
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

int main() {
    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN serverAddr, clientAddr;

    WSAStartup(MAKEWORD(2,2), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);

    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server, 0);

    printf("Listening for incoming connections...\n");

    int clientAddrSize = sizeof(clientAddr);

    client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize);

    char s_buffer[5];
    char r_buffer[5];

    if(client != INVALID_SOCKET) {
        printf("Client connected!\n");

        while(true) {
          recv(client, r_buffer, sizeof(r_buffer), 0);
          printf("Client says: %s\n", r_buffer);

          if(startsWith("disc", r_buffer)) {
            send(client, "UA", sizeof(s_buffer), 0);
            break;
          }

          printf("Send: ");
          fgets(s_buffer, 5, stdin);
          if(s_buffer[strlen(s_buffer)-1] == '\n')
            s_buffer[strlen(s_buffer)-1]='\0'; //removendo quebra de linha
          send(client, s_buffer, sizeof(s_buffer), 0);
        }

        closesocket(client);
        printf("Client disconnected.\n");
    }

    return 0;
}
