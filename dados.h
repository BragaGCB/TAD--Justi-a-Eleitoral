#ifndef DADOS_H
#define DADOS_H

#define LIMITE_PROCESSOS 1000
#define MAX_ASSUNTOS 10
#define MAX_LINHA 512

typedef struct {
    int id;
    char numero[25];
    char data_ajuizamento[20];
    int id_classe;
    int assuntos[MAX_ASSUNTOS];
    int total_assuntos;
    int ano_eleicao;
} Processo;

void carregar_dados(Processo lista[], int *qtd);
void ordenar_por_id(Processo lista[], int qtd);
void ordenar_por_data(Processo lista[], int qtd);
int contar_por_classe(Processo lista[], int qtd, int classe);
int contar_assuntos_diferentes(Processo lista[], int qtd);
void mostrar_multiplos_assuntos(Processo lista[], int qtd);
int calcular_dias_tramitando(Processo p);
void exportar_csv(const char *nome, Processo lista[], int qtd);

#endif
