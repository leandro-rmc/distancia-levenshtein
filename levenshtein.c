#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#define NOME_BANCO_DADOS "palavras.txt"
#define VERSAO_ATUAL "v1.4"

//O custo para a substitui��o muitas vezes � definido como 2 (delete + insert).
//O custo ser� 1 nesse projeto, pois � assim que foi pedido.
#define CUSTO_SUBSTITUICAO 1

//Determina qual � o tamanho m�ximo que uma palavra pode ter.
//Caso ultrapasse esse n�mero, a palavra � cortada.
#define TAM_PALAVRA 51

//C�digo usado pela fun��o mudarModo
#define MODO_PE_DA_LETRA -10

//1 = Ligado | 0 = Desligado
//Define se o programa seguir� ao p� da letra o que foi pedido no projeto.
//Usado como refer�ncia pelo programa para alterar as vari�veis erroEspaco e retornarCusto.
int modoPeDaLetra = 0;

//1 = Ligado | 0 = Desligado
//Define se ao digitar uma palavra com espa�o, deve aparecer uma mensagem de erro ao inv�s de remover o espa�o.
int erroEspaco = 0;

//1 = Ligado | 0 = Desligado
//Define se al�m das palavras, o custo de cada uma delas tamb�m ser� retornado.
int retornarCusto = 1;

//N�mero de palavras similares que o usu�rio deseja que o programa retorne.
int palavrasRetornadas;

//Palavra que o usu�rio escolheu.
char palavraEscolhida[TAM_PALAVRA];

//Palavra que atualmente o programa est� lendo do vocabul�rio.
char palavraAtualVocabulario[TAM_PALAVRA];

//Estrutura que � retornada ao final da opera��o.
struct palavraParecida{
    char nome[TAM_PALAVRA];
    int custo;
};
typedef struct palavraParecida palavraParecida;

//Controla a quantidade de elementos dentro do vetor para certas opera��es.
int qtPalavrasParecidasNoVetor = 0;

//Define um ponteiro para a estrutura que ser� retornada. Esse ponteiro ser� usado como vetor e ter� seu espa�o...
//...na mem�ria definido pela fun��o calloc.
palavraParecida *palavrasParecidas;

//Arquivo de vocabul�rio (palavras.txt)
FILE *arquivo;

//Limpa todas as vari�veis para que uma nova opera��o possa ser feita de novo.
void resetar(){
    fclose(arquivo);
    palavrasRetornadas = 0;
    qtPalavrasParecidasNoVetor = 0;
    free(palavrasParecidas);
}

void inicializarBancoDeDados(){
    char temp, resultado = '1';
    arquivo = fopen(NOME_BANCO_DADOS,"r+");
    if (arquivo == NULL){
        printf("\nERRO! Banco de dados \"" NOME_BANCO_DADOS "\" n�o p�de ser acessado.\n");
        resultado = '0';
    }
    else{
        if (fscanf(arquivo, "%c", &temp) == EOF){
            printf("\nERRO! Seu banco de dados \"" NOME_BANCO_DADOS "\" est� vazio.\n");
            resultado = '0';
        }
        rewind(arquivo);
    }
    if (resultado == '0'){
        printf("OBS: � necess�rio que esse arquivo contenha as palavras, todas seguidas de asterisco.\n");
        system("PAUSE");
        exit(0);
    }
}

//Define comportamentos diferentes para o programa em tempo de execu��o.
void mudarModo(int modo){
    if (palavrasRetornadas == MODO_PE_DA_LETRA){
        if (modoPeDaLetra == 1){
            modoPeDaLetra = 0;
            erroEspaco = 0;
            retornarCusto = 1;
            printf("\nModo p� da letra desativado.\n");
            printf("*Espa�os ser�o aceitos na entrada de dados, mas ser�o removidos durante o processo.\n");
            printf("*O custo de cada palavra ser� retornado.\n\n");
        }
        else{
            modoPeDaLetra = 1;
            erroEspaco = 1;
            retornarCusto = 0;
            printf("\nModo p� da letra ativado.\n");
            printf("*Espa�os ser�o bloqueados na entrada de dados e o usu�rio ter� que digitar de novo.\n");
            printf("*O custo de cada palavra n�o ser� retornado.\n\n");
        }
    }
}

