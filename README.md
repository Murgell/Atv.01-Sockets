# Calculadora Cliente-Servidor em C
## João Rocha Murgel 10410293
## Como Compilar e Executar

**Compilação:**
```bash
make
```

**Execução (em dois terminais):**
```bash
# Terminal 1: Iniciar o Servidor
./server

# Terminal 2: Iniciar o Cliente
./client 127.0.0.1 5050
```

## Formato do Protocolo e Exemplos

A comunicação é baseada em um protocolo de texto simples.

| Tipo de Mensagem  | Formato                  | Descrição                                                          |
| :---------------- | :----------------------- | :----------------------------------------------------------------- |
| **Requisição**    | `OP A B\n`               | `OP` é a operação (`ADD`, `SUB`, `MUL`, `DIV`), e `A`, `B` são os números. |
| **Resposta (OK)**   | `OK R\n`                 | `R` é o resultado do cálculo.                                      |
| **Resposta (Erro)**| `ERR <COD> <msg>\n` | `COD` é um código de erro (`EINV`, `EZDV`) e `msg` explica a falha.    |

**Exemplos de Interação:**
-   `ADD 100 25.5` → `OK 125.500000`
-   `DIV 10 0` → `ERR EZDV divisao_por_zero`

Para encerrar a conexão, o cliente deve enviar o comando `QUIT`.

## Decisões de Projeto e Limitações Conhecidas

-   **Servidor Iterativo:** O servidor foi implementado para atender **um cliente por vez**. Uma nova conexão só é aceita após o cliente atual se desconectar. Esta abordagem foi escolhida por sua simplicidade e por atender aos requisitos mínimos da atividade.
-   **Parsing com `sscanf`:** A análise das requisições do cliente é feita com a função `sscanf`, uma escolha simples e eficaz para um protocolo com formato bem definido.
-   **Protocolo Baseado em Texto:** Um protocolo de texto simples foi escolhido por ser fácil de depurar e entender.
-   **Tratamento de Erros:** O servidor valida o número de tokens, o tipo de operação e a divisão por zero, retornando mensagens de erro claras ao cliente.
-   **Bonus:** Não foi implementado nenhum dos requisitos bonus.
## Como Rodar Testes

Não foi criados scripts para testes automatizados, então a verificação do programa foi realizada de forma manual, seguindo os passos:

1.  Compilar e iniciar o servidor e o cliente.
2.  Enviar requisições com entradas válidas para todas as operações (`ADD`, `SUB`, `MUL`, `DIV`).
3.  Enviar requisições com casos de erro conhecidos (divisão por zero, operação inválida, formato inválido).
4.  Verificar se as respostas do servidor correspondem aos resultados esperados em todos os casos.
