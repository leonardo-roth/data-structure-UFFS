#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// incluindo arquivos necessários
#include "operacoes_alunos.h"
#include "arquivo_utils.h"
#include "aluno.h"

void create_new_student()
{
    // ab = append binary (cria o arquivo se não existir, escreve no final)
    FILE *arquivo = fopen(NOME_ARQUIVO, "ab");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir arquivo para atualização");
        // sai da função create_new_student se o arquivo não abrir
        return;
    }

    // variáveis para criação do novo estudante
    int id;
    int ativo = 1; // ao criar um novo estudante, ele sempre será ativo
    float media;
    char nome[TAMANHO_NOME];

    // recebendo os valores do usuário
    printf("Escreva o ID do estudante: ");
    scanf("%d", &id);
    // consome o \n deixado pelo scanf e limpa o buffer
    getchar();

    printf("Escreva o nome do estudante: ");
    // função para ler uma linha inteira do teclado. stdin = standard input (teclado)
    fgets(nome, TAMANHO_NOME, stdin);
    // substitui o \n por \0 (\o sempre fica no final de uma string)
    nome[strcspn(nome, "\n")] = '\0';

    printf("Escreva a média do estudante (0 a 10): ");
    scanf("%f", &media);
    printf("\n");

    // atribuindo os valores lidos para a struct estudante
    Aluno aluno;
    aluno.id = id;
    aluno.ativo = ativo;
    aluno.media = media;
    // função para armazenar o nome corretamente
    strcpy(aluno.nome, nome);

    /*
    fwrite grava dados binários em um arquivo.
    fwrite(ponteiro para os dados, tamanho dos dados, numero de elementos, ponteiro para o arquivo aberto)
    */
    if (fwrite(&aluno, sizeof(Aluno), 1, arquivo))
    // se conseguiu escrever, informa sucesso e imprime o estudante criado
    {
        printf("Estudante criado com sucesso.\n\n");
        printf("Id: %d\nNome: %s\nMédia: %.2f\n\n", aluno.id, aluno.nome, aluno.media);
    }
    else // se não, informa falha
    {
        printf("Falha ao criar estudante.\n\n");
    }
    fclose(arquivo);
}

void print_all_students()
{
    // rb = read binary
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo para leitura");
        return;
    }

    // cria um aluno temporário para percorrer o arquivo
    Aluno aluno_temp;
    printf("Lista de alunos matriculados: \n\n");

    /*
    fread lê dados binários de um arquivo
    fread(ponteiro onde guardamos os valores, tamanho de cada elemento, quantidade de elementos a serem lidos por vez, ponteiro para o arquivo aberto)
    */
    while (fread(&aluno_temp, sizeof(Aluno), 1, arquivo))
    // enquanto fread conseguir ler um estudante
    {
        // se o aluno estiver ativo (ativo == 1)
        if (aluno_temp.ativo == 1)
        {
            // imprime as informações do aluno
            printf("ID: %d, Nome: %s, Média: %.2f\n", aluno_temp.id, aluno_temp.nome, aluno_temp.media);
        }
    }
    printf("\n");
    // fecha o arquivo após as operações
    fclose(arquivo);
}

// função auxiliar para pegar a posição de um aluno no arquivo (útil para manipulação)
int get_student_position(int id)
{
    // read binary
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb");
    if (arquivo == NULL)
    {
        perror("Falha ao abrir arquivo.");
        return -1; // retorna -1 se o arquivo não foi aberto
    }

    // Declaramos um aluno para percorrer o arquivo e uma variável index para salvar sua posição.
    Aluno aluno;
    int index = 0;

    // enquanto fread conseguir ler estudantes
    while (fread(&aluno, sizeof(Aluno), 1, arquivo))
    {
        // se o id passado como parâmetro na função for igual ao id percorrido no bloco
        if (aluno.id == id)
        {
            // fecha o arquivo e retorna o index
            fclose(arquivo);
            return index;
        }
        // index incrementado após cada passada do while
        index++;
    }

    // fecha o arquivo e retorna -1 se não for encontrado estudantes com esse ID
    fclose(arquivo);
    return -1;
}

