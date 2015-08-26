#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define INF INT_MAX
#define NEGATIVE_INF INT_MIN

#define SIZE 6

/***********************************************************************
 * Zacatek funkci pro praci s prioritni frontou
 **********************************************************************/

/**
 * @brief Struktura Vertex slouzi k reprezentaci vrcholu ve fronte
 **/
typedef struct Vertex {
    int index;
    int priority;
} Vertex;

/**
 * @brief Struktura PriorityQueue slouzi k reprezentaci prioritni fronty
 * atribut array je pole prvku prioritni fronty
 * atribut positions slouzi k indexaci pro konstantni pristup k prvkum fronty
 * atribut size udava pocet prvku v halde
 **/
typedef struct PriorityQueue {
    Vertex *array[SIZE];
    int positions[SIZE];
    int size;
} PriorityQueue;

/**
 * Funkce slouzi k inicializaci prioritni fronty
 * Do fronty se pridaji vsechny vrcholy grafu se vzdalenostmi (prioritou) nekonecno,
 * pouze vrchol 'fromVertex' ma prioritu 0
 *
 * Vraci se ukazatel na nainicializovanou prioritni frontu
 */
PriorityQueue *priorityQueueInit(int fromVertex);

/**
 * Funkce snizi prioritu zadaneho vrcholu 'vertex' na hodnotu 'value' ve fronte 'queue'
 * Hodnota 'value' musi byt nizsi, nez byla priorita vrcholu pred volanim
 */
void decreaseKey(PriorityQueue* queue, int vertex, int value);

/**
 * Funkce vybere z fronty 'queue' prvek s nejmensi prioritou a vrati jej
 */
Vertex extractMin(PriorityQueue* queue);

/**
 * Test, zdali je fronta 'queue' prazdna a nebo ma jeji prvni prvek prioritu nekonecno
 */
bool isPriorityQueueEmpty(PriorityQueue* queue);

/**
 * Funkce pro uvolneni pameti po praci s prioritni frontou.
 *
 * Pro odstraneni memory leaku ji prosim volejte pred vracenim hodnoty v algoritmu
 */
void freeQueue(PriorityQueue* queue);


/***********************************************************************
 * Konec funkci pro praci s prioritni frontou
 **********************************************************************/

/**
 * Trida DiGraph slouzi k reprezentaci orientovaneho grafu
 * atribut matrix je matice, ktera obsahuje hrany grafu
 * atribut distances je pole vzdalenosti ze zadaneho vrcholu
 * atribut predecessors je pole predchudcu, podle ktereho lze zpetne urcit, kudy vedla cesta
 */
typedef struct DiGraph {
    int matrix[SIZE][SIZE];
    int distances[SIZE];
    int predecessors[SIZE];
} DiGraph;

/**
 * Funkce slouzi k inicializaci grafu 'graph'
 * nastavi vzdalenost inicialniho vrcholu 's' na 0 a zbytek na nekonecno
 * predchudce inicialniho vrcholu je -1 (jako reprezentace Nil)
 */
void initialize(DiGraph *graph, int s) {
    for(int i=0; i < SIZE; i++) {
        graph->distances[i] = INT_MAX;
    }
    graph->distances[s] = 0;
    graph->predecessors[s] = -1;
}

/**
 * Pokud v matici vzdalenosti 'matrix' existuje hrana (u,v), vraci true, jinak false
 */
bool isEdge(DiGraph *graph, int u, int v) {
    if(graph->matrix[u][v] < INT_MAX)
        return true;
    return false;
}

/**
 * Funkce slouzi k relaxaci hrany ('u', 'v') v orientovanem grafu 'graph'
 */
void relax(DiGraph *graph, int u, int v) {
    if((graph->distances[v] > graph->distances[u] + graph->matrix[u][v])) {
        graph->distances[v] = graph->distances[u] + graph->matrix[u][v];
        graph->predecessors[v] = u;
    }
}

/**
 * Naimplementujte Bellman-Forduv algoritmus pro nalezeni stromu
 * nejkratsich cest z vrcholu 'u' v grafu 'graph'
 * funkce vraci delku cesty z 'u' do 'v'
 * v poli 'graph->distances' budou po vypoctu vzdalenosti vrcholu od 'u'
 * a v poli 'graph->predecessors' budou prechudci vrcholu na ceste z 'u'
 */
