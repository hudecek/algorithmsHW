#include <stdio.h>
#include <stdlib.h>

typedef enum Color {
    RED, BLACK
} Color;

/**
 * @brief Struktura Node slouzi k reprezentaci uzlu v strome
 * atribut key klic daneho uzlu
 * atribut color muze nabyvat hodnoty 'RED' a 'BLACK'
 * atribut parent je reference na rodice uzlu
 * atribut left je reference na leveho potomka
 * atribut right je reference na praveho potomka
 * */
typedef struct Node {
    int key;
    Color color;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
} Node;

/**
 *  @brief Struktura RedBlackTree slouzi k reprezentaci
 *  binarniho vyhledavaciho stromu
 *  atribut root je reference na korenovy uzel typu Node
 * */
typedef struct RedBlackTree {
    Node* root;
} RedBlackTree;

/**
 * @brief Vykona rotaci doleva kolem uzlu 'rotationRoot' v strome 'tree'
 **/
void rotateLeft(RedBlackTree* tree, Node* rotationRoot) {
    //TODO pokuste se napsat medotu bez prepisu z pseudokodu, vyuzijte radsi tuzku a papir
    Node* toBeRoot = rotationRoot->right;
    if(toBeRoot!=NULL) {
        if(rotationRoot!=tree->root) {
            toBeRoot->parent = rotationRoot->parent;
            if(rotationRoot == rotationRoot->parent->right)
                rotationRoot->parent->right = toBeRoot;
            else rotationRoot->parent->left = toBeRoot;
        } else {
            tree->root = toBeRoot;
            toBeRoot->parent = NULL;
        }
        rotationRoot->parent = toBeRoot;
        rotationRoot->right = toBeRoot->left;
        if(toBeRoot->left!=NULL) {
            toBeRoot->left->parent = rotationRoot;
        }
        toBeRoot->left = rotationRoot;
    }
}

/**
 * @brief Vykona rotaci doprava kolem uzlu 'rotationRoot' v strome 'tree'
 **/
void rotateRight(RedBlackTree* tree, Node* rotationRoot) {
    //TODO pokuste se napsat medotu bez prepisu z pseudokodu, vyuzijte radsi tuzku a papir
    Node* toBeRoot = rotationRoot->left;
    if(toBeRoot!=NULL) {
        if(rotationRoot!=tree->root) {
            toBeRoot->parent = rotationRoot->parent;
            if(rotationRoot == rotationRoot->parent->left)
                rotationRoot->parent->left = toBeRoot;
            else rotationRoot->parent->right = toBeRoot;
        } else {
            tree->root = toBeRoot;
            toBeRoot->parent = NULL;
        }
        rotationRoot->parent = toBeRoot;
        rotationRoot->left = toBeRoot->right;
        if(toBeRoot->right!=NULL)
            toBeRoot->right->parent = rotationRoot;
        toBeRoot->right = rotationRoot;
    }
}

void initializeNode(Node* toInit) {
    toInit->color = BLACK;
    toInit->key = 0;
    toInit->left = NULL;
    toInit->right = NULL;
    toInit->parent = NULL;
}



int checkColor(Node* node, int counter) {
    if(node == NULL)
        return counter;
    if(node->color == BLACK)
        counter++;
    if(checkColor(node->left, counter) != checkColor(node->right, counter))
        return -1;
    if(node->color == RED) {
        if(node->left) {
            if(node->left->color == RED)
                return -2;
        }
        if(node->right) {
            if(node->right->color == RED)
                return -2;
        }
    }
    return counter;
}



Node* uncle(Node* nephew) {
    if(nephew->parent == NULL)
        return NULL;
    if(nephew->parent->left == nephew)
        return nephew->parent->right;
    return nephew->parent->left;
}

Node* grandparent(Node* grandson) {
    if(grandson->parent)
        return grandson->parent->parent;
    return NULL;
}

