#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#define MAX_INDICES 1000


typedef struct nascimento
{
    int dia;
    int mes;
    int ano;
} data;

typedef struct dados_paciente
{
    /* DADOS PARA MANIPULAÇÃO DA FILA */
    struct dados_paciente *anterior;
    struct dados_paciente *proximo;


    /* DADOS PESSOAIS */
    char nome[60];
    data datanasc;
    char sexo;
    long long int telefone;
    long long int cpf;
    char email[100];


    /* CONDIÇÕES FÍSICAS */
    float peso;
    float altura;
    char tipo_sanguineo[4];

} paciente;


typedef struct Fila
{
    paciente *inicio;
    paciente *fim;
} fila;


typedef struct busca
{
    char nome[60];
    paciente *posicao_paciente;


} NomeBusca;

NomeBusca lista_indices[MAX_INDICES];
int indice_paciente = 0;

fila *cria_agenda();
int fila_vazia(fila *f);
void ler_caracteres(char *string, char *string2);
int bissexto(int ano);
int verifica_sangue(char *sangue);
void insere_paciente(fila *f);
int remove_paciente(fila *f);
paciente *buscando_nome(char *nome_busca);
void procurar_paciente();
void navegar_agenda(fila *f);
void mostra_agenda(fila *f);
void libera_pacientes(fila *f);
void salvar_agenda(fila *f);
void salvar_em_txt(paciente *inicio);
void salvar_em_csv(paciente *inicio);
void salvar_em_xml(paciente *inicio);

void exibir_cadastro(paciente *aux);


void limpa_tela()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void limpa_buffer()
{
    int vassoura;
    while ((vassoura = getchar()) != '\n' && vassoura != EOF)
        ;
}


fila *cria_agenda()
{
    fila *f = (fila *)malloc(sizeof(fila));
    if (!f)
        exit(1);
    else
    {
        f->inicio = NULL;
        f->fim = NULL;
    }
    return f;
}


int fila_vazia(fila *f)
{
    /*if (f == NULL)
        return 1; // fila não inicializada*/
    if (f->inicio == NULL)
        return 1; // início da fila aponta para NULL
    else
        return 0;
}


void exibir_cadastro(paciente *aux)
{
    printf("\n******************************************************************************");
    printf("\n");
    printf("Nome completo: %s\n", aux->nome);
    printf("Data de nascimento: %d/%d/%d\n", aux->datanasc.dia, aux->datanasc.mes, aux->datanasc.ano);
    printf("Sexo: %c\n", aux->sexo);
    printf("Telefone: %lld\n", aux->telefone);
    printf("CPF: %lld\n", aux->cpf);
    printf("E-mail: %s\n", aux->email);
    printf("Peso: %.2f\n", aux->peso);
    printf("Altura: %.2f\n", aux->altura);
    printf("Tipo sanguíneo: %s\n", aux->tipo_sanguineo);
    printf("\n******************************************************************************\n\n");
}


void ler_caracteres(char *string, char *string2)
{
    printf("\n%s", string2);
    ;
    // getchar();
    fgets(string, 101, stdin);
    string[strcspn(string, "\n")] = '\0';
}


paciente *alocar(char *nome, int dia, int mes, int ano, char sexo, long long int telefone, long long int cpf, 
                 char *email, float peso, float altura, char *sangue, paciente *antecessor)
{
    paciente *aux = (paciente *)malloc(sizeof(paciente));
    if (!aux)
        exit(1);
    else
    {
        strcpy(aux->nome, nome);
        aux->datanasc.dia = dia;
        aux->datanasc.mes = mes;
        aux->datanasc.ano = ano;
        aux->sexo = sexo;
        aux->telefone = telefone;
        aux->cpf = cpf;
        strcpy(aux->email, email);
        aux->peso = peso;
        aux->altura = altura;
        strcpy(aux->tipo_sanguineo, sangue);


        aux->proximo = NULL;
        aux->anterior = antecessor;


        return aux;
    }
}


int bissexto(int ano) {
    return (ano % 400 == 0) || (ano % 4 == 0 && ano % 100 != 0);
}

