#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/**
 * Struktura Node slouzi k reprezentaci uzlu v B-strome
 * atribut keys je ukazatel na pole obsahujici klice daneho uzlu
 * atribut children je ukazatel na pole ukazatelu na 'n' potomku
 * atribut leaf urcuje, zdali je uzel list
 * atribut n je pocet klicu
 * */
typedef struct Node {
    int *keys;
    struct Node **children;
    bool leaf;
    int n;
} Node;

/**
 * Struktura BTree slouzi k reprezentaci B-stromu
 * atribut arity je pocet maximalnich potomku uzlu
 *  (stupen stromu je arity/2)
 * atribut root je ukazatel na koren stromu
 * atribut height je vyska stromu, je potrebna pro vykreslovani
 * */
typedef struct BTree {
    Node* root;
    int arity;
    int height;
} BTree;

/**
 * Nalokuje novy uzel s aritou, a flagom 'leaf',
 *  ktery znaci jestli je uzel list
 * Inicialne vsechny deti jsou NULL
 * Inicialne vsechny klice jsou INT_MAX
 * Inicialne je pocet klicu 'n' = 0
 * */
Node *mallocNode(int arity, bool leaf)
{
    Node *node = malloc(sizeof(Node));
    node->children = malloc(sizeof(Node*) * arity);
    for(int i = 0; i < arity;i++)
        node->children[i] = NULL;
    node->keys = malloc(sizeof(int) * (arity - 1));
    for(int i = 0; i < arity - 1; i++)
        node->keys[i]= INT_MAX;
    node->leaf = leaf;
    node->n = 0;
    return node;
}

/**
 * Vrati pocet klicu v B-strome s korenem 'node'
 **/
int nodeCount(Node* node)
{
    if(node == NULL) return 0;
    int sum = 0;
    sum += node->n;
    for (int j = 0; j < node->n + 1; ++j) {
        sum += nodeCount(node->children[j]);
    }
    return sum;
}


void inOrderHelp(Node* node, int* array, int* n) {
    if(node != NULL) {
        for(int i=0;i<node->n;i++) {
            inOrderHelp(node->children[i], array, n);
            array[*n] = node->keys[i];
            (*n)++;
        }
        inOrderHelp(node->children[node->n], array, n);
    }
}

/**
 * Vypise B-strom 'tree' pomoci inorder pruchodu
 * Hodnoty vypisujte do pole intu, ktere vratite
 * */
int *inOrderPrintBTree(BTree *tree)
{
    int count = nodeCount(tree->root);
    int *array = calloc(count,sizeof(int));
    int n=0;
    inOrderHelp(tree->root, array, &n);

    return array;
}

void preOrderHelp(Node* node, int* array, int* n) {
    if(node!=NULL) {
        for(int i=0;i<node->n;i++) {
            array[*n+i] = node->keys[i];
        }
        (*n)+=node->n;
        for(int i=0;i<node->n+1;i++)
            preOrderHelp(node->children[i], array, n);
    }
}

/**
 * Vypise B-strom 'tree' pomoci preorder pruchodu
 * Hodnoty vypisujte do pole intu, ktere vratite
 * */
int *preOrderPrintBTree(BTree *tree)
{
    int count = nodeCount(tree->root);
    int *array = calloc(count,sizeof(int));
    int n = 0;
    preOrderHelp(tree->root, array, &n);

    return array;
}

void postOrderHelp(Node* node, int* array, int* n) {
    if(node != NULL) {
        for(int i=0;i<=node->n;i++) {
            postOrderHelp(node->children[i], array, n);
        }
        for(int i=0;i<node->n;i++)
            array[*n+i] = node->keys[i];
        (*n)+=node->n;
    }
}

/**
 * Vypise B-strom 'tree' pomoci postorder pruchodu
 * Hodnoty vypisujte do pole intu, ktere vratite
 * */
int *postOrderPrintBTree(BTree *tree)
{
    int count = nodeCount(tree->root);
    int *array = calloc(count,sizeof(int));
    int n=0;
    postOrderHelp(tree->root, array, &n);

    return array;
}

