//-------------------------------------------------------
// Aluno: Vitor G. J. de Carvalho                       |
// RGM: 33046794                                        |
// Disciplina: Estruturas de Dados avancadas            |
// Professor: Marcelo T. Uemura                         |
// Data: 14/05/2025                                     |
//-------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição das cores para os nós da árvore Rubro-Negra
typedef enum { VERMELHO, PRETO } Cor;

// Estrutura para representar um produto
typedef struct Produto {
    int codigo;                 // Código do produto (chave)
    char nome[50];              // Nome do produto
    int quantidade;             // Quantidade em estoque
    float preco;                // Preço por unidade
    Cor cor;                    // Cor do nó (VERMELHO ou PRETO)
    struct Produto* esquerda;   // Ponteiro para o filho esquerdo
    struct Produto* direita;    // Ponteiro para o filho direito
    struct Produto* pai;        // Ponteiro para o pai
} Produto;

// Função para criar um novo produto
Produto* criarProduto(int codigo, const char* nome, int quantidade, float preco) {
    Produto* novoProduto = (Produto*)malloc(sizeof(Produto));
    novoProduto->codigo = codigo;
    strcpy(novoProduto->nome, nome);
    novoProduto->quantidade = quantidade;
    novoProduto->preco = preco;
    novoProduto->cor = VERMELHO; // Novo nó é sempre vermelho
    novoProduto->esquerda = NULL;
    novoProduto->direita = NULL;
    novoProduto->pai = NULL;
    return novoProduto;
}

// Função auxiliar para verificar se um nó é vermelho
int ehVermelho(Produto* no) {
    if (no == NULL)
        return 0; // Nós nulos são considerados pretos
    return no->cor == VERMELHO;
}

// Função auxiliar para verificar se um nó é preto
int ehPreto(Produto* no) {
    if (no == NULL)
        return 1; // Nós nulos são considerados pretos
    return no->cor == PRETO;
}

// Rotação simples à esquerda
Produto* rotacaoEsquerda(Produto* raiz, Produto* x) {
    Produto* y = x->direita;
    x->direita = y->esquerda;
    
    if (y->esquerda != NULL)
        y->esquerda->pai = x;
    
    y->pai = x->pai;
    
    if (x->pai == NULL)
        raiz = y;
    else if (x == x->pai->esquerda)
        x->pai->esquerda = y;
    else
        x->pai->direita = y;
    
    y->esquerda = x;
    x->pai = y;
    
    return raiz;
}

// Rotação simples à direita
Produto* rotacaoDireita(Produto* raiz, Produto* y) {
    Produto* x = y->esquerda;
    y->esquerda = x->direita;
    
    if (x->direita != NULL)
        x->direita->pai = y;
    
    x->pai = y->pai;
    
    if (y->pai == NULL)
        raiz = x;
    else if (y == y->pai->esquerda)
        y->pai->esquerda = x;
    else
        y->pai->direita = x;
    
    x->direita = y;
    y->pai = x;
    
    return raiz;
}

// Função para corrigir a árvore após inserção
Produto* corrigirInsercao(Produto* raiz, Produto* z) {
    while (z != raiz && ehVermelho(z->pai)) {
        if (z->pai == z->pai->pai->esquerda) {
            Produto* y = z->pai->pai->direita;
            
            // Caso 1: tio é vermelho
            if (ehVermelho(y)) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                // Caso 2: tio é preto e z é filho direito
                if (z == z->pai->direita) {
                    z = z->pai;
                    raiz = rotacaoEsquerda(raiz, z);
                }
                // Caso 3: tio é preto e z é filho esquerdo
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                raiz = rotacaoDireita(raiz, z->pai->pai);
            }
        } else {
            // Caso simétrico (direita em vez de esquerda)
            Produto* y = z->pai->pai->esquerda;
            
            // Caso 1: tio é vermelho
            if (ehVermelho(y)) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                // Caso 2: tio é preto e z é filho esquerdo
                if (z == z->pai->esquerda) {
                    z = z->pai;
                    raiz = rotacaoDireita(raiz, z);
                }
                // Caso 3: tio é preto e z é filho direito
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                raiz = rotacaoEsquerda(raiz, z->pai->pai);
            }
        }
    }
    raiz->cor = PRETO;
    return raiz;
}

