#include <cstdio>
#include <winsock2.h>
#include <unistd.h>

using namespace std;

// compilar com -lws2_32

//------------------------------------------------------------------------------
// Metodo para checkar se a string comeca com a substring desejada
//------------------------------------------------------------------------------
bool startsWith(const char *pre, const char *str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

//------------------------------------------------------------------------------
// Metodo para extrair o valor da string recebida
//------------------------------------------------------------------------------
int getValues(char buffer[100], int pos) {
    char *token;
    int actPos = 1;

    token = strtok(buffer, ",");
    while(pos != actPos) {
      token = strtok(buffer, ",");
      actPos += 1;
    }

    return atoi(token);
}

//==============================================================================
// METODO PRINCIPAL
//==============================================================================
int main(int argc, char* argv[]) {

    // caso nao exista o parametro informando o ip
    if (argc != 2) {
      printf("usage: %s server-name\n", argv[0]);
      return 1;
    }

    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2,2), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr));
    printf("Connected to server!\n");

    // variaveis do loop
    char s_buffer[100];
    char r_buffer[100];
    int ns = 0, nr = 0;
    int state = 0;
    int choose;


    while(true) {
      sleep(1);
      //printf("Estado = %d\n", state);

      switch (state) {
        case 0:
         recv(server, r_buffer, sizeof(r_buffer), 0);
         printf("Server: %s\n", r_buffer);
         state = 1;
         continue;
        case 1:
         sprintf(s_buffer, "UA");
         send(server, s_buffer, sizeof(s_buffer), 0);
         printf("Client: %s\n", s_buffer);
         state = 2;
         continue;
        case 2:
          recv(server, r_buffer, sizeof(r_buffer), 0);
          printf("Server: %s\n", r_buffer);

          if(startsWith("DISC", r_buffer)) {
            printf("Ha1");
            state = 11;
          } else {
            nr = getValues(r_buffer, 2)+1;
            ns = getValues(r_buffer, 3);
            state = 4;
          }
          continue;
        case 3:
          recv(server, r_buffer, sizeof(r_buffer), 0);
          printf("Server: %s\n", r_buffer);
          state = 11;
          continue;
        case 4:
          printf("\n-----------------------------------\n");
          printf("Escolha:\n");
          printf("0 - DISC\n");
          printf("1 - RNR\n");
          printf("2 - RR\n");
          printf("3 - I\n");
          printf("Opcao: ");
          scanf("%d", &choose);
          printf("\n");

          if (choose == 0) {
            sprintf(s_buffer, "DISC");
            send(server, s_buffer, sizeof(s_buffer), 0);
            printf("Client: %s\n", s_buffer);
            state = 5;
          } else if (choose == 1) {
            sprintf(s_buffer, "RNR %d", nr);
            send(server, s_buffer, sizeof(s_buffer), 0);
            printf("Client: %s\n", s_buffer);
            state = 6;
          } else if (choose == 2) {
            sprintf(s_buffer, "RR %d", nr);
            send(server, s_buffer, sizeof(s_buffer), 0);
            printf("Client: %s\n", s_buffer);
            state = 6;
          } else {
            sprintf(s_buffer, "I, %d, %d", nr, ns);
            ns++;
            send(server, s_buffer, sizeof(s_buffer), 0);
            printf("Client: %s\n", s_buffer);
            state = 8;
          }
          continue;
        case 5:
          recv(server, r_buffer, sizeof(r_buffer), 0);
          printf("Server: %s\n", r_buffer);
          state = 12;
          continue;
        case 6:
          recv(server, r_buffer, sizeof(r_buffer), 0);
          printf("Server: %s\n", r_buffer);
          state = 7;
          continue;
        case 7:
          printf("\n-----------------------------------\n");
          printf("Escolha:\n");
          printf("0 - RNR\n");
          printf("1 - RR\n");
          printf("Opcao: ");
          scanf("%d", &choose);
          printf("\n");

          if (choose == 0) {
            sprintf(s_buffer, "RNR %d, F", nr);
            send(server, s_buffer, sizeof(s_buffer), 0);
            printf("Client: %s\n", s_buffer);
            state = 6;
          } else {
            sprintf(s_buffer, "RR %d, F", nr);
            send(server, s_buffer, sizeof(s_buffer), 0);
            printf("Client: %s\n", s_buffer);
            state = 8;
          }
          continue;
        case 8:
          recv(server, r_buffer, sizeof(r_buffer), 0);
          printf("Server: %s\n", r_buffer);

          if (startsWith("I", r_buffer)) {
            nr = getValues(r_buffer, 2)+1;
            ns = getValues(r_buffer, 3);
            state = 4;
          } else if (startsWith("RR", r_buffer)) {
            ns = getValues(r_buffer, 2);
            state = 4;
          } else if (startsWith("RNR", r_buffer)) {
            ns = getValues(r_buffer, 2);
            state = 9;
          } else {
            state = 11;
          }
          continue;
        case 9:
          sprintf(s_buffer, "RR, %d, P", nr);
          state = 11;
          continue;
        case 10:
          recv(server, r_buffer, sizeof(r_buffer), 0);
          printf("Server: %s\n", r_buffer);

          if (startsWith("RNR", r_buffer)) {
            nr = getValues(r_buffer, 2);
            state = 9;
          } else {
            nr = getValues(r_buffer, 2);
            state = 4;
          }
          continue;
        case 11:
          sprintf(s_buffer, "UA");
          send(server, s_buffer, sizeof(s_buffer), 0);
          printf("Client: %s\n", s_buffer);
          state = 12;
          continue;
        case 12:
          break;
      }

      break;
    }

    closesocket(server);
    WSACleanup();
    printf("Socket closed.\n");
    return 0;
}