int closestIndex(Node* node, int key, int begin, int end) {
    if(begin == end)
        return begin;
    int middle = (begin + end)/2;
    if(node->keys[middle] == key)
        return middle;
    if(node->keys[middle] > key) {
        if(middle!=begin)
            return closestIndex(node, key, begin, middle-1);
        else
            return begin;
    }
    else {
        if(middle!=end)
            return closestIndex(node, key, middle+1, end);
        else
            return end;
    }
}

Node* recursiveSearch(Node* node, int key) {
    if(node == NULL)
        return NULL;
    int index = closestIndex(node, key, 0, node->n-1);
    if(index < 0)
        return NULL;
    if(node->keys[index] == key)
        return node;
    else if(node->keys[index] > key)
        return recursiveSearch(node->children[index], key);
    else return recursiveSearch(node->children[index+1], key);
}

/**
 * Vyhleda uzel s klicem 'key' v B-strome 'tree'
 * Vrati uzel, ve kterem se nachazi klic
 * Pokud se klic 'key' v B-strome nenachazi, vraci NULL
 * */
Node *searchBTree(BTree *tree, int key)
{
    return recursiveSearch(tree->root, key);
}

bool recursiveCmp(Node* node1, Node* node2) {
    bool result = true;
    if((node1 == NULL && node2 != NULL) || (node1 != NULL && node2 == NULL))
        result = false;
    if(node1->n == node2->n) {
        for(int i=0;i<node1->n;i++)
            if(node1->keys[i]!=node2->keys[i])
                result = false;
        if(result && !node1->leaf)
            for(int i=0;i<node1->n+1;i++)
                result *= recursiveCmp(node1->children[i], node2->children[i]);
    } else
        result = false;
    return result;
}


/**
 * Overi, jestli jsou 2 stromy ekvivalentni
 * Pokud ano, vraci true, jinak false
 * */
bool isEquivBTree(BTree *tree1, BTree *tree2)
{
    if(tree1->height != tree2->height)
        return false;
    return recursiveCmp(tree1->root, tree2->root);
}


void fork(BTree* tree, Node* node, Node* full, bool root) {
    if(root) {
        Node* newRoot = mallocNode(tree->arity, false);
        Node* right = mallocNode(tree->arity, full->leaf);
        newRoot->keys[0] = full->keys[tree->arity/2-1];
        for(int i=tree->arity/2;i<full->n;i++) {
            right->keys[i - tree->arity/2] = full->keys[i];
            right->children[i-tree->arity/2] = full->children[i];
        }
        right->children[tree->arity/2-1] = full->children[full->n];
        full->n = tree->arity/2 -1;
        right->n = tree->arity/2 -1;
        newRoot->n = 1;
        newRoot->children[0] = full;
        newRoot->children[1] = right;
        tree->root = newRoot;
    } else {
        Node* right = mallocNode(tree->arity, full->leaf);
        for(int i=tree->arity/2;i<full->n;i++) {
            right->keys[i - tree->arity/2] = full->keys[i];
            right->children[i - tree->arity/2] = full->children[i];
        }
        right->children[tree->arity/2] = full->children[full->n];
        int i=node->n;
        while(node->keys[i-1] > full->keys[tree->arity/2-1]) {
            node->keys[i] = node->keys[i-1];
            node->children[i+1] = node->children[i];
            i--;
        }
        node->keys[i] = full->keys[tree->arity/2-1];
        node->children[i] = full;
        node->children[i+1] = right;
        node->n += 1;
        full->n = tree->arity/2-1;
        right->n = tree->arity -2 -full->n;
    }
}