int bellmanFord(DiGraph *graph, int u, int v) {
    initialize(graph, u);
    for(int k = 0; k < SIZE - 1; k++) {
        for(int i=0;i < SIZE; i++) {
            for(int j=0; j < SIZE; j++) {
                if(graph->matrix[i][j] < INT_MAX)
                    relax(graph, i, j);
            }
        }
    }

    for(int i=0;i < SIZE; i++) {
        for(int j=0; j < SIZE; j++) {
            if(graph->matrix[i][j] != INT_MAX &&
                    graph->distances[j] > graph->distances[i] + graph->matrix[i][j])
                return -1;
        }
    }
    return graph->distances[v];
}

/**
 * Naimplementujte Dijkstruv algoritmus pro nalezeni nejkratsi cesty
 * z vrcholu 'u' do vrcholu 'v' v grafu 'graph'
 * funkce vraci delku cesty z 'u' do 'v'
 * K implementaci pouzijte pripravenou prioritni frontu
 */
int dijkstra(DiGraph *graph, int u, int v) {
    PriorityQueue* q = priorityQueueInit(u);
    initialize(graph, u);
    Vertex searching;
    while(!isPriorityQueueEmpty(q)) {
        searching = extractMin(q);
        for(int i=0; i<SIZE;i++) {
            if(i!=searching.index && graph->matrix[searching.index][i] < INT_MAX) {
                decreaseKey(q, i, graph->matrix[searching.index][i]+ graph->distances[searching.index]);
                relax(graph, searching.index, i);
            }
        }
    }
    freeQueue(q);
    return graph->distances[v];
}

/**
 * Kdykoliv byste meli problem s ladenim, zavolejte si vypis graphvizu,
 * nasledujici funkci predejte graf a retezec, ktery urci jmeno souboru
 *
 * Dodatek k graphvizu {
 * Graphviz je nastroj, ktery vam umozni vizualizaci datovych struktur,
 * coz se hodi predevsim pro ladeni.
 * Tento program generuje nekolik souboru neco.dot v mainu
 * Vygenerovane soubory nahrajte do online nastroje pro zobrazeni graphvizu {
 * http {//sandbox.kidstrythisathome.com/erdos/
 * nebo http {//graphviz-dev.appspot.com/ - zvlada i vetsi grafy
 *
 * Alternativne si muzete nainstalovat prekladac z jazyka dot do obrazku na svuj pocitac.
 **/
void makeGraphviz(DiGraph* g, FILE* outputFile) {
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

void makeGraph(DiGraph* g, const char* filename) {
    FILE* outputFile;
    outputFile = fopen(filename , "w");
    fprintf(outputFile, "digraph MyGraph {\n");
    makeGraphviz(g, outputFile);
    fprintf(outputFile, "}\n");
    fclose(outputFile);
}

/**
 * Prida hranu ('u','v') vahy 'w' do matice vzdalenosti 'matrix'
 * Funkce nic nedela v pripade, ze 'u' nebo 'v' je mimo rozsah matice
 */
void addEdge(DiGraph *graph, int u, int v, int w) {
    if (u >= 0 && v >= 0 && u < SIZE && v < SIZE) {
        graph->matrix[u][v] = w;
    }
}

void printGraph(DiGraph* g) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if(g->matrix[i][j] != INF)
                printf("%5d ", g->matrix[i][j]);
            else printf("infty ");
        }
        printf("\n");
    }
}

/**
 * Vytvori SIZE.SIZE matici vzdalenosti
 */
DiGraph *createGraph() {
    DiGraph *g = malloc(sizeof(DiGraph));
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            g->matrix[i][j] = INT_MAX;
            if (i == j)
                g->matrix[i][j] = 0;
        }
        g->predecessors[i] = -2;
        g->distances[i] = NEGATIVE_INF;
    }
    return g;
}

DiGraph *createTestGraph() {
    DiGraph *graph = createGraph();
    graph->matrix[0][1] = 7;
    graph->matrix[0][2] = 9;
    graph->matrix[0][5] = 14;
    graph->matrix[1][3] = 15;
    graph->matrix[1][2] = 10;
    graph->matrix[2][3] = 11;
    graph->matrix[2][5] = 2;
    graph->matrix[3][4] = 6;
    graph->matrix[4][5] = 9;
    graph->matrix[1][0] = 7;
    graph->matrix[2][0] = 9;
    graph->matrix[5][0] = 14;
    graph->matrix[3][1] = 15;
    graph->matrix[2][1] = 10;
    graph->matrix[3][2] = 11;
    graph->matrix[5][2] = 2;
    graph->matrix[4][3] = 6;
    graph->matrix[5][4] = 9;

    return graph;
}

