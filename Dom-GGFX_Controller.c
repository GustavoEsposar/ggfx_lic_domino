//Bibliotecas proprias

#include "Dom-GGFX_Model.c"
#include "Dom-GGFX_View.c"

//Funcoes
void inicializarJogo(); //Inicializa o menu inicial
void iniciarJogo(); //Inicia o jogo
void gameLoop(); //Loop do jogo (Menu de acoes do jogador)
void criarPecas(); //Gera as pecas e as coloca na mesa
void embaralharPecas(); //Embaralha as pecas da mesa
void separarPecas(); //Separar as pecas
void jogarPeca(); //Inicia o processo para o jogador jogar uma peca
void jogarPecaBot(); //Inicia o processo para o jogador jogar uma peca do bot
int checarValidadeJogar(tipoPeca peca, int apenasChecagem); //Checa a validade da peca que o jogador escolheu para ser jogada.
void trocarVezJogador(); //Troca a vez dos jogadores.
void escolherJogadorInicial(); //Escolhe o jogador que inicia com base em suas pecas.
void comprarPeca(); //Comprar uma peca.
void adicionarPecaMesaOrdenada(int lado, tipoPeca peca); //Adiciona os pecas na mesa.
void acabarJogoPeca(); //Checar se o jogo pode terminar.
void computadorJoga(); //Computador joga sua vez.
void salvarJogo(); //Salva o jogo.
int carregarJogo(); //Carrega o jogo.

void inicializarJogo(){ //Recebe o comando o usuario no menu geral
    char escolha = menuGeral();
	jogoEmProgresso = 0; //Jogo nao esta em progresso

    switch(escolha){
        case '1': //Iniciar jogo
        	iniciarJogo();
            break;
		case '2': //Continuar jogo
			if(!carregarJogo()) //Caso de algum erro para carregar, ele volta ao menu.
				inicializarJogo();
			break;
		case '3': //Regras
			mostrarRegras();
			inicializarJogo();
			break;
        case '4': //Voltar ao menu
			return;
		default:
			printf("\nEm desenvolvimento\n");
			inicializarJogo();
			break;
    }
}

void iniciarJogo(){ //Iniciar o jogo.
	jogoEmProgresso = 1; //Jogo entra em progresso
	pecasJogadas = 0;

	criarPecas(); //Gera as pecas da mesa.
	
	menuJogadores(); //Chama o menu para escolher a quantidade de jogadores e seus nomes.
	
	embaralharPecas(); //Embaralha as pecas.
	separarPecas(); //Separar as pecas entre os jogadores.

	escolherJogadorInicial(); //Escolhe o jogador que vai comecar com base em suas pecas.

	gameLoop(); //Loop menu jogador
}

void gameLoop(){ //Loop do jogo (Menu de acoes do jogador)
	if(!jogoEmProgresso) return;

	if(numeroDeJogadores == jogadorAtual){ //Caso haja e seja a vez do bot
		computadorJoga();
		return;
	}

    mostrarPecasMesa();
    mostrarPecasJogador(jogadorAtual);
	
	divisoria();
	printf("%s", aviso);
	divisoria();
    
    char escolha = menuJogo(jogadorAtual);
	switch(escolha){
	    case '1': //Jogar uma peca
			jogarPeca();
			break;
		case '2': //Comprar/Passar pecas
			if(pecasParaCompra > 0)
				comprarPeca();
			else
				trocarVezJogador();
			break;
			case '3':
				salvarJogo();
				break;
			case '4': //Voltar ao menu
				inicializarJogo();
				return;
		default:
			printf("\nEm desenvolvimento\n");
			break;
	}
	gameLoop();
}

void criarPecas(){
    int i, j, aux = 0;

    for(i = 0; i <= 6; i++){ //Gera as 28 pecas.
        for(j = i; j <= 6; j++){
        	pecas[aux].lado1 = i;
            pecas[aux].lado2 = j;
			pecas[aux].status = 2;
            aux++;
        }
    }
    
    for(i = 0; i < 28; i++) //Adiciona essas pecas na mesa.
    	mesa[i] = pecas[i];
    
    tipoPeca pecaHolder;
    pecaHolder.status = -1;
    for(i = 0; i < 28; i++) //Adiciona essas pecas na mesa.
    	mesaOrdenada[i].status = -1;
	
	pecasParaCompra = 14;
}