void insertRecursive(BTree* tree, Node* node, int key) {
    int index = closestIndex(node, key, 0, node->n-1);

    if(!node->leaf) {
        if(node->keys[index] < key) {
            if(node->children[index+1]->n == tree->arity -1) {
                fork(tree, node, node->children[index+1], false);
                index = closestIndex(node, key, 0, node->n-1);
            }
        } else {
            if(node->children[index]->n == tree->arity -1) {
                fork(tree, node, node->children[index], false);
                index = closestIndex(node, key, 0, node->n-1);
            }
        }
    }
    if(node->leaf == true) {
        int i=0;
        while(node->keys[i] < key)
            i++;
        for(int j=node->n;j>i;j--) {
            node->keys[j] = node->keys[j-1];
        }
        node->keys[i] = key;
        (node->n)++;
    } else {
        if(node->keys[index]>key)
            insertRecursive(tree, node->children[index], key);
        else
            insertRecursive(tree, node->children[index+1], key);
    }
}

/**
 * Vlozi klic 'key' do B-stromu 'tree'
 * Operace implementuje preemptivne stepeni
 * Muzete predpokladat, ze strom ma sudou aritu
 *
 * Pro alokaci noveho uzlu pouzijte pripravenou funkci mallocNode
 * */

void insertBTree(BTree *tree, int key)
{
    if(tree->root == NULL) {
        tree->root = mallocNode(tree->arity, true);
        tree->root->n = 1;
        tree->root->keys[0] = key;
    } else {
        if(tree->root->n == tree->arity -1) {
            fork(tree, NULL, tree->root, true);
        }
        insertRecursive(tree, tree->root, key);
    }
}

/**
 * Dodatek k graphvizu:
 * Graphviz je nastroj, ktery vam umozni vizualizaci datovych struktur,
 * coz se hodi predevsim pro ladeni.
 * Tento program generuje nekolik souboru neco.dot v mainu
 * Vygenerovane soubory nahrajte do online nastroje pro zobrazeni graphvizu:
 * http://sandbox.kidstrythisathome.com/erdos/
 * nebo http://graphviz-dev.appspot.com/ - zvlada i vetsi grafy
 *
 * Alternativne si muzete nainstalovat prekladac z jazyka dot do obrazku na svuj pocitac.
 **/
void makeGraphviz(Node* node, int arity, FILE* outputFile, int i) {
    if(node == NULL) return;
    fprintf(outputFile, "node%i[label = \"", i);
    int j = 0;
    for (j = 0; j < node->n; ++j) {
        fprintf(outputFile, "<f%i> |%i| ", j, node->keys[j]);
    }
    fprintf(outputFile, "<f%i>\"];\n", j);
    for (int j = 0; j < node->n + 1; ++j) {
        makeGraphviz(node->children[j], arity, outputFile, (i+1)*arity+j);
    }
    if(node->leaf) return;
    for (int j = 0; j < node->n + 1; ++j) {
        fprintf(outputFile, "\"node%i\":f%i -> \"node%i\"\n", i, j, (i+1)*arity+j);
    }
}

void makeGraph(BTree* tree, const char* filename) {
    FILE* outputFile;
    outputFile = fopen(filename , "w");
    fprintf(outputFile, "digraph BTree {\n");
    fprintf(outputFile, "node [shape = record,height=.%i];\n", tree->height-1);
    makeGraphviz(tree->root, tree->arity, outputFile, 0);
    fprintf(outputFile, "}\n");
    fclose(outputFile);
}

Node *createNode(int *keys, int nkeys, Node **children, int nchildren, int arity, bool leaf)
{
    Node *node = malloc(sizeof(Node));
    node->children = malloc(sizeof(Node*) * arity);
    for (int i = 0; i < arity; i++)
        node->children[i] = (i < nchildren) ? children[i] : NULL;
    node->keys = malloc(sizeof(int) * (arity - 1));
    for (int i = 0; i < arity - 1; i++)
        node->keys[i] = (i < nkeys) ? keys[i] : INT_MAX;
    node->leaf = leaf;
    node->n = nkeys;
    return node;
}

void freeNode(Node* node)
{
    if (node != NULL) {
        for (int j = 0; j < node->n + 1; ++j) {
            freeNode(node->children[j]);
        }
        free(node->children);
        free(node->keys);
        free(node);
    }
}