void printDist(DiGraph *g) {
    for (int i = 0; i < SIZE; ++i) {
        if (g->distances[i] == INF) {
            printf("infty, ");
        } else {
            printf("%5d, ", g->distances[i]);
        }
    }
    puts("");
}

void printPred(DiGraph *g) {
    for (int i = 0; i < SIZE; ++i) {
        if (g->predecessors[i] == -1) {
            printf("Nil, ");
        } else {
            printf("%3d, ", g->predecessors[i]);
        }
    }
    puts("");
}

bool testInitialize() {
    puts("Test 1. initialize pro Bellman-Forduv algoritmus:");

    DiGraph *graph1 = createGraph();
    initialize(graph1, 0);
    if (graph1->distances[0] != 0 || graph1->predecessors[0] != -1) {
        puts("NOK - init nefunguje jak ma.");
        puts("Matice vypada takto:");
        printGraph(graph1);
        puts("Seznam vzdalenosti z vrcholu 0 vypada takto:");
        printDist(graph1);
        puts("Seznam predchudcu na ceste z vrcholu 0 vypada takto:");
        printPred(graph1);
        free(graph1);
        return false;
    }
    free(graph1);

    DiGraph *graph2 = createTestGraph();
    initialize(graph2, 2);
    bool is_ok = (graph2->distances[2] == 0
                && graph2->distances[0] == INF
                && graph2->predecessors[2] == -1);
    if (!is_ok) {
        puts("NOK - init nefunguje jak ma.");
        puts("Matice vypada takto:");
        printGraph(graph2);
        puts("Seznam vzdalenosti z vrcholu 2 vypada takto:");
        printDist(graph2);
        puts("Seznam predchudcu na ceste z vrcholu 2 vypada takto:");
        printPred(graph2);
        free(graph2);
        return false;
    }
    free(graph2);

    puts("OK");
    return true;
}

void testRelax() {
    puts("Test 2. funkce relax:");

    DiGraph *graph1 = createTestGraph();
    initialize(graph1, 0);
    relax(graph1, 0, 1);
    if (graph1->distances[1] != 7 || graph1->predecessors[1] != 0) {
        puts("NOK - relax nefunguje jak ma na testovacim grafu pri volani na vrcholy 0 1");
        puts("Matice vypada takto:");
        printGraph(graph1);
        puts("Seznam vzdalenosti z vrcholu 0 vypada takto:");
        printDist(graph1);
        puts("Seznam predchudcu na ceste z vrcholu 0 vypada takto:");
        printPred(graph1);
        free(graph1);
        return;
    }
    free(graph1);

    DiGraph *graph2 = createTestGraph();
    initialize(graph2, 2);
    relax(graph2, 2, 0);
    relax(graph2, 0, 1);
    relax(graph2, 2, 1);
    if (graph2->distances[1] != 10 || graph2->predecessors[1] != 2) {
        puts("NOK - init nefunguje jak ma na grafu o velikosti 6.");
        puts("posloupnost provedenych akci - 1. initialize(g, 2):");
        DiGraph *tmp_graph = createTestGraph();
        initialize(tmp_graph, 2);
        puts("Matice vypada takto:");
        printGraph(tmp_graph);
        puts("Seznam vzdalenosti z vrcholu 2 vypada takto:");
        printDist(tmp_graph);
        puts("Seznam predchudcu na ceste z vrcholu 2 vypada takto:");
        printPred(tmp_graph);

        puts("\n2. relax(g, 2, 0):");
        relax(tmp_graph, 2, 0);
        puts("Seznam vzdalenosti z vrcholu 2 vypada takto:");
        printDist(tmp_graph);
        puts("Seznam predchudcu na ceste z vrcholu 2 vypada takto:");
        printPred(tmp_graph);

        puts("\n3. relax(g, 0, 1):");
        relax(tmp_graph, 0, 1);
        puts("Seznam vzdalenosti z vrcholu 2 vypada takto:");
        printDist(tmp_graph);
        puts("Seznam predchudcu na ceste z vrcholu 2 vypada takto:");
        printPred(tmp_graph);

        puts("\n4. relax(g, 2, 1):");
        relax(tmp_graph, 2, 1);
        puts("Seznam vzdalenosti z vrcholu 2 vypada takto:");
        printDist(tmp_graph);
        puts("Seznam predchudcu na ceste z vrcholu 2 vypada takto:");
        printPred(tmp_graph);
        free(graph2);
        return;
    }
    free(graph2);

    puts("OK");
}

