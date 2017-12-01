#include <cstdio>
#include <winsock2.h>

using namespace std;

// compilar com -lws2_32

int main(int argc, char* argv[]) {
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr(argv[1]); // ip mesma maquina "127.0.0.1"
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr));
    printf("Connected to server!\n");

    char s_buffer[5];
    char r_buffer[5];
    int numPac = 9;

    while(true) {
      if(!(r_buffer[0] == 'R' && r_buffer[0] == 'N' && r_buffer[0] == 'R')) {
        printf("Mensagem a ser enviada: ");
        fgets(s_buffer, 5, stdin);
        send(server, s_buffer, sizeof(s_buffer), 0);
      }
      else ;


      recv(server, r_buffer, sizeof(r_buffer), 0);
      printf("Mensagem recebida: %s\n", r_buffer);
    }

    closesocket(server);
    WSACleanup();
    printf("Socket closed.\n");
    return 0;
}
