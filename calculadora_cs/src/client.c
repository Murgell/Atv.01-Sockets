#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "proto.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <IP do Servidor> <Porta>\n", argv[0]);
        exit(1);
    }

    char *ip_servidor = argv[1];
    int porta = atoi(argv[2]);

    // criacao do socket do cliente
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("socket");
        exit(1);
    }

    // preenchimento do endereco do servidor
    struct sockaddr_in endereco_servidor;
    memset(&endereco_servidor, 0, sizeof(endereco_servidor));
    endereco_servidor.sin_family = AF_INET;
    endereco_servidor.sin_port = htons(porta);
    if (inet_pton(AF_INET, ip_servidor, &endereco_servidor.sin_addr) <= 0) {
        perror("inet_pton: Endereco IP invalido");
        exit(1);
    }

    // conectar ao servidor
    if (connect(client_socket, (struct sockaddr *)&endereco_servidor, sizeof(endereco_servidor)) < 0) {
        perror("connect");
        exit(1);
    }

    printf("Conectado ao servidor em %s:%d.\n", ip_servidor, porta);
    printf("Digite uma operacao (ex: ADD 10 5) ou 'QUIT' para sair.\n");

    char requisicao[BUFFER_SIZE];
    char resposta[BUFFER_SIZE];

    // lop de comunicacao
    while (1) {
        printf("> ");
        
        // le uma linha da entrada padrao
        if (fgets(requisicao, sizeof(requisicao), stdin) == NULL) {
            break; 
        }

        // envia a mensagem para o servidor
        if (send(client_socket, requisicao, strlen(requisicao), 0) < 0) {
            perror("send");
            break;
        }

        // se o comando for QUIT encerra o loop apos enviar
        if (strncmp(requisicao, "QUIT", 4) == 0) {
            printf("Desconectando do servidor.\n");
            break;
        }

        // recebe a resposta do servidor
        ssize_t bytes_recebidos = recv(client_socket, resposta, sizeof(resposta) - 1, 0);
        if (bytes_recebidos <= 0) {
            if (bytes_recebidos == 0) {
                printf("Servidor encerrou a conexao.\n");
            } else {
                perror("recv");
            }
            break;
        }
        
        resposta[bytes_recebidos] = '\0';

        // emprime a resposta
        printf("%s", resposta);
    }

    // fechar o socket
    close(client_socket);

    return 0;
}
