#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MSG 495
#define TAM_K 9

typedef struct simboloStruct {
	int simbolo;
	struct simboloStruct *next;
} tpSimbolo;

float getSymbleFrequency (tpSimbolo *pinicio, int simbolo) {
	tpSimbolo *p1;
	int freq = 0, size = 0;

	if(pinicio == NULL) {
		printf("Lista vazia.\n");
		return -1.0;
	} else {
		size = 1;
		p1 = pinicio;
		if(p1->simbolo == simbolo) {
			freq++;
		}
		p1 = p1->next;
		while (p1 != pinicio) {
			if(p1->simbolo == simbolo) {
				freq++;
			}
			p1 = p1->next;
			size++;
		}
		return (float) freq/size;
	}
}

int getSymbolIndexed (tpSimbolo *iniciocripto, int indice) {
	tpSimbolo *p1;
	int i;

	if(iniciocripto == NULL) {
		return -1;
	} else {
		p1 = iniciocripto;
		for(i = 0; i < indice; i++) {
			p1 = p1->next;
		}
		return p1->simbolo;
	}
}

void insertSymble (tpSimbolo **pinicio, int simbolo) {
	tpSimbolo *p1, *p2;
	if (*pinicio == NULL) {
		p1 = malloc(sizeof(tpSimbolo));
		p1->simbolo = simbolo;
		p1->next = p1;
		*pinicio = p1;
	}
	else {
		p1 = *pinicio;
		while(p1->next != (*pinicio)) {
			p1 = p1->next;
		}
		p2 = malloc(sizeof(tpSimbolo));
		p2->simbolo = simbolo;
		p2->next = *pinicio;
		p1->next = p2;
	}
}

int isNewSymble (tpSimbolo *pinicio, int simbolo) {
	tpSimbolo *p1;

	if (pinicio == NULL) {
		return 1;
	} else {
		p1 = pinicio;
		if (p1->simbolo == simbolo) {
			return 0;
		}
		p1 = p1->next;
		while(p1 != pinicio) {
			if (p1->simbolo == simbolo) {
				return 0;
			}
			p1 = p1->next;
		}
		return 1;
	}
}

void printList (tpSimbolo *pinicio) {
	tpSimbolo *p1;

	p1 = pinicio;
	if(pinicio == NULL) {
		printf("Lista vazia.\n");
	} else {
		printf("%d ", p1->simbolo);
		p1 = p1->next;
		while (p1 != pinicio) {
			printf("%d ", p1->simbolo);
			p1 = p1->next;
		}
	}
}

void printListChar (tpSimbolo *pinicio) {
	tpSimbolo *p1;

	p1 = pinicio;
	if(pinicio == NULL) {
		printf("Lista vazia.\n");
	} else {
		printf("%c", p1->simbolo);
		p1 = p1->next;
		while (p1 != pinicio) {
			printf("%c", p1->simbolo);
			p1 = p1->next;
		}
	}
}

void clearList (tpSimbolo **pinicio) {
	tpSimbolo *p1, *p2;

	if (*pinicio == NULL) {
		return;
	} else {
		p1 = (*pinicio)->next;
		while (p1->next != (*pinicio)){
			p2 = p1;
			p1 = p1->next;
			free(p2);
		}
		free(p1);
		*pinicio = NULL;
	}
}

int  main () {
	FILE *fp;
	tpSimbolo *criptograma = NULL, *chave = NULL, *freqCripto = NULL, *colunaSimbolos = NULL, *mensagem = NULL; 
	char str[100];
	float freqMaxima = 0.0;
	int i, j, shift, simbolo, simboloMaximo, coincidencias[TAM_MSG-1], coluna;
	/*int coincidencias[TAM_MSG-1];*/


	fp = fopen("c54.txt", "r");
	if (fp == NULL) {
		printf("Erro ao abrir o arquivo.\n");
	} else {
		/* pulas 2 linhas */
		fgets (str , 100 , fp);
		fgets (str , 100 , fp);
		/* ler 495 linhas (tamanho da mensagem). cada linha é um caractere. */
		for (i = 0; i < TAM_MSG; i++) {
			fscanf(fp, "%d", &simbolo);
			insertSymble(&criptograma, simbolo);
		}

		printf("Criptograma: \n");
		printList(criptograma);

		/* checar coincidencias com os deslocamentos de 1 a TAM_MSG - 1 (pois corresponde a um deslocamento 0) */
		for (shift = 1; shift < TAM_MSG-1; shift++) {
			coincidencias[shift-1] = 0;
			for(i = 0; i < TAM_MSG-shift; i++) {
				if(getSymbolIndexed(criptograma, (i+shift)) == getSymbolIndexed(criptograma, (i))) {
					coincidencias[shift-1]++;
				}
			}
		}

		printf("\n\nCoincidencias com shifts de 1 a TAM_MSG - 1:\n");
		for(i = 0; i < TAM_MSG-1; i++) {
			printf("%d ", coincidencias[i]);
		}
		
		/* preenche vetor de simbolos diferentes e suas frequencias */
		for(i = 0; i < TAM_MSG; i++) {
			simbolo = getSymbolIndexed(criptograma, i);
			if (isNewSymble(freqCripto, simbolo)) {
				insertSymble(&freqCripto, simbolo);
			}
		}

		for (coluna = 0; coluna < TAM_K; coluna++) {

			/* preenche simbolos da colunaSimbolos com a coluna atual */
			for(j = 0; j < TAM_MSG/TAM_K; j++) {
				simbolo = getSymbolIndexed(criptograma, (coluna + (j * TAM_K)));
				insertSymble(&colunaSimbolos, simbolo);
			}

			printf("\n\ncolunaSimbolos %d:\n", coluna);
			printList(colunaSimbolos);

			/* acha o caractere com mais frequencias */
			freqMaxima = getSymbleFrequency(colunaSimbolos, getSymbolIndexed(colunaSimbolos, 0));
			simboloMaximo = getSymbolIndexed(colunaSimbolos, 0);
			for (j = 1; j < 55; j++) {
				if (getSymbleFrequency(colunaSimbolos, getSymbolIndexed(colunaSimbolos, j)) > freqMaxima) {
					freqMaxima = getSymbleFrequency(colunaSimbolos, getSymbolIndexed(colunaSimbolos, j));
					simboloMaximo = getSymbolIndexed(colunaSimbolos, j);
				}
			}

			clearList(&colunaSimbolos);

			printf("\nsimbolo mais comum da colunaSimbolos %d: %d", coluna, simboloMaximo);

			/* faz o xor com o caractere mais comum do plaintext: ' ' (char 32 - espaco em branco) */
			if (coluna != 1 && coluna != 7) {
				simbolo = simboloMaximo ^ (32);
			} else {
				if (coluna == 1) {
					simbolo = simboloMaximo ^ (115); /* 's' */
				}
				else  {
					simbolo = simboloMaximo ^ (101); /* 'e' */
				}
			}
			insertSymble(&chave, simbolo);
		}

		printf("\n\nChave:\n");
		printList(chave);

		/* decripta mensagem com a chave obtida */
		j = 0;
		for (i = 0; i < TAM_MSG; i++) {
			if (j >= TAM_K) {
				j = 0;
			}
			simbolo = getSymbolIndexed(chave, j);
			simbolo = simbolo ^ (getSymbolIndexed(criptograma, i));
			insertSymble(&mensagem, simbolo);
			j++;
		}

		printf("\n\nMensagem:\n");
		printListChar(mensagem);

		printf("\n\n");
		fclose (fp);
	}
	return 0;
}