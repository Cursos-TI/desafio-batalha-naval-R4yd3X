#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Constantes do jogo
#define TAMANHO_TABULEIRO 10
#define TAMANHO_NAVIO 3
#define TAMANHO_HABILIDADE 5
#define AGUA 0
#define NAVIO 3
#define HABILIDADE 5

// Tipos de orientação para os navios
typedef enum {
    HORIZONTAL,
    VERTICAL,
    DIAGONAL_PRINCIPAL,
    DIAGONAL_SECUNDARIA
} Orientacao;

// Tipos de habilidades especiais
typedef enum {
    CONE,
    CRUZ,
    OCTAEDRO
} TipoHabilidade;

// Estrutura para representar uma habilidade
typedef struct {
    TipoHabilidade tipo;
    int linha;
    int coluna;
} Habilidade;

// Função para inicializar o tabuleiro com 0s (água)
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }
}

// Função para verificar se uma posição é válida para colocar um navio
bool posicaoValida(int linha, int coluna, Orientacao orientacao, int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    // Verifica se está dentro dos limites do tabuleiro
    switch (orientacao) {
        case HORIZONTAL:
            if (coluna + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return false;
            break;
        case VERTICAL:
            if (linha + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return false;
            break;
        case DIAGONAL_PRINCIPAL:
            if (linha + TAMANHO_NAVIO > TAMANHO_TABULEIRO || 
                coluna + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return false;
            break;
        case DIAGONAL_SECUNDARIA:
            if (linha + TAMANHO_NAVIO > TAMANHO_TABULEIRO || 
                coluna - TAMANHO_NAVIO + 1 < 0) return false;
            break;
    }
    
    // Verifica se não há navio já posicionado nessas coordenadas
    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        switch (orientacao) {
            case HORIZONTAL:
                if (tabuleiro[linha][coluna + i] != AGUA) return false;
                break;
            case VERTICAL:
                if (tabuleiro[linha + i][coluna] != AGUA) return false;
                break;
            case DIAGONAL_PRINCIPAL:
                if (tabuleiro[linha + i][coluna + i] != AGUA) return false;
                break;
            case DIAGONAL_SECUNDARIA:
                if (tabuleiro[linha + i][coluna - i] != AGUA) return false;
                break;
        }
    }
    
    return true;
}

// Função para posicionar um navio no tabuleiro
void posicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                     int linha, int coluna, Orientacao orientacao) {
    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        switch (orientacao) {
            case HORIZONTAL:
                tabuleiro[linha][coluna + i] = NAVIO;
                break;
            case VERTICAL:
                tabuleiro[linha + i][coluna] = NAVIO;
                break;
            case DIAGONAL_PRINCIPAL:
                tabuleiro[linha + i][coluna + i] = NAVIO;
                break;
            case DIAGONAL_SECUNDARIA:
                tabuleiro[linha + i][coluna - i] = NAVIO;
                break;
        }
    }
}

// Função para criar a matriz de uma habilidade
void criarMatrizHabilidade(int matriz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE], TipoHabilidade tipo) {
    int centro = TAMANHO_HABILIDADE / 2;
    
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            matriz[i][j] = 0; // Inicializa com 0
            
            switch (tipo) {
                case CONE:
                    // Formato de cone apontando para baixo
                    if (i >= j && i + j >= TAMANHO_HABILIDADE - 1) {
                        matriz[i][j] = 1;
                    }
                    break;
                    
                case CRUZ:
                    // Formato de cruz
                    if (i == centro || j == centro) {
                        matriz[i][j] = 1;
                    }
                    break;
                    
                case OCTAEDRO:
                    // Formato de octaedro (losango)
                    if (abs(i - centro) + abs(j - centro) <= centro) {
                        matriz[i][j] = 1;
                    }
                    break;
            }
        }
    }
}

// Função para aplicar uma habilidade ao tabuleiro
void aplicarHabilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                      Habilidade habilidade) {
    int matriz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
    criarMatrizHabilidade(matriz, habilidade.tipo);
    
    int centro = TAMANHO_HABILIDADE / 2;
    int linha_inicio = habilidade.linha - centro;
    int coluna_inicio = habilidade.coluna - centro;
    
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            int linha_tab = linha_inicio + i;
            int coluna_tab = coluna_inicio + j;
            
            // Verifica se está dentro dos limites do tabuleiro
            if (linha_tab >= 0 && linha_tab < TAMANHO_TABULEIRO && 
                coluna_tab >= 0 && coluna_tab < TAMANHO_TABULEIRO) {
                
                // Se a posição na matriz de habilidade for 1, marca no tabuleiro
                if (matriz[i][j] == 1) {
                    // Não sobrescreve os navios, apenas a água
                    if (tabuleiro[linha_tab][coluna_tab] == AGUA) {
                        tabuleiro[linha_tab][coluna_tab] = HABILIDADE;
                    }
                }
            }
        }
    }
}

// Função para imprimir o tabuleiro
void imprimirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("\nTabuleiro de Batalha Naval com Habilidades:\n");
    printf("   ");
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%2d ", i);
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            switch (tabuleiro[i][j]) {
                case AGUA:
                    printf(" . ");
                    break;
                case NAVIO:
                    printf(" N ");
                    break;
                case HABILIDADE:
                    printf(" * ");
                    break;
                default:
                    printf(" ? ");
            }
        }
        printf("\n");
    }
}

int main() {
    // Inicializa o tabuleiro
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    inicializarTabuleiro(tabuleiro);
    
    // Define as coordenadas e orientações dos navios
    // Navio 1 - Horizontal
    posicionarNavio(tabuleiro, 2, 3, HORIZONTAL);
    
    // Navio 2 - Vertical
    posicionarNavio(tabuleiro, 5, 7, VERTICAL);
    
    // Navio 3 - Diagonal Principal (i,i)
    posicionarNavio(tabuleiro, 1, 1, DIAGONAL_PRINCIPAL);
    
    // Navio 4 - Diagonal Secundária (i,9-i)
    posicionarNavio(tabuleiro, 0, 9, DIAGONAL_SECUNDARIA);
    
    // Define as habilidades especiais
    Habilidade habilidades[3] = {
        {CONE, 4, 4},     // Cone centrado em (4,4)
        {CRUZ, 7, 2},      // Cruz centrada em (7,2)
        {OCTAEDRO, 3, 6}   // Octaedro centrado em (3,6)
    };
    
    // Aplica as habilidades ao tabuleiro
    for (int i = 0; i < 3; i++) {
        aplicarHabilidade(tabuleiro, habilidades[i]);
    }
    
    // Imprime o tabuleiro com os navios e habilidades
    imprimirTabuleiro(tabuleiro);
    
    return 0;
}