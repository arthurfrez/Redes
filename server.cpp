#include <cstdio>
#include <winsock2.h>
#include <cstring>
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
    char miniBuffer[100];
    for(int i = 0; i<100; i++) miniBuffer[i] = buffer[i];
    char *token;
    int actPos = 1;

    token = strtok(miniBuffer, ", ");
    while(actPos != pos) {
      token = strtok(NULL, ", ");
      actPos += 1;
    }

    return atoi(token);
}

//==============================================================================
// METODO PRINCIPAL
//==============================================================================
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

    FILE * logFile;
    logFile = fopen ("log.txt","w");

    // variaveis do loop
    char s_buffer[100];
    char r_buffer[100];
    int ns = 0, nr = 0;
    int state = 0;
    int choose;

    if(client != INVALID_SOCKET) {
        while(true) {
          sleep(1);

          switch (state) {
      		  case 0:
             sprintf(s_buffer, "SABME");
      			 send(client, s_buffer, sizeof(s_buffer), 0);
             printf("Server: %s\n", s_buffer);
             if (logFile != NULL) {
               fputs ("Server: ", logFile);
               fputs (s_buffer, logFile);
               fputs ("\n", logFile);
             }
             state = 1;
      			 continue;
      			case 1:
      			 recv(client, r_buffer, sizeof(r_buffer), 0);
             printf("Client: %s\n", r_buffer);
             if (logFile != NULL) {
               fputs ("Client: ", logFile);
               fputs (r_buffer, logFile);
               fputs ("\n", logFile);
             }
      			 state = 2;
      			 continue;
      			case 2:
             sprintf(s_buffer, "I, %d, %d", nr, ns);
             ns++;
      			 send(client, s_buffer, sizeof(s_buffer), 0);
             printf("Server: %s\n", s_buffer);
             if (logFile != NULL) {
               fputs ("Server: ", logFile);
               fputs (s_buffer, logFile);
               fputs ("\n", logFile);
             }
      			 state = 4;
      			 continue;
      			case 3:
              sprintf(s_buffer, "DISC");
      				send(client, s_buffer, sizeof(s_buffer), 0);
              printf("Server: %s\n", s_buffer);
              if (logFile != NULL) {
                fputs ("Server: ", logFile);
                fputs (s_buffer, logFile);
                fputs ("\n", logFile);
              }
              state = 12;
      			  continue;
      			case 4:
      				recv(client, r_buffer, sizeof(r_buffer), 0);
              printf("Client: %s\n", r_buffer);
              if (logFile != NULL) {
                fputs ("Client: ", logFile);
                fputs (r_buffer, logFile);
                fputs ("\n", logFile);
              }

              if(startsWith("DISC", r_buffer)) {
      					state = 5;
      				} else if(startsWith("RNR", r_buffer)) {
      					ns = getValues(r_buffer, 3);
      					state = 6;
      				} else if(startsWith("RR", r_buffer)) {
      					ns = getValues(r_buffer, 3);
      					state = 8;
      				} else if (startsWith("I", r_buffer)) {
      					nr = getValues(r_buffer, 2)+1;
      					ns = getValues(r_buffer, 3);
      					state = 8;
      				}
      			  continue;
      			case 5:
              sprintf(s_buffer, "UA");
              send(client, s_buffer, sizeof(s_buffer), 0);
              printf("Server: %s\n", s_buffer);
              if (logFile != NULL) {
                fputs ("Server: ", logFile);
                fputs (s_buffer, logFile);
                fputs ("\n", logFile);
              }
              state = 12;
      			  continue;
      			case 6:
              sprintf(s_buffer, "RR, %d, P", nr);
      				send(client, s_buffer, sizeof(s_buffer), 0);
              printf("Server: %s\n", s_buffer);
              if (logFile != NULL) {
                fputs ("Server: ", logFile);
                fputs (s_buffer, logFile);
                fputs ("\n", logFile);
              }
              state = 7;
      			  continue;
      			case 7:
      				recv(client, r_buffer, sizeof(r_buffer), 0);
              printf("Client: %s\n", r_buffer);
              if (logFile != NULL) {
                fputs ("Client: ", logFile);
                fputs (r_buffer, logFile);
                fputs ("\n", logFile);
              }

      				if (startsWith("RNR", r_buffer)) {
      					ns = getValues(r_buffer, 3);
      					state = 6;
      				} else if(startsWith("RR", r_buffer)) {
      					ns = getValues(r_buffer, 2);
      					state = 8;
      				}
      			  continue;
      			case 8:
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
                send(client, s_buffer, sizeof(s_buffer), 0);
                printf("Server: %s\n", s_buffer);
                if (logFile != NULL) {
                  fputs ("Server: ", logFile);
                  fputs (s_buffer, logFile);
                  fputs ("\n", logFile);
                }
      					state = 11;
      				} else if (choose == 1) {
                sprintf(s_buffer, "RNR %d", nr);
      					send(client, s_buffer, sizeof(s_buffer), 0);
                printf("Server: %s\n", s_buffer);
                if (logFile != NULL) {
                  fputs ("Server: ", logFile);
                  fputs (s_buffer, logFile);
                  fputs ("\n", logFile);
                }
                state = 9;
      				} else if (choose == 2) {
                sprintf(s_buffer, "RR %d", nr);
      					send(client, s_buffer, sizeof(s_buffer), 0);
                printf("Server: %s\n", s_buffer);
                if (logFile != NULL) {
                  fputs ("Server: ", logFile);
                  fputs (s_buffer, logFile);
                  fputs ("\n", logFile);
                }
                state = 4;
      				} else {
                sprintf(s_buffer, "I, %d, %d", nr, ns);
                ns++;
                send(client, s_buffer, sizeof(s_buffer), 0);
                printf("Server: %s\n", s_buffer);
                if (logFile != NULL) {
                  fputs ("Server: ", logFile);
                  fputs (s_buffer, logFile);
                  fputs ("\n", logFile);
                }
                state = 4;
      				}
      			  continue;
      			case 9:
      				recv(client, r_buffer, sizeof(r_buffer), 0);
              printf("Client: %s\n", r_buffer);
              if (logFile != NULL) {
                fputs ("Client: ", logFile);
                fputs (r_buffer, logFile);
                fputs ("\n", logFile);
              }

      				ns = getValues(r_buffer, 2);
      				state = 10;
      			  continue;
      			case 10:
              printf("\n-----------------------------------\n");
              printf("Escolha:\n");
              printf("0 - RNR\n");
              printf("1 - RR\n");
              printf("Opcao: ");
              scanf("%d", &choose);
              printf("\n");

              if (choose == 0) {
                sprintf(s_buffer, "RNR %d, F", nr);
      					send(client, s_buffer, sizeof(s_buffer), 0);
                printf("Server: %s\n", s_buffer);
                if (logFile != NULL) {
                  fputs ("Server: ", logFile);
                  fputs (s_buffer, logFile);
                  fputs ("\n", logFile);
                }
                state = 9;
      				} else {
                sprintf(s_buffer, "RR %d, F", nr);
      					send(client, s_buffer, sizeof(s_buffer), 0);
                printf("Server: %s\n", s_buffer);
                if (logFile != NULL) {
                  fputs ("Server: ", logFile);
                  fputs (s_buffer, logFile);
                  fputs ("\n", logFile);
                }
                state = 4;
      				}
      			  continue;
      			case 11:
      				recv(client, r_buffer, sizeof(r_buffer), 0);
              printf("Client: %s\n", r_buffer);
              if (logFile != NULL) {
                fputs ("Client: ", logFile);
                fputs (r_buffer, logFile);
                fputs ("\n", logFile);
              }
      				state = 12;
      			  continue;
      			case 12:
      			  break;
      		}

          break;
        }

        fclose (logFile);
        closesocket(client);
        printf("Client disconnected.\n");
    }

    return 0;
}
