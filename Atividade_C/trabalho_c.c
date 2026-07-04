#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char resp;
int op;

#define MAX 100
#define ARQUIVO "dados_c.csv"

typedef struct {
	int sen;
	char tipo;
	char hora[6];
} senha;


typedef struct {
	senha dados[MAX];
	int inicio;
	int fim;
	int quantidade;
} fila;

typedef struct {
	senha dados[MAX];
	int inicio;
	int fim;
	int quantidade;
} fila_p;

fila n;
fila_p p;
int senha_g = 1;
int cont_normal = 0;

void inici_f();
int menu();
void cadastrar();
void atender();
void listar();
void buscar();
void salvar_csv();
void carregar_csv();

void main() {
	inici_f();
	carregar_csv();

	do {
		op = menu();
		switch(op) {
            
		case 1: cadastrar(); break;
			
		case 2: atender(); break;
            
		case 3: listar(); break;
            	    	
		case 4: buscar(); break;
		    
		case 5: salvar_csv(); break;

		case 6: carregar_csv(); break;
		
		case 7: printf("Obrigado pelo seu tempo!"); break;
		    
		default: printf("Opcao invalida. Tente novamente.\n"); break;
		}
	} while(op != 7);
}

void inici_f() {
	n.inicio = 0;
	n.fim = 0;
	n.quantidade = 0;
	p.inicio = 0;
	p.fim = 0;
	p.quantidade = 0;
}

int menu() {
	printf("==============================\n");
	printf("     Atendimento bancos       \n");
	printf("==============================\n");
	printf(" 1. Cadastrar senha           \n");
	printf(" 2. Atender proximo           \n");
	printf(" 3. Listar filas              \n");
	printf(" 4. Buscar senha              \n");
	printf(" 5. Salvar CSV                \n");
	printf(" 6. Carregar CSV              \n");
	printf(" 7. Sair                      \n");
	printf("==============================\n");
	scanf("%d",&op);
	return op;
}

void cadastrar() {
	char tipo;
	printf("Considere N como Normal e P como Preferencial");
	printf("Informe o tipo de cadastro: ");
	scanf(" %c",&tipo);

	if('n' ==  tolower(tipo)) {
		do {
			if(n.quantidade>=MAX) {
				printf("Fila normal cheia, libere mais memoria");
				return;
			}
			n.dados[n.fim].sen = senha_g++;
			n.dados[n.fim].tipo = 'N';

			printf("Seguindo o padrao *HH:MM*. \nInforme o horario: ");
			scanf("%s", n.dados[n.fim].hora);

			printf("Senha %d gerada com sucesso.", n.dados[n.fim].sen);
			n.fim = (n.fim + 1) % MAX;
			n.quantidade++;


			printf("Voce deseja adicionar outro cadastro comum?\n Use S ou N: ");
			scanf(" %c", &resp);

		} while(tolower(resp) == 's');


	} else if('p' ==  tolower(tipo)) {

		do {
			if(p.quantidade>=MAX) {
				printf("Fila Preferencial cheia, libere mais memoria");
				return;
			}
			p.dados[p.fim].sen = senha_g++;
			p.dados[p.fim].tipo = 'P';

			printf("Seguindo o padrao *HH:MM*. \nInforme o horario: ");
			scanf("%s", p.dados[p.fim].hora);

			printf("Senha %d gerada com sucesso.", p.dados[p.fim].sen);

			p.fim = (p.fim + 1) % MAX;
			p.quantidade++;

			printf("Voce deseja adicionar outro cadastro Preferencial?\n\n Use S ou N ");
			scanf(" %c", &resp);

		} while(tolower(resp) == 's');
	} else {
		return;
	}
}


void atender() {
	if (n.quantidade == 0 && p.quantidade == 0) {
		printf("Nenhuma fila com pacientes.\n");
		return;
	}

	senha atendido;
	int chamar_preferencial = (cont_normal >= 2 || n.quantidade == 0);

	if (chamar_preferencial && p.quantidade > 0) {

		atendido  = p.dados[p.inicio];
		p.inicio  = (p.inicio + 1) % MAX;
		cont_normal = 0;

	} else if (n.quantidade > 0) {

		atendido  = n.dados[n.inicio];
		n.inicio++;
		n.quantidade--;
		cont_normal++;

	} else {

		printf("Filas vazias.\n");
		return;
	}

	printf("\n>>> Chamando senha %d (%s) — chegada: %s\n",atendido.sen,atendido.tipo == 'N' ? "Normal" : "Preferencial",atendido.hora);
}