void insertCase2(RedBlackTree *tree, Node *added);
void insertCase3(RedBlackTree *tree, Node *added);
void insertCase4(RedBlackTree *tree, Node *added);
void insertCase5(RedBlackTree *tree, Node *added);

void insertCase1(RedBlackTree* tree, Node* added) {
    if(added->parent == NULL)
        added->color = BLACK;
    else insertCase2(tree, added);
}

void insertCase2(RedBlackTree* tree, Node* added) {
    if(added->parent->color == RED)
        insertCase3(tree, added);
}

void insertCase3(RedBlackTree* tree, Node* added) {
    Node* unc = uncle(added);
    if(unc && unc->color == RED) {
        added->parent->color = BLACK;
        unc->color = BLACK;
        Node* grandpa = grandparent(added);
        grandpa->color = RED;
        insertCase1(tree, grandpa);
    } else {
        insertCase4(tree, added);
    }
}

void insertCase4(RedBlackTree* tree, Node* added) {
    Node* grandpa = grandparent(added);

        if(added == added->parent->right && added->parent == grandpa->left) {
            rotateLeft(tree,added->parent);
            added = added->left;
        } else if(added == added->parent->left && added->parent == grandpa->right) {
            rotateRight(tree, added->parent);
            added = added->right;
        }
        insertCase5(tree, added);
}

void insertCase5(RedBlackTree* tree, Node* added) {
    Node* grandpa = grandparent(added);
    added->parent->color = BLACK;
    grandpa->color = RED;
    if(added == added->parent->left) {
        rotateRight(tree, grandpa);
    } else {
        rotateLeft(tree, grandpa);
    }
}

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
 * Operace zachova korektni cerveno-cerny strom
 **/
void insertNode(RedBlackTree *tree, int key) {
    Node* toAdd = malloc(sizeof(Node));
    initializeNode(toAdd);
    toAdd->key = key;
    Node* findPos = whereInsert(tree->root, key);

    if(findPos!=NULL) {
        toAdd->parent = findPos;
        if(key<findPos->key) {
            findPos->left = toAdd;
        }
        else {
            findPos->right = toAdd;
        }
    } else {
        tree->root = toAdd;
    }
    toAdd->color = RED;
    insertCase1(tree, toAdd);
}


/**
 * Vyhleda uzel s klicem 'key' v strome 'tree'
 * Vrati uzel s hledanym klicem
 * Pokud se klic 'key' v strome nenachazi vraci NULL
 * */
Node* searchNode(RedBlackTree* tree, int key){
    //TODO
    Node* search = tree->root;
    while(search!=NULL) {
        if(search->key > key)
            search = search->left;
        else if(search->key < key)
            search = search->right;
        else return search;
    }

    return NULL;
}

int treeCorrectness(Node* node) {
    if(node == NULL || (node->left == NULL && node->right == NULL))
        return 1;
    if(node->left!=NULL) {
        if(node->left->key >= node->key)
            return 0;
    }
    if(node->right!=NULL) {
        if(node->right->key < node->key)
            return 0;
    }
    return treeCorrectness(node->left)*treeCorrectness(node->right);
}

/**
 * @brief Overi jestli je strom 'tree' korektni cerveno-cerny vyhledavaci strom
 * Pokud ano vraci 1, jinak 0
 * */
int isCorrectRBTree(RedBlackTree* tree) {
    //TODO
    if(tree->root!=NULL)
        if(tree->root->color == RED)
            return 0;
    if(treeCorrectness(tree->root) && (checkColor(tree->root, 0)+1))
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
    if(node->color == RED) {
        fprintf(outputFile, "\"%i\" [color=red]\n", node->key);
    }
    else if(node->color == BLACK) {
        fprintf(outputFile, "\"%i\" [color=black, fontcolor=white]\n", node->key);
    }
    else {
        fprintf(outputFile, "\"%i\"\n", node->key);
    }
    if(node->left != NULL) {
        fprintf(outputFile, "\"%i\" -> \"%i\"\n", node->key, node->left->key);
        makeGraphviz(node->left, outputFile);
    } else {
        fprintf(outputFile, "L%p [label=\"\",color=white]\n%i -> L%p\n", (void *)node, node->key, (void *)node);
    }
    if(node->right != NULL) {
        fprintf(outputFile, "\"%i\" -> \"%i\"\n", node->key, node->right->key);
        makeGraphviz(node->right, outputFile);
    } else {
        fprintf(outputFile, "R%p [label=\"\",color=white]\n%i -> R%p\n", (void *)node, node->key, (void *)node);
    }
}