int verifica_sangue(char *sangue){
    if (strcmp(sangue, "A+") != 0 && strcmp(sangue, "A-") != 0 &&
        strcmp(sangue, "B+") != 0 && strcmp(sangue, "B-") != 0 &&
        strcmp(sangue, "O+") != 0 && strcmp(sangue, "O-") != 0 &&
        strcmp(sangue, "AB+") != 0 && strcmp(sangue, "AB-") != 0)
        return 1;
    else
        return 0;
}


void insere_paciente(fila *f)
{
    char nome[61];
    limpa_buffer();
    ler_caracteres(nome, "Digite o nome completo do paciente: ");


    int dia, mes, ano;
    int data_valida = 0;


    // ======== VALIDAÇÃO DE DATA ========
    while (!data_valida) {
        printf("\nDigite a data de nascimento do paciente [DD MM AAAA]: ");
        scanf("%d %d %d", &dia, &mes, &ano);
        limpa_buffer();


        if (ano < 1900 || ano > 2025) {
            printf("Ano inválido! Digite um ano entre 1900 e 2025.\n");
            continue;
        }


        if (mes < 1 || mes > 12) {
            printf("Mês inválido! Digite um mês entre 1 e 12.\n");
            continue;
        }


        int dias_mes;
        switch (mes) {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                dias_mes = 31;
                break;
            case 4: case 6: case 9: case 11:
                dias_mes = 30;
                break;
            case 2:
                dias_mes = bissexto(ano) ? 29 : 28; 
                // funciona como se fosse um if (bissexto(ano)) return 29, else if return 28
                break;
        }


        if (dia < 1 || dia > dias_mes) {
            printf("Dia inválido! Esse mês tem no máximo %d dias.\n", dias_mes);
            continue;
        }


        data_valida = 1; // Sai do laço se tudo estiver correto
    }


    // ======== VALIDAÇÃO DE SEXO ========
    char sexo;
    int sexo_valido = 0;


    while (!sexo_valido) {
        printf("\nQual o sexo do paciente? [M/F]: ");
        scanf(" %c", &sexo);
        limpa_buffer();


        // Converter para maiúsculo (caso o usuário digite 'm' ou 'f')
        sexo = toupper(sexo);


        if (sexo == 'M' || sexo == 'F') {
            sexo_valido = 1; // Válido!
        } else {
            printf("Opção inválida! Digite apenas 'M' para Masculino ou 'F' para Feminino.\n");
        }
    }


    // ======== RESTANTE DO CADASTRO ========
    long long int telefone;
    printf("\nDigite o telefone do paciente: ");
    scanf("%lld", &telefone);
    limpa_buffer();


    long long int cpf;
    printf("\nDigite o CPF do paciente: ");
    scanf("%lld", &cpf);
    limpa_buffer();


    char email[101];
    ler_caracteres(email, "Digite o e-mail do paciente: ");


    float peso;
    printf("\nDigite o peso do paciente (kg): ");
    scanf("%f", &peso);
    limpa_buffer();


    float altura;
    printf("\nDigite a altura do paciente (m): ");
    scanf("%f", &altura);
    limpa_buffer();


    char sangue[4];
    do{
        ler_caracteres(sangue, "Digite o tipo sanguíneo (Ex: A+): ");
        if (verifica_sangue(sangue))
        {
            printf("\nTipo sanguíneo inexistente.\n");
        }

    } while (verifica_sangue(sangue));


    paciente *aux = alocar(nome, dia, mes, ano, sexo, telefone, cpf, email, peso, altura, sangue, f->fim);


    if (!aux)
        exit(1);
    if (f->inicio == NULL)
        f->inicio = aux;
    else
        f->fim->proximo = aux;


    f->fim = aux;


    if (indice_paciente < MAX_INDICES)
    {
        strcpy(lista_indices[indice_paciente].nome, aux->nome);
        lista_indices[indice_paciente].posicao_paciente = aux;
        indice_paciente++;
    }
    else
    {
        printf("Lista de busca cheia!\n");
    }


    printf("\nPaciente cadastrado com sucesso!\n");
}

