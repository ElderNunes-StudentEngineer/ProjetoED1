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
    printf("Insira a data atual:\n");
    printf("dd/mm/yyyy\n");
    do {
        scanf("%d/%d/%d", &aux.dia, &aux.mes, &aux.ano);
    } while (aux.mes > 12 || aux.mes < 1 || aux.ano < 1930);
    return aux;
}

int calcularDiferencaDias(DATA data1, DATA data2) {
    int dias1 = data1.ano * 365 + data1.mes * 30 + data1.dia;
    int dias2 = data2.ano * 365 + data2.mes * 30 + data2.dia;
    return dias1 - dias2;
}

int calcularIdade(DATA data_nascimento, DATA data_atual) {
    int idade = data_atual.ano - data_nascimento.ano;
    if (data_atual.mes < data_nascimento.mes || (data_atual.mes == data_nascimento.mes && data_atual.dia < data_nascimento.dia)) {
        idade--;
    }
    return idade;
}

void exibirMenu() {
    printf("\n----- Menu Inicial -----\n");
    printf("1 - Pacientes da Liz\n");
    printf("2 - Pacientes do Moisés\n");
    printf("3 - Sair do sistema\n");
    printf("Digite a opção desejada: ");
}

void exibirMenuPaciente() {
    printf("\n----- Menu Paciente -----\n");
    printf("1 - Consultar Paciente\n");
    printf("2 - Cadastrar Paciente\n");
    printf("3 - Alterar Cadastro do Paciente\n");
    printf("4 - Voltar ao menu inicial\n");
    printf("5 - Sair do sistema\n");
    printf("Digite a opção desejada: ");
}

void ConsultarPaciente(PACIENTE *pacientes, int numero_pacientes, DATA data_atual) {
    char nome[41];
    printf("Digite o nome do paciente: ");
    scanf(" %[^\n]", nome);
    for (int i = 0; i < numero_pacientes; i++) {
        if (strcmp(pacientes[i].nome, nome) == 0) {
            int idade = calcularIdade(pacientes[i].nascimento, data_atual);
            int dias_ultima_consulta = calcularDiferencaDias(data_atual, pacientes[i].ultima_consulta);
            printf("Paciente encontrado:\n");
            printf("Nome: %s\n", pacientes[i].nome);
            printf("Gênero: %c\n", pacientes[i].genero);
            printf("Idade: %d anos\n", idade);
            printf("Dias desde a última consulta: %d\n", dias_ultima_consulta);
            return;
        }
    }
    printf("Paciente não encontrado.\n");
}

