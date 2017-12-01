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

int main(int argc, char* argv[]) {
    if (argc != 2) {
      printf("usage: %s server-name\n", argv[0]);
      return 1;
    }

    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2,2), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr(argv[1]); // ip mesma maquina "127.0.0.1"
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr));
    printf("Connected to server!\n");

    char s_buffer[5];
    char r_buffer[5];

    while(true) {
      if(startsWith("disc", s_buffer)) {
        send(server, "UA", sizeof(s_buffer), 0);
        break;
      }

      if(startsWith("rnr", r_buffer)) {
        send(server, "rr0p", sizeof(s_buffer), 0);
      }
      else if(startsWith("rr", r_buffer) && r_buffer[3] == 'f') {
        s_buffer[2] = r_buffer[2];
        send(server, s_buffer, sizeof(s_buffer), 0);
      }
      else if(startsWith("rej", r_buffer)) {
        s_buffer[1] = r_buffer[3];
        send(server, s_buffer, sizeof(s_buffer), 0);
      }
      else {
        printf("Send: ");
        fgets(s_buffer, 5, stdin);
        if(s_buffer[strlen(s_buffer)-1] == '\n')
          s_buffer[strlen(s_buffer)-1]='\0'; //removendo quebra de linha
        send(server, s_buffer, sizeof(s_buffer), 0);
      }

      recv(server, r_buffer, sizeof(r_buffer), 0);
      printf("Server: %s\n", r_buffer);
    }

    closesocket(server);
    WSACleanup();
    printf("Socket closed.\n");
    return 0;
}
