#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INT_MAX 2147483647

#define SIZE 5 //pocet vrcholu

/***********************************
 * Zacatek hlavicek podpurnych funkci
 ***********************************/
typedef struct Distance {
    int index;
    int distance;
} Distance;


/**
 * Struktura Element slouzi pro reprezentaci hodnoty ve fronte a zasobniku
 * atribut value je hodnota (slouzi pro ulozeni indexu vrcholu)
 * atribut next je ukazatel na nasledujici prvek ve strukture
 **/
typedef struct Element {
    int value;
    struct Element* next;
} Element;

/**
 * Struktura Stack reprezentuje zasobnik
 * atribut top je ukazatel na vrchol zasobniku
 **/
typedef struct Stack {
    struct Element* top;
} Stack;

/**
 * Struktura Queue reprezentuje frontu
 * atribut first je ukazatel na prvni pozici ve fronte (z te se odstranuje)
 * atribut last je ukazatel na posledni pozici ve fronte (na tu se vklada)
 **/
typedef struct Queue {
    struct Element* first; //dequeue from this
    struct Element* last; //enqueue to this
} Queue;

/**
 * Struktura Node slouzi pro reprezentaci objektu v oboustranne spojovanem seznamu
 * atribut pair je ukazatel na ulozenou dvojici (klic, data)
 * atribut next je ukazatel na nasledujici prvek v seznamu
 * atribut prev je ukazatel na predchazejici prvek v seznamu
 **/
typedef struct Node {
    int vertex;
    struct Node* next;
    struct Node* prev;
} Node;

/**
 * Struktura LinkedList reprezentuje spojovany seznam
 * atribut first je ukazatel na prvni prvek seznamu
 * atribut last je ukazatel na posledni prvek seznamu
 **/
typedef struct LinkedList {
    Node* first;
    Node* last;
} LinkedList;

/**
 * Inicializuje frontu 'q'
 */
void initQueue(Queue* q);

/**
 * Overi, zda je fronta 'q' prazdna, pokud ano, vraci true, jinak false
 */
bool isEmptyQueue(Queue* q);

/**
 * Vlozi prvek do fronty 'q'
 */
void enqueue(Queue* q, int value);

/**
 * Odstrani prvek z fronty 'q'
 */
int dequeue(Queue* q);

/**
 * Uvolni pamet vsech prvku, ktere fronta 'q' obsahuje
 */
void freeQueue(Queue* q);

/**
 * Inicializuje zasobnik 's'
 */
void initStack(Stack* s);

/**
 * Overi, zda je zasonik 's' prazdny, pokud ano, vraci true, jinak false
 */
bool isEmptyStack(Stack* s);

/**
 * Vlozi prvek do zasobniku 's'
 */
void push(Stack* s, int value);

/**
 * Odstrani prvek ze zasobniku 's'
 */
int pop(Stack* s);

/**
 * Uvolni pamet vsech prvku, ktere zasobnik 's' obsahuje
 */
void freeStack(Stack* s);

/**
 * Metoda insert() vlozi na konec (za prvek last) seznamu novy uzel s hodnotou pair
 **/
void listInsert(LinkedList* list, int vertex);

/**
 * Metoda delete() smaze uzel node ze seznamu
 **/
void freeList(LinkedList* list);

/***********************************
 * Konec hlavicek podpurnych funkci
 ***********************************/

/**
 * Struktura Graph reprezentuje orientovany graf pomoci matice vzdalenosti
 * atribut matrix je dvourozmerna matice. Pokud graf obsahuje hranu
 * (u, v) s delkou d, pak matrix[u][v] = d. Neexistence hrany je reprezentovana
 * hodnotou matrix[u][v] = INT_MAX
 **/
typedef struct Graph {
    int matrix[SIZE][SIZE];
} Graph;