void listar() {
	printf("\n== Fila Normal (%d na fila) ==\n", n.quantidade);
	if (n.quantidade == 0) {
		printf("   Vazia.\n");
	} else {
		for (int i = 0; i < n.quantidade; i++) {
			int idx = (n.inicio + i) % MAX;
			printf("   Senha %-4d | Tipo: %c | Hora: %s\n",
			       n.dados[idx].sen, n.dados[idx].tipo, n.dados[idx].hora);
		}
	}

	printf("\n== Fila Preferencial (%d na fila) ==\n", p.quantidade);
	if (p.quantidade == 0) {
		printf("   Vazia.\n");
	} else {
		for (int i = 0; i < p.quantidade; i++) {
			int idx = (p.inicio + i) % MAX;
			printf("   Senha %-4d | Tipo: %c | Hora: %s\n",
			       p.dados[idx].sen, p.dados[idx].tipo, p.dados[idx].hora);
		}
	}
}

void buscar() {
	int num;
	printf("Numero da senha: ");
	scanf("%d", &num);

	for (int i = 0; i < n.quantidade; i++) {
		int idx = (n.inicio + i) % MAX;
		if (n.dados[idx].sen == num) {
			printf("Encontrada — Senha %d | Normal | Hora: %s\n",
			       n.dados[idx].sen, n.dados[idx].hora);
			return;
		}
	}

	for (int i = 0; i < p.quantidade; i++) {
		int idx = (p.inicio + i) % MAX;
		if (p.dados[idx].sen == num) {
			printf("Encontrada — Senha %d | Preferencial | Hora: %s\n",
			       p.dados[idx].sen, p.dados[idx].hora);
			return;
		}
	}

	printf("Senha %d nao encontrada.\n", num);
}



void salvar_csv() {
	FILE *f = fopen(ARQUIVO, "w");
	if (f == NULL) {
		printf("Erro ao abrir arquivo.\n");
		return;
	}

	fprintf(f, "senha;tipo;hora;fila\n");

	for (int i = 0; i < n.quantidade; i++) {
		int idx = (n.inicio + i) % MAX;
		fprintf(f, "%d;%c;%s;N\n",
		        n.dados[idx].sen, n.dados[idx].tipo, n.dados[idx].hora);
	}

	for (int i = 0; i < p.quantidade; i++) {
		int idx = (p.inicio + i) % MAX;
		fprintf(f, "%d;%c;%s;P\n",
		        p.dados[idx].sen, p.dados[idx].tipo, p.dados[idx].hora);
	}

	fclose(f);
	printf("Salvo em '%s'.\n", ARQUIVO);
}


void carregar_csv() {
	FILE *f = fopen(ARQUIVO, "r");
	if (f == NULL) {
		printf("Erro ao abrir arquivo.\n");
		return;
	}

	char linha[MAX];
  fgets(linha, sizeof(linha), f);

	while (fgets(linha, sizeof(linha), f) != NULL) {
		linha[strcspn(linha, "\r\n")] = '\0';
		if (strlen(linha) == 0) continue;

		int  num;
    char tipo, hora[6], col_fila;

		if (sscanf(linha, "%d;%c;%5[^;];%c", &num, &tipo, hora, &col_fila) != 4) continue;

		if (num >= senha_g) senha_g = num + 1;

		if (col_fila == 'N' && n.quantidade < MAX) {
			n.dados[n.fim].sen  = num;
			n.dados[n.fim].tipo = tipo;
			strncpy(n.dados[n.fim].hora, hora, sizeof(n.dados[n.fim].hora) - 1);
			n.fim = (n.fim + 1) % MAX;
			n.quantidade++;

		} else if (col_fila == 'P' && p.quantidade < MAX) {
			p.dados[p.fim].sen  = num;
			p.dados[p.fim].tipo = tipo;
			strncpy(p.dados[p.fim].hora, hora, sizeof(p.dados[p.fim].hora) - 1);
			p.fim = (p.fim + 1) % MAX;
			p.quantidade++;
		}
    }

	fclose(f);
	printf("CSV carregado: %d normal(is), %d preferencial(is).\n",
	n.quantidade, p.quantidade);
}





