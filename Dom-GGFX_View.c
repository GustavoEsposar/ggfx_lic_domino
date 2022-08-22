//View - Interfaces

//Bibliotecas padrao

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
//Declaracoes funcoes
void limparBuffer(); //Limpa o buffer do teclado.
void mostrarPecasPossiveisDeJogar(); //Mostra as pecas possiveis do jogador atual.

char menuGeral(){ //Menu inicial
    printf("   _____    _____   ______  __   __\n"
           "  / ____|  / ____| |  ____| \\ \\ / /\n"
           " | |  __  | |  __  | |__     \\ V / \n"
           " | | |_ | | | |_ | |  __|     > <  \n"
           " | |__| | | |__| | | |       / . \\ \n"
           "  \\_____|  \\_____| |_|      /_/ \\_\\\n"
           "                                   \n");
    char escolha;
    printf("1) Iniciar novo jogo\n2) Continuar Jogo\n3) Regras do Jogo\n4) Sair do Jogo\n-> Escolha uma opcao: ");
    scanf(" %c", &escolha);
    return escolha;
}

void menuJogadores(){ //Menu apos o inicial para escolher quantidade de jogadores
    int qtJogadores;
    do{ //Roda enquanto a quantidade de jogadores nao for valida
        printf("\nQuantidade de jogadores (Max. 2): ");
        scanf("%d", &qtJogadores);
        if(qtJogadores > 2)
            printf("Valor maximo de jogadores: 2");
        else if(qtJogadores < 1)
            printf("Valor minimo de jogadores: 1");
    }while(qtJogadores > 2 || qtJogadores < 1); 
    
    limparBuffer();
    
    printf("Nome jogador 1: ");
    fgets(jogadores[0].nome, 100, stdin);
    jogadores[0].nome[strlen(jogadores[0].nome) - 1] = '\0'; //Remove o \n no final da string.
    
    strcpy(jogadores[1].nome, "Computador"); //Caso o jogador nao escolha 2 jogadores, o nome do 2 fica computador.
    
    if(qtJogadores == 2){
        printf("Nome jogador 2: ");
        fgets(jogadores[1].nome, 100, stdin);
        jogadores[1].nome[strlen(jogadores[1].nome) - 1] = '\0'; //Remove o \n no final da string.
    }

    numeroDeJogadores = qtJogadores; //Atualiza o numero de jogadores conforme a escolha.
}


char menuJogo(int jogador){ //Menu de acoes dentro do jogo
    char i, escolha, count = 0;
    printf("\nJogo de Domino (GGFX)\n");
    printf("Vez de: %s\n", jogadores[jogador].nome);

    printf("\n1) Jogar (Possiveis: ");

    mostrarPecasPossiveisDeJogar(); //Mostra as pecas possiveis de jogar.

    if(pecasParaCompra > 0)
        printf(")\n2) Comprar pecas (Restante: %d)\n3) Salvar Jogo\n4) Voltar menu principal\n-> Escolha uma opcao: ", pecasParaCompra);
    else
        printf(")\n2) Passar\n3) Salvar Jogo\n4) Voltar menu principal\n-> Escolha uma opcao: ");
        
    scanf(" %c", &escolha);
    return escolha;
}

int menuJogarPeca(int jogador){ //Menu para jogar a peca.
    int escolha;
    int i, aux = 1;
    printf("\n");
	for(i = 0; i < MAXPECA; i++){ //Roda pelas pecas do jogador e as numera para que ele possa escolher entre uma.
        if(mesa[i].status == jogador){
		    printf("(%d):\t[%d:%d]\n", aux, mesa[i].lado1, mesa[i].lado2);
		    aux++;
		}
    }
    printf("(%d):Voltar para o menu", aux);
    limparBuffer();
    
    printf("\n-> ");
    scanf("%d", &escolha);
    if(escolha >= aux)
        return 0;
    else
    	return escolha;
}

char menuEscolhaLado(){ //Menu de escolha para qual lado ele quer jogar a peca.
    char escolha;

    do{ //Garante que o usuario escolha 1 ou 2
        printf("Qual lado voce deseja jogar?\n1 - Esquerda\n2 - Direita\n-> ");
        scanf(" %c", &escolha);
    } while(escolha != '1' && escolha != '2');

    return escolha;
}