void CadastrarPaciente(PACIENTE *pacientes, int *numero_pacientes) {
    if (*numero_pacientes == 44) {
        printf("Número máximo de pacientes atingido!\n");
        return;
    }
    printf("Digite o nome do paciente: ");
    scanf(" %[^\n]", pacientes[*numero_pacientes].nome);
    printf("Digite o gênero do paciente (M/F): ");
    scanf(" %c", &pacientes[*numero_pacientes].genero);
    printf("Digite a data de nascimento (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &pacientes[*numero_pacientes].nascimento.dia, &pacientes[*numero_pacientes].nascimento.mes, &pacientes[*numero_pacientes].nascimento.ano);
    printf("Digite a data da última consulta (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &pacientes[*numero_pacientes].ultima_consulta.dia, &pacientes[*numero_pacientes].ultima_consulta.mes, &pacientes[*numero_pacientes].ultima_consulta.ano);
    (*numero_pacientes)++;
    printf("Paciente adicionado com sucesso!\n");
}

void AlterarCadastroPaciente(PACIENTE *pacientes, int numero_pacientes) {
    char nome[41];
    printf("Digite o nome do paciente: ");
    scanf(" %[^\n]", nome);
    for (int i = 0; i < numero_pacientes; i++) {
        if (strcmp(pacientes[i].nome, nome) == 0) {
            printf("Digite o novo nome do paciente: ");
            scanf(" %[^\n]", pacientes[i].nome);
            printf("Digite o novo gênero do paciente (M/F): ");
            scanf(" %c", &pacientes[i].genero);
            printf("Digite a nova data de nascimento (dd/mm/yyyy): ");
            scanf("%d/%d/%d", &pacientes[i].nascimento.dia, &pacientes[i].nascimento.mes, &pacientes[i].nascimento.ano);
            printf("Digite a nova data da última consulta (dd/mm/yyyy): ");
            scanf("%d/%d/%d", &pacientes[i].ultima_consulta.dia, &pacientes[i].ultima_consulta.mes, &pacientes[i].ultima_consulta.ano);
            printf("Cadastro alterado com sucesso!\n");
            return;
        }
    }
    printf("Paciente não encontrado.\n");
}

void gerarArquivoSaida(PACIENTE *pacientes, int numero_pacientes) {
    FILE *arquivo_Liz = fopen("pacientes_Liz.txt", "w");
    FILE *arquivo_Moises = fopen("pacientes_Moises.txt", "w");

    if (arquivo_Liz == NULL || arquivo_Moises == NULL) {
        printf("Erro ao abrir arquivo de texto\n");
        return;
    }

    for (int i = 0; i < numero_pacientes; i++) {
        if (pacientes[i].genero == 'F') {
            fprintf(arquivo_Liz, "<%s, %c, %d/%d/%d, %d/%d/%d>\n", pacientes[i].nome, pacientes[i].genero, pacientes[i].nascimento.dia, pacientes[i].nascimento.mes, pacientes[i].nascimento.ano, pacientes[i].ultima_consulta.dia, pacientes[i].ultima_consulta.mes, pacientes[i].ultima_consulta.ano);
        } else {
            fprintf(arquivo_Moises, "<%s, %c, %d/%d/%d, %d/%d/%d>\n", pacientes[i].nome, pacientes[i].genero, pacientes[i].nascimento.dia, pacientes[i].nascimento.mes, pacientes[i].nascimento.ano, pacientes[i].ultima_consulta.dia, pacientes[i].ultima_consulta.mes, pacientes[i].ultima_consulta.ano);
        }
    }
    fclose(arquivo_Liz);
    fclose(arquivo_Moises);
    printf("Arquivos gerados com sucesso!\n");
}

void MenuPaciente(PACIENTE *pacientes, int *numero_pacientes, DATA data_atual) {
    int opcao;
    do {
        exibirMenuPaciente();
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                ConsultarPaciente(pacientes, *numero_pacientes, data_atual);
                break;
            case 2:
                CadastrarPaciente(pacientes, numero_pacientes);
                break;
            case 3:
                AlterarCadastroPaciente(pacientes, *numero_pacientes);
                break;
            case 4:
                return;
            case 5:
                gerarArquivoSaida(pacientes, *numero_pacientes);
                exit(0);
            default:
                printf("Opção inválida\n");
        }
    } while (true);
}

int main(int argc, char *argv[]) {
    PACIENTE pacientes[44];
    int numero_pacientes = 0;
    DATA data_atual = RecebeDataAtual();

    if (argc < 2) {
        printf("Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    FILE *arquivo_entrada = fopen(argv[1], "r");
    if (arquivo_entrada == NULL) {
        printf("Erro ao abrir arquivo de texto, saindo do programa\n");
        return 1;
    }

    while (fscanf(arquivo_entrada, "<%[^,], %c, %d/%d/%d, %d/%d/%d>\n", pacientes[numero_pacientes].nome, &pacientes[numero_pacientes].genero, &pacientes[numero_pacientes].nascimento.dia, &pacientes[numero_pacientes].nascimento.mes, &pacientes[numero_pacientes].nascimento.ano, &pacientes[numero_pacientes].ultima_consulta.dia, &pacientes[numero_pacientes].ultima_consulta.mes, &pacientes[numero_pacientes].ultima_consulta.ano) != EOF) {
        numero_pacientes++;
    }
    fclose(arquivo_entrada);

    int opcao;
    do {
        exibirMenu();
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("Pacientes da Liz:\n");
                MenuPaciente(pacientes, &numero_pacientes, data_atual);
                break;
            case 2:
                printf("Pacientes do Moisés:\n");
                MenuPaciente(pacientes, &numero_pacientes, data_atual);
                break;
            case 3:
                gerarArquivoSaida(pacientes, numero_pacientes);
                printf("Encerrando o programa...\n");
                exit(0);
            default:
                printf("Opção inválida\n");
        }
    } while (true);

    return 0;
}