/**
 * Prida hranu ('u','v') vahy 'weight' do grafu 'g'
 * Funkce nic nedela v pripade, ze 'u' nebo 'v' je mimo rozsah velikosti grafu
 */
void addEdge(Graph* g, int u, int v, int weight){
    if(u < SIZE && v < SIZE && u >= 0 && v >= 0) {
        g->matrix[u][v] = weight;
    }
}

/**
 * Pokud v grafu 'g' existuje hrana ('u','v'), vraci true, jinak false
 */
bool isEdge(Graph* g, int u, int v){
    if(g->matrix[u][v] < INT_MAX)
        return true;
    return false;
}

Element* searchQueue(Queue* q, int key) {
    if(q == NULL)
        return NULL;
    Element* e = q->first;
    while(e!=NULL) {
        if(e->value == key)
            return e;
        e = e->next;
    }
    return NULL;
}


int containsKey(Graph* g, Queue* q, int u, int key) {
    for(int i=0;i<SIZE;i++) {
        if(g->matrix[u][i]<INT_MAX) {
            if(i == key)
                return 1;
            if(searchQueue(q, i) == 0 && i != u) {
                enqueue(q, i);
                return containsKey(g, q, i, key);
            }
        }
    }
    return 0;
}

/**
 * Overi, zdali se da z 'u' dostat do 'v' v grafu 'g'
 * Pokud ano, vraci true, jinak false
 */
bool isConnected(Graph* g, int u, int v) {
    Queue* q = malloc(sizeof(Queue));
    initQueue(q);
    int result = containsKey(g, q, u, v);
    freeQueue(q);
    if(result)
        return true;
    return false;
}

/**
 * Zkontroluje, zda je zadany graf 'g' neorientovany
 * pokud ano, vraci true, jinak false
 */
bool isUndirected(Graph *g) {
    for(int i=0;i<SIZE;i++) {
        for(int j=i;j<SIZE;j++) {
            if(g->matrix[i][j] != g->matrix[j][i])
                return false;
        }
    }
    return true;
}

void distanceFromVertexes(Graph* g, int* array, int now, int target, Queue* visited, int distanceFromStart) {
    for(int i=0;i<SIZE;i++) {
        if(g->matrix[now][i] < INT_MAX
                && array[i] > g->matrix[now][i] + distanceFromStart
                && now!=i) {
            array[i] = distanceFromStart + g->matrix[now][i];
                if(searchQueue(visited, i) == NULL) {
                    enqueue(visited, i);
                    distanceFromVertexes(g, array, i, target, visited, distanceFromStart + g->matrix[now][i]);
                }
        }
    }
}

/**
 * Nalezne nejkratsi cestu z 'u' do 'v' v grafu 'g'
 * Pokud cesta existuje, vraci jeji delku, pokud neexistuje, vraci INT_MAX
 */
int shortestPath(Graph* g, int u, int v) {
    int array[SIZE];
    for(int i=0;i<SIZE;i++)
        array[i] = INT_MAX;
    Queue visited;
    initQueue(&visited);
    enqueue(&visited, u);
    distanceFromVertexes(g,array, u, v, &visited, 0);
    return array[v];
}

int searchStack(Stack* s, int key) {
    if(s != NULL) {
        Element* item = s->top;
        while(item != NULL) {
            if(item->value == key)
                return 1;
            item = item->next;
        }
    }
    return 0;
}

int cycleRecursive(Graph* g, Stack* s) {
    if(isEmptyStack(s))
        return 0;
    int e = pop(s);
    for(int i=0;i<SIZE;i++) {
        if(g->matrix[e][i] < INT_MAX) {
            push(s, e);
            if(searchStack(s, i) == 1)
                return 1;
            push(s, i);
            if(cycleRecursive(g, s) == 1)
                return 1;
            else
                pop(s);
        }
    }
    return 0;
}

/**
 * Zkontroluje, zdali graf 'g' obsahuje cyklus.
 * Pokud ano, vraci true, jinak false
 */