paciente *buscando_nome(char *nome_busca)
{

    for (int i = 0; i < indice_paciente; i++)
    {
        if (strcmp(lista_indices[i].nome, nome_busca) == 0)
        {
            return lista_indices[i].posicao_paciente;
        }
    }
    return NULL;
}


void procurar_paciente()
{
    char nome_busca[60];
    paciente *identificar_paciente;
    limpa_buffer();


    ler_caracteres(nome_busca, "Digite o nome do paciente para buscar: ");


    identificar_paciente = buscando_nome(nome_busca);


    if (identificar_paciente != NULL)
    {
        printf("\nPaciente Encontrado:");
        exibir_cadastro(identificar_paciente);
    }
    else
    {
        printf("\nPaciente com o nome '%s' nao foi encontrado na lista.\n", nome_busca);
    }
}


int remove_paciente(fila *f)
{
    if (fila_vazia(f))
    {
        printf("Lista vazia! Impossível fazer a remoção.\n\n");
        return 0;
    }
    paciente *aux = f->inicio;
    printf("Paciente removido da fila: %s\n", aux->nome);

    // Precisamos remover o paciente também do array de índice
    int indice_encontrado = -1;
    // 1. Encontra a posição do paciente no array de índice
    for (int i = 0; i < indice_paciente; i++)
    {
        // Compara o ponteiro de memória
        if (lista_indices[i].posicao_paciente == aux)
        {
            indice_encontrado = i;
            break;
        }
    }
    // 2. Se encontrou, desloca os elementos seguintes para cobrir o espaço
    if (indice_encontrado != -1)
    {
        for (int i = indice_encontrado; i < indice_paciente - 1; i++)
        {
            lista_indices[i] = lista_indices[i + 1];
        }
        indice_paciente--; // Diminui o contador total de índices
    }

    f->inicio = f->inicio->proximo;
    if (f->inicio == NULL)
    {
        f->fim = NULL;
    }
    else
    {
        f->inicio->anterior = NULL;
    }
    free(aux);
    aux = NULL;
    return 1;
}


void navegar_agenda(fila *f)
{
    int opcao, continua = -1;
    if (fila_vazia(f))
    {
        printf("Lista vazia!\n\n");
        return;
    }
    paciente *elemento = f->inicio;
    exibir_cadastro(elemento);
    do
    {
        printf("\n Percorrer Fila;\n");
        printf("  0 Sair;\n");
        printf("  1 Proximo;\n");
        printf("  2 Anterior;\n");
        scanf("%d", &opcao);
        limpa_tela();
        switch (opcao)
        {
        case 0:
            continua = 0;
            break;
        case 1:
            if (elemento->proximo != NULL)
            {
                elemento = elemento->proximo;
                exibir_cadastro(elemento);
            }
            else
            {
                printf("FIM DA FILA!\n");
                exibir_cadastro(elemento);
            }
            break;
        case 2:
            if (elemento->anterior != NULL)
            {
                elemento = elemento->anterior;
                exibir_cadastro(elemento);
            }
            else
            {
                printf("INÍCIO DA FILA!\n");
                exibir_cadastro(elemento);
            }
            break;
        default:
            printf(" Opção Inválida!\n");
        }
    } while (continua);
}


void mostra_agenda(fila *f)
{
    if (fila_vazia(f))
    {
        printf(" Lista vazia!\n\n");
        return;
    }
    paciente *elemento = f->inicio;
    exibir_cadastro(elemento);
    while (elemento->proximo != NULL)
    {
        elemento = elemento->proximo;
        exibir_cadastro(elemento);
    }
}


