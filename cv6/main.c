#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_HEAP_SIZE 100
#define INT_MAX 2147483647
/**
 * @brief Struktura MinHeap slouzi k reprezentaci haldy
 * atribut size udava pocet prvku v halde
 * atribut array je pole prvku haldy
 **/
typedef struct MinHeap {
    int size;
    int array[MAX_HEAP_SIZE];
} MinHeap;

/**
 *  Naimplementujte nasledujici funkce pro ziskani prvku v halde:
 *  POZOR: Nezapomente ze indexujeme pole od 0
 **/

 /**
  * @brief Vrati index rodice prvku na pozici 'index'
  * Pokud neexistuje vrati INT_MAX
  **/
int parentIndex (int index) {
    if(index != 0)
        return (index-1)/2;
    //TODO
    return INT_MAX;
}

/**
 * @brief Vrati index leveho potomka prvku na pozici 'index'
 **/
int leftIndex (int index) {
    //TODO
    return 2*index+1;
}

/**
 * @brief Vrati index praveho potomka prvku na pozici 'index'
 **/
int rightIndex (int index) {
    //TODO
    return 2*index+2;
}

/**
 * @brief Vrati rodice prvku na pozici 'index' v halde 'heap'
 * Pokud neexistuje vrati INT_MAX
 */
int getParent (MinHeap* heap, int index) {
    //TODO
    if(heap == NULL || heap->size<=index || index <= 0)
        return INT_MAX;
    else
        return heap->array[(index-1)/2];
}

/**
 *  @brief Vrati leveho potomka prvku na pozici 'index' v halde 'heap'
 *  Pokud potomek neexistuje vrati INT_MAX
 **/
int getLeft(MinHeap* heap, int index) {
    //TODO
    if(2*index+1 >= heap->size || index < 0 || heap == NULL)
        return INT_MAX;\
    else
        return heap->array[2*index+1];
}

/**
 * @brief Vrati praveho potomka prvku na pozici 'index' v halde 'heap'
 * Pokud potomek neexistuje vrati INT_MAX
 **/
int getRight(MinHeap* heap, int index) {
    //TODO
    if(2*index+2 >= heap->size || index < 0 || heap == NULL)
        return INT_MAX;\
    else
        return heap->array[2*index+2];
}

/**
 * @brief Prohodi prvky s indexy 'first' a 'second' v halde 'heap'
 **/
void swap(MinHeap* heap, int first, int second) {
    //TODO
    int save = heap->array[first];
    heap->array[first] = heap->array[second];
    heap->array[second] = save;
}

/**
 * @brief Opravi haldu 'heap' tak aby splnovala vlastnost minimove haldy
 * Kontrola zacina u prvku na pozici 'index'
 */
void heapify(MinHeap* heap, int index) {
    //TODO
    int left = leftIndex(index);
    int right = rightIndex(index);
    int min = index;
    if(left < heap->size && heap!=NULL) {

        if(right<heap->size) {

            if(heap->array[left] < heap->array[right] &&
                    heap->array[left] < heap->array[index]) {
                min = left;
            }
            else if(heap->array[right] < heap->array[index] &&
                    heap->array[right] < heap->array[left]) {
                min = right;
            } else if(heap->array[right] < heap->array[index])
                min = right;
       } else {
            if(heap->array[left]<heap->array[index])
                min = left;
        }
        if(min!=index) {
            swap(heap, min, index);
            heapify(heap, min);
        }
    }

}

/**
 * @brief Vytvori korektni minimovou haldu z pole 'array'
 */
MinHeap* buildHeap(int* array, int size) {
    MinHeap *heap = malloc(sizeof(MinHeap));
    //TODO
    if(size > 100 || size <= 0)
        return NULL;
    memset(heap->array, 0, 100);
    heap->size = size;

    for(int i=0;i<size;i++) {
        heap->array[i] = array[i];
    }

    for(int i=(heap->size/2-1);i>=0;i--) {
        heapify(heap, i);
    }
    return heap;
}

/**
 * @brief Snizi hodnotu prvku haldy 'heap' na pozici 'index' na hodnotu 'value'
 * a opravi vlastnost haldy 'heap'
 */
void decreaseKey(MinHeap* heap, int index, int value) {
    if(heap!=NULL && index < heap->size) {
        if(value<heap->array[index]) {
            heap->array[index] = value;
            for(int i=index/2-1;i>=0;i--) {
                heapify(heap, i);
            }
        }
    }
}

/**
 * @brief Vlozi hodnotu 'value' do haldy 'heap'
 */
void insert(MinHeap* heap, int element) {
    if(heap->size>0) {
        heap->array[heap->size] = heap->array[0];
    }
    heap->array[0] = element;
    heap->size+=1;
    for(int i=heap->size/2-1;i>=0;i--)
        heapify(heap, i);
}