// função para deletar um estudante logicamente (ativo == 0)
int delete_student(int id)
{
    // read + (permissão de escrita) binary
    FILE *arquivo = fopen(NOME_ARQUIVO, "r+b");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo para leitura");
        return -1;
    }

    // utiliza a função auxiliar para pegar a posição do estudante
    int position = get_student_position(id);
    // se o arquivo não foi aberto ou o aluno com esse id não foi encontrado
    if (position == -1)
    {
        printf("Aluno com id %d não encontrado para remoção ou falha ao abrir arquivo\n", id);
        fclose(arquivo);
        return -1;
    }

    // posiciona o ponteiro na exata posição do aluno
    /*
    fseek(ponteiro para o arquivo aberto, posição do aluno, ponto de referência)
    position * sizeof(Aluno): Multiplicamos para achar a posição correta, se o aluno estiver na linha x, multiplica esse valor pela quantidade de bytes da struct, para iniciar exatamente na posição do aluno x.
    SEEK_SET -> começa do início do arquivo
    SEEK_CUR -> posição atual do cursor
    SEEK_END -> final do arquivo
    */
    fseek(arquivo, position * sizeof(Aluno), SEEK_SET);

    // declara um aluno temporário para gravar os dados do aluno a ser atualizado
    Aluno aluno;
    fread(&aluno, sizeof(Aluno), 1, arquivo);

    // Remove logicamente o aluno (ativo == 0)
    aluno.ativo = 0;

    // Reseta o ponteiro para o início da linha
    fseek(arquivo, position * sizeof(Aluno), SEEK_SET);
    // Sobscreve os dados antigos e atualiza desativando logicamente o aluno
    fwrite(&aluno, sizeof(Aluno), 1, arquivo);

    fclose(arquivo);

    // Informa sucesso e imprime o estudante atualizado.
    printf("\nAluno com ID %d removido com sucesso.\n\n", id);
    print_student_by_id(id);

    return 0;
}

// função auxiliar para imprimir um estudante pelo seu ID
void print_student_by_id(int id)
{
    // read binary
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo para leitura");
        return;
    }

    // utiliza a função auxiliar para pegar a posição do aluno e trata eventuais erros
    int position = get_student_position(id);
    if (position == -1)
    {
        printf("Aluno com id %d não encontrado para remoção ou falha ao abrir arquivo\n", id);
        fclose(arquivo);
        return;
    }

    // move o ponteiro para a exata linha do aluno a ser impresso
    fseek(arquivo, position * sizeof(Aluno), SEEK_SET);

    // cria um aluno temporário para armazenar os dados do aluno
    Aluno aluno;
    fread(&aluno, sizeof(Aluno), 1, arquivo);

    // imprime todos os dados do aluno
    printf("Id: %d\nNome: %s\nMédia: %.2f\nAtivo: %d\n", aluno.id, aluno.nome, aluno.media, aluno.ativo);
    printf("\n");

    fclose(arquivo);
}

// função para atualizar a média de um aluno
void update_student_media(int id, float media)
{
    // read (+ para permissão de escrita) binary
    FILE *arquivo = fopen(NOME_ARQUIVO, "r+b");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo para leitura");
        return;
    }

    // pega a posição correta do aluno e trata eventuais erros
    int position = get_student_position(id);
    if (position == -1)
    {
        printf("Aluno com id %d não encontrado para remoção ou falha ao abrir arquivo\n", id);
        fclose(arquivo);
        return;
    }

    // move o ponteiro para a posição correta
    fseek(arquivo, position * sizeof(Aluno), SEEK_SET);

    // cria um aluno temporário e copia os dados do aluno a ser atualizado, também salva a nova média
    Aluno aluno;
    fread(&aluno, sizeof(Aluno), 1, arquivo);
    aluno.media = media;

    // reseta o ponteiro para o início da linha e sobscreve os dados antigos com os dados atualizados
    fseek(arquivo, position * sizeof(Aluno), SEEK_SET);
    fwrite(&aluno, sizeof(Aluno), 1, arquivo);

    // fecha o arquivo, informa sucesso e imprime os dados atualizados do estudante
    fclose(arquivo);

    printf("\nAluno atualizado com sucesso.\n\n");
    print_student_by_id(id);
}