// ====== FUNÇÃO PARA SALVAR EM TXT ======
void salvar_em_txt(paciente *inicio)
{
    // Abre o arquivo "pacientes.txt" no modo escrita
    // Se o arquivo não existir, ele será criado. Se existir, será sobrescrito.
    FILE *arquivo = fopen("pacientes.txt", "w");
    if (arquivo == NULL)
    { // Verifica se o arquivo abriu corretamente
        printf("Erro ao abrir o arquivo.\n");
        return;
    }


    paciente *aux = inicio; // Ponteiro que percorre a lista
    while (aux != NULL)
    { // Enquanto houver pacientes na lista
        fprintf(arquivo, "\n******************************************************************************");
        fprintf(arquivo, "\n");
        fprintf(arquivo, "Nome completo: %s\n", aux->nome);
        fprintf(arquivo, "Data de nascimento: %d/%d/%d\n", aux->datanasc.dia, aux->datanasc.mes, aux->datanasc.ano);
        fprintf(arquivo, "Sexo: %c\n", aux->sexo);
        fprintf(arquivo, "Telefone: %lld\n", aux->telefone);
        fprintf(arquivo, "CPF: %lld\n", aux->cpf);
        fprintf(arquivo, "E-mail: %s\n", aux->email);
        fprintf(arquivo, "Peso: %.2f Kg\n", aux->peso);
        fprintf(arquivo, "Altura: %.2f m\n", aux->altura);
        fprintf(arquivo, "Tipo sanguíneo: %s\n", aux->tipo_sanguineo);
        fprintf(arquivo, "\n******************************************************************************\n\n");


        aux = aux->proximo;
    }


    fclose(arquivo); // Fecha o arquivo após a escrita (tipo um “fimprocedimento”)
    printf("Lista salva com sucesso em pacientes.txt\n");
}


// ====== FUNÇÃO PARA SALVAR EM CSV ======
void salvar_em_csv(paciente *inicio)
{
    // Abre o arquivo "pacientes.csv" no modo escrita
    FILE *arquivo = fopen("pacientes.csv", "w");
    if (arquivo == NULL)
    { // Verifica se o arquivo abriu corretamente.
        printf("Erro ao abrir o arquivo.\n");
        return;
    }


    // Cabeçalho do CSV (entre vírgulas):
    fprintf(arquivo, "nome;dia;mes;ano;sexo;telefone;cpf;email;peso;altura;tipo sanguineo\n");


    paciente *aux = inicio; // Ponteiro que percorre a lista
    while (aux != NULL)
    { // Enquanto houver pacientes
        // Escreve os dados no formato CSV: cada paciente em uma linha
        fprintf(arquivo, "%s;%02d;%02d;%04d;%c;%lld;%lld;%s;%f;%f;%s\n",
                aux->nome, aux->datanasc.dia, aux->datanasc.mes, aux->datanasc.ano, aux->sexo, 
                aux->telefone, aux->cpf, aux->email, aux->peso, aux->altura, aux->tipo_sanguineo);
        aux = aux->proximo; // Avança para o próximo paciente
    }


    fclose(arquivo); // Fecha o arquivo após a escrita (tipo um “fimprocedimento”)
    printf("Lista salva com sucesso em pacientes.csv\n");
}


// ====== FUNÇÃO PARA SALVAR EM XML ======
void salvar_em_xml(paciente *inicio)
{
    // Abre o arquivo "pacientes.xml" no modo escrita ("w")
    FILE *arquivo = fopen("pacientes.xml", "w");
    if (arquivo == NULL)
    { // Verifica se o arquivo abriu corretamente
        printf("Erro ao abrir o arquivo.\n");
        return;
    }


    fprintf(arquivo, "<pacientes>\n"); // Tag raiz do XML (tem tags que nem o html)


    paciente *aux = inicio; // Ponteiro que percorre a lista
    while (aux != NULL)
    {                                                             // Enquanto houver pacientes
        fprintf(arquivo, "    <paciente>\n");                     // Tag de abertura do paciente
        fprintf(arquivo, "        <nome>%s</nome>\n", aux->nome); // Nome
        // Tag de data de nascimento com sub-tags para dia, mês e ano
        fprintf(arquivo, "        <datanasc>\n");
        fprintf(arquivo, "            <dia>%02d</dia>\n", aux->datanasc.dia); // formata certinho
        fprintf(arquivo, "            <mes>%02d</mes>\n", aux->datanasc.mes);
        fprintf(arquivo, "            <ano>%04d</ano>\n", aux->datanasc.ano);
        fprintf(arquivo, "        </datanasc>\n");
        fprintf(arquivo, "        <sexo>%c</sexo>\n", aux->sexo);
        fprintf(arquivo, "        <telefone>%lld</telefone>\n", aux->telefone);
        fprintf(arquivo, "        <cpf>%lld</cpf>\n", aux->cpf);
        fprintf(arquivo, "        <email>%s</email>\n", aux->email);
        fprintf(arquivo, "        <peso>%f</peso>\n", aux->peso);
        fprintf(arquivo, "        <altura>%f</altura>\n", aux->altura);
        fprintf(arquivo, "    </paciente>\n"); // Fecha a tag do paciente
        aux = aux->proximo;                    // Avança para o próximo paciente
    }


    fprintf(arquivo, "</pacientes>\n"); // Fecha a tag raiz
    fclose(arquivo);                    // Fecha o arquivo após a escrita (tipo um “fimprocedimento”)
    printf("Lista salva com sucesso em pacientes.xml\n");
}


