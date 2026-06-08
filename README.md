
# Sistema de Manutenção de Contas Bancárias

Atividade prática desenvolvida em **Linguagem C** focada no gerenciamento, persistência e manipulação de registros em **arquivos binários de tamanho fixo**. O sistema simula de forma fiável as rotinas operacionais de manutenção de contas de uma agência bancária, aplicando conceitos avançados de ponteiros de arquivos, posicionamento físico em disco e persistência de dados estruturados.

---

## Tecnologias e Conceitos Computacionais Implementados

* **Manipulacao de Arquivos Binarios (.bin):** Armazenamento otimizado usando estruturas de tamanho fixo (struct), eliminando a conversao de dados e garantindo tamanho previsivel em disco.
* **Acesso Aleatorio com fseek():** Salta diretamente para qualquer posicao do arquivo sem ler os registros anteriores sequencialmente, calculando o deslocamento em bytes.
* **Leitura e Escrita Direta em Bloco (fread / fwrite):** Transferencia eficiente de blocos inteiros de memoria direto para o disco (e vice-versa) com uma unica chamada.
* **Restauracao de Fluxo com rewind():** Reposiciona o cursor de leitura instantaneamente de volta para o inicio absoluto do arquivo (byte zero).
* **Remocao Logica (Soft Delete):** Desativacao de registros via flag de controle (ativo = 0) para ocultar dados sem a necessidade de reestruturar o arquivo fisico.

---

## O Menu do Sistema 

O fluxo de interacao e a interface do usuario via terminal foram desenvolvidos seguindo rigorosamente a ordem, os termos e os criterios estabelecidos no enunciado da atividade academica:

1. **Cadastrar um novo cliente em uma posicao especifica:** Aloca a estrutura do cliente diretamente no indice fisico do arquivo escolhido pelo usuario, permitindo a criacao de arquivos esparsos.
2. **Consultar um cliente pelo numero da conta:** Realiza uma varredura sequencial do inicio ao fim do arquivo para localizar o identificador exclusivo, filtrando apenas registros logicamente ativos.
3. **Atualizar o saldo de um cliente:** Localiza a conta informada e altera o valor do saldo diretamente no arquivo fisico, utilizando posicionamento relativo do ponteiro.
4. **Encerrar conta (remover cliente):** Altera a flag do cliente para inativo (remocao logica).
5. **Listar todos os clientes:** Exibe na tela todos os dados atualmente guardados em disco que constam como ativos, indicando as suas respectivas posicoes fisicas.
6. **Restaurar leitura do inicio (rewind) e listar:** Executa nativamente a funcao rewind() para forcar o cursor do arquivo a regressar ao byte inicial e repete o algoritmo de listagem completa.
7. **Encerrar:** Garante o encerramento seguro e a liberacao de todos os descritores de arquivos abertos (fclose) antes de finalizar o processo do sistema.

---

## Arquitetura do Registro (Tamanho Fixo)

Para que o arquivo binario se conforme como um array em disco e o mapeamento aleatorio funcione, cada elemento guardado possui um tamanho estatico previsivel. 

A formula utilizada pelo fseek para encontrar qualquer cliente instantaneamente e:  
Posicao Fisica (Byte) = Indice Desejado x sizeof(Cliente)

```c
typedef struct {
    int numero_conta;   // 4 bytes - Identificador unico da conta
    char nome[50];      // 50 bytes - String estatica para o nome do cliente
    double saldo;       // 8 bytes - Ponto flutuante de dupla precisao para valores de saldo
    int ativo;          // 4 bytes - Flag de controle de integridade (1 = Ativo, 0 = Inativo)
} Cliente;
```

## Robustez do Código 

Durante o desenvolvimento do código-fonte, foram aplicadas boas práticas de programação em C para mitigar problemas clássicos de baixo nível:

* **Prevenção de Salto de Inputs:** Implementação sistemática da função `getchar()` após cada leitura numérica (`scanf`). Isto limpa o caractere de nova linha (`\n`) residual que fica preso no buffer do teclado, impedindo que as leituras de strings seguintes (`fgets`) sejam ignoradas.
* **Correção do Avanço Automático do Cursor:** As funções `fread()` avançam o ponteiro do ficheiro automaticamente após lerem um bloco. Nas funções de modificação (`atualizarCliente` e `removerCliente`), o sistema utiliza `fseek(arq, -sizeof(Cliente), SEEK_CUR)` para fazer o ponteiro "dar um passo atrás" antes de executar o `fwrite()`, garantindo que o registo atualizado seja gravado exatamente por cima do original.

---

## Como Executar o Projeto

Siga o passo a passo abaixo para descarregar, compilar, executar e testar o projeto no seu ambiente local.

### 1. Pré-requisitos

Certifique-se de que possui o **Git** e um compilador para C (como o **GCC**) instalados no seu computador.

### 2. Clonar o Repositório e Navegar

Abra o seu terminal e execute os seguintes comandos:

```bash
# Clonar o repositorio remoto do GitHub
git clone https://github.com/Yasmink-godinho/sistema-manutencao-contas-c.git

# Aceder a pasta do projeto
cd sistema-manutencao-contas-c

```

### 3. Compilar o Código-Fonte

Utilize o GCC para gerar o ficheiro executável binário do sistema:

```bash
gcc main.c -o sistema_contas

```

### 4. Executar o Programa

Dependendo do seu sistema operativo ou terminal, execute o comando correspondente:

* **No Linux, macOS ou Git Bash (Windows):**
```bash
./sistema_contas

```


* **No Prompt de Comando do Windows (CMD padrão):**
```cmd
sistema_contas.exe

```

---

## Roteiro de Testes Recomendado

Para validar todas as funcionalidades exigidas pelos critérios da atividade académica, siga esta sequência operacional de testes assim que abrir o programa:

1. **Cadastrar Cliente (Opção 1):** Cadastre um cliente na posição `0` (ex: Conta `1010`, Nome `Yasmim`, Saldo `1500.00`).
2. **Cadastrar Pulando Posição (Opção 1):** Cadastre um segundo cliente pulando propositadamente uma casa, inserindo-o na posição `2` (ex: Conta `2020`, Nome `Miguel`, Saldo `3000.00`).
3. **Listar Todos (Opção 5):** Veja que o sistema lista as duas contas mantendo as posições originais (`0` e `2`), provando que o espaço `1` ficou livre no arquivo binário graças ao `fseek`.
4. **Consultar por Conta (Opção 2):** Digite o número `2020` e comprove que o sistema traz os dados corretos do Miguel instantaneamente.
5. **Atualizar Saldo (Opção 3):** Escolha a conta `1010` e mude o saldo. Use a opção 5 em seguida para verificar se o saldo foi atualizado com sucesso.
6. **Encerrar Conta / Remoção Lógica (Opção 4):** Remova a conta `2020`. Ao listar novamente (Opção 5), note que ela já não será exibida, comprovando a eficácia do algoritmo de desativação lógica.
7. **Testar Persistência em Disco:** Escolha a opção `7` para sair do programa. Execute o programa novamente (Passo 4) e escolha a opção `5`. Note que os dados do cliente da posição `0` continuam guardados lá, provando que o arquivo persistiu em disco mesmo após fechar o sistema!