void embaralharPecas(){ //Embaralha as pecas na mesa.
	tipoPeca pecaHolder;
	
	int i;
    	
	srand(time(NULL));
	for(i = 0; i < MAXPECA; i++){ //Escolhe um numero aleatorio e troca as pecas.
		int n = rand() % MAXPECA;
		pecaHolder = mesa[i];
		mesa[i] = mesa[n];
		mesa[n] = pecaHolder;
	}
}

void separarPecas(){ //Separa as pecas entre os jogadores.
	int i, aux = 0;
	for(i = 0; i < 14; i++){
		if(i % 7 == 0)
			aux++;

		mesa[i].status = aux - 1;
	}
}

void comprarPeca(){ //Comprar pecas
	int i;
	for (i = 0; i < MAXPECA; i++){ //Roda por todas as pecas, a primeira que estiver disponivel para compra vira do jogador.
		if(mesa[i].status == 2){
			mesa[i].status = jogadorAtual;
			pecasParaCompra--;
			if(numeroDeJogadores != jogadorAtual){ //Bot esta jogando.
				sprintf(aviso, "\nPeca comprada: [%d:%d]\n", mesa[i].lado1, mesa[i].lado2);
			}else{
				printf("\nComputador comprou uma peca\n");
				divisoria();
			}
			return;
		}
	}
}

void jogarPeca(){ //Jogar peca.
	int i, aux = 1;
	int escolha = menuJogarPeca(jogadorAtual); //Chama o menu para o jogador escolher qual peca quer jogar

	if(escolha != 0){
	    for(i = 0; i < MAXPECA; i++){ //Roda pelas pecas e encontra a que o jogador escolheu no menu
	        if(mesa[i].status == jogadorAtual){
	            if(escolha == aux){
	                break;
	            }
	            aux++;
	        }
	    }

	    int checagem = checarValidadeJogar(mesa[i], 0); //Checa se a peca pode ser jogada.
	    if(checagem != -1){ //A peca pode ser jogada.
	    	tipoPeca pecaAux;
	        if(mesa[i].lado1 == ponta[checagem]){ //Faz a checagem em qual ponta a peca e valida e vira ela de acordo.
	        	ponta[checagem] = mesa[i].lado2;
				if(checagem == 1){ //Jogar no lado direito
	                pecaAux.lado2 = mesa[i].lado2;
	                pecaAux.lado1 = mesa[i].lado1;
	            }else{ //Jogar no lado esquerdo
	                pecaAux.lado1 = mesa[i].lado2;
	                pecaAux.lado2 = mesa[i].lado1;
	            }
	        }else{
	        	ponta[checagem] = mesa[i].lado1;
	            if(checagem == 1){ //Jogador no lado direito
	                pecaAux.lado2 = mesa[i].lado1;
	                pecaAux.lado1 = mesa[i].lado2;
	            }else{ //Jogar no lado esquerdo
	                pecaAux.lado1 = mesa[i].lado1;
	                pecaAux.lado2 = mesa[i].lado2;
	            }
	        }
	        adicionarPecaMesaOrdenada(checagem, pecaAux);
	        mesa[i].status = 3; //A peca vira da mesa.

	        sprintf(aviso, "\n%s jogou a peca: [%d:%d]\n", jogadores[jogadorAtual].nome, mesa[i].lado1, mesa[i].lado2);
	        trocarVezJogador(); //Troca a vez dos jogadores.
	    }else{ //Caso a peca nao possa ser jogada, avisa o jogador.
	        strcpy(aviso, "\nEssa peca nao pode ser jogada.\n"); //Peca invalida
	    }
	}
	divisoria();
}

