#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


/**
 * @brief Struktura Node slouzi k reprezentaci uzlu ve strome
 * atribut key je klic daneho uzlu
 * atribut parent je ukazatel na rodice uzlu (NULL pro koren stromu)
 * atribut left je ukazatel na leveho potomka (NULL pokud neexistuje)
 * atribut right je ukazatel na praveho potomka
 * */
typedef struct Node {
    int key;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
} Node;

/**
 *  @brief Struktura BinarySearchTree slouzi k reprezentaci
 *  binarniho vyhledavaciho stromu
 *  atribut root je reference na korenovy uzel typu Node
 * */
typedef struct BinarySearchTree {
    Node* root;
} BinarySearchTree;

/**
 * @brief whereInsert - najde prvok ktory by sa mal stat otcom vkladaneho prvku
 * @param parent - otec podstromu v ktorom hladame
 * @param key - key vkladaneho prvku
 * @return - poiner na buduceho otca :)
 */

Node* whereInsert(Node *parent, int key) {
    if(parent == NULL)
        return NULL;
    if(key >= parent->key) {
        if(parent->right != NULL)
            return whereInsert(parent->right, key);
        else return parent;
    } else {
        if(parent->left !=NULL)
            return whereInsert(parent->left, key);
        else return parent;
    }
}


/**
 * @brief Vlozi novy uzel s klicem 'key' do stromu 'tree'
 * */
void insertNode(BinarySearchTree *tree, int key) {
    Node* toAdd = malloc(sizeof(Node));
    toAdd->key = key;
    Node* findPos = whereInsert(tree->root, key);

    if(findPos!=NULL) {
        toAdd->parent = findPos;
        if(key<findPos->key)
            findPos->left = toAdd;
        else
            findPos->right = toAdd;
    } else {
        tree->root = toAdd;
    }
}



/**
 * Vyhleda uzel s klicem 'key' ve strome 'tree'
 * Vrati uzel s hledanym klicem
 * Pokud se klic 'key' ve strome nenachazi, vraci NULL
 * */

Node *searchNode(BinarySearchTree *tree, int key) {
    BinarySearchTree* nextTree=malloc(sizeof(BinarySearchTree));
    if(tree->root == NULL)
        return NULL;

    if(key == tree->root->key)
        return tree->root;
    else
        if(tree->root->key > key) {
            if(tree->root->left !=NULL)
                nextTree->root = tree->root->left;
            else return NULL;

        } else {
            if(tree->root->right != NULL)
                nextTree->root = tree->root->right;
            else return NULL;
        }
    return searchNode(nextTree, key);
}


Node* findReplacement(Node* beingRemoved) {
    Node* finder = NULL;
    if(beingRemoved != NULL && beingRemoved->right != NULL) {
        finder = beingRemoved->right;
        while(finder->left!=NULL)
            finder = finder->left;
    } else {
        finder = beingRemoved->left;
        while(finder!=NULL && finder->right!=NULL)
            finder = finder->right;
    }
    return finder;
}



/**
 * @brief Smaze uzel 'node' ve strome 'tree'
 * Obnovi vlastnost vyhledavaciho stromu
 * */
void deleteNode(BinarySearchTree *tree, Node *node) {
    Node* replacement = findReplacement(node);
    if(replacement == NULL) {
        if(node->parent->left == node)
            node->parent->left = NULL;
        else
            node->parent->right = NULL;
    } else {
        if(replacement == node->left) {
            if(node->parent->left == node)
                node->parent->left = node->left;
            else
                node->parent->right = node->left;
            replacement->parent = node->parent;
        } else {
            if(replacement->parent != node) {
                replacement->parent->left = replacement->right;
                replacement->right = node->right;
                replacement->left = node->left;
                if(replacement->left!=NULL)
                    replacement->left->parent = replacement;
                if(replacement->right!=NULL)
                    replacement->right->parent = replacement;
            } else {
                replacement->left = node->left;
                if(replacement->left !=NULL)
                    replacement->left->parent = replacement;
                replacement->parent = node->parent;
            }
            if(node->parent!=NULL) {
                if(node->parent->left == node)
                    node->parent->left = replacement;
                else
                    node->parent->right = replacement;
            } else {
                tree->root = replacement;
            }
            replacement->parent = node->parent;
        }

    }
    free(node);

}