/**
 * @brief Odstrani minimalni prvek haldy 'heap'
 * Vraci hodnotu odstraneneho prvku
 * Pokud je halda prazdna vraci INT_MAX
 */
int extractMin(MinHeap* heap) {
    if(heap == NULL || heap->size == 0)
        return INT_MAX;

    swap(heap, heap->size-1, 0);
    heap->size -= 1;
    for(int i=heap->size/2-1;i>=0;i--)
        heapify(heap, i);
    return heap->array[heap->size];
}

/**
 * @brief Seradi pole 'array' pomoci haldy od nejvetsiho prvku po nejmensi
 * Pole 'array' je velikosti 'size'
 * Vraci serazene pole
 **/
int* heapSort(int *array, int size) {
    MinHeap*heap = buildHeap(array, size);
    for(int i=heap->size-1;i>0;i--) {
        swap(heap, 0, i);
        heap->size-=1;
        for(int j=heap->size/2-1;j>=0;j--)
            heapify(heap, j);
    }
    for(int i=0;i<size;i++)
        array[i] = heap->array[i];
    free(heap);
    return array;
}

/**
 * @brief Graphviz funkce
 * vytvori haldu jako graf ve formatu ".dot"
 *
 * Dodatek k graphvizu:
 * Graphviz je nastroj, ktery vam umozni vizualizaci datovych struktur,
 * coz se hodi predevsim k ladeni.
 * Tento program generuje nekolik souboru neco.dot v mainu
 * Vygenerovane soubory nahrajte do online nastroje pro zobrazeni graphvizu:
 * http://sandbox.kidstrythisathome.com/erdos/
 * Alternativne si muzete nainstalovat prekladac z jazyka dot do obrazku na svuj
 * pocitac.
 **/
void makeGraphviz(MinHeap* heap, int index, FILE* pFile) {
    fprintf(pFile, "\"%i\"\n", heap->array[index]);
    if(leftIndex(index) < heap->size) {
        fprintf(pFile, "\"%i\" -> \"%i\"\n", heap->array[index], getLeft(heap, index));
        makeGraphviz(heap, leftIndex(index), pFile);
    }
    if(rightIndex(index) < heap->size) {
        fprintf(pFile, "\"%i\" -> \"%i\"\n", heap->array[index], getRight(heap, index));
        makeGraphviz(heap, rightIndex(index), pFile);
    }
}

void makeGraph(MinHeap* heap, const char* name) {
    FILE* pFile;
    pFile = fopen(name , "w");
    fprintf(pFile, "digraph Heap {\n");
    fprintf(pFile, "node [color=lightblue2, style=filled];\n");
    if (heap->size > 0)
        makeGraphviz(heap, 0, pFile);
    fprintf(pFile, "}\n");
    fclose(pFile);
}

int testIndexes() {
    printf("Test 1. indexovani parent, left, right: ");
    if (parentIndex(2) != 0 || parentIndex(1) != 0 || parentIndex(0) != INT_MAX) {
        printf("NOK - chybny parentIndex\n");
        return 0;
    }
    if (leftIndex(0) != 1 || leftIndex(3) != 7) {
        printf("NOK - chybny leftIndex\n");
        return 0;
    }
    if (rightIndex(0) != 2 || rightIndex(3) != 8) {
        printf("NOK - chybny rightIndex\n");
        return 0;
    }

    MinHeap heap;
    int tmp[] = {1,2,3};
    heap.size = 3;
    for (unsigned int i = 0; i < heap.size; i++) {
        heap.array[i] = tmp[i];
    }

    if (getParent(&heap,0) != INT_MAX || getParent(&heap,1) != 1 ||
            getParent(&heap,2) != 1) {
        printf("NOK - chyba ve funkci getParent\n");
        return 0;
    }
    if (getLeft(&heap,0) != 2 || getLeft(&heap,1) != INT_MAX) {
        printf("NOK - chyba ve funkci getLeft\n");
        return 0;
    }
    if (getRight(&heap,0) != 3 || getRight(&heap,1) != INT_MAX) {
        printf("NOK - chyba ve funkci getRight\n");
        return 0;
    }
    printf("OK\n");
    return 1;
}

int cmpArray(int *a1, int *a2, int size) {
    for (unsigned int i = 0; i < size; i++) {
        if (a1[i] != a2[i])
            return 0;
    }
    return 1;
}

void testBuildHeap() {
    printf("Test 2. buildHeap: ");
    int tmp[3] = {4, 3, 1};
    MinHeap *heap = buildHeap(tmp, 3);
    int res1[3] = {1,3,4};
    int res2[3] = {1,4,3};

    if ((cmpArray(heap->array, res1, 3) ||
        cmpArray(heap->array, res2, 3)) &&
        heap->size == 3) {
        printf("OK\n");
    } else {
        printf("NOK - chyba ve funkci buildHeap\n");
    }

    makeGraph(heap, "built.dot");
    printf("Vykreslenou haldu najdete v souboru build.dot\n");
    free(heap);
}

