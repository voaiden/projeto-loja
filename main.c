#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUTOS 100
#define ARQUIVO_DADOS "produtos.txt"
#define ARQUIVO_TEMP "temp.txt"

// Definição da estrutura do Produto
typedef struct {
    int id;
    char nome[50];
    char categoria[40];
    float preco;
    int quantidade;
    char fornecedor[50];
} Produto;

// Função auxiliar para converter string para minúsculas (busca case-insensitive)
void paraMinusculas(char *dest, const char *orig) {
    int i = 0;
    while (orig[i] != '\0') {
        dest[i] = tolower((unsigned char)orig[i]);
        i++;
    }
    dest[i] = '\0';
}

// Carrega os dados do arquivo de texto para o vetor de estruturas
int carregarArquivo(Produto lista[], int *tam) {
    FILE *file = fopen(ARQUIVO_DADOS, "r");
    if (file == NULL) {
        *tam = 0;
        return 0; // Arquivo ainda não existe
    }

    *tam = 0;
    // Lê formato: ID|Nome|Categoria|Preço|Quantidade|Fornecedor
    while (fscanf(file, "%d|%49[^|]|%39[^|]|%f|%d|%49[^\n]\n", 
                  &lista[*tam].id, 
                  lista[*tam].nome, 
                  lista[*tam].categoria, 
                  &lista[*tam].preco, 
                  &lista[*tam].quantidade, 
                  lista[*tam].fornecedor) == 6) {
        (*tam)++;
        if (*tam >= MAX_PRODUTOS) break;
    }

    fclose(file);
    return 1;
}

// Salva todo o vetor de estruturas de volta para o arquivo de texto original
void salvarArquivo(Produto lista[], int tam) {
    FILE *file = fopen(ARQUIVO_DADOS, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita!\n");
        return;
    }

    for (int i = 0; i < tam; i++) {
        fprintf(file, "%d|%s|%s|%.2f|%d|%s\n", 
                lista[i].id, 
                lista[i].nome, 
                lista[i].categoria, 
                lista[i].preco, 
                lista[i].quantidade, 
                lista[i].fornecedor);
    }

    fclose(file);
}

// Gera automaticamente o próximo ID incremental
int gerarProximoID(Produto lista[], int tam) {
    if (tam == 0) return 1;
    int maiorID = 0;
    for (int i = 0; i < tam; i++) {
        if (lista[i].id > maiorID) {
            maiorID = lista[i].id;
        }
    }
    return maiorID + 1;
}

// Verifica se já existe um produto com o mesmo nome (Case-Insensitive)
int existeProduto(Produto lista[], int tam, char *nome) {
    char nomeBusca[50], nomeAtual[50];
    paraMinusculas(nomeBusca, nome);

    for (int i = 0; i < tam; i++) {
        paraMinusculas(nomeAtual, lista[i].nome);
        if (strcmp(nomeAtual, nomeBusca) == 0) {
            return 1; // Encontrou duplicata
        }
    }
    return 0; // Não existe
}

// Exibe os produtos em formato de tabela formatada
void listarProdutos(Produto lista[], int tam) {
    if (tam == 0) {
        printf("\nNenhum produto cadastrado.\n");
        return;
    }

    printf("\n-------------------------------------------------------------------------------------------------\n");
    printf("| ID  | Produto                        | Categoria         | Preço (R$) | Qtd | Fornecedor            |\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < tam; i++) {
        printf("| %-3d | %-30s | %-17s | R$ %-8.2f | %-3d | %-21s |\n", 
               lista[i].id, lista[i].nome, lista[i].categoria, 
               lista[i].preco, lista[i].quantidade, lista[i].fornecedor);
    }
    printf("-------------------------------------------------------------------------------------------------\n");
}

// Insere um novo produto no vetor e atualiza o arquivo
void inserirProduto(Produto lista[], int *tam) {
    if (*tam >= MAX_PRODUTOS) {
        printf("\nLimite máximo de produtos atingido!\n");
        return;
    }

    Produto p;
    p.id = gerarProximoID(lista, *tam); // Geração automática de ID

    printf("\n--- Cadastro de Novo Produto --- \n");
    printf("Nome do produto: ");
    scanf(" %[^\n]", p.nome);

    // Verificação de duplicatas
    if (existeProduto(lista, *tam, p.nome)) {
        printf("\n[Erro] Já existe um produto cadastrado com este nome!\n");
        return;
    }

    printf("Categoria: ");
    scanf(" %[^\n]", p.categoria);
    printf("Preço (R$): ");
    scanf("%f", &p.preco);
    printf("Quantidade em estoque: ");
    scanf("%d", &p.quantidade);
    printf("Fornecedor: ");
    scanf(" %[^\n]", p.fornecedor);

    lista[*tam] = p;
    (*tam)++;

    salvarArquivo(lista, *tam);
    printf("\nProduto cadastrado com sucesso! ID gerado: %d\n", p.id);
}