void entrarComDados(){
    int i;
    printf("- In�cio:\n");
    do{
        fflush(stdin);
        printf("Quantidade de palavras similares a serem retornadas (> 0): ");
        palavrasRetornadas = 0;
        scanf("%i",&palavrasRetornadas);
        mudarModo(palavrasRetornadas);
    } while (palavrasRetornadas < 1);
    //Definimos o tamanho do nosso vetor de estruturas de palavras parecidas.
    palavrasParecidas = calloc(palavrasRetornadas, sizeof(palavraParecida));
    getchar();
    fflush(stdin);
    printf("Palavra (Max. 50 caracteres / Sem espa�o): ");
    for (i = 0; i < TAM_PALAVRA - 1; i++){
        scanf("%c",&palavraEscolhida[i]);
        if (palavraEscolhida[i] == ' '){
            if (erroEspaco == 1){
                printf("Espa�o n�o � permitido.\n");
                printf("Palavra (Max. 50 caracteres / Sem espa�o): ");
                fflush(stdin);
                i = -1;
                continue;
            }
            i--;
        }
        if (palavraEscolhida[i] == '\n'){
            palavraEscolhida[i] = '\0';
            break;
        }
    }
    palavraEscolhida[i + 1] = '\0';
}

//Move o ponteiro interno do arquivo para o in�cio da pr�xima palavra
void movPtrProxPalavraVocabulario(){
    char temp;
    while (fscanf(arquivo,"%c",&temp) != EOF){
        if (temp == '*')
            break;
    }
}

int avancarPalavraDoVocabulario(){
    int parar = 0, i, resultado = 1;
    for (i = 0; i <= TAM_PALAVRA - 2; i++){
        if (fscanf(arquivo,"%c",&palavraAtualVocabulario[i]) == EOF){
            parar = 1;
            resultado = 0;
        }
        if (palavraAtualVocabulario[i] == '\n' || palavraAtualVocabulario[i] == ' ')
            i--;
        if (palavraAtualVocabulario[i] == '*')
            parar = 1;
        if (parar == 1){
            palavraAtualVocabulario[i] = '\0';
            break;
        }
        //Se a vari�vel de itera��o tiver chegado ao seu fim, mas a palavra a ser lida do vocabul�rio...
        //... n�o, o ponteiro interno do arquivo precisa ser movido para a pr�xima palavra.
        if (i == TAM_PALAVRA - 2 && palavraAtualVocabulario[i] != '\0'){
            movPtrProxPalavraVocabulario();
        }
    }
    palavraAtualVocabulario[i + 1] = '\0';
    return resultado;
}

int menorDe3(int a, int b, int c){
    int menor = a;
    if (b < a && b <= c)
        menor = b;
    if (c < a && c <= b)
        menor = c;
    return menor;
}

int distanciaLevenshtein(char *palavraA, char *palavraB){
    int tamA, tamB, x, y, custo;
    tamA = strlen(palavraA);
    tamB = strlen(palavraB);
    int tab[tamA+1][tamB+1];

    for (x = 0; x <= tamA; x++)
        tab[x][0] = x;
    for (y = 0; y <= tamB; y++)
        tab[0][y] = y;

    for (x = 1; x <= tamA; x++){
       for (y = 1; y <= tamB; y++){
            if (palavraA[x - 1] == palavraB[y - 1]){
                custo = 0;
            }
            else{
                custo = CUSTO_SUBSTITUICAO;
            }
            tab[x][y] = menorDe3(tab[x - 1][y] + 1, tab[x][y - 1] + 1, tab[x - 1][y - 1] + custo);
       }
    }

    /*
    //Mostra as Matrizes geradas. Usei isso para fins de debug.
    printf("Para %s\n", palavraB);
    for (x = 0; x <= tamA; x++){
       for (y = 0; y <= tamB; y++){
            printf("%i ", tab[x][y]);
       }
       printf("\n");
    }
    printf("\n");
    */

    return tab[tamA][tamB];
}

//Verifica se h� espa�o vazio no vetor de estrutura (onde ficam os resultados finais)
int temEspacoVazioNoRetorno(){
    int i;
    for (i = 0; i <= palavrasRetornadas - 1; i++){
        if (palavrasParecidas[i].nome[0] == '\0'){
            return 1;
        }
    }
    return 0;
}

//Usa o m�todo bolha para organizar o vetor, do que tem o menor custo para o maior.
void ordenarVetorVocabulario(){
    int i, j, mudouAlgo = 0;
    palavraParecida temp;

    for (i = 0; i <= qtPalavrasParecidasNoVetor - 1; i++){
       for (j = 0; j <= qtPalavrasParecidasNoVetor - 2; j++){
            if (palavrasParecidas[j].custo > palavrasParecidas[j+1].custo){
                temp = palavrasParecidas[j];
                palavrasParecidas[j] = palavrasParecidas[j+1];
                palavrasParecidas[j+1] = temp;
                mudouAlgo = 1;
            }
       }
       //Interrompe o m�todo bolha se o vetor j� foi organizado e nada mais mudar�.
       if (mudouAlgo == 0)
        break;
       mudouAlgo = 0;
    }
}