// Função para inserir um produto na árvore Rubro-Negra
Produto* inserirProduto(Produto* raiz, int codigo, const char* nome, int quantidade, float preco) {
    Produto* novo = criarProduto(codigo, nome, quantidade, preco);
    Produto* y = NULL;
    Produto* x = raiz;
    
    // Encontra a posição de inserção (como em BST normal)
    while (x != NULL) {
        y = x;
        if (novo->codigo < x->codigo)
            x = x->esquerda;
        else
            x = x->direita;
    }
    
    novo->pai = y;
    
    if (y == NULL)
        raiz = novo;
    else if (novo->codigo < y->codigo)
        y->esquerda = novo;
    else
        y->direita = novo;
    
    // Corrige as propriedades da árvore Rubro-Negra
    raiz = corrigirInsercao(raiz, novo);
    
    return raiz;
}

// Função auxiliar para encontrar o nó com menor valor
Produto* encontrarMinimo(Produto* no) {
    while (no->esquerda != NULL)
        no = no->esquerda;
    return no;
}

// Função auxiliar para substituir um nó por outro
void transplantar(Produto** raiz, Produto* u, Produto* v) {
    if (u->pai == NULL)
        *raiz = v;
    else if (u == u->pai->esquerda)
        u->pai->esquerda = v;
    else
        u->pai->direita = v;
    
    if (v != NULL)
        v->pai = u->pai;
}

// Função para corrigir a árvore após remoção
Produto* corrigirRemocao(Produto* raiz, Produto* x) {
    while (x != raiz && ehPreto(x)) {
        if (x == x->pai->esquerda) {
            Produto* w = x->pai->direita;
            
            // Caso 1: irmão é vermelho
            if (ehVermelho(w)) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                raiz = rotacaoEsquerda(raiz, x->pai);
                w = x->pai->direita;
            }
            
            // Caso 2: ambos os filhos do irmão são pretos
            if (ehPreto(w->esquerda) && ehPreto(w->direita)) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                // Caso 3: filho esquerdo do irmão é vermelho e direito é preto
                if (ehPreto(w->direita)) {
                    w->esquerda->cor = PRETO;
                    w->cor = VERMELHO;
                    raiz = rotacaoDireita(raiz, w);
                    w = x->pai->direita;
                }
                
                // Caso 4: filho direito do irmão é vermelho
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->direita->cor = PRETO;
                raiz = rotacaoEsquerda(raiz, x->pai);
                x = raiz;
            }
        } else {
            // Caso simétrico (direita em vez de esquerda)
            Produto* w = x->pai->esquerda;
            
            // Caso 1: irmão é vermelho
            if (ehVermelho(w)) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                raiz = rotacaoDireita(raiz, x->pai);
                w = x->pai->esquerda;
            }
            
            // Caso 2: ambos os filhos do irmão são pretos
            if (ehPreto(w->direita) && ehPreto(w->esquerda)) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                // Caso 3: filho direito do irmão é vermelho e esquerdo é preto
                if (ehPreto(w->esquerda)) {
                    w->direita->cor = PRETO;
                    w->cor = VERMELHO;
                    raiz = rotacaoEsquerda(raiz, w);
                    w = x->pai->esquerda;
                }
                
                // Caso 4: filho esquerdo do irmão é vermelho
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->esquerda->cor = PRETO;
                raiz = rotacaoDireita(raiz, x->pai);
                x = raiz;
            }
        }
    }
    x->cor = PRETO;
    return raiz;
}