// Busca por nome (parcial e case-insensitive)
void buscarPorNome(Produto lista[], int tam) {
    char termo[50], nomeAtual[50], termoLower[50];
    printf("\nDigite o nome (ou parte do nome) para buscar: ");
    scanf(" %[^\n]", termo);
    paraMinusculas(termoLower, termo);

    int encontrados = 0;
    printf("\nResultados da Busca por Nome:\n");
    for (int i = 0; i < tam; i++) {
        paraMinusculas(nomeAtual, lista[i].nome);
        if (strstr(nomeAtual, termoLower) != NULL) {
            printf("- [%d] %s | Categoria: %s | R$ %.2f | Qtd: %d | Fornecedor: %s\n",
                   lista[i].id, lista[i].nome, lista[i].categoria, 
                   lista[i].preco, lista[i].quantidade, lista[i].fornecedor);
            encontrados++;
        }
    }
    if (encontrados == 0) printf("Nenhum produto encontrado com esse termo.\n");
}

// Busca por categoria (case-insensitive)
void buscarPorCategoria(Produto lista[], int tam) {
    char termo[40], catAtual[40], termoLower[40];
    printf("\nDigite a categoria para buscar: ");
    scanf(" %[^\n]", termo);
    paraMinusculas(termoLower, termo);

    int encontrados = 0;
    printf("\nResultados da Busca por Categoria:\n");
    for (int i = 0; i < tam; i++) {
        paraMinusculas(catAtual, lista[i].categoria);
        if (strstr(catAtual, termoLower) != NULL) {
            printf("- [%d] %s | Categoria: %s | R$ %.2f | Qtd: %d | Fornecedor: %s\n",
                   lista[i].id, lista[i].nome, lista[i].categoria, 
                   lista[i].preco, lista[i].quantidade, lista[i].fornecedor);
            encontrados++;
        }
    }
    if (encontrados == 0) printf("Nenhum produto encontrado nesta categoria.\n");
}

// Altera um produto utilizando arquivo temporário para segurança contra corrupção
void alterarProduto(Produto lista[], int *tam) {
    int idBusca, encontrado = -1;
    printf("\nDigite o ID do produto que deseja alterar: ");
    scanf("%d", &idBusca);

    for (int i = 0; i < *tam; i++) {
        if (lista[i].id == idBusca) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("\nProduto com ID %d não encontrado.\n", idBusca);
        return;
    }

    printf("\nProduto selecionado: %s\n", lista[encontrado].nome);
    printf("Novo nome: ");
    scanf(" %[^\n]", lista[encontrado].nome);
    printf("Nova categoria: ");
    scanf(" %[^\n]", lista[encontrado].categoria);
    printf("Novo preço: ");
    scanf("%f", &lista[encontrado].preco);
    printf("Nova quantidade: ");
    scanf("%d", &lista[encontrado].quantidade);
    printf("Novo fornecedor: ");
    scanf(" %[^\n]", lista[encontrado].fornecedor);

    // Técnica do arquivo temporário para atualizar de forma segura
    FILE *temp = fopen(ARQUIVO_TEMP, "w");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporário!\n");
        return;
    }

    for (int i = 0; i < *tam; i++) {
        fprintf(temp, "%d|%s|%s|%.2f|%d|%s\n", 
                lista[i].id, lista[i].nome, lista[i].categoria, 
                lista[i].preco, lista[i].quantidade, lista[i].fornecedor);
    }
    fclose(temp);

    remove(ARQUIVO_DADOS);
    rename(ARQUIVO_TEMP, ARQUIVO_DADOS);

    printf("\nProduto alterado com sucesso!\n");
}

// Exclui um produto usando arquivo temporário para segurança
void excluirProduto(Produto lista[], int *tam) {
    int idBusca, encontrado = -1;
    printf("\nDigite o ID do produto que deseja excluir: ");
    scanf("%d", &idBusca);

    for (int i = 0; i < *tam; i++) {
        if (lista[i].id == idBusca) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("\nProduto com ID %d não encontrado.\n", idBusca);
        return;
    }

    // Criar arquivo temporário omitindo o item a ser excluído
    FILE *temp = fopen(ARQUIVO_TEMP, "w");
    if (temp == NULL) {
        printf("Erro ao criar arquivo temporário!\n");
        return;
    }

    for (int i = 0; i < *tam; i++) {
        if (lista[i].id != idBusca) {
            fprintf(temp, "%d|%s|%s|%.2f|%d|%s\n", 
                    lista[i].id, lista[i].nome, lista[i].categoria, 
                    lista[i].preco, lista[i].quantidade, lista[i].fornecedor);
        }
    }
    fclose(temp);

    remove(ARQUIVO_DADOS);
    rename(ARQUIVO_TEMP, ARQUIVO_DADOS);

    // Reorganiza o vetor na memória
    for (int i = encontrado; i < *tam - 1; i++) {
        lista[i] = lista[i + 1];
    }
    (*tam)--;

    printf("\nProduto excluído com sucesso!\n");
}