bool containsCycle(Graph* g) {
    Stack s;
    initStack(&s);
    for(int i=0;i<SIZE;i++) {
        push(&s, i);
        if(cycleRecursive(g, &s))
            return true;
    }
    return false;
}

int searchList(LinkedList* l, int key) {
    Node* n = l->first;
    while(n!=NULL) {
        if(n->vertex == key)
            return 1;
        n = n->next;
    }
    return 0;
}

/**
 * Vytvori seznam sousednosti z grafu 'g'
 * Vraci tento seznam, pokud vrchol nema sousedy, vrati pro nej prazdny seznam
 * V seznamu se vyskytuji indexy sousedu, ne nic jineho
 */
LinkedList *convertToList(Graph *g) {
    LinkedList *lists = calloc(SIZE, sizeof(LinkedList));
    for (int i = 0; i < SIZE; ++i) {
        lists[i].first = NULL;
        lists[i].last = NULL;
    }
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<SIZE;j++)
            if(g->matrix[i][j] < INT_MAX)
                listInsert(&lists[i], j);
    }

    return lists;
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
void makeGraphviz(Graph* g, FILE* outputFile) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if(isEdge(g, i, j)) {
                if(i == j) {
                    fprintf(outputFile, "\"%i\"\n", i);
                }
                else {
                    fprintf(outputFile, "\"%i\" -> \"%i\"[ label = %i ];\n", i, j, g->matrix[i][j]);
                }
            }
        }
    }
}

void makeGraph(Graph* g, const char* filename) {
    FILE* outputFile;
    outputFile = fopen(filename , "w");
    fprintf(outputFile, "digraph MyGraph {\n");
    makeGraphviz(g, outputFile);
    fprintf(outputFile, "}\n");
    fclose(outputFile);
}

/**
 * Creates graph without edges, edge between vertex v and v is 0
 * @param V vertices of graph
 */
void initGraph(Graph* g) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if(i == j) {
                g->matrix[i][j] = 0;
            }
            else {
                g->matrix[i][j] = INT_MAX;
            }
        }
    }
}

void printGraph(Graph* g) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if(g->matrix[i][j] != INT_MAX)
                printf("%i     ", g->matrix[i][j]);
            else printf("infty ");
        }
        printf("\n");
    }
}

bool compareGraphs(Graph* graph1, Graph* graph2) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (graph1->matrix[i][j] != graph2->matrix[i][j])
                return false;
        }
    }
    return true;
}

/**
 * Vytvori n.n matici vzdalenosti
 */
Graph *createGraph() {
    Graph *g = malloc(sizeof(Graph));
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            g->matrix[i][j] = INT_MAX;
        }
    }
    return g;
}

Graph *createTestGraph() {
    Graph *graph = createGraph();
    graph->matrix[0][1] = 1;
    graph->matrix[0][3] = 1;
    graph->matrix[2][0] = 1;
    graph->matrix[2][1] = 1;
    graph->matrix[3][4] = 1;
    graph->matrix[4][0] = 1;
    graph->matrix[4][3] = 1;

    return graph;
}

bool isEmptyList(LinkedList *l) {
    return l->first == NULL;
}

void printList(LinkedList *l) {
    if (l == NULL) {
        puts("Chyba, seznam neni korektni");
        return;
    }
    Node *tmp = l->first;
    while (tmp != NULL) {
        if (tmp == NULL)
            puts("BIG ERROR");
        printf("%d ", tmp->vertex);
        tmp = tmp->next;
    }
}

void printLists(LinkedList *l, int listCount) {
    for (int i = 0; i < listCount; ++i) {
        printf("%d: ", i);
        printList(&l[i]);
        puts("");
    }
}

bool testList(LinkedList *l, int *array, int size) {
    if (l == NULL) {
        return size == 0;
    }
    Node *tmp = l->first;
    for (int i = 0; i < size; ++i) {
        if (tmp == NULL || tmp->vertex != array[i]) {
            return false;
        }
        tmp = tmp->next;
    }
    return true;
}

