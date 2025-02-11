#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char nome[41];
    char genero;
    int nascimento_dia, nascimento_mes, nascimento_ano;
    int ultima_consulta_dia, ultima_consulta_mes, ultima_consulta_ano;
} PACIENTE;

int main(int argc, char *argv[]) {
    PACIENTE *pacientes;
    FILE *arquivo_entrada = fopen(argv[1],"r");
    int numero_pacientes = 44;

    if (arquivo_entrada == NULL) {
        printf("\nErro ao abrir arquivo de texto, saindo do programa\n");
        return 0;
    }

    pacientes = (PACIENTE*) malloc(sizeof(PACIENTE) * numero_pacientes);
    if (pacientes == NULL) {
        printf("\nErro ao alocar memória, saindo do programa\n");
        return 0;
    }
    
    for (int i = 0; i < numero_pacientes; i++) {
        fscanf(arquivo_entrada, "<%[^,], %c, %d/%d/%d, %d/%d/%d>\n", pacientes[i].nome, &pacientes[i].genero, &pacientes[i].nascimento_dia, &pacientes[i].nascimento_mes, &pacientes[i].nascimento_ano, &pacientes[i].ultima_consulta_dia, &pacientes[i].ultima_consulta_mes, &pacientes[i].ultima_consulta_ano); //coleta dados do arquivo
    }//coleta dados do arquivo csv

    for (int i = 0; i < numero_pacientes; i++) {
        printf("<%s, %c, %d/%d/%d, %d/%d/%d>\n", pacientes[i].nome, pacientes[i].genero, pacientes[i].nascimento_dia, pacientes[i].nascimento_mes, pacientes[i].nascimento_ano, pacientes[i].ultima_consulta_dia, pacientes[i].ultima_consulta_mes, pacientes[i].ultima_consulta_ano);
    }//coleta dados do arquivo csv


    free(pacientes);
    fclose(arquivo_entrada);
    return 0;
}