// ====== FUNÇÃO PARA O MENU DO SALVAMENTO ======
void salvar_agenda(fila *f)
{
    // Verifica se a lista está vazia antes de tentar salvar
    if (fila_vazia(f))
    {
        printf("Fila vazia! Nada a salvar.\n\n");
        return;
    }


    int op_salvar;
    do
    {
        // Menu para o usuário escolher o formato de arquivo
        printf("\nEscolha o formato de arquivo para salvar:\n");
        printf("[1] TXT\n");
        printf("[2] CSV\n");
        printf("[3] XML\n");
        printf("[0] Voltar ao menu\n");
        printf("Opção: ");
        scanf("%d", &op_salvar);
        limpa_buffer(); // Limpa o buffer do teclado para evitar problemas com fgets ou scanf


        // Executa a ação de acordo com a escolha do usuário
        switch (op_salvar)
        {
        case 1:
            salvar_em_txt(f->inicio); // Chama função para salvar em TXT
            break;
        case 2:
            salvar_em_csv(f->inicio); // Chama função para salvar em CSV
            break;
        case 3:
            salvar_em_xml(f->inicio); // Chama função para salvar em XML
            break;
        case 0:
            printf("Voltando ao menu...\n"); // Sai sem salvar
            break;
        default:
            printf("Opção inválida! Tente novamente.\n"); // Validação de entrada
            break;
        }
    } while (op_salvar != 0); // Mantém o menu até o usuário escolher voltar
}


void libera_pacientes(fila *f)
{
    paciente *aux = f->inicio;
    do
    {
        remove_paciente(f);
    } while (!fila_vazia(f));
}


void menu()
{
    printf("----- LISTA DE PACIENTES -----\n\n");
    printf("O que deseja fazer?\n\n");
    printf("[1] Inserir paciente\n");
    printf("[2] Remover paciente\n");
    printf("[3] Navegar na lista\n");
    printf("[4] Buscar nome na lista\n");
    printf("[5] Exibir lista\n");
    printf("[6] Salvar lista em arquivo\n");
    printf("[7] Sair\n");
}


int main()
{
    setlocale(LC_ALL, "Portuguese");
    int opcao;
    fila *f = cria_agenda();
    menu();
    scanf("%d", &opcao);
    limpa_tela();
    while (opcao != 7)
    {
        switch (opcao)
        {
        case 1:
            insere_paciente(f);
            break;


        case 2:
            remove_paciente(f);
            break;


        case 3:
            navegar_agenda(f);
            break;


        case 4:
            procurar_paciente();
            break;


        case 5:
            mostra_agenda(f);
            break;


        case 6:
            salvar_agenda(f);
            break;


        default:
            printf("Opção inválida!\n");
            break;
        }
        menu();
        scanf("%d", &opcao);
        limpa_tela();
    }
    libera_pacientes(f);
    free(f);
    limpa_tela();
    printf("Até logo!");
    return 0;
}