int max(int a, int b) {
    if(a>b)
        return a;
    else return b;
}

int recursiveHeightHelp(Node* node) {
    if(node != NULL) {
        int maximum =1;
        return maximum + max(recursiveHeightHelp(node->left),
                recursiveHeightHelp(node->right));
    }
    return 0;
}


/**
 * @brief Vraci vysku stromu 'tree'
 * */
int height(BinarySearchTree *tree) {
    int height = 0;
    Node* researcher = tree->root;
    if(researcher) {
        height = recursiveHeightHelp(researcher);
    }
    return height;
}

int treeCorrectnessRecursive(Node* parent, int low, int high) {
    if(parent == NULL)
        return 1;
    if(parent->key < low || parent->key > high)
        return 0;
    else return treeCorrectnessRecursive(parent->left, low, parent->key)*
            treeCorrectnessRecursive(parent->right, parent->key, high);
}

/**
 * @brief Overi jestli je strom 'tree' korektni binarni vyhledavaci strom
 * Pokud ano vraci 1, jinak 0
 * */
 int isCorrectBST(BinarySearchTree *tree) {
     if(treeCorrectnessRecursive(tree->root, INT_MIN, INT_MAX))
         return 1;
     return 0;
 }

/**
 * Dodatek k graphvizu:
 * Graphviz je nastroj, ktery vam umozni vizualizaci datovych struktur,
 * coz se hodi predevsim pro ladeni.
 * Tento program generuje nekolik souboru neco.dot v mainu
 * Vygenerovane soubory nahrajte do online nastroje pro zobrazeni graphvizu:
 * http://sandbox.kidstrythisathome.com/erdos/
 *
 * Alternativne si muzete nainstalovat prekladac z jazyka dot do obrazku na svuj pocitac.
 **/
 void makeGraphviz(Node* node, FILE* outputFile) {
    if(node == NULL) return;
    fprintf(outputFile, "\"%i\"\n", node->key);
    if(node->left != NULL) {
        fprintf(outputFile, "\"%i\" -> \"%i\"\n", node->key, node->left->key);
        makeGraphviz(node->left, outputFile);
    }
    if(node->right != NULL) {
        fprintf(outputFile, "\"%i\" -> \"%i\"\n", node->key, node->right->key);
        makeGraphviz(node->right, outputFile);
    }
}

void makeGraph(BinarySearchTree* tree, const char* filename) {
    FILE* outputFile;
    outputFile = fopen(filename , "w");
    fprintf(outputFile, "digraph Tree {\n");
    fprintf(outputFile, "node [color=lightblue2, style=filled];\n");
    makeGraphviz(tree->root, outputFile);
    fprintf(outputFile, "}\n");
    fclose(outputFile);
}

void freeNode(Node *node) {
    if (node == NULL) return;

    freeNode(node->left);
    freeNode(node->right);

    free(node);
}

void freeTree(BinarySearchTree *tree) {
    freeNode(tree->root);
    free(tree);
}

void testInsert() {
    printf("Test 1. insert: ");
    BinarySearchTree *tree = malloc(sizeof(BinarySearchTree));

    tree->root = NULL;

    insertNode(tree, 3);

    if ((tree->root == NULL) || (tree->root->key != 3)) {
        printf("NOK - chybne vkladani do prazdneho stromu\n");
    } else {
        insertNode(tree, 1);

        if ((tree->root->key != 3) || (tree->root->left->key != 1)) {
            printf("NOK - chybne vkladani do leveho podstromu\n");
        } else {
            insertNode(tree,5);
            if ((tree->root->key != 3) || (tree->root->right->key != 5)) {
                printf("NOK - chybne vkladani do praveho podstromu\n");
            }else{
                insertNode(tree,2);
                if (tree->root->left->right->key != 2) {
                    printf("NOK - chybne vkladani do leveho podstromu\n");
                }else{
                    insertNode(tree,4);
                    if (tree->root->right->left->key != 4) {
                        printf("NOK - chybne vkladani do praveho podstromu\n");
                    }else{
                        printf("OK\n");
                    }
                }
            }
        }
    }

    makeGraph(tree, "builded.dot");
    printf("Vykresleny strom najdete v souboru insert.dot\n");

    freeTree(tree);
}

