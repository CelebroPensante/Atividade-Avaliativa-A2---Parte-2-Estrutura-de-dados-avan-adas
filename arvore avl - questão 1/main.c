//-------------------------------------------------------
// Aluno: Vitor G. J. de Carvalho                       |
// RGM: 33046794                                        |
// Disciplina: Estruturas de Dados avancadas            |
// Professor: Marcelo T. Uemura                         |
// Data: 13/05/2025                                     |
//-------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar um usuário
typedef struct Usuario {
    char nome[50];               // Nome do usuário (chave para ordenação)
    int id;                      // ID único
    char email[50];              // E-mail do usuário
    int altura;                  // Altura do nó na árvore
    struct Usuario* esquerda;    // Ponteiro para o filho esquerdo
    struct Usuario* direita;     // Ponteiro para o filho direito
} Usuario;

// Função para criar um novo usuário
Usuario* criarUsuario(const char* nome, int id, const char* email) {
    Usuario* novoUsuario = (Usuario*)malloc(sizeof(Usuario));
    strcpy(novoUsuario->nome, nome);
    novoUsuario->id = id;
    strcpy(novoUsuario->email, email);
    novoUsuario->altura = 1;     // Novo nó é inicializado com altura 1
    novoUsuario->esquerda = NULL;
    novoUsuario->direita = NULL;
    return novoUsuario;
}

// Função para obter a altura de um nó
int altura(Usuario* no) {
    if (no == NULL)
        return 0;
    return no->altura;
}

// Função para calcular o fator de balanceamento de um nó
int fatorBalanceamento(Usuario* no) {
    if (no == NULL)
        return 0;
    return altura(no->esquerda) - altura(no->direita);
}

// Função para atualizar a altura de um nó
void atualizarAltura(Usuario* no) {
    int alturaEsq = altura(no->esquerda);
    int alturaDir = altura(no->direita);
    no->altura = (alturaEsq > alturaDir ? alturaEsq : alturaDir) + 1;
}

// Rotação simples à direita
Usuario* rotacaoDireita(Usuario* y) {
    Usuario* x = y->esquerda;
    Usuario* T2 = x->direita;

    // Realiza a rotação
    x->direita = y;
    y->esquerda = T2;

    // Atualiza alturas
    atualizarAltura(y);
    atualizarAltura(x);

    return x;
}

// Rotação simples à esquerda
Usuario* rotacaoEsquerda(Usuario* x) {
    Usuario* y = x->direita;
    Usuario* T2 = y->esquerda;

    // Realiza a rotação
    y->esquerda = x;
    x->direita = T2;

    // Atualiza alturas
    atualizarAltura(x);
    atualizarAltura(y);

    return y;
}

