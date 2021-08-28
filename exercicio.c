#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

typedef struct _contato{
	int id;
	char nome[50], telefone[50], email[80];
} TContato;

typedef struct _lista{
	int inicio, fim;
	TContato items[MAX];
} TLista;

TLista *criarLista(){
	TLista *novaLista = (TLista*) malloc(sizeof(TLista));
	if (novaLista) {
		novaLista->inicio = 0;
		novaLista->fim = 0;
	}
	return novaLista;
}

int listaVazia(TLista *lista){
	return lista->fim == 0;
}

void inserir(TLista *lista, TContato contato){
	if (lista->fim == MAX){
		printf("Lista cheia\n");
		return;
	}
	lista->items[lista->fim] = contato;
	lista->fim++;
}

void lerArquivo(FILE *file, TLista *lista, int numCont){
	TContato contato;
	for(int i=0; i<numCont; i++){
		fscanf(file, "%i %s %s %s\n", &contato.id, contato.nome, contato.telefone, contato.email);
		inserir(lista, contato);
	}
}

TContato remover(TLista *lista, int posicao){
	TContato retorno = lista->items[posicao];
	for(int i = posicao; i < lista->fim-1; i++){
		lista->items[i] = lista->items[i+1];
	}
	lista->fim--;
	return retorno;
}

void imprimir(TLista *lista){
	if (listaVazia(lista)) {
        printf("Lista vazia\n");
        return;
    }

	printf("|Id  | Nome           | Telefone        | E-mail\n");
	for(int i=lista->inicio; i<lista->fim; i++){
		printf("|%-4i| %-14s | %-15s | %-20s\n", lista->items[i].id, lista->items[i].nome, lista->items[i].telefone, lista->items[i].email);
	}
}

//--------------------METODO QUICK-SORT---------------------------------------
/*
	Nome da Função: particiona
	Parâmetros:
	- pInicio: primeira posição do vetor a ser ordenado.
	- pFim   : última posição do vetor a ser ordenado
	- esq    : posição esquerda do subvetor a ser particionado (passado por referência)
	- dir    : posição direita do subvetor a ser particionado (passado por referência)
	- *pVetor: vetor a ser ordenado
	Retorno  : Sem retorno.
	Descrição: O método recebe a posição inicial, final, posição esq do subvetor, posição dir do subvetor e o vetor a ser ordenado. 
	O método particiona o vertor original em subvetores de modo recursivo.
*/
void particiona(int pInicio, int pFim, int *esq, int *dir, TContato *pVetor) {
	TContato temp;
	TContato pivo;
	// pivô neste caso será o elemento central		
	*esq = pInicio;
	*dir = pFim;
	pivo = pVetor[(*esq + *dir) / 2];

	while (*esq < *dir) {
		while (pVetor[*esq].id < pivo.id) (*esq)++;

		while (pVetor[*dir].id > pivo.id) (*dir)--;

		// se a condiçao for satisfeita então a troca é realizada
		if (*esq <= *dir) {
			temp         = pVetor[*esq];
			pVetor[*esq] = pVetor[*dir];
			pVetor[*dir] = temp;			
			(*dir)--;
			(*esq)++;
		}
	}
}

/*
Nome da Função: quick_sort
Parâmetros:
- pPosicaoInicial: primeira posição do vetor a ser ordenado.
- pPosicaoFinal: última posição do vetor a ser ordenado
- *pVetor: vetor a ser ordenado
Retorno: Sem retorno.
Descrição: O método recebe a posição inicial, final e o vetor a ser ordenado. Posteriormente
é utilizado o recurso de pivô (neste caso o elemento do meio) para ordenar o vetor.
*/
void quick_sort(int pInicio, int pFim, TContato *pVetor) {
	int esq, dir;

	particiona(pInicio, pFim, &esq, &dir, pVetor);
	if (pInicio < dir) quick_sort(pInicio, dir, pVetor);
	if (esq < pFim) quick_sort(esq, pFim, pVetor);
	
}

int main(){
    
    FILE *arquivo;
	char nome_arquivo[20];
	int numCont;


	scanf("%s", nome_arquivo);
	scanf("%i", &numCont);

	if(numCont < 1){
		printf("Informe um valor maior que 0.");
		return 1;
	}

	if(strcmp(nome_arquivo, "contacts_dataset.dat")){
		printf("Arquivo nao encontrado.");
		return 1;
	}

	arquivo = fopen(nome_arquivo, "r");

	TLista *lista = criarLista();

	lerArquivo(arquivo, lista, numCont);

	quick_sort(lista->inicio, lista->fim, lista->items);

	imprimir(lista);

	free(lista);
    return 0;
}
