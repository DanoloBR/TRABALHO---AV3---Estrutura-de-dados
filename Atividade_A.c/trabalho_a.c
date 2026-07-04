#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>

#define MAX_MUSICAS   100
#define TAM_TITULO    100
#define TAM_ARTISTA   100
#define TAM_GENERO    50
#define ARQUIVO_CSV   "playlist.csv"

/* --------------------------- Estrutura de dados ------------------------- */
typedef struct {
    int  id;
    char titulo[TAM_TITULO];
    char artista[TAM_ARTISTA];
    int  duracaoMin;
    int  duracaoSeg;
    char genero[TAM_GENERO];
} Musica;


Musica playlist[MAX_MUSICAS];
int totalMusicas = 0;

/* ------------------------- Protótipos das funções ------------------------ */
void  exibirMenu(void);
void  lerLinha(char *destino, int tamanho);
void  sanitizarTexto(char *s);
int   lerInteiroEntre(const char *mensagem, int min, int max);
void  lerCampoObrigatorio(const char *rotulo, char *destino, int tamanho);
int   contemSubstring(const char *texto, const char *busca);
int   proximoId(void);
int   buscarIndicePorId(int id);
void  exibirMusica(const Musica *m);

void  cadastrarMusica(void);
void  listarMusicas(void);
void  buscarMusica(void);
void  editarMusica(void);
void  excluirMusica(void);
void  salvarCSV(const char *nomeArquivo);
void  carregarCSV(const char *nomeArquivo);