void testInsert() {
    puts("Test 1. vkladani hran do grafu:");

    Graph *graph1 = createGraph();
    Graph *toCompareGraph = createGraph();
    toCompareGraph->matrix[0][1] = 1;
    addEdge(graph1, 0, 1, 1);
    if (!compareGraphs(graph1, toCompareGraph)) {
        puts("NOK - nevlozili jste hranu.");
        puts("Ocekavany graf je:");
        printGraph(toCompareGraph);
        puts("Vas graf je:");
        printGraph(graph1);
        free(graph1);
        free(toCompareGraph);
        return;
    }

    toCompareGraph->matrix[0][1] = 2;
    addEdge(graph1, 0, 1, 2);
    if (!compareGraphs(graph1, toCompareGraph)) {
        puts("NOK - neprepsali jste hranu.");
        puts("Ocekavany graf je:");
        printGraph(toCompareGraph);
        puts("Vas graf je:");
        printGraph(graph1);
        free(graph1);
        free(toCompareGraph);
        return;
    }
    free(graph1);
    free(toCompareGraph);

    Graph *graph2 = createGraph();
    Graph *emptyGraph = createGraph();
    addEdge(graph2, 5, 5, 1);
    if (!compareGraphs(graph2, emptyGraph)) {
        puts("NOK - menili jste hrany, ackoliv zadana hrana do grafu nepatri.");
        puts("Ocekavany graf je:");
        printGraph(emptyGraph);
        puts("Vas graf je:");
        printGraph(graph2);
        free(graph2);
        free(emptyGraph);
        return;
    }

    puts("OK");
    free(graph2);
    free(emptyGraph);
    return;
}

void testIsEdge() {
    puts("Test 2. kontrola existence hrany:");

    Graph *graph1 = createTestGraph();
    if (!isEdge(graph1, 0, 3)) {
        puts("NOK - z vrcholu 0 do vrcholu 3 je hrana, vy vracite False.");
        puts("Testovany graf je:");
        printGraph(graph1);
        free(graph1);
        return;
    }
    free(graph1);

    Graph *graph2 = createTestGraph();
    if (isEdge(graph2, 3, 0)) {
        puts("NOK - z vrcholu 3 do vrcholu 0 neni hrana, vy vracite True.");
        puts("Testovany graf je:");
        printGraph(graph2);
        free(graph2);
        return;
    }
    free(graph2);

    puts("OK");
}

void testIsUndirected() {
    puts("Test 3. zjisteni, zdali je graf neorientovany:");

    Graph *graph1 = createGraph();
    graph1->matrix[1][1] = 1;
    if (!isUndirected(graph1)) {
        puts("NOK - zadany graf je neorientovany, vy vracite False.");
        puts("Testovany graf je:");
        printGraph(graph1);
        free(graph1);
        return;
    }
    free(graph1);

    Graph *graph2 = createGraph();
    graph2->matrix[0][1] = 1;
    graph2->matrix[1][0] = 1;
    if (!isUndirected(graph2)) {
        puts("NOK - zadany graf je neorientovany, vy vracite False.");
        puts("Testovany graf je:");
        printGraph(graph2);
        free(graph2);
        return;
    }
    free(graph2);

    Graph *graph3 = createGraph();
    graph3->matrix[0][1] = 1;
    graph3->matrix[1][0] = 2;
    if (isUndirected(graph3)) {
        puts("NOK - zadany graf je orientovany, vy vracite True.");
        puts("Testovany graf je:");
        printGraph(graph3);
        free(graph3);
        return;
    }
    free(graph3);

    Graph *graph4 = createTestGraph();
    if (isUndirected(graph4)) {
        puts("NOK - zadany graf je orientovany, vy vracite True.");
        puts("Testovany graf je:");
        printGraph(graph4);
        free(graph4);
        return;
    }
    free(graph4);

    puts("OK");
}