void iniciarOperacao(){
    int custo, i, primeiraOrdenacao = 1, pular = 0;

    while(avancarPalavraDoVocabulario() != 0){
        //Se a palavra escolhida pelo usu�rio encontra-se no vocabul�rio, ent�o a opera��o � interrompida.
        if (strcmp(palavraEscolhida,palavraAtualVocabulario) == 0){
            printf("\n\tResultado(s) para \"%s\":\n", palavraEscolhida);
            printf("\tA palavra foi encontrada na lista.\n");
            pular = 1;
            break;
        }
        custo = distanciaLevenshtein(palavraEscolhida, palavraAtualVocabulario);
        //Caso tenha espa�o dispon�vel no vetor de estruturas, ent�o � s� jogar a palavra atual do vocabul�rio...
        //...l�, independente do custo dela. O �ndice que ela ser� inserida � igual a quantidade de palavras l� dentro.
        //Exemplo: Se h� 1 elemento l� dentro, ent�o o �ndice ser� o 1, pois aquele elemento estar� no �ndice 0...
        //...e o pr�ximo vazio ser� o pr�ximo �ndice.
        if (temEspacoVazioNoRetorno() == 1){
            palavrasParecidas[qtPalavrasParecidasNoVetor].custo = custo;
            strcpy(palavrasParecidas[qtPalavrasParecidasNoVetor].nome, palavraAtualVocabulario);
            qtPalavrasParecidasNoVetor++;
        }
        else{
            //Caso n�o tenha mais espa�o no vetor, esse bloco sempre ser� executado.
            //Esse if verifica se � a primeira vez que voc� faz uma ordena��o, pois anteriormente n�o era necess�rio...
            //...ordenar o vetor, execto no final. Caso seja a primeira vez, apenas uma �nica ordena��o j� basta neste momento.
            if (primeiraOrdenacao == 1){
                ordenarVetorVocabulario();
                primeiraOrdenacao = 0;
            }
            //Como o vetor j� foi organizado, o �ltimo elemento ser� sempre o que tem mais custo, portanto...
            //...qualquer nova palavra com custo menor ir� substituir a �ltima e o vetor ser� organizado de novo.
            if (palavrasParecidas[palavrasRetornadas - 1].custo > custo){
                palavrasParecidas[palavrasRetornadas - 1].custo = custo;
                strcpy(palavrasParecidas[palavrasRetornadas - 1].nome, palavraAtualVocabulario);
                ordenarVetorVocabulario();
            }
        }
    }
    //Apenas verifica se o c�digo deve prosseguir com sua execu��o ap�s a finaliza��o do while.
    //Pular ser� sempre 1, a n�o ser que a palavra escolhida esteja presente no vocabul�rio.
    if (pular == 0){
        ordenarVetorVocabulario();
        printf("\n\tResultado(s) para \"%s\":\n", palavraEscolhida);
        for (i = 0; i <= qtPalavrasParecidasNoVetor - 1; i++){
             if (retornarCusto == 1)
                printf("\tNome: %s \t Custo: %i\n",palavrasParecidas[i].nome, palavrasParecidas[i].custo);
             else
                printf("\tNome: %s\n",palavrasParecidas[i].nome);
        }
    }
}

int main()
{
    int rodando = 1;
    setlocale(LC_ALL, "Portuguese");
    printf("Dist�ncia Levenshtein ");
    printf(VERSAO_ATUAL);
    printf(".\n");
    printf("Desenvolvido por Leandro Rocha Musser Carneiro, UEZO.\n\n");
    while (rodando == 1){
        //Linha abaixo movida, pois reduz o tempo que o arquivo ficar� aberto.
        //inicializarBancoDeDados();
        entrarComDados();
        inicializarBancoDeDados();
        iniciarOperacao();
        resetar();
        printf("\nDeseja iniciar outra opera��o? (1) Sim | (Outro) N�o \n");
        printf("Sua resposta: ");
        fflush(stdin);
        scanf("%i",&rodando);
        if (rodando == 1){
            printf("\n");
        }
    }
    printf("\nObrigado por utilizar o programa.\n");
    system("PAUSE");
    return 0;
}
