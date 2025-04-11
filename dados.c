#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dados.h"

static int comparar_datas(const char *data1, const char *data2) {
    struct tm dt1 = {0}, dt2 = {0};
    sscanf(data1, "%d-%d-%d", &dt1.tm_year, &dt1.tm_mon, &dt1.tm_mday);
    sscanf(data2, "%d-%d-%d", &dt2.tm_year, &dt2.tm_mon, &dt2.tm_mday);

    dt1.tm_year -= 1900; dt2.tm_year -= 1900;
    dt1.tm_mon--; dt2.tm_mon--;

    time_t t1 = mktime(&dt1);
    time_t t2 = mktime(&dt2);

    return (int)difftime(t2, t1);
}

void carregar_dados(Processo lista[], int *qtd) {
    FILE *arquivo = fopen("processo_043_202409032338.csv", "r");
    if (!arquivo) {
        perror("Erro ao abrir CSV");
        exit(1);
    }

    char linha[MAX_LINHA];
    fgets(linha, MAX_LINHA, arquivo); // cabeçalho
    *qtd = 0;

    while (fgets(linha, MAX_LINHA, arquivo) && *qtd < LIMITE_PROCESSOS) {
        Processo *proc = &lista[*qtd];
        char *token = strtok(linha, ",");

        proc->id = atoi(token);
        strcpy(proc->numero, strtok(NULL, ","));
        strncpy(proc->data_ajuizamento, strtok(NULL, ","), 19);

        char *classe_raw = strtok(NULL, ",");
        proc->id_classe = atoi(strtok(classe_raw, "{}"));

        char *assuntos_raw = strtok(NULL, ",");
        char *ptr = strtok(assuntos_raw, "{}");
        proc->total_assuntos = 0;
        while (ptr != NULL) {
            proc->assuntos[proc->total_assuntos++] = atoi(ptr);
            ptr = strtok(NULL, "{}");
        }

        proc->ano_eleicao = atoi(strtok(NULL, ","));
        (*qtd)++;
    }

    fclose(arquivo);
}

void ordenar_por_id(Processo lista[], int qtd) {
    for (int i = 0; i < qtd - 1; ++i) {
        for (int j = 0; j < qtd - i - 1; ++j) {
            if (lista[j].id > lista[j + 1].id) {
                Processo tmp = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = tmp;
            }
        }
    }
}

void ordenar_por_data(Processo lista[], int qtd) {
    for (int i = 0; i < qtd - 1; ++i) {
        for (int j = 0; j < qtd - i - 1; ++j) {
            if (comparar_datas(lista[j].data_ajuizamento, lista[j + 1].data_ajuizamento) < 0) {
                Processo temp = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = temp;
            }
        }
    }
}

int contar_por_classe(Processo lista[], int qtd, int classe) {
    int total = 0;
    for (int i = 0; i < qtd; ++i)
        if (lista[i].id_classe == classe)
            total++;
    return total;
}

int contar_assuntos_diferentes(Processo lista[], int qtd) {
    int mapa[10000] = {0}, unicos = 0;
    for (int i = 0; i < qtd; ++i) {
        for (int j = 0; j < lista[i].total_assuntos; ++j) {
            int a = lista[i].assuntos[j];
            if (!mapa[a]) {
                mapa[a] = 1;
                unicos++;
            }
        }
    }
    return unicos;
}

void mostrar_multiplos_assuntos(Processo lista[], int qtd) {
    printf("\nProcessos com múltiplos assuntos:\n");
    for (int i = 0; i < qtd; ++i)
        if (lista[i].total_assuntos > 1)
            printf("ID: %d | Nº: %s\n", lista[i].id, lista[i].numero);
}

int calcular_dias_tramitando(Processo p) {
    struct tm dt = {0};
    sscanf(p.data_ajuizamento, "%d-%d-%d", &dt.tm_year, &dt.tm_mon, &dt.tm_mday);
    dt.tm_year -= 1900;
    dt.tm_mon--;
    time_t inicio = mktime(&dt);
    time_t agora = time(NULL);
    return (int)((agora - inicio) / 86400);
}

void exportar_csv(const char *nome, Processo lista[], int qtd) {
    FILE *f = fopen(nome, "w");
    if (!f) return;

    fprintf(f, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");
    for (int i = 0; i < qtd; ++i) {
        fprintf(f, "%d,\"%s\",%s,{", lista[i].id, lista[i].numero, lista[i].data_ajuizamento);
        for (int j = 0; j < lista[i].total_assuntos; ++j) {
            fprintf(f, "%d", lista[i].assuntos[j]);
            if (j < lista[i].total_assuntos - 1) fprintf(f, ",");
        }
        fprintf(f, "},%d\n", lista[i].ano_eleicao);
    }

    fclose(f);
}