void testShortestPath() {
    puts("Test 4. nalezeni nejkratsi cesty: ");

    Graph *graph1 = createGraph();
    graph1->matrix[1][0] = 1;
    graph1->matrix[0][1] = 1;
    graph1->matrix[0][0] = 1;
    graph1->matrix[1][1] = 1;
    if (shortestPath(graph1, 0, 1) != 1) {
        printf("NOK - nejkratsi cesta z 0 do 1 v zadanem grafu je 1, vy vracite: %d.\n", shortestPath(graph1, 0, 1));
        puts("Testovany graf je:");
        printGraph(graph1);
        free(graph1);
        return;
    }
    free(graph1);

    Graph *graph2 = createGraph();
    graph2->matrix[1][0] = 1;
    graph2->matrix[0][1] = 1;
    graph2->matrix[0][0] = 1;
    graph2->matrix[1][1] = 1;
    if (shortestPath(graph2, 0, 2) != INT_MAX) {
        printf("NOK - vrcholy 0 a 2 v zadanem grafu nejsou propojeny, vy vracite: %d.\n", shortestPath(graph2, 0, 1));
        puts("Testovany graf je:");
        printGraph(graph2);
        free(graph2);
        return;
    }
    free(graph2);

    Graph *graph3 = createTestGraph();
    if (shortestPath(graph3, 3, 2) != INT_MAX) {
        printf("NOK - vrcholy 3 a 2 v zadanem grafu nejsou propojeny, vy vracite: %d.\n", shortestPath(graph3, 3, 2));
        puts("Testovany graf je:");
        printGraph(graph3);
        free(graph3);
        return;
    }
    free(graph3);

    Graph *graph4 = createTestGraph();
    if (shortestPath(graph4, 3, 1) != 3) {
        printf("NOK - nejkratsi cesta z 3 do 1 v zadanem grafu je 3, vy vracite: %d.\n", shortestPath(graph4, 3, 1));
        puts("Testovany graf je:");
        printGraph(graph4);
        free(graph4);
        return;
    }
    free(graph4);

    puts("OK");
}

void testIsConnected() {
    puts("Test 5. otestovani, zdali jsou vrcholy propojeny: ");

    Graph *graph1 = createGraph();
    graph1->matrix[1][0] = 1;
    graph1->matrix[0][1] = 1;
    graph1->matrix[0][0] = 1;
    graph1->matrix[1][1] = 1;
    if (!isConnected(graph1, 0, 1)) {
        puts("NOK - vrcholy 0 a 1 jsou propojeny, vy vracite False");
        puts("Testovany graf je:");
        printGraph(graph1);
        free(graph1);
        return;
    }
    free(graph1);

    Graph *graph2 = createGraph();
    graph2->matrix[1][0] = 1;
    graph2->matrix[0][1] = 1;
    graph2->matrix[0][0] = 1;
    graph2->matrix[1][1] = 1;
    if (isConnected(graph2, 0, 2)) {
        puts("NOK - vrcholy 0 a 2 nejsou propojeny, vy vracite True");
        puts("Testovany graf je:");
        printGraph(graph2);
        free(graph2);
        return;
    }
    free(graph2);

    Graph *graph3 = createTestGraph();
    if (isConnected(graph3, 3, 2)) {
        puts("NOK - vrcholy 3 a 2 nejsou propojeny, vy vracite True");
        puts("Testovany graf je:");
        printGraph(graph3);
        free(graph3);
        return;
    }
    free(graph3);

    Graph *graph4 = createTestGraph();
    if (!isConnected(graph4, 3, 1)) {
        puts("NOK - vrcholy 3 a 1 jsou propojeny, vy vracite False");
        puts("Testovany graf je:");
        printGraph(graph4);
        free(graph4);
        return;
    }
    free(graph4);

    puts("OK");
}

