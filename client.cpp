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

    while(true) {
      if(startsWith("disc", s_buffer)) break;

      if(startsWith("rnr", r_buffer)) {
        send(server, "rr0p", sizeof(s_buffer), 0);
      }
      else if(startsWith("rr", r_buffer) && r_buffer[3] == 'f') {
        send(server, s_buffer, sizeof(s_buffer), 0);
      }
      else if(startsWith("rej", r_buffer)) {
        s_buffer[1] = r_buffer[3];
        send(server, s_buffer, sizeof(s_buffer), 0);
      }
      else {
        printf("Send: ");
        fgets(s_buffer, 5, stdin);
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