void mostrarPecasMesa(){ //Mostrar a mesa geral
    printf("\nMesa do jogo: ");

	int i, j;
	
	printf("\n");

    printf("    ________");
    
    for(i=0; i < pecasJogadas; i++)
        printf("_____");
    
    printf("\n   /        ");
    
    for(i=0; i < pecasJogadas; i++)
        printf("     ");
        
    printf("\\");
    printf("\n  /     "); 
    
    for(i = 0; i < MAXPECA; i++){ //Mostra todas as pecas na mesa.
        if(mesaOrdenada[i].status != -1)
		    printf("[%d:%d]", mesaOrdenada[i].lado1, mesaOrdenada[i].lado2);
    }
    
    printf("     \\");

    printf("\n /            ");
    for(i=0; i < pecasJogadas; i++)
        printf("     ");
        
    printf("\\");

    printf("\n/______________");
    for(i=0; i < pecasJogadas; i++)
        printf("_____");
        
    printf("\\");

    printf("\n|______________");
    for(i=0; i < pecasJogadas; i++)
        printf("_____");
        
    printf("|");
    
    for(j = 0; j < 3; j++){
        printf("\n     |    ");
        for(i=0; i < pecasJogadas; i++)
            printf("     ");
        printf("|     ");
    }
}

void mostrarPecasJogador(int jogador){ //Mostra as pecas de um jogador especifico.
    int i;
    printf("\nPecas de %s: ", jogadores[jogador].nome);
	for(i = 0; i < MAXPECA; i++){
        if(mesa[i].status == jogador)
		    printf("[%d:%d] ", mesa[i].lado1, mesa[i].lado2);
    }
	printf("\n");
}

void divisoria(){ //Monta as divisorias entre cada jogada
    int k;
    printf("\n-=-=-=-=-=-=-=-=-=-=-");
    if(pecasJogadas != 0)
        for(k = 1; k < pecasJogadas; k++)
            printf("=-=-=-");
    printf("\n");
}

void mostrarPecasPossiveisDeJogar(){
    int i;
    for(i = 0; i < MAXPECA; i++) //Roda pelas pecas e mostra as que sao possiveis jogar.
        if(mesa[i].status == jogadorAtual)
            if(checarValidadeJogar(mesa[i], 1) != -1)
                printf("[%d:%d] ", mesa[i].lado1, mesa[i].lado2);
}

void mostrarRegras(){
	printf("\nCada jogador pega sete pecas e pode comprar sempre "
	"que precisar de uma peca para poder jogar e quantas vezes for necessario. Da-se inicio ao jogo o "
	"jogador que tirar a peca \"[6:6]\". Caso ninguem a tenha tirado, inicia-se o jogador que tiver a "
	"peca com os numeros repetidos mais altos, ou seja, \"[5:5]\", \"[4:4]\", e assim "
	"sucessivamente. Os jogadores devem colocar pecas que "
	"tenham os mesmos numeros das pecas que se encontram na ponta do jogo.\n\n");
	
	printf("Se um jogador nao puder jogar, devera \"comprar\" do monte tantas pecas como forem "
		"necessarias. Se nao houver pecas no monte, passara o turno ao seguinte jogador.\n\n");
	
	printf("Quando um jogador coloca sua ultima pedra na mesa, essa acao e chamada de bater. "
	"Quando joga-se sozinho, o jogador que ganhou a partida soma os pontos de todos os seus "
	"adversarios. Jogando em dupla, somam-se os pontos de todos os jogadores incluindo os "
	"do seu companheiro.\n\n");
	
	printf("Existem casos onde nenhum dos jogadores pode continuar a partida. Isto ocorre quando "
	"o numero das extremidades saiu 7 vezes antes. Nesse momento se diz que a partida esta "
	"fechada. Os jogadores contarao os pontos das pedras que ficaram; o jogador ou dupla com "
	"menos pontos vencem e somam-se os pontos da maneira habitual.\n\n");
	
	limparBuffer();
	
	printf("Aperte qualquer tecla para continuar.");
	getchar();
}

void limparBuffer(){ //Limpar buffer do teclado.
    char c;
    while ((c = fgetc(stdin)) != EOF && c != '\n'){}
}