void jogarPecaBot(int peca){ //Cuida do bot jogar a peca.
	tipoPeca pecaAux;
	int n = rand() % 1;
	int checagem = checarValidadeJogar(mesa[peca], 1);
	if(checagem == 2) //Escolha uma posicao aleatoria para jogar a peca se os dois lados estiverem disponiveis
		checagem = n;
	
	if(mesa[peca].lado1 == ponta[checagem]){ //Faz a checagem em qual ponta a peca e valida e vira ela de acordo.
		ponta[checagem] = mesa[peca].lado2;
		if(checagem == 1){ //Jogar no lado direito
			pecaAux.lado2 = mesa[peca].lado2;
			pecaAux.lado1 = mesa[peca].lado1;
		}else{ //Jogar no lado esquerdo
			pecaAux.lado1 = mesa[peca].lado2;
			pecaAux.lado2 = mesa[peca].lado1;
		}
	}else{
		ponta[checagem] = mesa[peca].lado1;
		if(checagem == 1){ //Jogador no lado direito
			pecaAux.lado2 = mesa[peca].lado1;
			pecaAux.lado1 = mesa[peca].lado2;
		}else{ //Jogar no lado esquerdo
			pecaAux.lado1 = mesa[peca].lado1;
			pecaAux.lado2 = mesa[peca].lado2;
		}
	}
	adicionarPecaMesaOrdenada(checagem, pecaAux);
	mesa[peca].status = 3; //A peca vira da mesa.

	sprintf(aviso, "\n%s jogou a peca: [%d:%d]\n", jogadores[jogadorAtual].nome, mesa[peca].lado1, mesa[peca].lado2);
	trocarVezJogador(); //Troca a vez dos jogadores.
}

int checarValidadeJogar(tipoPeca peca, int apenasChecagem){ //Checa a validade de uma peca ser jogada.
	char i, aux = 0, valido = 0;
	for(i = 0; i < 2; i++){ //Checa se nas pontas existe uma peca de mesmo valor.
		if(ponta[i] == peca.lado1 || ponta[i] == peca.lado2){
			aux++;
			valido = i;
		}
	}
	switch(aux){ //Verifica qual lado esta disponivel.
		case 0:
			return -1;
		case 1:
			return valido;
		case 2: //Se dois lados estao disponiveis, da ao jogador a possibilidade de escolher qual lado quer jogar.
			if(!apenasChecagem)
				switch (menuEscolhaLado()){//Perguntar qual lado
					case '1':
						return 0;
					case '2':
						return 1;
				}
			else
				return 2;

	}
}

void escolherJogadorInicial(){ //Roda pelas pecas dos jogadores e encontra a peca inicial.
	int i, max = -1, pecaAux;
	for(i = 0; i < 14; i++){ //Checa quem tem a dupla maior
		if(mesa[i].lado1 == mesa[i].lado2){
			if(mesa[i].lado1 > max){
				max = mesa[i].lado1;
				pecaAux = i;
			}
		}
	}
	
	if(pecaAux != -1){ //Caso tenha encontrado uma peca dupla o jogador inicia com essa peca.
		divisoria();
		sprintf(aviso, "\n%s iniciou com a peca: [%d:%d]\n", jogadores[mesa[pecaAux].status].nome, mesa[pecaAux].lado1, mesa[pecaAux].lado1);
		jogadorAtual = mesa[pecaAux].status; //Jogador atual vira o que possui a peca.
		mesa[pecaAux].status = 3; //Ele joga a peca na mesa.
		trocarVezJogador(); //Troca a vez do jogador.
		
		for(i = 0; i < 2; i++) ponta[i] = max; //Adiciona as pecas da ponta.
		tipoPeca pecaHolder;
		pecaHolder.lado1 = max;
		pecaHolder.lado2 = max;
		adicionarPecaMesaOrdenada(0, mesa[pecaAux]);
		return;
	}
	
	max = -1;
	for(i = 0; i < 14; i++){ //Checa quem tem a maior peca somando os dois lados.
		if(mesa[i].lado1 + mesa[i].lado2 > max){
			max = mesa[i].lado1;
			pecaAux = i;
		}
	}
	
	divisoria();
	sprintf(aviso, "\nJogador %s iniciou com a peca: [%d:%d]\n", jogadores[mesa[pecaAux].status].nome, mesa[pecaAux].lado1, mesa[pecaAux].lado2);
	jogadorAtual = mesa[pecaAux].status; //Jogador atual vira o que possui a peca.
	mesa[pecaAux].status = 3; //Ele joga a peca na mesa.
	trocarVezJogador(); //Troca a vez do jogador.
	
	//Adiciona as pecas na ponta.
	ponta[0] = mesa[pecaAux].lado1;
	ponta[1] = mesa[pecaAux].lado2;
	adicionarPecaMesaOrdenada(0, mesa[pecaAux]);
}

