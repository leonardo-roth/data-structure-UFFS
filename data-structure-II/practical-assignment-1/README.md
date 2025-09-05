# Sistema de Registros de Alunos em Arquivo Binário

Este projeto foi desenvolvido para a disciplina de **Estrutura de Dados II** do curso de Ciência da Computação da **Universidade Federal da Fronteira Sul (UFFS)**. Trata-se de um sistema simples de gerenciamento de alunos que opera diretamente sobre um arquivo binário, permitindo a manipulação de registros de forma eficiente.

O sistema implementa as operações básicas de um CRUD (Create, Read, Update, Delete) com foco em performance, evitando a reescrita desnecessária de todo o arquivo ao realizar modificações pontuais.

## Como Compilar e Executar

### Pré-requisitos
- Compilador C (GCC).

### Compilação
Abra o terminal na pasta raiz do projeto e execute o seguinte comando para compilar todos os arquivos-fonte e gerar o executável:

```bash
gcc main.c arquivo_utils.c operacoes_alunos.c -o sistema_de_alunos
```

### Execução
Após a compilação, execute o programa com o comando:

```bash
./sistema_de_alunos
```

O programa irá criar automaticamente o arquivo `alunos.bin` e apresentar um menu interativo para o usuário.

## Autor

**Leonardo Vitor Roth**
- **Curso:** Ciência da Computação
- **Universidade:** Universidade Federal da Fronteira Sul (UFFS)
- **Disciplina:** Estrutura de Dados II