BinarySearchTree *initTestTree() {
    BinarySearchTree *tree = malloc(sizeof(BinarySearchTree));

    Node *nodes[7];
    for(int i = 0; i < 7; i++) {
        Node *node = malloc(sizeof(Node));
        node->key = i;
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        nodes[i] = node;
    }

    tree->root = nodes[3];

    tree->root->left = nodes[1];
    nodes[1]->parent = tree->root;
    nodes[1]->left = nodes[0];
    nodes[0]->parent = nodes[1];
    nodes[1]->right = nodes[2];
    nodes[2]->parent = nodes[1];

    tree->root->right = nodes[5];
    nodes[5]->parent = tree->root;
    nodes[5]->left = nodes[4];
    nodes[4]->parent = nodes[5];
    nodes[5]->right = nodes[6];
    nodes[6]->parent = nodes[5];

    return tree;
}

void testDelete() {
    printf("Test 2. delete: ");
    BinarySearchTree *tree = initTestTree();

    deleteNode(tree, tree->root->left->left);

    if ((tree->root->left->key != 1) ||(tree->root->left->left != NULL)) {
        printf("NOK - chybne mazani listu\n");
    }else{
        deleteNode(tree, tree->root);
        if ((tree->root == NULL) ||
            (tree->root->key != 4) ||
            (tree->root->left->key != 1) ||
            (tree->root->left->left != NULL)) {
                printf("NOK - chybne mazani korenu\n");
        }else{
            deleteNode(tree, tree->root->left);
            if (tree->root->left->key != 2) {
                printf("NOK - chybne mazani uzlu v levem podstrome\n");
            }else{
                printf("OK\n");
            }
        }
    }

    makeGraph(tree, "delete.dot");
    printf("Vykresleny strom najdete v souboru delete.dot\n");

    freeTree(tree);
}

void testSearch() {
    printf("Test 3. search: ");
    BinarySearchTree *tree = initTestTree();

    Node *node = searchNode(tree, 3);

    if ((node == NULL) || (node->key != 3)) {
        printf("NOK - chybne hledani korene s hodnotou 3\n");
    }else{
        node = searchNode(tree, 2);
        if ((node == NULL) || (node->key != 2)) {
            printf("NOK - chybne hledani listu s hodnotou 2\n");
        }else{
            node = searchNode(tree, 7);
            if (node != NULL) {
                printf("NOK - hledany prvek 7 se ve strome nevyskytuje\n");
            }else{
                printf("OK\n");
            }
        }
    }

    makeGraph(tree, "search.dot");
    printf("Vykresleny strom najdete v souboru search.dot\n");

    freeTree(tree);
}

void testHeight() {
    printf("Test 4. height: ");
    BinarySearchTree *tree = initTestTree();
    int h = height(tree);
    if (h != 3) {
        printf("NOK - vyska 3 != vase vyska %d\n", h);
    }else{
        Node *n = malloc(sizeof(Node));
        n->key = 1.5;
        n->left = NULL;
        n->right = NULL;
        n->parent = NULL;
        tree->root->left->right->left = n;
        n->parent = tree->root->left->right;
        h = height(tree);
        if (h != 4) {
            printf("NOK - vyska 3 != vase vyska %d\n", h);
        }else{
            printf("OK\n");
        }
    }

    makeGraph(tree, "height.dot");
    printf("Vykresleny strom najdete v souboru height.dot\n");

    freeTree(tree);
}

void testIsCorrectBST() {
    printf("Test 5. is_correct_bst: ");
    BinarySearchTree *tree = initTestTree();

    if (!isCorrectBST(tree)) {
        printf("NOK - strom je korektni binarni vyhledavaci strom\n");
    }else{
        tree->root->key = 0;
        free(tree->root->left->left);
        tree->root->left->left = NULL;
        if (isCorrectBST(tree)) {
            printf("NOK - strom neni korektni binarni vyhledavaci strom\n");
        }else{
            tree->root->key = 3;
            tree->root->left->right->key = 4;
            tree->root->right->left->key = 2;
            if (isCorrectBST(tree)){
                printf("NOK - strom neni korektni binarni vyhledavaci strom\n");
            }else{
                printf("OK\n");
            }
        }
    }

    makeGraph(tree, "correct.dot");
    printf("Vykresleny strom najdete v souboru correct.dot\n");

    freeTree(tree);
}

int main(void) {
    testInsert();
    testDelete();
    testSearch();
    testHeight();
    testIsCorrectBST();
}
