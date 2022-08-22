//Modelo (Estruturas, Variaveis Globais, etc..)

#define MAXPECA 28

int jogoEmProgresso = 0; //Joga esta rodando

typedef struct peca{
    int lado1, lado2;
    int status; //0 - Jogador 1 | 1 - Jogador 2 | 2 - Compra | 3 - Mesa
} tipoPeca;

tipoPeca pecas[MAXPECA]; //Pecas geral
tipoPeca mesa[MAXPECA]; //Mesa do jogo

int pecasJogadas = 0; //Numero de pecas jogadas
int pecasParaCompra = 14; //Numero de pecas para compra
tipoPeca mesaOrdenada[MAXPECA]; //Mesa do jogo ordenada

int ponta[2]; //Pecas na ponta. Ponta esquerda [0:2] | Ponta direita [3:1]

typedef struct jogador{
    char nome[100];
} tipoJogador;

tipoJogador jogadores[2]; //Lista de jogadores
int jogadorAtual = 0; //Jogador atual para jogar

int numeroDeJogadores = 0; //Numero de jogadores jogando.

char aviso[500]; //Mensagens de avisos (Vez do jogador, jogou tal peca, etc...).