void testContainsCycle() {
    puts("Test 6. otestovani, zdali graf obsahuje cyklus: ");

    Graph *graph1 = createGraph();
    if (containsCycle(graph1)) {
        puts("NOK - zadany graf neobsahuje cykly, vy vracite True");
        puts("Testovany graf je:");
        printGraph(graph1);
        free(graph1);
        return;
    }
    free(graph1);

    Graph *graph2 = createGraph();
    graph2->matrix[2][3] = 1;
    graph2->matrix[3][2] = 1;
    if (!containsCycle(graph2)) {
        puts("NOK - zadany graf obsahuje cykly, vy vracite False");
        puts("Testovany graf je:");
        printGraph(graph2);
        free(graph2);
        return;
    }
    free(graph2);

    Graph *graph3 = createGraph();
    graph3->matrix[1][0] = 1;
    graph3->matrix[0][1] = 1;
    graph3->matrix[0][0] = 1;
    graph3->matrix[1][1] = 1;
    if (!containsCycle(graph3)) {
        puts("NOK - zadany graf obsahuje cykly, vy vracite False");
        puts("Testovany graf je:");
        printGraph(graph3);
        free(graph3);
        return;
    }
    free(graph3);

    Graph *graph4 = createGraph();
    graph4->matrix[1][0] = 1;
    graph4->matrix[1][2] = 1;
    graph4->matrix[1][4] = 1;
    graph4->matrix[2][3] = 1;
    graph4->matrix[3][4] = 1;
    if (containsCycle(graph4)) {
        puts("NOK - zadany graf neobsahuje cykly, vy vracite True");
        puts("Testovany graf je:");
        printGraph(graph4);
        free(graph4);
        return;
    }
    free(graph4);

    Graph *graph5 = createTestGraph();
    if (!containsCycle(graph5)) {
        puts("NOK - zadany graf obsahuje cykly, vy vracite False");
        puts("Testovany graf je:");
        printGraph(graph5);
        free(graph5);
        return;
    }
    free(graph5);

    puts("OK");
}

void testConvertToList() {
    puts("Test 7. prevod reprezentace grafu na seznam nasledniku:");

    Graph *graph1 = createGraph();
    LinkedList *lists1 = convertToList(graph1);
    bool isOk = true;
    for (int i = 0; i < 5; ++i) {
        isOk &= isEmptyList(&lists1[i]);
    }

    if (!isOk) {
        puts("NOK - matice neodpovida vracenemu seznamu nasledniku");
        puts("Testovany graf je:");
        printGraph(graph1);
        puts("Vas seznam nasledniku:");
        printLists(lists1, SIZE);
        for (int i = 0; i < 5; ++i) {
            freeList(&lists1[i]);
        }
        free(lists1);
        free(graph1);
        return;
    }
    for (int i = 0; i < 5; ++i) {
        freeList(&lists1[i]);
    }
    free(lists1);
    free(graph1);

    Graph *graph2 = createGraph();
    graph2->matrix[1][0] = 1;
    graph2->matrix[0][1] = 1;
    LinkedList *lists2 = convertToList(graph2);
    int tmpArray1[1] = {1};
    isOk &= testList(&lists2[0], tmpArray1, 1);
    int tmpArray2[1] = {0};
    isOk &= testList(&lists2[1], tmpArray2, 1);
    for (int i = 2; i < 5; ++i) {
        isOk &= isEmptyList(&lists2[i]);
    }

    if (!isOk) {
        puts("NOK - matice neodpovida vracenemu seznamu nasledniku");
        puts("Testovany graf je:");
        printGraph(graph2);
        puts("Vas seznam nasledniku:");
        printLists(lists2, SIZE);
        for (int i = 0; i < 5; ++i) {
            freeList(&lists2[i]);
        }
        free(lists2);
        free(graph2);
        return;
    }
    for (int i = 0; i < 5; ++i) {
        freeList(&lists2[i]);
    }
    free(lists2);
    free(graph2);

    Graph *graph3 = createTestGraph();
    LinkedList *lists3 = convertToList(graph3);

    int tmpArray3[2] = {1, 3};
    isOk &= testList(&lists3[0], tmpArray3, 2);
    isOk &= isEmptyList(&lists3[1]);
    int tmpArray4[2] = {0, 1};
    isOk &= testList(&lists3[2], tmpArray4, 2);
    int tmpArray5[1] = {4};
    isOk &= testList(&lists3[3], tmpArray5, 1);
    int tmpArray6[2] = {0, 3};
    isOk &= testList(&lists3[4], tmpArray6, 2);

    if (!isOk) {
        puts("NOK - matice neodpovida vracenemu seznamu nasledniku");
        puts("Testovany graf je:");
        printGraph(graph3);
        puts("Vas seznam nasledniku:");
        printLists(lists3, SIZE);
        for (int i = 0; i < 5; ++i) {
            freeList(&lists3[i]);
        }
        free(lists3);
        free(graph3);
        return;
    }
    for (int i = 0; i < 5; ++i) {
        freeList(&lists3[i]);
    }
    free(lists3);
    free(graph3);

    puts("OK");
}

