#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct nascimento
{
    int dia;
    int mes;
    int ano;
} data;

typedef struct dados_paciente
{
    /* DADOS PARA MANIULAÇÃO DA FILA */
    struct dados_paciente *anterior;
    struct dados_paciente *proximo;

    /* DADOS PESSOAIS */
    char nome[60];
    data datanasc;
    char sexo;
    int telefone;
    int cpf;
    char email[40];

    /* CONDIÇÕES FÍSICAS */
    float peso;
    float altura;
    char tipo_sanguineo[3];
/*  char usa_remedio[20];
    char fuma; // respostas: s ou n
    char bebe; // respostas: s ou n
    char doenca_cronica[20];
*/
} paciente;

typedef struct Fila
{
    paciente *inicio;
    paciente *fim;
} fila;

void limpa_tela()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void menu()
{
    printf("----- AGENDA DE CLIENTES -----\n\n");
    printf("O que deseja fazer?\n\n");
    printf("[1] Inserir paciente\n");
    printf("[2] Remover paciente\n");
    printf("[3] Navegar na agenda\n");
    printf("[4] Buscar nome na agenda\n");
    printf("[5] Exibir agenda\n");
    printf("[6] Salvar agenda em arquivo\n");
    printf("[7] Sair\n");
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    int opcao;
    menu();
    scanf("%d", &opcao);
    limpa_tela();
    while (opcao != 7)
    {
        switch (opcao)
        {
        case 1:
            // insere_paciente();
            break;

        case 2:
            // remove_paciente();
            break;

        case 3:
            // navegar_agenda();
            break;

        case 4:
            // busca_nome();
            break;

        case 5:
            // mostra_agenda();
            break;

        case 6:
            // salva_agenda();
            break;

        default:
            printf("Opção inválida!\n");
            break;
        }
        menu();
        scanf("%d", &opcao);
        limpa_tela();
    }

    return 0;
}