// Ordenação usando Bubble Sort com base na opção escolhida
void ordenarProdutos(Produto lista[], int tam) {
    if (tam <= 1) {
        printf("\nPoucos elementos para ordenar.\n");
        return;
    }

    int opcao;
    printf("\n--- Opções de Ordenação (Bubble Sort) ---\n");
    printf("1. Nome (A - Z)\n");
    printf("2. Nome (Z - A)\n");
    printf("3. Preço (Menor -> Maior)\n");
    printf("4. Preço (Maior -> Menor)\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    // Cria uma cópia do vetor para exibir sem alterar permanentemente a ordem padrão do arquivo
    Produto copia[MAX_PRODUTOS];
    for (int i = 0; i < tam; i++) {
        copia[i] = lista[i];
    }

    // Algoritmo Bubble Sort
    Produto temp;
    for (int i = 0; i < tam - 1; i++) {
        for (int j = 0; j < tam - i - 1; j++) {
            int trocar = 0;
            if (opcao == 1 && strcasecmp(copia[j].nome, copia[j+1].nome) > 0) trocar = 1;
            else if (opcao == 2 && strcasecmp(copia[j].nome, copia[j+1].nome) < 0) trocar = 1;
            else if (opcao == 3 && copia[j].preco > copia[j+1].preco) trocar = 1;
            else if (opcao == 4 && copia[j].preco < copia[j+1].preco) trocar = 1;

            if (trocar) {
                temp = copia[j];
                copia[j] = copia[j+1];
                copia[j+1] = temp;
            }
        }
    }

    // Exibe o resultado ordenado
    printf("\n--- Produtos Ordenados ---\n");
    listarProdutos(copia, tam);
}

// Exporta os dados armazenados para um arquivo CSV separado por vírgulas
void exportarCSV(Produto lista[], int tam) {
    FILE *csv = fopen("produtos_exportado.csv", "w");
    if (csv == NULL) {
        printf("Erro ao gerar arquivo CSV!\n");
        return;
    }

    fprintf(csv, "ID,Produto,Categoria,Preco,Quantidade,Fornecedor\n");
    for (int i = 0; i < tam; i++) {
        fprintf(csv, "%d,%s,%s,%.2f,%d,%s\n", 
                lista[i].id, lista[i].nome, lista[i].categoria, 
                lista[i].preco, lista[i].quantidade, lista[i].fornecedor);
    }

    fclose(csv);
    printf("\nDados exportados com sucesso para 'produtos_exportado.csv'!\n");
}

// Função Principal (Menu)
int main() {
    Produto lista[MAX_PRODUTOS];
    int tam = 0;
    int opcao;

    carregarArquivo(lista, &tam);

    do {
        printf("\n=====================================\n");
        printf("     SISTEMA DE CADASTRO DE LOJA     \n");
        printf("=====================================\n");
        printf("1. Listar Produtos\n");
        printf("2. Inserir Novo Produto\n");
        printf("3. Buscar Produto\n");
        printf("4. Alterar Produto\n");
        printf("5. Excluir Produto\n");
        printf("6. Ordenar Produtos\n");
        printf("7. Exportar para CSV\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                listarProdutos(lista, tam);
                break;
            case 2:
                inserirProduto(lista, &tam);
                break;
            case 3:
                {
                    int tipoBusca;
                    printf("\nBuscar por:\n1. Nome\n2. Categoria\nEscolha: ");
                    scanf("%d", &tipoBusca);
                    if (tipoBusca == 1) buscarPorNome(lista, tam);
                    else if (tipoBusca == 2) buscarPorCategoria(lista, tam);
                    else printf("Opção inválida.\n");
                }
                break;
            case 4:
                alterarProduto(lista, &tam);
                break;
            case 5:
                excluirProduto(lista, &tam);
                break;
            case 6:
                ordenarProdutos(lista, tam);
                break;
            case 7:
                exportarCSV(lista, tam);
                break;
            case 0:
                printf("\nSaindo do sistema. Até logo!\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}