void makeGraph(Node* treeRoot, const char* filename) {
    FILE* outputFile;
    outputFile = fopen(filename , "w");
    fprintf(outputFile, "digraph RBTree {\n");
    fprintf(outputFile, "node [color=lightblue2, style=filled];\n");
    makeGraphviz(treeRoot, outputFile);
    fprintf(outputFile, "}\n");
    fclose(outputFile);
}

void freeNode(Node *node) {
    if (node == NULL) return;

    freeNode(node->left);
    freeNode(node->right);

    free(node);
}

void freeTree(RedBlackTree *tree) {
    freeNode(tree->root);
    free(tree);
}

RedBlackTree *initRBTree() {
    RedBlackTree *tree = malloc(sizeof(RedBlackTree));

    Node *nodes[7];
    for(int i = 0; i < 7; i++) {
        Node *node = malloc(sizeof(Node));
        node->key = i;
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->color = BLACK;
        nodes[i] = node;
    }

    tree->root = nodes[3];

    tree->root->left = nodes[1];
    nodes[1]->parent = tree->root;
    nodes[1]->color = RED;
    nodes[1]->left = nodes[0];
    nodes[0]->parent = nodes[1];
    nodes[1]->right = nodes[2];
    nodes[2]->parent = nodes[1];

    tree->root->right = nodes[5];
    nodes[5]->parent = tree->root;
    nodes[5]->left = nodes[4];
    nodes[5]->color = RED;
    nodes[4]->parent = nodes[5];
    nodes[5]->right = nodes[6];
    nodes[6]->parent = nodes[5];

    return tree;
}

RedBlackTree *initUnbalancedTreeRight() {
    RedBlackTree *tree = malloc(sizeof(RedBlackTree));

    Node *nodes[7];
    for(int i = 0; i < 7; i++) {
        Node *node = malloc(sizeof(Node));
        node->key = i;
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->color = BLACK;
        nodes[i] = node;
    }

    tree->root = nodes[0];

    for (int i = 1; i < 7; i++) {
        nodes[i]->parent = nodes[i - 1];
    }
    for (int i = 0; i < 6; i++) {
        nodes[i]->right = nodes[i + 1];
    }
    return tree;
}

RedBlackTree *initUnbalancedTreeLeft() {
    RedBlackTree *tree = malloc(sizeof(RedBlackTree));

    Node *nodes[7];
    for(int i = 0; i < 7; i++) {
        Node *node = malloc(sizeof(Node));
        node->key = i;
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->color = BLACK;
        nodes[i] = node;
    }

    tree->root = nodes[6];

    for (int i = 0; i < 6; i++) {
        nodes[i]->parent = nodes[i + 1];
    }
    for (int i = 1; i < 7; i++) {
        nodes[i]->left = nodes[i - 1];
    }
    return tree;
}