// Função para inserir um usuário na árvore AVL
Usuario* inserirUsuario(Usuario* raiz, Usuario* novoUsuario) {
    // 1. Inserção normal de BST
    if (raiz == NULL)
        return novoUsuario;

    int cmp = strcmp(novoUsuario->nome, raiz->nome);
    if (cmp < 0)
        raiz->esquerda = inserirUsuario(raiz->esquerda, novoUsuario);
    else if (cmp > 0)
        raiz->direita = inserirUsuario(raiz->direita, novoUsuario);
    else // Nomes iguais não são permitidos
        return raiz;

    // 2. Atualizar altura do nó ancestral
    atualizarAltura(raiz);

    // 3. Verificar o fator de balanceamento
    int balance = fatorBalanceamento(raiz);

    // Casos de desbalanceamento

    // Esquerda-Esquerda
    if (balance > 1 && strcmp(novoUsuario->nome, raiz->esquerda->nome) < 0)
        return rotacaoDireita(raiz);

    // Direita-Direita
    if (balance < -1 && strcmp(novoUsuario->nome, raiz->direita->nome) > 0)
        return rotacaoEsquerda(raiz);

    // Esquerda-Direita
    if (balance > 1 && strcmp(novoUsuario->nome, raiz->esquerda->nome) > 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }

    // Direita-Esquerda
    if (balance < -1 && strcmp(novoUsuario->nome, raiz->direita->nome) < 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// Função para encontrar o nó com menor valor (mais à esquerda)
Usuario* encontrarMinimo(Usuario* no) {
    Usuario* atual = no;
    while (atual->esquerda != NULL)
        atual = atual->esquerda;
    return atual;
}

// Função para remover um usuário da árvore AVL
Usuario* removerUsuario(Usuario* raiz, const char* nome) {
    // 1. Remoção normal de BST
    if (raiz == NULL)
        return raiz;

    int cmp = strcmp(nome, raiz->nome);
    if (cmp < 0)
        raiz->esquerda = removerUsuario(raiz->esquerda, nome);
    else if (cmp > 0)
        raiz->direita = removerUsuario(raiz->direita, nome);
    else {
        // Nó com apenas um filho ou sem filhos
        if ((raiz->esquerda == NULL) || (raiz->direita == NULL)) {
            Usuario* temp = raiz->esquerda ? raiz->esquerda : raiz->direita;

            // Caso sem filhos
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else // Caso com um filho
                *raiz = *temp; // Copia os dados do filho

            free(temp);
        } else {
            // Nó com dois filhos: pega o sucessor in-order (menor na subárvore direita)
            Usuario* temp = encontrarMinimo(raiz->direita);

            // Copia os dados do sucessor
            strcpy(raiz->nome, temp->nome);
            raiz->id = temp->id;
            strcpy(raiz->email, temp->email);

            // Remove o sucessor
            raiz->direita = removerUsuario(raiz->direita, temp->nome);
        }
    }

    // Se a árvore tinha apenas um nó, retorna
    if (raiz == NULL)
        return raiz;

    // 2. Atualizar altura do nó atual
    atualizarAltura(raiz);

    // 3. Verificar o fator de balanceamento
    int balance = fatorBalanceamento(raiz);

    // Casos de desbalanceamento

    // Esquerda-Esquerda
    if (balance > 1 && fatorBalanceamento(raiz->esquerda) >= 0)
        return rotacaoDireita(raiz);

    // Esquerda-Direita
    if (balance > 1 && fatorBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }

    // Direita-Direita
    if (balance < -1 && fatorBalanceamento(raiz->direita) <= 0)
        return rotacaoEsquerda(raiz);

    // Direita-Esquerda
    if (balance < -1 && fatorBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// Função para buscar um usuário pelo nome
Usuario* buscarUsuario(Usuario* raiz, const char* nome) {
    if (raiz == NULL)
        return NULL;

    int cmp = strcmp(nome, raiz->nome);
    if (cmp < 0)
        return buscarUsuario(raiz->esquerda, nome);
    else if (cmp > 0)
        return buscarUsuario(raiz->direita, nome);
    else
        return raiz;
}

// Função para listar usuários em ordem alfabética (in-order traversal)
void listarUsuarios(Usuario* raiz) {
    if (raiz != NULL) {
        listarUsuarios(raiz->esquerda);
        printf("Nome: %s\nID: %d\nEmail: %s\n---------------------\n", 
               raiz->nome, raiz->id, raiz->email);
        listarUsuarios(raiz->direita);
    }
}

// Função principal
int main() {
    Usuario* raiz = NULL;
    int opcao, id;
    char nome[50], email[50];

    do {
        system("cls"); // Limpa a tela
        printf("--------------------------------------------------------\n");
        printf("|       Sistema de Gerenciamento de Usuários AVL        |\n");
        printf("|                                                      |\n");
        printf("|1. Cadastrar usuário                                  |\n");
        printf("|2. Remover usuário                                    |\n");  
        printf("|3. Buscar usuário                                     |\n");
        printf("|4. Listar usuários em ordem alfabética                |\n");
        printf("|5. Sair                                               |\n");
        printf("--------------------------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        system("cls"); // Limpa a tela antes de executar a operação

        switch (opcao) {
            case 1:
                printf("Digite o nome do usuário: ");
                scanf(" %[^\n]", nome); // Lê até encontrar uma quebra de linha
                printf("Digite o ID do usuário: ");
                scanf("%d", &id);
                printf("Digite o email do usuário: ");
                scanf(" %[^\n]", email);
                
                raiz = inserirUsuario(raiz, criarUsuario(nome, id, email));
                printf("Usuário cadastrado com sucesso!\n");
                break;

            case 2:
                printf("Digite o nome do usuário a ser removido: ");
                scanf(" %[^\n]", nome);
                raiz = removerUsuario(raiz, nome);
                printf("Usuário removido com sucesso!\n");
                break;

            case 3:
                printf("Digite o nome do usuário para buscar: ");
                scanf(" %[^\n]", nome);
                Usuario* encontrado = buscarUsuario(raiz, nome);
                if (encontrado != NULL) {
                    printf("Usuário encontrado:\n");
                    printf("Nome: %s\nID: %d\nEmail: %s\n",
                           encontrado->nome, encontrado->id, encontrado->email);
                } else {
                    printf("Usuário não encontrado.\n");
                }
                break;

            case 4:
                printf("Lista de usuários em ordem alfabética:\n");
                printf("------------------------------------\n");
                listarUsuarios(raiz);
                break;

            case 5:
                printf("Saindo do sistema...\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }

        if (opcao != 6) {
            printf("\nPressione qualquer tecla para continuar...");
            getchar(); // Aguarda o usuário pressionar uma tecla
            getchar(); // Para capturar o Enter
        }

    } while (opcao != 6);

    return 0;
}