void testBellmanFord() {
    puts("Test 3. Bellman-Forduv algoritmus:");

    DiGraph *graph1 = createGraph();
    addEdge(graph1, 0, 1, 1);
    int ret = bellmanFord(graph1, 0, 1);
    if (ret != 1) {
        puts("NOK - cesta z 0 do 1 ma delku 1.");
        printf("Vas vystup je: %d\n", ret);
        puts("Matice vypada takto:");
        printGraph(graph1);
        puts("Seznam vzdalenosti z vrcholu 0 vypada takto:");
        printDist(graph1);
        puts("Seznam predchudcu na ceste z vrcholu 0 vypada takto:");
        printPred(graph1);
        free(graph1);
        return;
    }
    free(graph1);

    DiGraph *graph2 = createGraph();
    addEdge(graph2, 0, 1, 1);
    ret = bellmanFord(graph2, 1, 2);
    if (ret != INF) {
        printf("NOK - cesta z 1 do 2 neexistuje (delka je %d).", INF);
        printf("Vas vystup je: %d\n", ret);
        puts("Matice vypada takto:");
        printGraph(graph2);
        puts("Seznam vzdalenosti z vrcholu 1 vypada takto:");
        printDist(graph2);
        puts("Seznam predchudcu na ceste z vrcholu 1 vypada takto:");
        printPred(graph2);
        free(graph2);
        return;
    }
    free(graph2);

    DiGraph *graph3 = createGraph();
    ret = bellmanFord(graph3, 0, 0);
    if (ret != 0) {
        puts("NOK - cesta z 0 do 0 ma delku 0.");
        printf("Vas vystup je: %d\n", ret);
        puts("Matice vypada takto:");
        printGraph(graph3);
        puts("Seznam vzdalenosti z vrcholu 0 vypada takto:");
        printDist(graph3);
        puts("Seznam predchudcu na ceste z vrcholu 0 vypada takto:");
        printPred(graph3);
        free(graph3);
        return;
    }
    free(graph3);

    DiGraph *graph4 = createTestGraph();
    ret = bellmanFord(graph4, 0, 4);
    if (ret != 20) {
        puts("NOK - cesta z 0 do 4 ma delku 20.");
        printf("Vas vystup je: %d\n", ret);
        puts("Matice vypada takto:");
        printGraph(graph4);
        puts("Seznam vzdalenosti z vrcholu 0 vypada takto:");
        printDist(graph4);
        puts("Seznam predchudcu na ceste z vrcholu 0 vypada takto:");
        printPred(graph4);
        free(graph4);
        return;
    }
    free(graph4);

    puts("OK");
}

void testDijkstra() {
    puts("Test 4. Dijkstruv algoritmus:");

    DiGraph *graph1 = createGraph();
    addEdge(graph1, 0, 1, 1);
    int ret = dijkstra(graph1, 0, 1);
    if (ret != 1) {
        puts("NOK - cesta z 0 do 1 ma delku 1.");
        printf("Vas vystup je: %d\n", ret);
        puts("Matice vypada takto:");
        printGraph(graph1);
        puts("Seznam vzdalenosti z vrcholu 0 vypada takto:");
        printDist(graph1);
        puts("Seznam predchudcu na ceste z vrcholu 0 vypada takto:");
        printPred(graph1);
        free(graph1);
        return;
    }
    free(graph1);

    DiGraph *graph2 = createGraph();
    addEdge(graph2, 0, 1, 1);
    ret = dijkstra(graph2, 1, 2);
    if (ret != INF) {
        printf("NOK - cesta z 1 do 2 neexistuje (delka je %d).", INF);
        printf("Vas vystup je: %d\n", ret);
        puts("Matice vypada takto:");
        printGraph(graph2);
        puts("Seznam vzdalenosti z vrcholu 1 vypada takto:");
        printDist(graph2);
        puts("Seznam predchudcu na ceste z vrcholu 1 vypada takto:");
        printPred(graph2);
        free(graph2);
        return;
    }
    free(graph2);

    DiGraph *graph3 = createGraph();
    ret = dijkstra(graph3, 0, 0);
    if (ret != 0) {
        puts("NOK - cesta z 0 do 0 ma delku 0.");
        printf("Vas vystup je: %d\n", ret);
        puts("Matice vypada takto:");
        printGraph(graph3);
        puts("Seznam vzdalenosti z vrcholu 0 vypada takto:");
        printDist(graph3);
        puts("Seznam predchudcu na ceste z vrcholu 0 vypada takto:");
        printPred(graph3);
        free(graph3);
        return;
    }
    free(graph3);

    DiGraph *graph4 = createTestGraph();
    ret = dijkstra(graph4, 0, 4);
    if (ret != 20) {
        puts("NOK - cesta z 0 do 4 ma delku 20.");
        printf("Vas vystup je: %d\n", ret);
        puts("Matice vypada takto:");
        printGraph(graph4);
        puts("Seznam vzdalenosti z vrcholu 0 vypada takto:");
        printDist(graph4);
        puts("Seznam predchudcu na ceste z vrcholu 0 vypada takto:");
        printPred(graph4);
        free(graph4);
        return;
    }
    free(graph4);

    puts("OK");
}


