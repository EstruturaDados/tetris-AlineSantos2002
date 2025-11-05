#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura da peça
typedef struct {
    char nome;  // 'I', 'O', 'T', 'L'
    int id;     // Identificador único
} Peca;

// Estrutura da Fila Circular
typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

// Estrutura da Pilha
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
    int tamanho;
} Pilha;

// Variáveis globais
FilaCircular fila;
Pilha pilha;
int proximoId = 0;// Protótipos das funções
void inicializarSistema();
Peca gerarPeca();
void exibirEstado();
void exibirMenu();
void jogarPeca();
void reservarPeca();
void usarPecaReservada();
void trocarPecaAtual();
void trocaMultipla();

// Funções da Fila Circular
bool filaVazia();
bool filaCheia();
void enfileirar(Peca peca);
Peca desenfileirar();

// Funções da Pilha
bool pilhaVazia();
bool pilhaCheia();
void empilhar(Peca peca);
Peca desempilhar();

int main() {
    srand(time(NULL));
    inicializarSistema();
    
    printf("=== SISTEMA DE GERENCIAMENTO DE PEÇAS ===\n");
    printf("Fila Circular (%d peças) + Pilha (%d peças)\n\n", TAM_FILA, TAM_PILHA);
    
    int opcao;
    
    do {
        exibirEstado();
        exibirMenu();
        
        printf("Opção escolhida: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                jogarPeca();
                break;
            case 2:
                reservarPeca();
                break;
            case 3:
                usarPecaReservada();
                break;
            case 4:
                trocarPecaAtual();
                break;
            case 5:
                trocaMultipla();
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
        
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar(); getchar();
        }
        
    } while(opcao != 0);
    
    return 0;
}void inicializarSistema() {
    // Inicializar fila circular
    fila.frente = 0;
    fila.tras = 0;
    fila.tamanho = 0;
    
    // Inicializar pilha
    pilha.topo = -1;
    pilha.tamanho = 0;
    
    // Preencher a fila com peças iniciais
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(gerarPeca());
    }
}

Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    novaPeca.nome = tipos[rand() % 4];
    novaPeca.id = proximoId++;
    
    return novaPeca;
}

void exibirEstado() {
    printf("\n=== ESTADO ATUAL ===\n");
    
    // Exibir fila de peças
    printf("Fila de peças: ");
    if (filaVazia()) {
        printf("[ ] [ ] [ ] [ ] [ ]");
    } else {
        int i = fila.frente;
        int count = 0;
        
        while (count < fila.tamanho) {
            printf("[%c %d] ", fila.pecas[i].nome, fila.pecas[i].id);
            i = (i + 1) % TAM_FILA;
            count++;
        }
        
        // Preencher espaços vazios
        for (int j = fila.tamanho; j < TAM_FILA; j++) {
            printf("[ ] ");
        }
    }
    printf("\n");
    
    // Exibir pilha de reserva
    printf("Pilha de reserva: (Topo → base): ");
    if (pilhaVazia()) {
        printf("[ ] [ ] [ ]");
    } else {
        // Exibir do topo para a base
        for (int i = pilha.topo; i >= 0; i--) {
            printf("[%c %d] ", pilha.pecas[i].nome, pilha.pecas[i].id);
        }
        
        // Preencher espaços vazios
        for (int j = pilha.tamanho; j < TAM_PILHA; j++) {
            printf("[ ] ");
        }
    }
    printf("\n\n");
}

void exibirMenu() {
    printf("=== OPÇÕES DISPONÍVEIS ===\n");
    printf("1 - Jogar peça da frente da fila\n");
    printf("2 - Enviar peça da fila para a pilha de reserva\n");
    printf("3 - Usar peça da pilha de reserva\n");
    printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
    printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
    printf("0 - Sair\n");
}void jogarPeca() {
    if (filaVazia()) {
        printf("Erro: Fila vazia! Não há peças para jogar.\n");
        return;
    }
    
    Peca pecaJogada = desenfileirar();
    printf("Ação: Peça [%c %d] jogada!\n", pecaJogada.nome, pecaJogada.id);
    
    // Gerar nova peça para manter a fila cheia (se possível)
    if (!filaCheia()) {
        enfileirar(gerarPeca());
        printf("Nova peça gerada automaticamente.\n");
    }
}

