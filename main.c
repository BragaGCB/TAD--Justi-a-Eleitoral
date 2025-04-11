#include <stdio.h>
#include "dados.h"

int main() {
    Processo processos[LIMITE_PROCESSOS];
    int quantidade;

    carregar_dados(processos, &quantidade);

    ordenar_por_id(processos, quantidade);
    exportar_csv("ordenado_id.csv", processos, quantidade);
    printf("Arquivo 'ordenado_id.csv' gerado com sucesso.\n");

    ordenar_por_data(processos, quantidade);
    exportar_csv("ordenado_data.csv", processos, quantidade);
    printf("Arquivo 'ordenado_data.csv' gerado com sucesso.\n");

    int classe = 0;
    printf("\nInforme o id_classe para contar: ");
    if (scanf("%d", &classe) == 1) {
        int total = contar_por_classe(processos, quantidade, classe);
        printf("Processos com id_classe %d: %d\n", classe, total);
    }

    int unicos = contar_assuntos_diferentes(processos, quantidade);
    printf("Assuntos únicos na base: %d\n", unicos);

    mostrar_multiplos_assuntos(processos, quantidade);

    int indice = 0;
    printf("\nInforme o índice de processo (0 a %d) para calcular dias de tramitação: ", quantidade - 1);
    if (scanf("%d", &indice) == 1 && indice >= 0 && indice < quantidade) {
        int dias = calcular_dias_tramitando(processos[indice]);
        printf("O processo está em tramitação há %d dias.\n", dias);
    } else {
        printf("Índice inválido.\n");
    }

    return 0;
}