/* ================================ MAIN =================================== */
int main(void) {
    carregarCSV(ARQUIVO_CSV);

    int opcao;
    do {
        exibirMenu();
        opcao = lerInteiroEntre("Escolha uma opcao: ", 0, 7);

        switch (opcao) {
            case 1: cadastrarMusica(); break;
            case 2: listarMusicas();   break;
            case 3: buscarMusica();    break;
            case 4: editarMusica();    break;
            case 5: excluirMusica();   break;
            case 6:
                salvarCSV(ARQUIVO_CSV);
                printf("\n[OK] Dados salvos em \"%s\".\n", ARQUIVO_CSV);
                break;
            case 7:
                carregarCSV(ARQUIVO_CSV);
                break;
            case 0:
                printf("\nEncerrando o programa. Até logo!\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}

/* ============================ FUNÇÕES AUXILIARES ============================ */

void exibirMenu(void) {
    printf("\n================ SISTEMA DE PLAYLIST ================\n");
    printf(" 1 - Cadastrar musica\n");
    printf(" 2 - Listar musicas\n");
    printf(" 3 - Buscar musica\n");
    printf(" 4 - Editar musica\n");
    printf(" 5 - Excluir musica\n");
    printf(" 6 - Salvar em arquivo CSV\n");
    printf(" 7 - Carregar de arquivo CSV\n");
    printf(" 0 - Sair\n");
    printf("=======================================================\n");
    printf("Musicas cadastradas: %d/%d\n", totalMusicas, MAX_MUSICAS);
}

void lerLinha(char *destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }
    size_t len = strlen(destino);
    if (len > 0 && (destino[len - 1] == '\n' || destino[len - 1] == '\r')) {
        destino[len - 1] = '\0';
    }
}

void sanitizarTexto(char *s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == ';') s[i] = ',';
    }
}

int lerInteiroEntre(const char *mensagem, int min, int max) {
    char buffer[100];
    int valor;
    int valido;

    do {
        if (mensagem != NULL && strlen(mensagem) > 0) {
            printf("%s", mensagem);
        }
        lerLinha(buffer, sizeof(buffer));
        valido = (sscanf(buffer, "%d", &valor) == 1) && valor >= min && valor <= max;

        if (!valido) {
            printf("  -> Entrada invalida. Digite um numero inteiro entre %d e %d.\n", min, max);
        }
    } while (!valido);

    return valor;
}

void lerCampoObrigatorio(const char *rotulo, char *destino, int tamanho) {
    char buffer[256];

    do {
        printf("%s", rotulo);
        lerLinha(buffer, sizeof(buffer));
        sanitizarTexto(buffer);
        if (strlen(buffer) == 0) {
            printf("  -> Este campo nao pode ficar em branco. Tente novamente.\n");
        }
    } while (strlen(buffer) == 0);

    strncpy(destino, buffer, tamanho - 1);
    destino[tamanho - 1] = '\0';
}

int contemSubstring(const char *texto, const char *busca) {
    char t[256], b[256];
    int i;

    for (i = 0; texto[i] != '\0' && i < 255; i++) t[i] = (char) tolower((unsigned char) texto[i]);
    t[i] = '\0';

    for (i = 0; busca[i] != '\0' && i < 255; i++) b[i] = (char) tolower((unsigned char) busca[i]);
    b[i] = '\0';

    return strstr(t, b) != NULL;
}

int proximoId(void) {
    int maior = 0;
    for (int i = 0; i < totalMusicas; i++) {
        if (playlist[i].id > maior) maior = playlist[i].id;
    }
    return maior + 1;
}

int buscarIndicePorId(int id) {
    for (int i = 0; i < totalMusicas; i++) {
        if (playlist[i].id == id) return i;
    }
    return -1;
}

void exibirMusica(const Musica *m) {
    printf("\n-------------------------------------\n");
    printf("ID.......: %d\n", m->id);
    printf("Titulo...: %s\n", m->titulo);
    printf("Artista..: %s\n", m->artista);
    printf("Duracao..: %02d:%02d\n", m->duracaoMin, m->duracaoSeg);
    printf("Genero...: %s\n", m->genero);
    printf("-------------------------------------\n");
}

/* ============================ CRUD ============================ */

void cadastrarMusica(void) {
    if (totalMusicas >= MAX_MUSICAS) {
        printf("\n[ERRO] A playlist esta cheia (limite de %d musicas). "
               "Nao e possivel cadastrar mais.\n", MAX_MUSICAS);
        return;
    }

    Musica nova;
    nova.id = proximoId();

    printf("\n--- Cadastro de nova musica (ID %d) ---\n", nova.id);
    lerCampoObrigatorio("Titulo..: ",  nova.titulo,  sizeof(nova.titulo));
    lerCampoObrigatorio("Artista.: ",  nova.artista, sizeof(nova.artista));
    lerCampoObrigatorio("Genero..: ",  nova.genero,  sizeof(nova.genero));

    nova.duracaoMin = lerInteiroEntre("Duracao - minutos.......: ", 0, 999);
    nova.duracaoSeg = lerInteiroEntre("Duracao - segundos(0-59): ", 0, 59);

    playlist[totalMusicas] = nova;
    totalMusicas++;

    printf("\n[OK] Musica cadastrada com sucesso! (ID: %d)\n", nova.id);
    salvarCSV(ARQUIVO_CSV);
}

void listarMusicas(void) {
    if (totalMusicas == 0) {
        printf("\n[INFO] A playlist esta vazia. Cadastre uma musica primeiro.\n");
        return;
    }

    printf("\n%-4s %-25s %-20s %-8s %-15s\n", "ID", "Titulo", "Artista", "Duracao", "Genero");
    printf("---------------------------------------------------------------------------\n");
    for (int i = 0; i < totalMusicas; i++) {
        printf("%-4d %-25s %-20s %02d:%02d    %-15s\n",
               playlist[i].id, playlist[i].titulo, playlist[i].artista,
               playlist[i].duracaoMin, playlist[i].duracaoSeg, playlist[i].genero);
    }
    printf("---------------------------------------------------------------------------\n");
    printf("Total de musicas: %d/%d\n", totalMusicas, MAX_MUSICAS);
}

void buscarMusica(void) {
    if (totalMusicas == 0) {
        printf("\n[INFO] A playlist esta vazia. Nao ha nada para buscar.\n");
        return;
    }

    int opcao = lerInteiroEntre(
        "\n--- Buscar musica ---\n"
        "1 - Por ID\n"
        "2 - Por titulo\n"
        "3 - Por artista\n"
        "0 - Voltar\n"
        "Opcao: ", 0, 3);

    switch (opcao) {
        case 1: {
            int id = lerInteiroEntre("Digite o ID: ", 1, 999999);
            int idx = buscarIndicePorId(id);
            if (idx == -1) {
                printf("\n[INFO] Nenhuma musica encontrada com ID %d.\n", id);
            } else {
                exibirMusica(&playlist[idx]);
            }
            break;
        }
        case 2: {
            char termo[100];
            printf("Digite parte do titulo: ");
            lerLinha(termo, sizeof(termo));
            int achou = 0;
            for (int i = 0; i < totalMusicas; i++) {
                if (contemSubstring(playlist[i].titulo, termo)) {
                    exibirMusica(&playlist[i]);
                    achou = 1;
                }
            }
            if (!achou) printf("\n[INFO] Nenhuma musica encontrada com esse titulo.\n");
            break;
        }
        case 3: {
            char termo[100];
            printf("Digite parte do artista: ");
            lerLinha(termo, sizeof(termo));
            int achou = 0;
            for (int i = 0; i < totalMusicas; i++) {
                if (contemSubstring(playlist[i].artista, termo)) {
                    exibirMusica(&playlist[i]);
                    achou = 1;
                }
            }
            if (!achou) printf("\n[INFO] Nenhuma musica encontrada com esse artista.\n");
            break;
        }
        case 0:
        default:
            return;
    }
}

void editarMusica(void) {
    if (totalMusicas == 0) {
        printf("\n[INFO] A playlist esta vazia. Nao ha nada para editar.\n");
        return;
    }

    listarMusicas();
    int id  = lerInteiroEntre("\nDigite o ID da musica que deseja editar: ", 1, 999999);
    int idx = buscarIndicePorId(id);

    if (idx == -1) {
        printf("\n[INFO] ID nao encontrado.\n");
        return;
    }

    Musica *m = &playlist[idx];
    char buffer[256];

    printf("\nEditando musica ID %d. Deixe em branco para manter o valor atual.\n", m->id);

    printf("Titulo atual [%s]\nNovo titulo: ", m->titulo);
    lerLinha(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) {
        sanitizarTexto(buffer);
        strncpy(m->titulo, buffer, sizeof(m->titulo) - 1);
        m->titulo[sizeof(m->titulo) - 1] = '\0';
    }

    printf("Artista atual [%s]\nNovo artista: ", m->artista);
    lerLinha(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) {
        sanitizarTexto(buffer);
        strncpy(m->artista, buffer, sizeof(m->artista) - 1);
        m->artista[sizeof(m->artista) - 1] = '\0';
    }

    printf("Genero atual [%s]\nNovo genero: ", m->genero);
    lerLinha(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) {
        sanitizarTexto(buffer);
        strncpy(m->genero, buffer, sizeof(m->genero) - 1);
        m->genero[sizeof(m->genero) - 1] = '\0';
    }

    char pergunta[128];
    snprintf(pergunta, sizeof(pergunta),
             "Duracao atual [%02d:%02d]. Deseja alterar? (1-Sim / 0-Nao): ",
             m->duracaoMin, m->duracaoSeg);
    int alterar = lerInteiroEntre(pergunta, 0, 1);
    if (alterar) {
        m->duracaoMin = lerInteiroEntre("Novos minutos........: ", 0, 999);
        m->duracaoSeg = lerInteiroEntre("Novos segundos(0-59)..: ", 0, 59);
    }

    printf("\n[OK] Musica atualizada com sucesso!\n");
    salvarCSV(ARQUIVO_CSV);
}

void excluirMusica(void) {
    if (totalMusicas == 0) {
        printf("\n[INFO] A playlist esta vazia. Nao ha nada para excluir.\n");
        return;
    }

    listarMusicas();
    int id  = lerInteiroEntre("\nDigite o ID da musica que deseja excluir: ", 1, 999999);
    int idx = buscarIndicePorId(id);

    if (idx == -1) {
        printf("\n[INFO] ID nao encontrado.\n");
        return;
    }

    char pergunta[300];
    snprintf(pergunta, sizeof(pergunta),
             "\nTem certeza que deseja excluir \"%s\" de %s? (1-Sim / 0-Nao): ",
             playlist[idx].titulo, playlist[idx].artista);
    int confirmar = lerInteiroEntre(pergunta, 0, 1);

    if (!confirmar) {
        printf("Operacao cancelada.\n");
        return;
    }

    for (int i = idx; i < totalMusicas - 1; i++) {
        playlist[i] = playlist[i + 1];
    }
    totalMusicas--;

    printf("\n[OK] Musica removida com sucesso!\n");
    salvarCSV(ARQUIVO_CSV);
}

/* ============================ PERSISTÊNCIA (CSV) ============================ */

void salvarCSV(const char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "w");
    if (f == NULL) {
        printf("\n[ERRO] Nao foi possivel abrir \"%s\" para salvar.\n", nomeArquivo);
        return;
    }

    fprintf(f, "id;titulo;artista;duracao_min;duracao_seg;genero\n");
    for (int i = 0; i < totalMusicas; i++) {
        fprintf(f, "%d;%s;%s;%d;%d;%s\n",
                playlist[i].id, playlist[i].titulo, playlist[i].artista,
                playlist[i].duracaoMin, playlist[i].duracaoSeg, playlist[i].genero);
    }

    fclose(f);
}

