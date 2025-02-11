#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int dia, mes, ano;
} DATA;

typedef struct {
    char nome[41];
    char genero;
    DATA nascimento;
    DATA ultima_consulta;
} PACIENTE;

DATA RecebeDataAtual() {
    DATA aux;
    printf("Insira da data:\n");
    printf("dd/mm/yyyy\n");
    do{
        scanf("%d/%d/%d", &aux.dia, &aux.mes, &aux.ano);
    } while (aux.mes > 12 || aux.mes < 1 || aux.ano < 1930);
    
}


int main(int argc, char *argv[]) {
    PACIENTE *pacientes;
    DATA data_atual = RecebeDataAtual();
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
        fscanf(arquivo_entrada, "<%[^,], %c, %d/%d/%d, %d/%d/%d>\n", pacientes[i].nome, &pacientes[i].genero, &pacientes[i].nascimento.dia, &pacientes[i].nascimento.mes, &pacientes[i].nascimento.ano, &pacientes[i].ultima_consulta.dia, &pacientes[i].ultima_consulta.mes, &pacientes[i].ultima_consulta.ano); //coleta dados do arquivo
    }//coleta dados do arquivo csv

    for (int i = 0; i < numero_pacientes; i++) {
        printf("<%s, %c, %d/%d/%d, %d/%d/%d>\n", pacientes[i].nome, pacientes[i].genero, pacientes[i].nascimento.dia, pacientes[i].nascimento.mes, pacientes[i].nascimento.ano, pacientes[i].ultima_consulta.dia, pacientes[i].ultima_consulta.mes, pacientes[i].ultima_consulta.ano);
    }//coleta dados do arquivo csv


    free(pacientes);
    fclose(arquivo_entrada);
    return 0;
}