void freeTree(BTree *tree)
{
    freeNode(tree->root);
    free(tree);
}

BTree *testTree1()
{
    BTree *tree = malloc(sizeof(BTree));
    tree->arity = 6;

    int n1k[5] = {1, 8, 12, 16, 25};
    Node* n1 = createNode(n1k, 5, NULL, 0, tree->arity, true);

    tree->root = n1;
    tree->height = 1;

    return tree;
}

BTree *testTree2()
{
    BTree *tree = malloc(sizeof(BTree));
    tree->arity = 4;

    int n12k[2] = {55, 75};
    Node* n12 = createNode(n12k, 2, NULL, 0, tree->arity, true);
    int n11k[1] = {25};
    Node* n11 = createNode(n11k, 1, NULL, 0, tree->arity, true);
    int n10k[1] = {17};
    Node* n10 = createNode(n10k, 1, NULL, 0, tree->arity, true);
    int n9k[2] = {14, 15};
    Node* n9 = createNode(n9k, 2, NULL, 0, tree->arity, true);
    int n8k[3] = {9, 10, 12};
    Node* n8 = createNode(n8k, 3, NULL, 0, tree->arity, true);
    int n7k[2] = {6, 7};
    Node* n7 = createNode(n7k, 2, NULL, 0, tree->arity, true);
    int n6k[1] = {3};
    Node* n6 = createNode(n6k, 1, NULL, 0,tree->arity, true);
    int n5k[2] = {0,1};
    Node* n5 = createNode(n5k, 2, NULL, 0,tree->arity, true);
    int n4k[3] = {16, 18, 50}; Node* n4ch[4] = {n9, n10, n11, n12};
    Node* n4 = createNode(n4k, 3, n4ch, 4, tree->arity, false);
    int n3k[1] = {8}; Node* n3ch[2] = {n7, n8};
    Node* n3 = createNode(n3k, 1, n3ch, 2, tree->arity, false);
    int n2k[1] = {2}; Node* n2ch[2] = {n5, n6};
    Node* n2 = createNode(n2k, 1, n2ch, 2,tree->arity, false);
    int n1k[2] = {5, 13}; Node* n1ch[3] = {n2, n3, n4};
    Node* n1 = createNode(n1k, 2, n1ch, 3, tree->arity, false);

    tree->root = n1;
    tree->height = 2;

    return tree;
}

BTree *testTree3()
{
    BTree *tree = malloc(sizeof(BTree));
    tree->arity = 8;

    int n9k[7] = {66, 67, 68, 69, 70, 73, 79};
    Node* n9 = createNode(n9k, 7, NULL, 0, tree->arity, true);
    int n8k[7] = {40, 42, 47, 48, 50, 52, 56};
    Node* n8 = createNode(n8k, 7, NULL, 0, tree->arity, true);
    int n7k[3] = {36, 37, 38};
    Node* n7 = createNode(n7k, 3, NULL, 0, tree->arity, true);
    int n6k[5] = {29, 31, 32, 33, 34};
    Node* n6 = createNode(n6k, 5, NULL, 0,tree->arity, true);
    int n5k[3] = {23, 24, 27};
    Node* n5 = createNode(n5k, 3, NULL, 0,tree->arity, true);
    int n4k[4] = {18, 19, 20, 21};
    Node* n4 = createNode(n4k, 4, NULL, 0, tree->arity, true);
    int n3k[3] = {13, 15, 16};
    Node* n3 = createNode(n3k, 3, NULL, 0, tree->arity, true);
    int n2k[3] = {1, 3, 8};
    Node* n2 = createNode(n2k, 3, NULL, 0,tree->arity, true);
    int n1k[8] = {12, 17, 22, 28, 35, 39, 65}; Node* n1ch[8] = {n2, n3, n4, n5, n6,
                                                                n7, n8, n9};
    Node* n1 = createNode(n1k, 7, n1ch, 8, tree->arity, false);

    tree->root = n1;
    tree->height = 1;

    return tree;
}

