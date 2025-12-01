#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* =============================================================
   ======================= NÍVEL 1 =============================
   ========== SISTEMA DE LOOT – MOCHILA INICIAL =================
   ============================================================= */

// Struct principal do jogo para itens coletados
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

/* ---------------------- VETOR ---------------------- */

void inserirItemVetor(Item mochila[], int *qtd) {
    if (*qtd >= 10) {
        printf("\n⚠ A mochila está cheia! Não cabe mais nada!\n");
        return;
    }

    printf("\nDigite o nome do item coletado: ");
    scanf(" %29[^\n]", mochila[*qtd].nome);

    printf("Tipo do item (arma, munição, cura): ");
    scanf(" %19[^\n]", mochila[*qtd].tipo);

    printf("Quantidade encontrada: ");
    scanf("%d", &mochila[*qtd].quantidade);

    (*qtd)++;
    printf("\n✔ Item guardado na mochila!\n");
}

void listarItensVetor(Item mochila[], int qtd) {
    printf("\n📦 Itens atuais da mochila:\n");
    if (qtd == 0) {
        printf("   (vazia)\n");
        return;
    }
    for (int i = 0; i < qtd; i++) {
        printf(" - %s | Tipo: %s | Qtd: %d\n",
               mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

int buscarSequencialVetor(Item mochila[], int qtd, char nome[], int *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < qtd; i++) {
        (*comparacoes)++;
        if (strcmp(mochila[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

void removerItemVetor(Item mochila[], int *qtd) {
    char nome[30];
    int comparacoes;

    printf("\nQual item você quer descartar? ");
    scanf(" %29[^\n]", nome);

    int pos = buscarSequencialVetor(mochila, *qtd, nome, &comparacoes);

    if (pos == -1) {
        printf("\n❌ Item não encontrado!\n");
        return;
    }

    for (int i = pos; i < *qtd - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    (*qtd)--;

    printf("\n🗑 Item descartado! (%d comparações)\n", comparacoes);
}

/* ---------------------- LISTA ENCADEADA ---------------------- */

typedef struct No {
    Item dados;
    struct No *proximo;
} No;

void inserirItemLista(No **inicio) {
    No *novo = (No*) malloc(sizeof(No));

    printf("\nNome do item coletado: ");
    scanf(" %29[^\n]", novo->dados.nome);

    printf("Tipo (arma, munição, cura): ");
    scanf(" %19[^\n]", novo->dados.tipo);

    printf("Quantidade: ");
    scanf("%d", &novo->dados.quantidade);

    novo->proximo = *inicio;
    *inicio = novo;

    printf("\n✔ Item colocado na mochila!\n");
}

void listarItensLista(No *inicio) {
    printf("\n📦 Mochila (lista encadeada):\n");
    if (inicio == NULL) {
        printf("   (vazia)\n");
        return;
    }

    while (inicio != NULL) {
        printf(" - %s | Tipo: %s | Qtd: %d\n",
               inicio->dados.nome, inicio->dados.tipo, inicio->dados.quantidade);
        inicio = inicio->proximo;
    }
}

int buscarItemLista(No *inicio, char nome[], int *comparacoes) {
    *comparacoes = 0;

    while (inicio != NULL) {
        (*comparacoes)++;
        if (strcmp(inicio->dados.nome, nome) == 0)
            return 1;
        inicio = inicio->proximo;
    }
    return 0;
}

void removerItemLista(No **inicio) {
    char nome[30];
    int comparacoes;

    printf("\nQual item deseja descartar? ");
    scanf(" %29[^\n]", nome);

    No *atual = *inicio;
    No *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nome) == 0)
            break;

        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("\n❌ Item não encontrado!\n");
        return;
    }

    if (anterior == NULL) {
        *inicio = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);
    printf("\n🗑 Item removido!\n");
}

/* =============================================================
   ======================= NÍVEL 2 =============================
   ============ ORDENAÇÃO, BUSCA BINÁRIA & TORRE ===============
   ============================================================= */

typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

int compBubble = 0, compInsertion = 0, compSelection = 0;

/* ----------- Bubble Sort (nome) ----------- */
void bubbleSortNome(Componente v[], int n) {
    compBubble = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            compBubble++;
            if (strcmp(v[j].nome, v[j + 1].nome) > 0) {
                Componente tmp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = tmp;
            }
        }
    }
}

/* ----------- Insertion Sort (tipo) ----------- */
void insertionSortTipo(Componente v[], int n) {
    compInsertion = 0;

    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;

        while (j >= 0 && strcmp(v[j].tipo, chave.tipo) > 0) {
            compInsertion++;
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}

/* ----------- Selection Sort (prioridade) ----------- */
void selectionSortPrioridade(Componente v[], int n) {
    compSelection = 0;

    for (int i = 0; i < n - 1; i++) {
        int min = i;

        for (int j = i + 1; j < n; j++) {
            compSelection++;
            if (v[j].prioridade < v[min].prioridade)
                min = j;
        }

        Componente temp = v[i];
        v[i] = v[min];
        v[min] = temp;
    }
}

/* ----------- Busca Binária ----------- */
int buscaBinaria(Componente v[], int n, char chave[], int *comparacoes) {
    int inicio = 0, fim = n - 1;
    *comparacoes = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;

        int cmp = strcmp(v[meio].nome, chave);

        if (cmp == 0) return meio;
        if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

/* ----------- Mostrar componentes ----------- */
void mostrarComponentes(Componente v[], int n) {
    printf("\n🔧 Lista de Componentes:\n");
    for (int i = 0; i < n; i++) {
        printf(" - %s | Tipo: %s | Prioridade: %d\n",
               v[i].nome, v[i].tipo, v[i].prioridade);
    }
}

/* =============================================================
   ======================== MENU GERAL ==========================
   ============================================================= */

void menuPrincipal() {
    printf("\n==============================\n");
    printf("     🎒 MENU DO JOGADOR\n");
    printf("==============================\n");
    printf("1. Mochila – Vetor\n");
    printf("2. Mochila – Lista Encadeada\n");
    printf("3. Torre de Resgate – Ordenação\n");
    printf("0. Sair do jogo\n");
    printf("Escolha: ");
}

/* =============================================================
   ============================ MAIN ============================
   ============================================================= */

int main() {
    int opc;

    /* ======== SUB-SISTEMAS ======== */

    Item vetorMochila[10];
    int qtdVetor = 0;

    No *lista = NULL;

    Componente torre[20];
    int qtdComp = 0;

    while (1) {
        menuPrincipal();
        scanf("%d", &opc);

        /* =====================================
                 SISTEMA DE MOCHILA – VETOR
           ===================================== */

        if (opc == 1) {
            int opV;
            do {
                printf("\n--- 🎒 Mochila (Vetor) ---\n");
                printf("1. Guardar item\n");
                printf("2. Descartar item\n");
                printf("3. Ver mochila\n");
                printf("4. Buscar item\n");
                printf("0. Voltar\n");
                printf("Escolha: ");
                scanf("%d", &opV);

                if (opV == 1) inserirItemVetor(vetorMochila, &qtdVetor);
                if (opV == 2) removerItemVetor(vetorMochila, &qtdVetor);
                if (opV == 3) listarItensVetor(vetorMochila, qtdVetor);
                if (opV == 4) {
                    char nome[30];
                    int comp;
                    printf("\nItem procurado: ");
                    scanf(" %29[^\n]", nome);

                    int pos = buscarSequencialVetor(vetorMochila, qtdVetor, nome, &comp);

                    if (pos == -1) printf("\n❌ Não encontrado (%d comparações)\n", comp);
                    else {
                        printf("\n✔ Encontrado: %s | Tipo: %s | Qtd: %d (%d comparações)\n",
                               vetorMochila[pos].nome,
                               vetorMochila[pos].tipo,
                               vetorMochila[pos].quantidade,
                               comp);
                    }
                }

            } while (opV != 0);
        }

        /* =====================================
                 SISTEMA DE MOCHILA – LISTA
           ===================================== */

        else if (opc == 2) {
            int opL;
            do {
                printf("\n--- 🎒 Mochila (Lista Encadeada) ---\n");
                printf("1. Guardar item\n");
                printf("2. Descartar item\n");
                printf("3. Ver mochila\n");
                printf("4. Buscar item\n");
                printf("0. Voltar\n");
                printf("Escolha: ");
                scanf("%d", &opL);

                if (opL == 1) inserirItemLista(&lista);
                if (opL == 2) removerItemLista(&lista);
                if (opL == 3) listarItensLista(lista);
                if (opL == 4) {
                    char nome[30];
                    int comp;
                    printf("Item procurado: ");
                    scanf(" %29[^\n]", nome);

                    int ok = buscarItemLista(lista, nome, &comp);

                    if (ok == 0) printf("\n❌ Não encontrado (%d comparações)\n", comp);
                    else printf("\n✔ Item presente na mochila! (%d comparações)\n", comp);
                }

            } while (opL != 0);
        }

        /* =====================================
              SISTEMA DA TORRE DE RESGATE
           ===================================== */

        else if (opc == 3) {
            int opT;
            do {
                printf("\n--- 🏗 Torre de Resgate – Montagem Estratégica ---\n");
                printf("1. Adicionar componente\n");
                printf("2. Ver lista\n");
                printf("3. Ordenar por nome (Bubble)\n");
                printf("4. Ordenar por tipo (Insertion)\n");
                printf("5. Ordenar por prioridade (Selection)\n");
                printf("6. Buscar componente-chave (binária)\n");
                printf("0. Voltar\n");
                printf("Escolha: ");
                scanf("%d", &opT);

                if (opT == 1) {
                    if (qtdComp >= 20) {
                        printf("\n⚠ Limite de componentes atingido!\n");
                    } else {
                        printf("\nNome do componente: ");
                        scanf(" %29[^\n]", torre[qtdComp].nome);
                        printf("Tipo: ");
                        scanf(" %19[^\n]", torre[qtdComp].tipo);
                        printf("Prioridade (1 a 10): ");
                        scanf("%d", &torre[qtdComp].prioridade);
                        qtdComp++;
                        printf("\n✔ Componente adicionado!\n");
                    }
                }

                if (opT == 2) mostrarComponentes(torre, qtdComp);

                if (opT == 3) {
                    clock_t t = clock();
                    bubbleSortNome(torre, qtdComp);
                    t = clock() - t;

                    printf("\n✔ Ordenado por nome!\n");
                    printf("Comparações: %d\n", compBubble);
                    printf("Tempo: %.4f s\n", (double)t / CLOCKS_PER_SEC);
                }

                if (opT == 4) {
                    clock_t t = clock();
                    insertionSortTipo(torre, qtdComp);
                    t = clock() - t;

                    printf("\n✔ Ordenado por tipo!\n");
                    printf("Comparações: %d\n", compInsertion);
                    printf("Tempo: %.4f s\n", (double)t / CLOCKS_PER_SEC);
                }

                if (opT == 5) {
                    clock_t t = clock();
                    selectionSortPrioridade(torre, qtdComp);
                    t = clock() - t;

                    printf("\n✔ Ordenado por prioridade!\n");
                    printf("Comparações: %d\n", compSelection);
                    printf("Tempo: %.4f s\n", (double)t / CLOCKS_PER_SEC);
                }

                if (opT == 6) {
                    char chave[30];
                    int comp;

                    printf("\nNome do componente-chave: ");
                    scanf(" %29[^\n]", chave);

                    int pos = buscaBinaria(torre, qtdComp, chave, &comp);

                    if (pos == -1)
                        printf("\n❌ Componente-chave NÃO encontrado! (%d comparações)\n", comp);
                    else
                        printf("\n✔ Componente encontrado! Torre pronta para montar! (%d comparações)\n", comp);
                }

            } while (opT != 0);
        }

        else if (opc == 0) {
            printf("\n🏆 Obrigado por jogar! Até a próxima missão!\n");
            break;
        }

        else {
            printf("\n❌ Opção inválida!\n");
        }
    }

    return 0;
}