int main(void) {
    if (testInitialize()) {
        testRelax();
        testBellmanFord();
        testDijkstra();
    }
    return 0;
}

int parentIndex(int index) {
    return ((index + 1)/2 - 1 >= 0) ? (index + 1)/2 - 1 : INT_MAX;
}

int leftIndex(int index) {
    return ((index + 1)*2) - 1;
}

int rightIndex(int index) {
    return (index + 1)*2;
}

int getParentPriority(PriorityQueue* queue, int index) {
    return (parentIndex(index) != INT_MAX) ? queue->array[parentIndex(index)]->priority : INT_MAX;
}

int getLeftPriority(PriorityQueue* queue, int index) {
    return (leftIndex(index) < queue->size) ? queue->array[leftIndex(index)]->priority : INT_MAX;
}

int getRightPriority(PriorityQueue* queue, int index) {
    return (rightIndex(index) < queue->size) ? queue->array[rightIndex(index)]->priority : INT_MAX;
}

void swap(PriorityQueue* queue, int first, int second) {
    Vertex *tmp = queue->array[first];
    queue->array[first] = queue->array[second];
    queue->array[second] = tmp;
    queue->positions[first] = second;
    queue->positions[second] = first;
}

PriorityQueue *priorityQueueInit(int fromVertex) {
    PriorityQueue *pq = malloc(sizeof(PriorityQueue));
    for (int i = 0; i < SIZE; ++i) {
        pq->array[i] = malloc(sizeof(Vertex));
        pq->array[i]->index = i;
        pq->array[i]->priority = INF;
        pq->positions[i] = i;
    }
    pq->array[fromVertex]->index = fromVertex;
    pq->array[fromVertex]->priority = 0;
    swap(pq, 0, fromVertex);
    pq->size = SIZE;
    return pq;
}

void heapify(PriorityQueue* queue, int index) {
    int smallest = index;
    if (leftIndex(index) <= queue->size - 1 && getLeftPriority(queue, index) < queue->array[smallest]->priority) {
        smallest = leftIndex(index);
    }
    if (rightIndex(index) <= queue->size - 1 && getRightPriority(queue, index) < queue->array[smallest]->priority) {
        smallest = rightIndex(index);
    }
    if (smallest != index) {
        swap(queue, index, smallest);
        heapify(queue, smallest);
    }
}

void decreaseKey(PriorityQueue* queue, int vertex, int value) {
    int index = queue->positions[vertex];
    if (queue->array[index]->priority > value) {
        queue->array[index]->priority = value;
        while (index > 0 && getParentPriority(queue, index) > queue->array[index]->priority) {
            swap(queue, index, parentIndex(index));
            index = parentIndex(index);
        }
    }
}

Vertex extractMin(PriorityQueue* queue) {
    Vertex min = {queue->array[0]->index, queue->array[0]->priority};
    free(queue->array[0]);
    queue->array[0] = queue->array[queue->size-1];
    queue->positions[queue->array[0]->index] = 0;
    queue->array[queue->size-1] = NULL;
    queue->size--;
    heapify(queue, 0);
    return min;
}

void freeQueue(PriorityQueue* queue) {
    for (int i = 0; i < queue->size; ++i) {
        free(queue->array[i]);
    }
    free(queue);
}

bool isPriorityQueueEmpty(PriorityQueue* queue) {
    return queue->size == 0 || queue->array[0]->priority == INT_MAX;
}