void carregarCSV(const char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "r");
    if (f == NULL) {
        return;
    }

    char linha[512];
    if (fgets(linha, sizeof(linha), f) == NULL) {
        fclose(f);
        return;
    }

    totalMusicas = 0;
    while (fgets(linha, sizeof(linha), f) != NULL && totalMusicas < MAX_MUSICAS) {
        linha[strcspn(linha, "\r\n")] = '\0';
        if (strlen(linha) == 0) continue;

        Musica m;
        char *token;

        token = strtok(linha, ";"); if (!token) continue; m.id = atoi(token);

        token = strtok(NULL, ";"); if (!token) continue;
        strncpy(m.titulo, token, sizeof(m.titulo) - 1); m.titulo[sizeof(m.titulo) - 1] = '\0';

        token = strtok(NULL, ";"); if (!token) continue;
        strncpy(m.artista, token, sizeof(m.artista) - 1); m.artista[sizeof(m.artista) - 1] = '\0';

        token = strtok(NULL, ";"); if (!token) continue; m.duracaoMin = atoi(token);
        token = strtok(NULL, ";"); if (!token) continue; m.duracaoSeg = atoi(token);

        token = strtok(NULL, ";"); if (!token) continue;
        strncpy(m.genero, token, sizeof(m.genero) - 1); m.genero[sizeof(m.genero) - 1] = '\0';

        playlist[totalMusicas] = m;
        totalMusicas++;
    }

    fclose(f);
    printf("\n[INFO] %d musica(s) carregada(s) de \"%s\".\n", totalMusicas, nomeArquivo);
}