BTree *testTree4()
{
    BTree *tree = malloc(sizeof(BTree));
    tree->arity = 4;

    int n12k[2] = {55, 75};
    Node* n12 = createNode(n12k, 2, NULL, 0, tree->arity, true);
    int n11k[1] = {25};
    Node* n11 = createNode(n11k, 1, NULL, 0, tree->arity, true);
    int n10k[1] = {17};
    Node* n10 = createNode(n10k, 1, NULL, 0, tree->arity, true);
    int n9k[2] = {14, 15};
    Node* n9 = createNode(n9k, 2, NULL, 0, tree->arity, true);
    int n8k[3] = {9, 10, 12};
    Node* n8 = createNode(n8k, 3, NULL, 0, tree->arity, true);
    int n7k[2] = {5, 7};
    Node* n7 = createNode(n7k, 2, NULL, 0, tree->arity, true);
    int n6k[1] = {3};
    Node* n6 = createNode(n6k, 1, NULL, 0,tree->arity, true);
    int n5k[2] = {0,1};
    Node* n5 = createNode(n5k, 2, NULL, 0,tree->arity, true);
    int n4k[3] = {16, 19, 50}; Node* n4ch[4] = {n9, n10, n11, n12};
    Node* n4 = createNode(n4k, 3, n4ch, 4, tree->arity, false);
    int n3k[1] = {8}; Node* n3ch[2] = {n7, n8};
    Node* n3 = createNode(n3k, 1, n3ch, 2, tree->arity, false);
    int n2k[1] = {2}; Node* n2ch[2] = {n5, n6};
    Node* n2 = createNode(n2k, 1, n2ch, 2,tree->arity, false);
    int n1k[2] = {5, 13}; Node* n1ch[3] = {n2, n3, n4};
    Node* n1 = createNode(n1k, 2, n1ch, 3, tree->arity, false);

    tree->root = n1;
    tree->height = 2;

    return tree;
}

bool compareKeys(int* a1, int* a2, int n)
{
    if (a2 == NULL || a1 == NULL)
        return a1 == a2;
    for(int i = 0; i < n; i++)
        if (a1[i] != a2[i]) return false;
    return true;
}

void testInOrderPrint()
{
    printf("###########################\n");
    printf("Test 1. in_order_print: ");

    int res1[5] = {1, 8, 12, 16, 25};
    int res2[21] = {0,1,2,3,5,6,7,8,9,10,12,13,14,15,16,17,18,25,50,55,75};
    int res3[42] = {1, 3, 8, 12, 13, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 27,
                    28, 29, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 42, 47, 48,
                    50, 52, 56, 65, 66, 67, 68, 69, 70, 73, 79};

    BTree *tree = testTree1();

    int* res = inOrderPrintBTree(tree);

    if (!compareKeys(res1, res, 5)) {
        printf("NOK\n");
        printf("vysledek:\t   ");
        if (res != NULL) {
            for(int i = 0; i < 5; i++) printf("%d ",res[i]); putchar('\n');
        }else{
            printf("[]\n");
        }
        printf("ocekavany vysledek:");
        for(int i = 0; i < 5; i++) printf("%d ",res1[i]); putchar('\n');
    } else {
        free(res);
        freeTree(tree);
        tree = testTree2();
        res = inOrderPrintBTree(tree);
        if (!compareKeys(res2, res, 21)){
            printf("NOK\n");
            printf("vysledek:\t   ");
            if (res != NULL) {
                for(int i = 0; i < 21; i++) printf("%d ",res[i]); putchar('\n');
            }else{
                printf("[]\n");
            }
            printf("ocekavany vysledek:");
            for(int i = 0; i < 21; i++) printf("%d ",res2[i]); putchar('\n');
        }else{
            free(res);
            freeTree(tree);
            tree = testTree3();
            res = inOrderPrintBTree(tree);
            if (!compareKeys(res3, res, 42)){
                printf("NOK\n");
                printf("vysledek:\t   ");
                if (res != NULL) {
                    for(int i = 0; i < 42; i++) printf("%d ",res[i]); putchar('\n');
                }else{
                    printf("[]\n");
                }
                printf("ocekavany vysledek:");
                for(int i = 0; i < 42; i++) printf("%d ",res3[i]); putchar('\n');
            }else{
                printf("OK\n");
            }
        }
    }

    makeGraph(tree, "in_order_print.dot");
    printf("Vykresleny B-strom najdete v souboru in_order_print.dot\n");
    free(res);
    freeTree(tree);
}

