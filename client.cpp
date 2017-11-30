#include <cstdio>
#include <winsock2.h>

#define byte char

using namespace std;

const byte flag = (char)126;

// compilar com -lws2_32

int main() {
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip mesma maquina
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr));
    printf("Connected to server!\n");


    while(true) {
      printf("Mensagem a ser enviada: ");
      byte buffer[1024];
      fgets(buffer, 1024, stdin);

      // removendo a quebra de linha
      int pos = strlen(buffer);
      if(buffer[pos-1] == '\n') buffer[pos-1] = '\0';

      send(server, buffer, sizeof(buffer), 0);
      printf("Message sent!\n");
    }

    closesocket(server);
    WSACleanup();
    printf("Socket closed.\n");
    return 0;
}
