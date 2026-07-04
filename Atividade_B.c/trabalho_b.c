#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10
#define ARQUIVO_CSV "historico.csv"


typedef struct {
    char url[100];
    char titulo[100];
    char data[11]; // Formato DD/MM/AAAA
    char hora[6];  // Formato HH:MM
} Pagina;

typedef struct {
    Pagina itens[MAX];
    int topo;
} Pilha;

void inicializar(Pilha *p) {
    p->topo = -1;
}

int vazia(Pilha *p) {
    return p->topo == -1;
}

int cheia(Pilha *p) {
    return p->topo == MAX - 1;
}

void visitarPagina(Pilha *p, char url[], char titulo[], char data[], char hora[]) {
    if (cheia(p)) {
        printf("\n[AVISO] A pilha esta cheia. Limpe o historico antes de navegar mais.\n");
        return;
    }

    p->topo++;
    

    strcpy(p->itens[p->topo].url, url);
    strcpy(p->itens[p->topo].titulo, titulo);
    strcpy(p->itens[p->topo].data, data);
    strcpy(p->itens[p->topo].hora, hora);

    printf("\nPagina '%s' visitada com sucesso!\n", titulo);
}

void voltarPagina(Pilha *p) {
    if (vazia(p)) {
        printf("\nVocê não pesquisou nada ainda.\n");
        return;
    }

    printf("\nRetornando de: %s\n", p->itens[p->topo].titulo);
    p->topo--;
    
    if (!vazia(p)) {
        printf("Agora voce esta em: %s (%s)\n", p->itens[p->topo].titulo, p->itens[p->topo].url);
    } else {
        printf("Voce voltou para a pagina inicial vazia.\n");
    }
}

void paginaAtual(Pilha *p) {
    if (vazia(p)) {
        printf("\nVocê nem pesquisou nada ainda.\n");
    } else {
        printf("\n--- PAGINA ATUAL ---\n");
        printf("Titulo: %s\n", p->itens[p->topo].titulo);
        printf("URL: %s\n", p->itens[p->topo].url);
        printf("Acesso em: %s as %s\n", p->itens[p->topo].data, p->itens[p->topo].hora);
    }
}

void mostrarHistorico(Pilha *p) {
    if (vazia(p)) {
        printf("\nHistórico vazio, bobinho.\n");
        return;
    }

    printf("\n---- HISTÓRICO DE NAVEGAÇÃO ----\n");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%d] %s | %s | Data: %s %s\n", 
               i + 1, 
               p->itens[i].titulo, 
               p->itens[i].url, 
               p->itens[i].data, 
               p->itens[i].hora);
    }
}


void salvarCSV(Pilha *p, const char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "w");
    if (f == NULL) {
        printf("\n[ERRO] Nao foi possivel abrir \"%s\" para salvar.\n", nomeArquivo);
        return;
    }

    fprintf(f, "url;titulo;data;hora\n"); 
    
    for (int i = 0; i <= p->topo; i++) {
        fprintf(f, "%s;%s;%s;%s\n", 
                p->itens[i].url, 
                p->itens[i].titulo, 
                p->itens[i].data, 
                p->itens[i].hora);
    }

    fclose(f);
    printf("\n[INFO] Historico salvo com sucesso em \"%s\".\n", nomeArquivo);
}


void carregarCSV(Pilha *p, const char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "r");
    if (f == NULL) return; 

    char linha[300]; 
    
    if (fgets(linha, sizeof(linha), f) == NULL) {
        fclose(f);
        return;
    }

    while (fgets(linha, sizeof(linha), f) != NULL) {
        if (cheia(p)) {
            printf("\n[AVISO] Arquivo atingiu o limite maximo de %d links.\n", MAX);
            break; 
        }

        linha[strcspn(linha, "\r\n")] = '\0';
        if (strlen(linha) == 0) continue;

        p->topo++;
        char *token;

       
        token = strtok(linha, ";");
        if (token) strcpy(p->itens[p->topo].url, token);

        token = strtok(NULL, ";");
        if (token) strcpy(p->itens[p->topo].titulo, token);

        token = strtok(NULL, ";");
        if (token) strcpy(p->itens[p->topo].data, token);

        token = strtok(NULL, ";");
        if (token) strcpy(p->itens[p->topo].hora, token);
    }

    fclose(f);
    printf("\n[INFO] Historico anterior carregado com sucesso!\n");
}

int main() {
    Pilha historico;
    inicializar(&historico);
    carregarCSV(&historico, ARQUIVO_CSV);

    int opcao;
    
    char url[100], titulo[100], data[11], hora[6];

    do {
        printf("\n-----------------------------\n");
        printf("     WABA LABA DUB DUB\n");
        printf("-----------------------------\n");
        printf("1 - Pesquisar pagina\n");
        printf("2 - Retornar a pagina anterior\n");
        printf("3 - Mostrar pagina aberta agora\n");
        printf("4 - Mostrar historico completo\n");
        printf("5 - Fechar everything\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1:
                printf("Digite o URL da pagina: ");
                fgets(url, sizeof(url), stdin);
                url[strcspn(url, "\n")] = '\0';

                printf("Digite o Titulo da pagina: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';

                printf("Digite a Data (DD/MM/AAAA): ");
                fgets(data, sizeof(data), stdin);
                data[strcspn(data, "\n")] = '\0';

                printf("Digite a Hora (HH:MM): ");
                fgets(hora, sizeof(hora), stdin);
                hora[strcspn(hora, "\n")] = '\0';

                visitarPagina(&historico, url, titulo, data, hora);
                break;

            case 2:
                voltarPagina(&historico);
                break;

            case 3:
                paginaAtual(&historico);
                break;

            case 4:
                mostrarHistorico(&historico);
                break;

            case 5:
                salvarCSV(&historico, ARQUIVO_CSV);
                printf("\nFechando...\n");
                break;

            default:
                printf("\nEssa opcao nao existe.\n");
        }

    } while(opcao != 5);

    return 0;
}