void testInsertHeap() {
    printf("Test 3. insertHeap: ");
    MinHeap *heap = malloc(sizeof(MinHeap));
    heap->size = 0;

    insert(heap, 2);

    int res1[1] = {2};
    int res2[3] = {2,3,4};
    int res3[4] = {2,3,4,5};
    int res4[5] = {1,2,4,5,3};

    if (!cmpArray(heap->array, res1, 1) || (heap->size != 1)) {
        printf("NOK - chyba ve funkci insert na prazdne halde\n");
    } else {
        insert(heap, 3);
        insert(heap, 4);
        if (!cmpArray(heap->array, res2, 3) || (heap->size != 3)) {
            printf("NOK - chyba ve funkci insert na neprazdne halde\n");
        } else {
            insert(heap, 5);
            if (!cmpArray(heap->array, res3, 4) || (heap->size != 4)) {
                printf("NOK - chyba ve funkci insert na neprazdne halde\n");
            } else {
                insert(heap, 1);
                if (!cmpArray(heap->array, res4, 5) || (heap->size != 5)) {
                    printf("NOK - chyba ve funkci insert na neprazdne halde\n");
                }else{
                    printf("OK\n");
                }
            }
        }
    }
    makeGraph(heap, "insert.dot");
    printf("Vykreslenou haldu najdete v souboru insert.dot\n");

    free(heap);
}

void testDecreaseKey() {
    printf("Test 4. decreaseKey: ");
    MinHeap heap;
    int tmp[] = {2,3,4};
    heap.size = 3;
    for (unsigned int i = 0; i < heap.size; i++) {
        heap.array[i] = tmp[i];
    }

    decreaseKey(&heap, 2, 1);

    int res1[3] = {1,3,2};

    if (!cmpArray(heap.array, res1, 3) || (heap.size != 3)) {
        printf("NOK - chyba ve funkci decreaseKey\n");
    } else {
        decreaseKey(&heap, 0, 4);
        if (!cmpArray(heap.array, res1, 3) || (heap.size != 3)) {
            printf("NOK - chyba ve funkci decreaseKey\n");
        } else {
            printf("OK\n");
        }
    }
    makeGraph(&heap, "decrease.dot");
    printf("Vykreslenou haldu najdete v souboru decrease.dot\n");
}

void testExtractMin() {
    printf("Test 5. extractMin: ");
    MinHeap heap;
    int array[] = {2,3,4,5};
    heap.size = 4;
    for (unsigned int i = 0; i < heap.size; i++) {
        heap.array[i] = array[i];
    }

    int tmp = extractMin(&heap);

    int res1[3] = {3,5,4};
    int res2[2] = {4,5};
    int res3[1] = {5};

    if (!cmpArray(heap.array, res1, 3) || (heap.size != 3) || (tmp != 2)) {
        printf("NOK - chyba ve funkci extractMin\n");
    } else {
        tmp = extractMin(&heap);
        if (!cmpArray(heap.array, res2, 2) || (heap.size != 2) || (tmp != 3)) {
            printf("NOK - chyba ve funkci extractMin\n");
        } else{
            tmp = extractMin(&heap);
            if (!cmpArray(heap.array, res3, 1) || (heap.size != 1) || (tmp != 4)) {
                printf("NOK - chyba ve funkci extractMin\n");
            } else {
                tmp = extractMin(&heap);
                if ((heap.size != 0) || (tmp != 5)) {
                    printf("NOK - chyba ve funkci extractMin\n");
                } else {
                    if (extractMin(&heap) == INT_MAX) {
                        printf("OK\n");
                    } else {
                        printf("NOK - chyba ve funkci extractMin na prazne halde\n");
                    }
                }
            }
        }
    }
    makeGraph(&heap, "extract.dot");
    printf("Vykreslenou haldu najdete v souboru extract.dot\n");
}

void testHeapSort() {
    int array[] = {8,4,9,3,2,7,5,0,6,1};
    printf("Test 6. heapSort: ");

    int res[10] = {9,8,7,6,5,4,3,2,1,0};

    if (!cmpArray(heapSort(array, 10), res, 10)) {
        printf("NOK - chyba ve funkci heapSort, vraci neserazene pole\n");
    } else {
        printf("OK\n");
    }
}

int main() {
    if (testIndexes()) {
        testBuildHeap();
        testInsertHeap();
        testDecreaseKey();
        testExtractMin();
        testHeapSort();
    }
    int array[9] = {4,2,3,5,1,6,4,7,9};
    MinHeap* myHeap = buildHeap(array, 9);
    printf("%d", extractMin(myHeap));
    int* a = heapSort(array, 9);

    return 0;
}