void testPreOrderPrint()
{
    printf("###########################\n");
    printf("Test 2. pre_order_print: ");

    int res1[5] = {1, 8, 12, 16, 25};
    int res2[21] = {5, 13, 2, 0, 1, 3, 8, 6, 7, 9, 10, 12, 16, 18, 50, 14, 15,
                    17, 25, 55, 75};
    int res3[42] = {12, 17, 22, 28, 35, 39, 65, 1, 3, 8, 13, 15, 16, 18, 19, 20,
                    21, 23, 24, 27, 29, 31, 32, 33, 34, 36, 37, 38, 40, 42, 47,
                    48, 50, 52, 56, 66, 67, 68, 69, 70, 73, 79};

    BTree *tree = testTree1();

    int* res = preOrderPrintBTree(tree);

    if (!compareKeys(res1, res, 5)) {
        printf("NOK\n");
        printf("vysledek:\t   ");
        if (res != NULL) {
            for(int i = 0; i < 5; i++) printf("%d ",res[i]); putchar('\n');
        }else{
            printf("[]\n");
        }
        printf("ocekavany vysledek:");
        for(int i = 0; i < 5; i++) printf("%d ",res1[i]); putchar('\n');
    } else {
        free(res);
        freeTree(tree);
        tree = testTree2();
        res = preOrderPrintBTree(tree);
        if (!compareKeys(res2, res, 21)){
            printf("NOK\n");
            printf("vysledek:\t   ");
            if (res != NULL) {
                for(int i = 0; i < 21; i++) printf("%d ",res[i]); putchar('\n');
            }else{
                printf("[]\n");
            }
            printf("ocekavany vysledek:");
            for(int i = 0; i < 21; i++) printf("%d ",res2[i]); putchar('\n');
        }else{
            free(res);
            freeTree(tree);
            tree = testTree3();
            res = preOrderPrintBTree(tree);
            if (!compareKeys(res3, res, 42)){
                printf("NOK\n");
                printf("vysledek:\t   ");
                if (res != NULL) {
                    for(int i = 0; i < 42; i++) printf("%d ",res[i]); putchar('\n');
                }else{
                    printf("[]\n");
                }
                printf("ocekavany vysledek:");
                for(int i = 0; i < 42; i++) printf("%d ",res3[i]); putchar('\n');
            }else{
                printf("OK\n");
            }
        }
    }

    makeGraph(tree, "pre_order_print.dot");
    printf("Vykresleny B-strom najdete v souboru pre_order_print.dot\n");
    free(res);
    freeTree(tree);
}

