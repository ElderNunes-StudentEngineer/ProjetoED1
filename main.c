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

typedef struct noAvl {
    PACIENTE paciente;
    struct noAvl *esq;
    struct noAvl *dir;
    int altura;
} AVL;

typedef struct nolista {
    PACIENTE paciente;
    struct nolista *proximo;
    struct nolista *anterior;
}NOLISTA;


typedef struct lista {
    NOLISTA *inicio;
    int tamanho;
}LISTADUPLAMENTEENCADEADA;

void InicializaLista(LISTADUPLAMENTEENCADEADA *lista) {
    lista->inicio = NULL;
    lista->tamanho = 0;
}

bool EstaVazia(LISTADUPLAMENTEENCADEADA *lista) {
    return lista->tamanho == 0;
}


void InsereElemento(LISTADUPLAMENTEENCADEADA *lista, PACIENTE novo_paciente) {
    NOLISTA *novo_elemento = (NOLISTA*) malloc(sizeof(NOLISTA));
    if (novo_elemento == NULL) {
        printf("Erro ao alocar memória para novo elemento!!\n\n");
        return;
    }
    novo_elemento->paciente.genero = novo_paciente.genero;
    novo_elemento->paciente.nascimento = novo_paciente.nascimento;
    strcpy(novo_elemento->paciente.nome, novo_paciente.nome);
    novo_elemento->paciente.ultima_consulta = novo_paciente.ultima_consulta;
    
    if (EstaVazia(lista) == true) {
        lista->inicio = novo_elemento;
        lista->inicio->anterior = lista->inicio->proximo = NULL;
    } else if (strcmp(lista->inicio->paciente.nome, novo_paciente.nome) < 0) {
        novo_elemento->proximo = lista->inicio;
        novo_elemento->anterior = NULL; 
        lista->inicio->anterior = novo_elemento;
        lista->inicio = novo_elemento;
    } else {
        NOLISTA *aux = lista->inicio;
        while (aux->proximo != NULL && strcmp(aux->proximo->paciente.nome, novo_paciente.nome) > 0) {
            aux = aux->proximo;
        }

        novo_elemento->proximo = aux->proximo;
        novo_elemento->anterior = aux;
        if (aux->proximo != NULL) {
            aux->proximo->anterior = novo_elemento;
        }
        aux->proximo = novo_elemento;

        if (aux != NULL) {
            aux->anterior = novo_elemento;
        }
        
    }
    
    lista->tamanho++;

    return;
}