void trocarVezJogador(){ //Troca a vez entre os jogadores.
    acabarJogoPeca();
	if(jogadorAtual == 0)
		jogadorAtual = 1;
	else
		jogadorAtual = 0;
}

void adicionarPecaMesaOrdenada(int lado, tipoPeca peca){ //Adicionar uma peca a mesa ordenada.
	int i;
	if(lado == 0){ //Jogar na esquerda
		for(i = 26; i >= 0; i--){ //Joga todas as pecas para uma posicao na frente
			mesaOrdenada[i + 1] = mesaOrdenada[i];
		}
		
		mesaOrdenada[0] = peca; //Coloca a peca na primeira posicao
		
	}else{
		for(i = 0; i < 28; i++){ //Encontra um lugar vazio no final da lista e coloca a peca
			if(mesaOrdenada[i].status == -1){
				mesaOrdenada[i] = peca;
				break;
			}
		}
	}
	
	pecasJogadas++; //Aumenta o numero de pecas jogadas.
}

void acabarJogoPeca(){ //Checa o final do jogo
    int i, j, count = 0;
    for(i = 0; i < MAXPECA; i++){ //Roda pelas pecas e verifica se o jogador tem peca
        if(mesa[i].status == jogadorAtual)
            count++;
    }

    if(count == 0){ //Caso o jogador nao tenha mais nenhuma peca, ele vence
        mostrarPecasMesa();
        if(jogadorAtual == 0)
            mostrarPecasJogador(1);
        else
            mostrarPecasJogador(0);
        printf("\nO jogador %s venceu!\n", jogadores[jogadorAtual].nome);
        printf("Aperte enter para continuar!");
        getchar();
        limparBuffer();
        inicializarJogo();
		return;
    }

	if(pecasParaCompra > 0) return; //Verifica se ainda tem pecas para comprar.

	for(i = 0; i < 2; i++) //Roda pelas pecas dos jogadores e verifica se tem alguma valida
		for(j = 0; j < MAXPECA; j++){
			if(mesa[j].status == i){
				if(checarValidadeJogar(mesa[j], 1) != -1)
					return;
			}
		}

	int somaPecasJogadores[2] = {0, 0};
	for(i = 0; i < 2; i++) //Roda pelas pecas dos jogadores e soma seus lados para verificar quem tem a menor soma
		for(j = 0; j < MAXPECA; j++){
			if(mesa[j].status == i){
				somaPecasJogadores[i] += mesa[j].lado1 + mesa[j].lado2;
			}
		}

	int vencedor = 0;
	if(somaPecasJogadores[0] < somaPecasJogadores[1]) //Checa a soma e atribui um vencedor
		vencedor = 0;
	else if(somaPecasJogadores[1] < somaPecasJogadores[0])
		vencedor = 1;
	else{ //Os dois jogadores tem a mesma soma de pecas.
		mostrarPecasMesa();
		mostrarPecasJogador(0);
		mostrarPecasJogador(1);
		printf("\nO jogo empatou!\n");
	    printf("Aperte enter para continuar!");
	    getchar();
	    limparBuffer();
	    inicializarJogo();
		return;
	}

	mostrarPecasMesa();
	mostrarPecasJogador(0);
	mostrarPecasJogador(1);
	printf("\nO jogador %s venceu!\n", jogadores[vencedor].nome);
    printf("Aperte enter para continuar!");
    getchar();
    limparBuffer();
    inicializarJogo();
}

