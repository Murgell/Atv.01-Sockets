#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>

#include "proto.h"

// funcao para tratar a logica de um cliente conectado
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_recebidos;

    printf("Cliente conectado. Aguardando requisicoes...\n");

    // loop para receber e processar mensagens
    while ((bytes_recebidos = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_recebidos] = '\0';

        // remove a quebra de linha para um parsing mais seguro
        buffer[strcspn(buffer, "\r\n")] = 0;

        printf("Recebido: \"%s\"\n", buffer);
        
        // checa pelo comando de saída
        if (strncmp(buffer, "QUIT", 4) == 0) {
            printf("Cliente solicitou desconexao.\n");
            break;
        }

        char op[4];
        double a, b;
        char resposta[BUFFER_SIZE];

        // tenta fazer o parsing da requisicao no formato "OP A B"
        if (sscanf(buffer, "%s %lf %lf", op, &a, &b) != 3) {
            snprintf(resposta, BUFFER_SIZE, "ERR EINV entrada_invalida\n");
        } else {
            double resultado = 0.0;
            int erro = 0;

            if (strcmp(op, "ADD") == 0) {
                resultado = a + b;
            } else if (strcmp(op, "SUB") == 0) {
                resultado = a - b;
            } else if (strcmp(op, "MUL") == 0) {
                resultado = a * b;
            } else if (strcmp(op, "DIV") == 0) {
                if (fabs(b) < 1e-9) { // checa dividir por zero
                    snprintf(resposta, BUFFER_SIZE, "ERR EZDV divisao_por_zero\n");
                    erro = 1;
                } else {
                    resultado = a / b;
                }
            } else {
                snprintf(resposta, BUFFER_SIZE, "ERR EINV operacao_desconhecida\n");
                erro = 1;
            }

            if (!erro) {
                snprintf(resposta, BUFFER_SIZE, "OK %.6f\n", resultado);
            }
        }
        
        // envia a resposta ao cliente
        send(client_socket, resposta, strlen(resposta), 0);
    }

    if (bytes_recebidos < 0) {
        perror("recv");
    }

    close(client_socket);
    printf("Conexao com cliente fechada.\n");
}

int main(int argc, char *argv[]) {
    int porta = DEFAULT_PORT;

    if (argc > 1) {
        porta = atoi(argv[1]);
    }

    // criacao do socket do servidor
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket");
        exit(1);
    }

    // definir o enderecamento do servidor
    struct sockaddr_in endereco_servidor;
    memset(&endereco_servidor, 0, sizeof(endereco_servidor));
    endereco_servidor.sin_family = AF_INET;
    endereco_servidor.sin_addr.s_addr = INADDR_ANY;
    endereco_servidor.sin_port = htons(porta);

    // bind do socket na porta
    if (bind(server_socket, (struct sockaddr *)&endereco_servidor, sizeof(endereco_servidor)) < 0) {
        perror("bind");
        exit(1);
    }

    // listen para aguardar conexoes
    if (listen(server_socket, 5) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Servidor escutando na porta %d\n", porta);

    // loop principal para aceitar conexoes
    while (1) {
        // accept para aceitar uma nova conexao de cliente
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("accept");
            continue; 
        }

        // lida com o cliente
        handle_client(client_socket);
    }

    // fecha o socket do servidor
    close(server_socket);
    return 0;
}
