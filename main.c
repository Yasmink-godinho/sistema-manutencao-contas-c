#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int numero_conta;
    char nome[50];
    double saldo;
    int ativo; // 1 para ativo, 0 para encerrada
} Cliente;

void cadastrarCliente(FILE *arq);
void consultarCliente(FILE *arq);
void atualizarCliente(FILE *arq);
void removerCliente(FILE *arq);
void listarClientes(FILE *arq);

int main() {
    FILE *arquivo = fopen("contas.bin", "rb+");
    
    if (arquivo == NULL) {
        arquivo = fopen("contas.bin", "wb+");
        if (arquivo == NULL) {
            printf("Erro ao abrir ou criar o arquivo.\n");
            return 1;
        }
    }

    int opcao;
    do {
        printf("\n=====================================\n");
        printf("     SISTEMA DE MANUTENCAO DE CONTAS\n");
        printf("=====================================\n");
        printf("1. Cadastrar um novo cliente em uma posicao especifica\n");
        printf("2. Consultar um cliente pelo numero da conta\n");
        printf("3. Atualizar o saldo de um cliente\n");
        printf("4. Encerrar conta (remover cliente)\n");
        printf("5. Listar todos os clientes\n");
        printf("6. Restaurar leitura do inicio (rewind) e listar\n");
        printf("7. Encerrar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado 

        switch(opcao) {
            case 1: cadastrarCliente(arquivo);
                break;
            case 2: consultarCliente(arquivo);
                break;
            case 3: atualizarCliente(arquivo);
                break;
            case 4: removerCliente(arquivo); 
                break;
            case 5: listarClientes(arquivo); 
                break;
            case 6: 
                rewind(arquivo); 
                printf("\n[rewind] Leitura restaurada para o inicio do arquivo!\n");
                listarClientes(arquivo); 
                break;
            case 7: printf("Encerrando o sistema... Ate logo!\n"); 
                break;
            default: printf("Opcao invalida! Tente novamente.\n");
        }
    } while(opcao != 7);

    fclose(arquivo);
    return 0;
}

void cadastrarCliente(FILE *arq) {
    int posicao;
    Cliente c;

    printf("\n-- CADASTRO DE CLIENTE --\n");
    printf("Digite a posicao desejada no arquivo (0, 1, 2...): ");
    scanf("%d", &posicao);
    getchar();

    printf("Digite o numero da conta: ");
    scanf("%d", &c.numero_conta);
    getchar();
    
    printf("Digite o nome do cliente: ");
    fgets(c.nome, 50, stdin);
    strtok(c.nome, "\n"); // Remove a quebra de linha do fgets

    printf("Digite o saldo inicial: ");
    scanf("%lf", &c.saldo);
    
    c.ativo = 1; 

    // fseek move o ponteiro para a posicao exata calculada em bytes
    fseek(arq, posicao * sizeof(Cliente), SEEK_SET);
    
    // Escreve a struct no arquivo
    fwrite(&c, sizeof(Cliente), 1, arq);
    printf("Cliente cadastrado com sucesso na posicao %d!\n", posicao);
}

void consultarCliente(FILE *arq) {
    Cliente c;
    int conta_busca;
    int achou = 0;

    printf("\n-- CONSULTAR CLIENTE --\n");
    printf("Digite o numero da conta que deseja buscar: ");
    scanf("%d", &conta_busca);

    rewind(arq); // Volta ao inicio para varrer o arquivo inteiro

    // Le registro por registro ate o fim do arquivo
    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo == 1 && c.numero_conta == conta_busca) {
            printf("\nCliente Encontrado:\n");
            printf("Conta: %d | Nome: %s | Saldo: R$ %.2f\n", c.numero_conta, c.nome, c.saldo);
            achou = 1;
            break; 
        }
    }

    if (achou == 0) {
        printf("Conta numero %d nao encontrada ou esta inativa.\n", conta_busca);
    }
}

void atualizarCliente(FILE *arq) {
    Cliente c;
    int conta_busca;
    int achou = 0;

    printf("\n-- ATUALIZAR SALDO --\n");
    printf("Digite o numero da conta para atualizar o saldo: ");
    scanf("%d", &conta_busca);

    rewind(arq);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo == 1 && c.numero_conta == conta_busca) {
            achou = 1;
            
            printf("Cliente atual: %s | Saldo Atual: R$ %.2f\n", c.nome, c.saldo);
            printf("Digite o novo saldo: ");
            scanf("%lf", &c.saldo);

            // O fread joga o ponteiro do arquivo uma posicao para a frente com isso voltamos um struct atras para sobrescrever o registro correto
            fseek(arq, -sizeof(Cliente), SEEK_CUR);
            
            // Grava os dados atualizados por cima
            fwrite(&c, sizeof(Cliente), 1, arq);
            printf("Saldo atualizado com sucesso!\n");
            break;
        }
    }

    if (achou == 0) {
        printf("Conta numero %d nao encontrada para atualizacao.\n", conta_busca);
    }
}

void removerCliente(FILE *arq) {
    Cliente c;
    int conta_busca;
    int achou = 0;

    printf("\n-- ENCERRAR CONTA (REMOVER) --\n");
    printf("Digite o numero da conta que deseja encerrar: ");
    scanf("%d", &conta_busca);

    rewind(arq);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo == 1 && c.numero_conta == conta_busca) {
            achou = 1;         
            c.ativo = 0; 

            // Volta o ponteiro para a posicao deste registro
            fseek(arq, -sizeof(Cliente), SEEK_CUR);
            
            // Sobrescreve o registro com o campo ativo atualizado para 0
            fwrite(&c, sizeof(Cliente), 1, arq);
            printf("Conta numero %d encerrada com sucesso!\n", conta_busca);
            break;
        }
    }

    if (achou == 0) {
        printf("Conta numero %d nao encontrada ou ja esta inativa.\n", conta_busca);
    }
}

void listarClientes(FILE *arq) {
    Cliente c;
    int contador = 0, posicaoArq = 0;

    printf("\n-- LISTA DE CLIENTES --\n");
    rewind(arq); 

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.ativo == 1) { 

            printf("Posicao arquivo: %d | Conta: %d | Nome: %s | Saldo: R$ %.2f\n", posicaoArq, c.numero_conta, c.nome, c.saldo);
            contador++;
        }
        posicaoArq++;
    }

    if (contador == 0) {
        printf("Nenhum cliente ativo encontrado.\n");
    }
}