void testRotateLeft() {
    printf("Test 1. rotateLeft: ");
    RedBlackTree *tree = initUnbalancedTreeRight();
    rotateLeft(tree, tree->root);

    if (tree->root->key != 1 ||
            tree->root->left == NULL ||
            tree->root->left->key != 0 ||
            tree->root->right->key != 2) {
        printf("NOK - zla rotace kolem korene stromu\n");
    } else {
        Node *rnode = tree->root->right;
        rotateLeft(tree,rnode);

        if (tree->root->right->key != 3 ||
                tree->root->right->left == NULL ||
                tree->root->right->left->right != NULL ||
                tree->root->right->left->key != 2 ||
                tree->root->right->right->key != 4) {
            printf("NOK - zla rotace kolem uzlu stromu\n");
        } else {
            Node *rnode = tree->root->left;
            rotateLeft(tree, rnode);
            if (rnode->left != NULL ||
                    rnode->right != NULL ||
                    tree->root->key != 1 ||
                    tree->root->left->key != 0) {
                printf("NOK - zla rotace kolem uzlu bez potomku\n");
            }else{
                rotateLeft(tree, tree->root);
                if (tree->root->key != 3 ||
                        tree->root->right->key != 4 ||
                        tree->root->left->key != 1 ||
                        tree->root->left->right->key != 2) {
                    printf("NOK - zla rotace kolem korene stromu, testovane prevesovani potomku\n");
                }else{
                    printf("OK\n");
                }
            }
        }
    }
    makeGraph(tree->root, "rotateLeft.dot");
    printf("Vykresleny strom najdete v souboru rotateLeft.dot\n");

    freeTree(tree);
}

void testRotateRight(){
    printf("Test 2. rotateRight: ");
    RedBlackTree *tree = initUnbalancedTreeLeft();

    rotateRight(tree, tree->root);
    if (tree->root->key != 5 ||
            tree->root->right == NULL ||
            tree->root->right->key != 6 ||
            tree->root->left->key != 4) {
        printf("NOK - zla rotace kolem korene stromu\n");
    }else{
        Node *rnode = tree->root->left;
        rotateRight(tree,rnode);

        if (tree->root->left->key != 3 ||
                tree->root->left->right == NULL ||
                tree->root->left->right->left != NULL ||
                tree->root->left->left->key != 2){
            printf("NOK - zla rotace kolem uzlu stromu\n");
        }else{
            rnode = tree->root->right;
            rotateRight(tree, rnode);
            if (tree->root->right->left != NULL ||
                    tree->root->right->right != NULL ||
                    tree->root->key != 5 ||
                    tree->root->right->key != 6){
                printf("NOK - zla rotace kolem uzlu bez potomku\n");
            }else{
                rotateRight(tree, tree->root);
                if (tree->root->key != 3 ||
                        tree->root->left->key != 2 ||
                        tree->root->right->key != 5 ||
                        tree->root->right->left->key != 4){
                    printf("NOK - zla rotace kolem korene stromu, testovane prevesovani potomku\n");
                }else{
                    printf("OK\n");
                }
            }
        }
    }
    makeGraph(tree->root, "rotateRight.dot");
    printf("Vykresleny strom najdete v souboru rotateRight.dot\n");

    freeTree(tree);
}

void testInsert() {
    printf("Test 3. insertNode: ");

    RedBlackTree *tree = malloc(sizeof(RedBlackTree));
    tree->root = NULL;
    insertNode(tree, 5);

    if (tree->root == NULL ||
            tree->root->color != BLACK){
        printf("NOK - koren je cerveny\n");
    }else{
        insertNode(tree, 9);
        if (tree->root->right == NULL ||
                tree->root->right->color != RED ||
                tree->root->right->key != 9){
            printf("NOK - chybne vlozeny cerveny uzel\n");
        }else{
            insertNode(tree, 3);
            insertNode(tree, 4);
            if (tree->root->color != BLACK ||
                    tree->root->right->color != BLACK ||
                    tree->root->left->color != BLACK ||
                    tree->root->left->right->color != RED ||
                    tree->root->left->right->key != 4){
                printf("NOK - chybne prefarbovani a vkladani\n");
            }else{
                insertNode(tree, 6);
                insertNode(tree, 7);
                if (tree->root->right->key != 7 ||
                        tree->root->right->left->key != 6 ||
                        tree->root->right->right->key != 9 ||
                        tree->root->right->color != BLACK ||
                        tree->root->right->left->color != RED ||
                        tree->root->right->right->color != RED){
                    printf("NOK - chybna rotace vpravo s prefarbenim\n");
                }else{
                    insertNode(tree, 10);
                    Node *tnode = tree->root->right;
                    if (tnode->color != RED ||
                            tnode->left->color != BLACK ||
                            tnode->right->color != BLACK ||
                            tnode->right->right->color != RED ||
                            tnode->right->right->key != 10){
                        printf("NOK - chybne prefarbovani bez rotace\n");
                    }else{
                        insertNode(tree, 8);
                        insertNode(tree, 12);
                        Node *r = tree->root;
                        if (r->key != 7 ||
                                r->left->key != 5 ||
                                r->left->right->key != 6 ||
                                r->right->left->key != 8 ||
                                r->right->right->right->key != 12 ||
                                r->color != BLACK ||
                                r->left->color != RED ||
                                r->right->color != RED ||
                                r->left->right->color != BLACK ||
                                r->right->left->color != BLACK ||
                                r->right->right->right->color != RED){
                            printf("NOK - chybna rotace kolem k||ene s prefarbenim\n");
                        }else{
                            printf("OK\n");
                        }
                    }
                }
            }
        }
    }
    makeGraph(tree->root, "insert.dot");
    printf("Vykresleny strom najdete v souboru insert.dot\n");

    freeTree(tree);
}