void reservarPeca() {
    if (filaVazia()) {
        printf("Erro: Fila vazia! Não há peças para reservar.\n");
        return;
    }
    
    if (pilhaCheia()) {
        printf("Erro: Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    
    Peca pecaReservada = desenfileirar();
    empilhar(pecaReservada);
    
    printf("Ação: Peça [%c %d] movida da fila para a pilha.\n", 
           pecaReservada.nome, pecaReservada.id);
    
    // Gerar nova peça para manter a fila cheia
    if (!filaCheia()) {
        enfileirar(gerarPeca());
        printf("Nova peça gerada automaticamente.\n");
    }
}

void usarPecaReservada() {
    if (pilhaVazia()) {
        printf("Erro: Pilha vazia! Não há peças reservadas para usar.\n");
        return;
    }
    
    Peca pecaUsada = desempilhar();
    printf("Ação: Peça reservada [%c %d] usada!\n", pecaUsada.nome, pecaUsada.id);
}

void trocarPecaAtual() {
    if (filaVazia() || pilhaVazia()) {
        printf("Erro: É necessário ter peças na fila E na pilha para trocar.\n");
        return;
    }
    
    Peca pecaFila = desenfileirar();
    Peca pecaPilha = desempilhar();
    
    enfileirar(pecaPilha);
    empilhar(pecaFila);
    
    printf("Ação: Troca realizada entre [%c %d] (fila) e [%c %d] (pilha).\n",
           pecaFila.nome, pecaFila.id, pecaPilha.nome, pecaPilha.id);
}

void trocaMultipla() {
    if (fila.tamanho < 3 || pilha.tamanho < 3) {
        printf("Erro: É necessário ter pelo menos 3 peças na fila E na pilha.\n");
        return;
    }
    
    printf("Ação: Troca múltipla realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
    
    // Array temporário para armazenar as 3 peças da fila
    Peca tempFila[3];
    for (int i = 0; i < 3; i++) {
        tempFila[i] = desenfileirar();
    }
    
    // Array temporário para armazenar as 3 peças da pilha
    Peca tempPilha[3];
    for (int i = 0; i < 3; i++) {
        tempPilha[i] = desempilhar();
    }
    
    // Colocar as peças da pilha na fila (na ordem inversa para manter a ordem)
    for (int i = 2; i >= 0; i--) {
        enfileirar(tempPilha[i]);
    }
    
    // Colocar as peças da fila na pilha
    for (int i = 0; i < 3; i++) {
        empilhar(tempFila[i]);
    }
}// --- IMPLEMENTAÇÃO DA FILA CIRCULAR ---
bool filaVazia() {
    return fila.tamanho == 0;
}

bool filaCheia() {
    return fila.tamanho == TAM_FILA;
}

void enfileirar(Peca peca) {
    if (filaCheia()) {
        printf("Erro: Fila cheia!\n");
        return;
    }
    
    fila.pecas[fila.tras] = peca;
    fila.tras = (fila.tras + 1) % TAM_FILA;
    fila.tamanho++;
}

Peca desenfileirar() {
    if (filaVazia()) {
        printf("Erro: Fila vazia!\n");
        Peca vazia = {' ', -1};
        return vazia;
    }
    
    Peca peca = fila.pecas[fila.frente];
    fila.frente = (fila.frente + 1) % TAM_FILA;
    fila.tamanho--;
    
    return peca;
}

// --- IMPLEMENTAÇÃO DA PILHA ---
bool pilhaVazia() {
    return pilha.topo == -1;
}

bool pilhaCheia() {
    return pilha.topo == TAM_PILHA - 1;
}

void empilhar(Peca peca) {
    if (pilhaCheia()) {
        printf("Erro: Pilha cheia!\n");
        return;
    }
    
    pilha.pecas[++pilha.topo] = peca;
    pilha.tamanho++;
}

Peca desempilhar() {
    if (pilhaVazia()) {
        printf("Erro: Pilha vazia!\n");
        Peca vazia = {' ', -1};
        return vazia;
    }
    
    pilha.tamanho--;
    return pilha.pecas[pilha.topo--];
}