// Funções da árvore AVL
int altura(AVL *no) {
    if (no == NULL) {
        return -1;
    } else {
        return no->altura;
    }
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

AVL *CriarNo(PACIENTE paciente) {
    AVL *novo = (AVL *)malloc(sizeof(AVL));
    novo->paciente = paciente;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->altura = 0;
    return novo;
}

AVL *RotacaoDireita(AVL *raiz) {
    AVL *aux = raiz->esq;
    raiz->esq = aux->dir;
    aux->dir = raiz;
    raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;
    aux->altura = max(altura(aux->esq), raiz->altura) + 1;
    return aux;
}

AVL *RotacaoEsquerda(AVL *raiz) {
    AVL *aux = raiz->dir;
    raiz->dir = aux->esq;
    aux->esq = raiz;
    raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;
    aux->altura = max(altura(aux->dir), raiz->altura) + 1;
    return aux;
}

int fatorBalanceamento(AVL *no) {
    return (no == NULL) ? 0 : altura(no->esq) - altura(no->dir);
}

AVL *Inserir(AVL *raiz, PACIENTE paciente) {
    if (raiz == NULL) {
        return CriarNo(paciente);
    } else if (strcmp(paciente.nome, raiz->paciente.nome) < 0) {
        raiz->esq = Inserir(raiz->esq, paciente);
    } else if (strcmp(paciente.nome, raiz->paciente.nome) > 0) {
        raiz->dir = Inserir(raiz->dir, paciente);
    } else {
        return raiz; 
    }
    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int balanceamento = fatorBalanceamento(raiz);
    if (balanceamento > 1 && strcmp(paciente.nome, raiz->esq->paciente.nome) < 0) {
        return RotacaoDireita(raiz);
    }
    if (balanceamento < -1 && strcmp(paciente.nome, raiz->dir->paciente.nome) > 0) {
        return RotacaoEsquerda(raiz);
    }
    if (balanceamento > 1 && strcmp(paciente.nome, raiz->esq->paciente.nome) > 0) {
        raiz->esq = RotacaoEsquerda(raiz->esq);
        return RotacaoDireita(raiz);
    }
    if (balanceamento < -1 && strcmp(paciente.nome, raiz->dir->paciente.nome) < 0) {
        raiz->dir = RotacaoDireita(raiz->dir);
        return RotacaoEsquerda(raiz);
    }
    return raiz;
}

void emOrdem(AVL *raiz, FILE *arquivo) {
    if (raiz != NULL) {
        emOrdem(raiz->esq, arquivo);
        fprintf(arquivo, "<%s, %c, %02d/%02d/%04d, %02d/%02d/%04d>\n",
                raiz->paciente.nome, raiz->paciente.genero,
                raiz->paciente.nascimento.dia, raiz->paciente.nascimento.mes, raiz->paciente.nascimento.ano,
                raiz->paciente.ultima_consulta.dia, raiz->paciente.ultima_consulta.mes, raiz->paciente.ultima_consulta.ano);
        emOrdem(raiz->dir, arquivo);
    }
}

DATA RecebeDataAtual() {
    DATA aux;
    printf("Insira a data atual:\n");
    printf("dd/mm/yyyy\n");
    do {
        scanf("%d/%d/%d", &aux.dia, &aux.mes, &aux.ano);
        while (getchar() != '\n'); 
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

AVL *Remover(AVL* raiz, char *nome){
    if(raiz == NULL){
        return raiz;
    }
    if(strcmp(nome, raiz->paciente.nome) < 0){
        raiz->esq = Remover(raiz->esq, nome);
    } else if(strcmp(nome, raiz->paciente.nome) > 0){
        raiz->dir = Remover(raiz->dir, nome);
    } else {
        if(raiz->esq == NULL || raiz->dir == NULL){
            AVL *temp = raiz->esq ? raiz->esq : raiz->dir;
            if(temp == NULL){
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        } else {
            AVL *temp = raiz->dir;
            while(temp->esq != NULL){
                temp = temp->esq;
            }
            raiz->paciente = temp->paciente;
            raiz->dir = Remover(raiz->dir, temp->paciente.nome);
        }
    }
    if(raiz == NULL){
        return raiz;
    }
    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int balanceamento = fatorBalanceamento(raiz);
    if(balanceamento > 1 && fatorBalanceamento(raiz->esq) >= 0){
        return RotacaoDireita(raiz);
    }
    if(balanceamento > 1 && fatorBalanceamento(raiz->esq) < 0){
        raiz->esq = RotacaoEsquerda(raiz->esq);
        return RotacaoDireita(raiz);
    }
    if(balanceamento < -1 && fatorBalanceamento(raiz->dir) <= 0){
        return RotacaoEsquerda(raiz);
    }
    if(balanceamento < -1 && fatorBalanceamento(raiz->dir) > 0){
        raiz->dir = RotacaoDireita(raiz->dir);
        return RotacaoEsquerda(raiz);
    }
    return raiz;
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

void salvarPacientesNoArquivo(PACIENTE *pacientes, int numero_pacientes, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo %s para escrita\n", nome_arquivo);
        return;
    }

    for (int i = 0; i < numero_pacientes; i++) {
        fprintf(arquivo, "<%s, %c, %02d/%02d/%04d, %02d/%02d/%04d>\n",
                pacientes[i].nome, pacientes[i].genero,
                pacientes[i].nascimento.dia, pacientes[i].nascimento.mes, pacientes[i].nascimento.ano,
                pacientes[i].ultima_consulta.dia, pacientes[i].ultima_consulta.mes, pacientes[i].ultima_consulta.ano);
    }
    fclose(arquivo);
    printf("Arquivo %s atualizado com sucesso!\n", nome_arquivo);
}

void CadastrarPaciente(PACIENTE **pacientes, int *numero_pacientes, AVL **raiz, const char *nome_arquivo) {
    *pacientes = (PACIENTE *)realloc(*pacientes, (*numero_pacientes + 1) * sizeof(PACIENTE));
    if (*pacientes == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }

    PACIENTE novoPaciente;

    printf("Digite o nome do paciente: ");
    scanf(" %[^\n]", novoPaciente.nome);
    printf("Digite o gênero do paciente (M/F): ");
    scanf(" %c", &novoPaciente.genero);
    printf("Digite a data de nascimento (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &novoPaciente.nascimento.dia, &novoPaciente.nascimento.mes, &novoPaciente.nascimento.ano);
    printf("Digite a data da última consulta (dd/mm/yyyy): ");
    scanf("%d/%d/%d", &novoPaciente.ultima_consulta.dia, &novoPaciente.ultima_consulta.mes, &novoPaciente.ultima_consulta.ano);

    (*pacientes)[*numero_pacientes] = novoPaciente;
    (*numero_pacientes)++;

    if (novoPaciente.genero == 'F') {
        *raiz = Inserir(*raiz, novoPaciente);
    }

    salvarPacientesNoArquivo(*pacientes, *numero_pacientes, nome_arquivo);

    printf("Paciente adicionado com sucesso!\n");
}

void AlterarCadastroPaciente(PACIENTE *pacientes, int numero_pacientes, AVL **raiz, const char *nome_arquivo) {
    char nome[41];
    printf("Digite o nome do paciente: ");
    scanf(" %[^\n]", nome);
    for (int i = 0; i < numero_pacientes; i++) {
        if (strcmp(pacientes[i].nome, nome) == 0) {

            if(pacientes[i].genero == 'F'){
                *raiz = Remover(*raiz, nome);
            }

            printf("Digite o novo nome do paciente: ");
            scanf(" %[^\n]", pacientes[i].nome);
            printf("Digite o novo gênero do paciente (M/F): ");
            scanf(" %c", &pacientes[i].genero);
            printf("Digite a nova data de nascimento (dd/mm/yyyy): ");
            scanf("%d/%d/%d", &pacientes[i].nascimento.dia, &pacientes[i].nascimento.mes, &pacientes[i].nascimento.ano);
            printf("Digite a nova data da última consulta (dd/mm/yyyy): ");
            scanf("%d/%d/%d", &pacientes[i].ultima_consulta.dia, &pacientes[i].ultima_consulta.mes, &pacientes[i].ultima_consulta.ano);
            
            if(pacientes[i].genero == 'F'){
                *raiz = Inserir(*raiz, pacientes[i]);
            }

            salvarPacientesNoArquivo(pacientes, numero_pacientes, nome_arquivo);

            printf("Cadastro do paciente alterado com sucesso!\n");
            return;
        }
    }
    printf("Paciente não encontrado.\n");
}


void gerarArquivoSaida(AVL *raiz) {
    FILE *arquivo_Liz = fopen("pacientes_Liz.txt", "w");
    if (arquivo_Liz == NULL) {
        printf("Erro ao abrir arquivo de texto\n");
        return;
    }

    emOrdem(raiz, arquivo_Liz);
    fclose(arquivo_Liz);
    printf("Arquivo pacientes_Liz.txt gerado com sucesso!\n");
}

void gerarArquivoSaidaMoises(LISTADUPLAMENTEENCADEADA *lista) {
    FILE *arquivo = fopen("pacientes_Moises.txt","w");
    NOLISTA *aux = lista->inicio;
    while (aux != NULL) {
        fprintf(arquivo, "<%s, %c, %02d/%02d/%04d, %02d/%02d/%04d>\n", aux->paciente.nome, aux->paciente.genero, aux->paciente.ultima_consulta.dia, aux->paciente.ultima_consulta.mes, aux->paciente.ultima_consulta.ano, aux->paciente.nascimento.dia, aux->paciente.nascimento.mes, aux->paciente.nascimento.ano);
        aux = aux->proximo;
    }

    fclose(arquivo);
    printf("Arquivo pacientes_Moises.txt gerado com sucesso!");
    return;
}

void MenuPaciente(PACIENTE **pacientes, int *numero_pacientes, DATA data_atual, AVL **raiz, const char *nome_arquivo, LISTADUPLAMENTEENCADEADA *lista) {
    int opcao;
    do {
        exibirMenuPaciente();
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                ConsultarPaciente(*pacientes, *numero_pacientes, data_atual);
                break;
            case 2:
                CadastrarPaciente(pacientes, numero_pacientes, raiz, nome_arquivo);
                break;
            case 3:
                AlterarCadastroPaciente(*pacientes, *numero_pacientes, raiz, nome_arquivo);
                break;
            case 4:
                return;
            case 5:
                gerarArquivoSaida(*raiz);
                gerarArquivoSaidaMoises(lista);
                exit(0);
            default:
                printf("Opção inválida\n");
        }
    } while (true);
}

void CadastraPacienteMoises(PACIENTE **pacientes, int *numero_pacientes, LISTADUPLAMENTEENCADEADA *lista) {
    *pacientes = (PACIENTE *)realloc(*pacientes, (*numero_pacientes + 1) * sizeof(PACIENTE));
    if (*pacientes == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }

    PACIENTE novoPaciente;

    printf("Digite o nome do paciente: ");
    scanf(" %[^\n]", novoPaciente.nome);
    printf("Digite o gênero do paciente (M/F): ");
    do {
        scanf(" %c", &novoPaciente.genero);
        if (novoPaciente.genero != 'F' && novoPaciente.genero != 'M') {
            printf("\nValor inválido, insira um valor válido!\n");
        }
        setbuf(stdin,NULL);
    } while (novoPaciente.genero != 'F' && novoPaciente.genero != 'M');
    printf("Digite a data de nascimento (dd/mm/yyyy): ");
    do {
        scanf("%d/%d/%d", &novoPaciente.nascimento.dia, &novoPaciente.nascimento.mes, &novoPaciente.nascimento.ano);
        if (novoPaciente.nascimento.dia < 0 || novoPaciente.nascimento.dia > 31 ||novoPaciente.nascimento.dia < 0 || novoPaciente.nascimento.mes > 12) {
            printf("\nValor inválido, insira um valor válido!\n");
        }
        setbuf(stdin,NULL);
    } while (novoPaciente.nascimento.dia < 0 || novoPaciente.nascimento.dia > 31 ||novoPaciente.nascimento.dia < 0 || novoPaciente.nascimento.mes > 12);
    
    printf("Digite a data da última consulta (dd/mm/yyyy): ");
    do {
        scanf("%d/%d/%d", &novoPaciente.nascimento.dia, &novoPaciente.nascimento.mes, &novoPaciente.nascimento.ano);
        if (novoPaciente.ultima_consulta.dia < 0 || novoPaciente.ultima_consulta.dia > 31 ||novoPaciente.ultima_consulta.dia < 0 || novoPaciente.ultima_consulta.mes > 12) {
            printf("\nValor inválido, insira um valor válido!\n");
        }
        setbuf(stdin,NULL);
    } while (novoPaciente.ultima_consulta.dia < 0 || novoPaciente.ultima_consulta.dia > 31 ||novoPaciente.ultima_consulta.dia < 0 || novoPaciente.ultima_consulta.mes > 12);

    (*pacientes)[*numero_pacientes] = novoPaciente;
    (*numero_pacientes)++;

    if (novoPaciente.genero == 'M') {
        InsereElemento(lista, novoPaciente);
    }

    printf("Paciente adicionado com sucesso!\n");
    return;
}

void AlterarCadastroPacienteMoises(LISTADUPLAMENTEENCADEADA *lista) {
    char nome[41];
    int opcao;
    printf("\nInsira o nome do paciente a ser alterado:");
    scanf(" %[^\n]", nome);
    NOLISTA *aux = lista->inicio;
    while (aux != NULL && strcmp(aux->paciente.nome, nome) != 0) {
        aux = aux->proximo;
    }
    if (aux == NULL) {
        printf("\nNome não encontrado\n");
        return;
    } else {
        printf("Nome localizado, o que deseja alterar?\n");
        while (opcao < 1 || opcao > 4) {
            printf(
            "1: Nome\n"
            "2: Genero\n"
            "3: Ultima consulta\n"
            "4: Nascimento\n");
            scanf("%d", &opcao);
            if (opcao < 1 || opcao > 4) {
                printf("Valor inválido, insira um avlor válido\n");
            }
        }
        switch (opcao) {
        case 1:
            printf("Novo nome: ");
            scanf("%[^\n]", aux->paciente.nome);
            printf("Nome Alterado para %s", aux->paciente.nome);
            break;
        case 2: 
            if (aux->paciente.genero == 'F') {
                aux->paciente.genero = 'M';
            } else {
                aux->paciente.genero = 'F';
            }
            printf("Genero Alterado para %c", aux->paciente.genero);
            break;
        case 3:
            printf("Data da nova consulta (dd/mm/yyyy): ");
            scanf("%d/%d/%d", &aux->paciente.ultima_consulta.dia, &aux->paciente.ultima_consulta.mes, &aux->paciente.ultima_consulta.ano);
            printf("Data alterada para %d/%d/%d", aux->paciente.ultima_consulta.dia, aux->paciente.ultima_consulta.mes, aux->paciente.ultima_consulta.ano);
            break;
        case 4: 
            printf("Nova data de nascimento (dd/mm/yyyy): ");
            scanf("%d/%d/%d", &aux->paciente.nascimento.dia, &aux->paciente.nascimento.mes, &aux->paciente.nascimento.ano);
            printf("Data alterada para %d/%d/%d", aux->paciente.nascimento.dia, aux->paciente.nascimento.mes, aux->paciente.nascimento.ano);
        default:
            break;
        }
        
    }
    return;
}



void MenuPacienteMoises(PACIENTE **pacientes, int *numero_pacientes, DATA data_atual, LISTADUPLAMENTEENCADEADA *lista) {
    int opcao;
    do {
        exibirMenuPaciente();
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                ConsultarPaciente(*pacientes, *numero_pacientes, data_atual);
                break;
            case 2:
                CadastraPacienteMoises(pacientes, numero_pacientes, lista);
                break;
            case 3:
                AlterarCadastroPacienteMoises(lista);
                break;
            case 4:
                return;
            case 5:
                gerarArquivoSaidaMoises(lista);
                exit(0);
            default:
                printf("Opção inválida\n");
        }
    } while (true);
}

void liberaAVL(AVL *raiz) {
    if (raiz != NULL) {
        liberaAVL(raiz->esq);
        liberaAVL(raiz->dir);
        free(raiz);
    }
}

void liberaLista(LISTADUPLAMENTEENCADEADA *lista) {
    NOLISTA *aux = lista->inicio;
    while (aux != NULL) {
        NOLISTA *temp = aux;
        aux = aux->proximo;
        free(temp);
    }
}


int main(int argc, char *argv[]) {
    AVL *raiz = NULL;
    PACIENTE *pacientes = NULL; 
    int numero_pacientes = 0;
    DATA data_atual = RecebeDataAtual();
    LISTADUPLAMENTEENCADEADA lista;

    InicializaLista(&lista);

    if (argc < 2) {
        printf("Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    const char *nome_arquivo = argv[1];

    FILE *arquivo_entrada = fopen(nome_arquivo, "r");
    if (arquivo_entrada == NULL) {
        printf("Erro ao abrir arquivo de texto, saindo do programa\n");
        return 1;
    }

    PACIENTE temp;
    while (fscanf(arquivo_entrada, "<%[^,], %c, %d/%d/%d, %d/%d/%d>\n", temp.nome, &temp.genero, &temp.nascimento.dia, &temp.nascimento.mes, &temp.nascimento.ano, &temp.ultima_consulta.dia, &temp.ultima_consulta.mes, &temp.ultima_consulta.ano) == 8) {
        pacientes = (PACIENTE *)realloc(pacientes, (numero_pacientes + 1) * sizeof(PACIENTE));
        if (pacientes == NULL) {
            printf("Erro ao alocar memória!\n");
            exit(1);
        }
        pacientes[numero_pacientes] = temp;
        if (temp.genero == 'F') {
            raiz = Inserir(raiz, temp);
        }
        if (temp.genero == 'M') {
            InsereElemento(&lista, temp);
        }
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
                MenuPaciente(&pacientes, &numero_pacientes, data_atual, &raiz, nome_arquivo, &lista);
                break;
            case 2:
                printf("Pacientes do Moisés:\n");
                MenuPacienteMoises(&pacientes, &numero_pacientes, data_atual, &lista);
                break;
            case 3:
                gerarArquivoSaida(raiz);
                gerarArquivoSaidaMoises(&lista);
                free(pacientes); 
                printf("Encerrando o programa...\n");
                exit(0);
            default:
                printf("Opção inválida\n");
        }
    } while (true);

    liberaAVL(raiz);
    liberaLista(&lista);

    return 0;
}