void testPostOrderPrint()
{
    printf("###########################\n");
    printf("Test 3. post_order_print: ");

    int res1[5] = {1, 8, 12, 16, 25};
    int res2[21] = {0, 1, 3, 2, 6, 7, 9, 10, 12, 8, 14, 15, 17, 25, 55, 75, 16,
                    18, 50, 5, 13};
    int res3[42] = {1, 3, 8, 13, 15, 16, 18, 19, 20, 21, 23, 24, 27, 29, 31, 32,
                    33, 34, 36, 37, 38, 40, 42, 47, 48, 50, 52, 56, 66, 67, 68,
                    69, 70, 73, 79, 12, 17, 22, 28, 35, 39, 65};

    BTree *tree = testTree1();

    int* res = postOrderPrintBTree(tree);

    if (!compareKeys(res1, res, 5)) {
        printf("NOK\n");
        printf("vysledek:\t   ");
        if (res != NULL) {
            for(int i = 0; i < 5; i++) printf("%d ",res[i]); putchar('\n');
        }else{
            printf("[]\n");
        }
        printf("ocekavany vysledek:");
        for(int i = 0; i < 5; i++) printf("%d ",res1[i]); putchar('\n');
    } else {
        free(res);
        freeTree(tree);
        tree = testTree2();
        res = postOrderPrintBTree(tree);
        if (!compareKeys(res2, res, 21)){
            printf("NOK\n");
            printf("vysledek:\t   ");
            if (res != NULL) {
                for(int i = 0; i < 21; i++) printf("%d ",res[i]); putchar('\n');
            }else{
                printf("[]\n");
            }
            printf("ocekavany vysledek:");
            for(int i = 0; i < 21; i++) printf("%d ",res2[i]); putchar('\n');
        }else{
            free(res);
            freeTree(tree);
            tree = testTree3();
            res = postOrderPrintBTree(tree);
            if (!compareKeys(res3, res, 42)){
                printf("NOK\n");
                printf("vysledek:\t   ");
                if (res != NULL) {
                    for(int i = 0; i < 42; i++) printf("%d ",res[i]); putchar('\n');
                }else{
                    printf("[]\n");
                }
                printf("ocekavany vysledek:");
                for(int i = 0; i < 42; i++) printf("%d ",res3[i]); putchar('\n');
            }else{
                printf("OK\n");
            }
        }
    }

    makeGraph(tree, "post_order_print.dot");
    printf("Vykresleny B-strom najdete v souboru post_order_print.dot\n");
    free(res);
    freeTree(tree);
}

bool inKeys(int k, Node *node)
{
    int i = 0;
    while (i < node->n) {
        if (k == node->keys[i]) return true;
        i++;
    }
    return false;
}

void testSearch()
{
    printf("###########################\n");
    printf("Test 4. search: ");
    BTree *tree = testTree1();
    Node *node = searchBTree(tree, 16);
    if ((node == NULL) || (!inKeys(16, node))) {
        printf("NOK - chybne hledani klice 16, ktery je v koreni B-stromu\n");
    }else{
        node = searchBTree(tree, 24);
        if (node != NULL){
            printf("NOK - chybne hledani klice, ktery se v B-strome nenachazi\n");
        }else{
            freeTree(tree);
            tree = testTree2();
            node = searchBTree(tree, 15);
            if ((node == NULL) || (!inKeys(15, node))){
                printf("NOK - chybne hledani klice 15, ktery je v listu\n");
            }else{
                node = searchBTree(tree, 50);
                if ((node == NULL) || (!inKeys(50, node))){
                    printf("NOK - chybne hledani klice 50, ktery je ve vnitrnim uzlu\n");
                }else{
                    node = searchBTree(tree, 19);
                    if (node != NULL){
                        printf("NOK - chybne hledani klice, ktery se v B-strome nenachazi\n");
                    }else{
                        printf("OK\n") ;
                    }
                }
            }
        }
    }
    makeGraph(tree, "search.dot");
    printf("Vykresleny B-strom najdete v souboru search.dot\n");
    freeTree(tree);
}

void testIsEquiv()
{
    printf("###########################\n");
    printf("Test 5. is_equiv: ");
    BTree *t1 = testTree1();
    BTree *t2 = testTree2();
    if (isEquivBTree(t1, t2)){
        printf("NOK - B-stromy nejsou ekvivalentni, nemaji shodnou aritu\n");
    }else{
        freeTree(t1); freeTree(t2);
        t1 = testTree2();
        t2 = testTree4();
        if (isEquivBTree(t1, t2)){
            printf("NOK - B-stromy nejsou ekvivalentni, nemaji shodne hodnoty\n");
        }else{
            freeTree(t1);
            t1 = testTree2();
            if (isEquivBTree(t1, t1)){
                printf("OK\n");
            }else{
                printf("NOK - B-stromy jsou ekvivalentni\n");
            }
        }
    }
    freeTree(t1); freeTree(t2);
}