void computadorJoga(){ //Cuida da jogada do bot
	int i, j, aux;
	do{ //Loopa enquanto o bot nao, jogar uma peca ou passar a vez
		aux = -1;
		for(i = 0; i < MAXPECA; i++){ //Roda por todas as pecas do bot, encontrando uma valida para jogar
			if(mesa[i].status == jogadorAtual){
				if(checarValidadeJogar(mesa[i], 1) != -1){
					aux = i;
				}
			}
		}

		if(aux != -1){ //Caso tenha encontrado uma peca valida para jogar, ele a joga
			jogarPecaBot(aux);
		}else{ //Caso nao tenha encontrado
			if(pecasParaCompra > 0) //Verifica se a pecas para comprar
				comprarPeca(); //Comprar uma peca
			else{ //Caso nao haja, ele passa a vez
				trocarVezJogador();
				aux = 0;
			}
		}
	}while(aux == -1);
	gameLoop(); //Chama novamente o loop do jogo
}

void salvarJogo(){ //Salva o jogo
	int i;
	FILE *fp;
	
	if((fp = fopen("ARQPECAS", "w")) == NULL){ //Cria o arquivo ARQPECAS
		printf("\nHouve um erro para salvar o arquivo. (ARQPECAS)\n");
		return;
	}

	for(i = 0; i < MAXPECA; i++){ //Adiciona ao ARQPECAS a estrutura da mesa
		if(fwrite(&mesa[i], sizeof(struct peca), 1, fp) != 1){
			printf("\nErro MESA\n");
			return;
		}
	}

	fclose(fp); //Fecha o arquivo ARQPECAS

	if((fp = fopen("ARQMESA", "w")) == NULL){ //Cria o arquivo ARQMESA
		printf("\nHouve um erro para salvar o arquivo. (ARQMESA)\n");
		return;
	}

	for(i = 0; i < MAXPECA; i++){ //Adiciona ao ARQMESA a estrutura da mesa ordenada
		if(fwrite(&mesaOrdenada[i], sizeof(struct peca), 1, fp) != 1){
			printf("\nErro MESA_ORDENADA\n");
			return;
		}
	}
	
	for(i = 0; i < 2; i++){ //Adiciona ao ARQMESA as pecas da ponta
		if(fwrite(&ponta[i], sizeof(ponta), 1, fp) != 1){
			printf("\nErro PONTA\n");
			return;
		}
	}

	fclose(fp); //Fecha o arquivo ARQMESA

	if((fp = fopen("ARQVARIAVEIS", "w")) == NULL){ //Cria o arquivo ARQVARIAVEIS
		printf("\nHouve um erro para salvar o arquivo. (ARQVARIAVEIS)\n");
		return;
	}

	//Jogadores
	for(i = 0; i < 2; i++){ //Adiciona ao ARQVARIAVEIS a estrutura dos jogadores
		if(fwrite(&jogadores[i], sizeof(struct jogador), 1, fp) != 1){
			printf("\nErro JOGADORES\n");
			return;
		}
	}

	//Numero de jogadores
	if(fwrite(&numeroDeJogadores, sizeof(numeroDeJogadores), 1, fp) != 1){ //Adiciona ao ARQVARIAVEIS a variavel numeroDeJogadores
		printf("\nErro NUMERO_DE_JOGADORES\n");
		return;
	}

	//Jogador Atual
	if(fwrite(&jogadorAtual, sizeof(jogadorAtual), 1, fp) != 1){ //Adiciona ao ARQVARIAVEIS a variavel jogadorAtual
		printf("\nErro JOGADOR_ATUAL\n");
		return;
	}

	//Pecas Jogadas
	if(fwrite(&pecasJogadas, sizeof(pecasJogadas), 1, fp) != 1){ //Adiciona ao ARQVARIAVEIS a variavel pecasJogadas
		printf("\nErro PECAS_JOGADAS\n");
		return;
	}

	//Pecas Para Compra
	if(fwrite(&pecasParaCompra, sizeof(pecasParaCompra), 1, fp) != 1){ //Adiciona ao ARQVARIAVEIS a variavel pecasParaCompra
		printf("\nErro PECAS_PARA_COMPRA\n");
		return;
	}
	
	//Aviso
	if(fwrite(&aviso, sizeof(aviso), 1, fp) != 1){ //Adiciona ao ARQVARIAVEIS a variavel aviso
		printf("\nErro AVISO\n");
		return;
	}

	fclose(fp); //Fecha o arquivo ARQVARIAVEIS

	printf("\nJogo salvo com sucesso!\n");
}