int main(void) {
    testInsert();
    testIsEdge();
    testIsUndirected();
    testShortestPath();
    testIsConnected();
    testContainsCycle();
    testConvertToList();
}
/*****************************************************
 * Funkce pro manipulaci s daty v pomocnych strukturach
 *****************************************************/
void initQueue(Queue* q) {
    q->first = NULL;
    q->last = NULL;
}

bool isEmptyQueue(Queue* q) {
    return (q->first == NULL);
}

int dequeue(Queue* q) {
    if(isEmptyQueue(q)) return -1;
    else {
        Element* toDelete = q->first;
        q->first = q->first->next;
        int tmp = toDelete->value;
        free(toDelete);
        return tmp;
    }
}

void enqueue(Queue* q, int value) {
    Element* toInsert = malloc(sizeof(Element));
    toInsert->value = value;
    toInsert->next = NULL;
    if (isEmptyQueue(q)) {
        q->first = toInsert;
        q->last = toInsert;
    } else {
        Element* tmpElement = q->last;
        q->last = toInsert;
        tmpElement->next = toInsert;
    }
}

void freeQueue(Queue* q) {
    while(!isEmptyQueue(q)) {
        dequeue(q);
    }
}

void initStack(Stack* s) {
    s->top = NULL;
}

bool isEmptyStack(Stack* s) {
    return (s->top == NULL);
}

void push(Stack* s, int value) {
    Element* toInsert = malloc(sizeof(Element));
    toInsert->value = value;
    toInsert->next = s->top;
    s->top = toInsert;
}

int pop(Stack* s) {
    if(isEmptyStack(s)) return -1;
    Element* toRemove = s->top;
    s->top = toRemove->next;
    int tmp = toRemove->value;
    free(toRemove);
    return tmp;
}

void freeStack(Stack* s) {
    while(!isEmptyStack(s)) {
        pop(s);
    }
}

/**
 * Metoda insert() vlozi na konec (za prvek last) seznamu novy uzel s hodnotou pair
 **/
void listInsert(LinkedList* list, int vertex) {
    Node* added = malloc(sizeof(Node));
    added->vertex = vertex;
    added->next = NULL;
    added->prev = list->last;
    if(list->first == NULL) {
        list->first = added;
    }
    else {
        list->last->next = added;
    }
    list->last = added;
}

/**
 * Metoda delete() smaze uzel node ze seznamu
 **/
void freeList(LinkedList* list) {
    Node *tmp;
    Node *actual = list->first;
    while (actual != NULL) {
        tmp = actual;
        actual = actual->next;
        free(tmp);
    }
}