// Função para remover um produto da árvore Rubro-Negra
Produto* removerProduto(Produto* raiz, int codigo) {
    Produto* z = raiz;
    Produto* x, *y;
    
    // Encontra o nó a ser removido
    while (z != NULL) {
        if (codigo == z->codigo)
            break;
        else if (codigo < z->codigo)
            z = z->esquerda;
        else
            z = z->direita;
    }
    
    if (z == NULL) {
        printf("Produto com código %d não encontrado.\n", codigo);
        return raiz;
    }
    
    y = z;
    Cor yCorOriginal = y->cor;
    
    if (z->esquerda == NULL) {
        x = z->direita;
        transplantar(&raiz, z, z->direita);
    } else if (z->direita == NULL) {
        x = z->esquerda;
        transplantar(&raiz, z, z->esquerda);
    } else {
        y = encontrarMinimo(z->direita);
        yCorOriginal = y->cor;
        x = y->direita;
        
        if (y->pai == z)
            x->pai = y;
        else {
            transplantar(&raiz, y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }
        
        transplantar(&raiz, z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }
    
    free(z);
    
    if (yCorOriginal == PRETO)
        raiz = corrigirRemocao(raiz, x);
    
    return raiz;
}

// Função para buscar um produto pelo código
Produto* buscarProduto(Produto* raiz, int codigo) {
    if (raiz == NULL || raiz->codigo == codigo)
        return raiz;
    
    if (codigo < raiz->codigo)
        return buscarProduto(raiz->esquerda, codigo);
    else
        return buscarProduto(raiz->direita, codigo);
}

// Função para listar produtos em ordem (in-order traversal)
void listarProdutos(Produto* raiz) {
    if (raiz != NULL) {
        listarProdutos(raiz->esquerda);
        printf("Código: %d\nNome: %s\nQuantidade: %d\nPreço: %.2f\nCor: %s\n---------------------\n",
               raiz->codigo, raiz->nome, raiz->quantidade, raiz->preco,
               raiz->cor == VERMELHO ? "Vermelho" : "Preto");
        listarProdutos(raiz->direita);
    }
}

// Função principal
int main() {
    Produto* raiz = NULL;
    int opcao, codigo, quantidade;
    float preco;
    char nome[50];
    
    do {
        system("cls"); // Limpa a tela
        printf("--------------------------------------------------------\n");
        printf("|       Sistema de Inventário com Árvore Rubro-Negra    |\n");
        printf("|                                                      |\n");
        printf("|1. Cadastrar produto                                  |\n");
        printf("|2. Remover produto                                    |\n");  
        printf("|3. Buscar produto                                     |\n");
        printf("|4. Listar produtos em ordem                           |\n");
        printf("|5. Sair                                               |\n");
        printf("--------------------------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        system("cls"); // Limpa a tela antes de executar a operação
        
        switch (opcao) {
            case 1:
                printf("Digite o código do produto: ");
                scanf("%d", &codigo);
                printf("Digite o nome do produto: ");
                scanf(" %[^\n]", nome);
                printf("Digite a quantidade em estoque: ");
                scanf("%d", &quantidade);
                printf("Digite o preço por unidade: ");
                scanf("%f", &preco);
                
                raiz = inserirProduto(raiz, codigo, nome, quantidade, preco);
                printf("Produto cadastrado com sucesso!\n");
                break;
                
            case 2:
                printf("Digite o código do produto a ser removido: ");
                scanf("%d", &codigo);
                raiz = removerProduto(raiz, codigo);
                printf("Produto removido com sucesso!\n");
                break;
                
            case 3:
                printf("Digite o código do produto para buscar: ");
                scanf("%d", &codigo);
                Produto* encontrado = buscarProduto(raiz, codigo);
                if (encontrado != NULL) {
                    printf("Produto encontrado:\n");
                    printf("Código: %d\nNome: %s\nQuantidade: %d\nPreço: %.2f\nCor: %s\n",
                           encontrado->codigo, encontrado->nome, encontrado->quantidade, 
                           encontrado->preco, encontrado->cor == VERMELHO ? "Vermelho" : "Preto");
                } else {
                    printf("Produto não encontrado.\n");
                }
                break;
                
            case 4:
                printf("Lista de produtos em ordem:\n");
                printf("---------------------------\n");
                listarProdutos(raiz);
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