int carregarJogo(){ //Carrega o jogo
	int i;
	FILE *fp;

	if((fp = fopen("ARQPECAS", "r")) == NULL){ //Abre o arquivo ARQPECAS
		printf("\nHouve um erro para abrir o arquivo. (ARQPECAS)\n");
		return 0;
	}

	for(i = 0; i < MAXPECA; i++){ //Le do arquivo ARQPECAS a estrutura da mesa
		if(fread(&mesa[i], sizeof(struct peca), 1, fp) != 1){
			printf("\nErro MESA\n");
			return 0;
		}
	}

	fclose(fp); //Fecha o arquivo ARQPECAS

	if((fp = fopen("ARQMESA", "r")) == NULL){ //Abre o arquivo ARQMESA
		printf("\nHouve um erro para abrir o arquivo. (ARQMESA)\n");
		return 0;
	}

	for(i = 0; i < MAXPECA; i++){ //Le do arquivo ARQMESA a estrutura da mesa ordenada
		if(fread(&mesaOrdenada[i], sizeof(struct peca), 1, fp) != 1){
			printf("\nErro MESA_ORDENADA\n");
			return 0;
		}
	}
	
	for(i = 0; i < 2; i++){ //Le do arquivo ARQMESA as pecas da ponta
		if(fread(&ponta[i], sizeof(ponta), 1, fp) != 1){
			printf("\nErro PONTA\n");
			return 0;
		}
	}

	fclose(fp); //Fecha o arquivo ARQMESA

	if((fp = fopen("ARQVARIAVEIS", "r")) == NULL){ //Abre o arquivo ARQVARIAVEIS
		printf("\nHouve um erro para abrir o arquivo. (ARQVARIAVEIS)\n");
		return 0;
	}

	//Jogadores
	for(i = 0; i < 2; i++){ //Le do arquivo ARQVARIAVEIS a estrutura dos jogadores
		if(fread(&jogadores[i], sizeof(struct jogador), 1, fp) != 1){
			printf("\nErro JOGADORES\n");
			return 0;
		}
	}

	//Numero de jogadores
	if(fread(&numeroDeJogadores, sizeof(numeroDeJogadores), 1, fp) != 1){ //Adiciona ao ARQVARIAVEIS a variavel numeroDeJogadores
		printf("\nErro NUMERO_DE_JOGADORES\n");
		return;
	}

	//Jogador Atual
	if(fread(&jogadorAtual, sizeof(jogadorAtual), 1, fp) != 1){ //Le do arquivo ARQVARIAVEIS a variavel jogadorAtual
		printf("\nErro JOGADOR_ATUAL\n");
		return 0;
	}

	//Pecas Jogadas
	if(fread(&pecasJogadas, sizeof(pecasJogadas), 1, fp) != 1){ //Le do arquivo ARQVARIAVEIS a variavel pecasJogadas
		printf("\nErro PECAS_JOGADAS\n");
		return 0;
	}

	//Pecas Para Compra
	if(fread(&pecasParaCompra, sizeof(pecasParaCompra), 1, fp) != 1){ //Le do arquivo ARQVARIAVEIS a variavel pecasParaCompra
		printf("\nErro PECAS_PARA_COMPRA\n");
		return 0;
	}
	
	//Aviso
	if(fread(&aviso, sizeof(aviso), 1, fp) != 1){ //Le do arquivo ARQVARIAVEIS a variavel aviso
		printf("\nErro AVISO\n");
		return 0;
	}

	fclose(fp); //Fecha o arquivo ARQVARIAVEIS

	printf("\nJogo carregado com sucesso.\n");

	jogoEmProgresso = 1; //Jogo em progresso
	gameLoop(); //Inicia o loop do jogo
	return 1;
}