void testInsert()
{
    printf("###########################\n");
    printf("Test 6. insert: ");
    BTree *tree = malloc(sizeof(BTree));
    tree->root = NULL;
    tree->arity = 4;
    tree->height = 0;

    insertBTree(tree, 1);
    int a1[1] = {1};
    if ((tree->root == NULL) || (!compareKeys(tree->root->keys,a1, 1))){
        printf("NOK - vkladani do prazdneho B-stromu stupne 2\n");
    }else{
        insertBTree(tree, 7);
        insertBTree(tree, 2);
        int a2[3] = {1,2,7};
        if((tree->root == NULL) || (!compareKeys(tree->root->keys,a2, 3))){
            printf("NOK - vkladani do B-stromu bez stepeni\n");
        }else{
            int a3_1[1] = {2}; int a3_2[1] = {1}; int a3_3[2] = {5, 7};
            insertBTree(tree,5);
            if(tree->root == NULL ||
                    (!compareKeys(tree->root->keys,a3_1, 1)) ||
                    (!compareKeys(tree->root->children[0]->keys,a3_2, 1)) ||
                    (!compareKeys(tree->root->children[1]->keys,a3_3, 2))){
                printf("NOK - vkladani se stepenim korene\n");
            }else{
                insertBTree(tree, 12);
                insertBTree(tree, 8);
                int a4_1[2] = {2,7}; int a4_2[1] = {1}; int a4_3[1] = {5}; int a4_4[2] = {8, 12};
                if(tree->root == NULL ||
                        (!compareKeys(tree->root->keys,a4_1, 2)) ||
                        (!compareKeys(tree->root->children[0]->keys,a4_2, 1)) ||
                        (!compareKeys(tree->root->children[1]->keys,a4_3, 1)) ||
                        (!compareKeys(tree->root->children[2]->keys,a4_4, 2))){
                    printf("NOK - vkladani se stepenim listu\n");
                }else{
                    insertBTree(tree, 4);
                    insertBTree(tree, 3);
                    insertBTree(tree, 6);
                    int a5_1[3] = {2,4,7}; int a5_2[1] = {3}; int a5_3[2] = {5, 6}; int a5_4[2] = {8, 12};
                    if(tree->root == NULL ||
                            (!compareKeys(tree->root->keys,a5_1, 3)) ||
                            (!compareKeys(tree->root->children[1]->keys,a5_2, 1)) ||
                            (!compareKeys(tree->root->children[2]->keys,a5_3, 2)) ||
                            (!compareKeys(tree->root->children[3]->keys,a5_4, 2))){
                        printf("NOK - vkladani se stepenim listu\n");
                    }else{
                        insertBTree(tree, 11);
                        int a6_1[1] = {4}; int a6_2[1] = {2}; int a6_3[1] = {7}; int a6_4[2] = {5, 6}; int a6_5[3] = {8, 11, 12};
                        if(tree->root == NULL ||
                                (!compareKeys(tree->root->keys,a6_1, 1)) ||
                                (!compareKeys(tree->root->children[0]->keys,a6_2, 1)) ||
                                (!compareKeys(tree->root->children[1]->keys,a6_3, 1)) ||
                                (!compareKeys(tree->root->children[1]->children[0]->keys,a6_4, 2)) ||
                                (!compareKeys(tree->root->children[1]->children[1]->keys,a6_5, 3))){
                            printf("NOK - vkladani se stepenim korene\n");
                        }else{
                            printf("OK\n");
                        }
                    }
                }
            }
        }
    }
    makeGraph(tree, "insert.dot");
    printf("Vykresleny B-strom najdete v souboru insert.dot\n");
    freeTree(tree);
}

int main()
{
    testInOrderPrint();
    testPreOrderPrint();
    testPostOrderPrint();
    testSearch();
    testIsEquiv();
    testInsert();
    printf("###########################\n");
}