void testSearch() {
    printf("Test 4. searchNode: ");
    RedBlackTree *tree = initRBTree();

    Node *node = searchNode(tree, 3);

    if ((node == NULL) || (node->key != 3)) {
        printf("NOK - chybny hledani korene s hodnotou 3\n");
    }else{
        node = searchNode(tree, 2);
        if ((node == NULL) || (node->key != 2)) {
            printf("NOK - chybny hledani listu s hodnotou 2\n");
        }else{
            node = searchNode(tree, 7);
            if (node != NULL) {
                printf("NOK - hledani prvku ktery se v strome nevyskytuje\n");
            }else{
                printf("OK\n");
            }
        }
    }

    makeGraph(tree->root, "search.dot");
    printf("Vykresleny strom najdete v souboru search.dot\n");

    freeTree(tree);
}

void testIsCorrectRBTree(){
    printf("Test 5. isCorrectRBTree: ");

    RedBlackTree* tree = initRBTree();
    if (! isCorrectRBTree(tree)){
        printf("NOK - strom je korektni\n");
    }else{
        tree->root->color = RED;
        if (isCorrectRBTree(tree)){
            printf("NOK - strom ma cerveny koren\n");
        }else{
            tree->root->color = BLACK;
            tree->root->left->color = BLACK;
            if (isCorrectRBTree(tree)){
                printf("NOK - strom nema stejnou cernou hloubku\n");
            }else{
                freeTree(tree);

                Node *node = malloc(sizeof(Node));
                node->left = NULL;
                node->right = NULL;
                node->parent = NULL;
                node->color = BLACK;
                node->key = 1;
                tree = malloc(sizeof(RedBlackTree));
                tree->root = node;
                if (!isCorrectRBTree(tree)){
                    printf("NOK - strom je korektni\n");
                }else{
                    node = malloc(sizeof(Node));
                    node->left = NULL;
                    node->right = NULL;
                    node->parent = tree->root;
                    node->color = BLACK;
                    node->key = 0;
                    tree->root->left = node;
                    if (isCorrectRBTree(tree)){
                        printf("NOK - strom nema stejnou cernou hloubku\n");
                    }else{
                        tree->root->left->color = RED;
                        if (! isCorrectRBTree(tree)){
                            printf("NOK - strom je korektni\n");
                        }else{
                            printf("OK\n");
                        }
                    }
                }
            }
        }
    }
    makeGraph(tree->root, "correct.dot");
    printf("Vykresleny strom najdete v souboru correct.dot\n");

    freeTree(tree);
}
int main() {
    testRotateLeft();
    testRotateRight();
    testInsert();
    testSearch();
    testIsCorrectRBTree();
}
