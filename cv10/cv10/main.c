#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/**
 * Struktura reprezentujici dvojici klice 'key' a hodnoty 'data'
 **/
typedef struct Pair {
    int key;
    char data;
} Pair;

/**
 * Struktura Node slouzi pro reprezentaci objektu v oboustranne spojovanem seznamu
 * atribut pair je ukazatel na ulozenou dvojici (klic, data)
 * atribut next je ukazatel na nasledujici prvek v seznamu
 * atribut prev je ukazatel na predchazejici prvek v seznamu
 **/
typedef struct Node {
    Pair* pair;
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

//velikost hasovaci tabulky
#define SIZE 10

/**
 * Struktura HashTable reprezentujici hasovaci tabulku
 * atribut 'array' je pole ukazatelu na zretezene seznamy
 * zretezene seznamy obsahuji ukazatele na dvojice Pair,
 * ktere jsou indexovany podle indexu pole
 **/
typedef struct HashTable {
    LinkedList* array[SIZE];
} HashTable;

/**
 * Metoda insert() vlozi na konec (za prvek last) seznamu novy uzel s hodnotou pair
 **/
void listInsert(LinkedList* list, Pair* pair) {
    Node* added = malloc(sizeof(Node));
    added->pair = pair;
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
void listDelete(LinkedList* list, Node* element) {
    if(element->prev == NULL) {
        list->first = element->next;
    }
    else {
        element->prev->next = element->next;
    }
    if(element->next == NULL) {
        list->last = element->prev;
    }
    else {
        element->next->prev = element->prev;
    }
    free(element->pair);
    free(element);
}

/**
 * Metoda search() vraci referenci na prvni vyskyt uzlu s klicem 'key'
 * pokud se hodnota v seznamu nenachazi, vraci NULL
 **/
Node* listSearch(LinkedList* list, int key) {
    Node* element = list->first;
    while (element != NULL && element->pair->key != key) {
        element = element->next;
    }
    return element;
}

/**
 * Inicializuje hashovaci tabulku
 **/
HashTable* hashTableInit() {
    HashTable* table = malloc(sizeof(HashTable));
    for (int i = 0; i < SIZE; ++i) {
        table->array[i] = malloc(sizeof(LinkedList));
        table->array[i]->first = table->array[i]->last = NULL;
    }
    return table;
}

/**
 * Funkce vypocita hodnotu hasovaci funkce pro klic 'key' na zaklade velikosti tabulky
 * Hashovaci funkce f(n) = n mod 'SIZE'
 **/
int hash(int key) {
    return key % SIZE;
}

/**
 * Vytvori dvojici 'Pair' z hodnot 'key' a 'data'
 * Pote vlozi vytvorenu dvojici do tabulky
 **/
void insertHashtable(HashTable* table, int key, char data)
{
    Pair *p = malloc(sizeof(Pair));
    p->data = data;
    p->key = key;
    listInsert(table->array[hash(key)], p);
}

/**
 * Najde dvojici s klicem 'key' a vrati klici prirazenou hodnotu 'data'
 * Pokud se klic v tabulce nenachazi vraci '\0'
 **/
char getHashtable(HashTable* table, int key)
{
    int index = hash(key);
    Node* found = listSearch(table->array[index], key);
    if(found!=NULL && found->pair->key == key)
        return found->pair->data;
    return '\0';
}

/**
 * Odstrani prvni vyskyt dvojice s klicem 'key'
 **/
void removeHashtable(HashTable* table, int key)
{
    int index = hash(key);
    Node* found = listSearch(table->array[index], key);
    if(found!=NULL) {
        if(!found->prev && !found->next) {
            table->array[index]->last = table->array[index]->first = NULL;
        } else if(!found->next) {
            table->array[index]->last = found->prev;
            found->prev->next = NULL;
        } else if(!found->prev) {
            found->next->prev = NULL;
            table->array[index]->first = found->next;
        } else {
            found->prev->next = found->next;
            found->next->prev = found->prev;
        }
        free(found->pair);
        free(found);
    }
}

int numOfKeys(HashTable* table) {
    int num = 0;
    Node* ptr = NULL;
    for(int i=0;i<SIZE;i++) {
        ptr = table->array[i]->first;
        while(ptr!=NULL){
            ptr = ptr->next;
            num++;
        }
    }
    return num;
}

/**
 * Vrati pole vsech klicu v tabulce
 * Funkce musi vypocitat pocet klicu a naalokovat si dostatecne velke pole
 **/
int* keysHashtable(HashTable* table)
{
    Node* ptr = NULL;
    int num = numOfKeys(table);
    int j=0;
    int *array = calloc(num*sizeof(int),5);
    for(int i=0;i<SIZE;i++) {
        ptr = table->array[i]->first;
        while(ptr!=NULL){
            array[j] = ptr->pair->key;
            ptr = ptr->next;
            j++;
        }
    }
    return array;
}

/**
 * Vrati pole vsech hodnot v tabulce
 * Funkce musi vypocitat pocet hodnot a naalokovat si dostatecne velke pole
 **/
char* valuesHashtable(HashTable* table)
{
    int num = numOfKeys(table);
    char *array = calloc(num*sizeof(char),5);
    Node* ptr = NULL;
    int j=0;

    for(int i=0;i<SIZE;i++) {
        ptr = table->array[i]->first;
        while(ptr!=NULL){
            array[j] = ptr->pair->data;
            ptr = ptr->next;
            j++;
        }
    }

    return array;
}

//XXXXXXXXXXXXXXXXXXX TESTY XXXXXXXXXXXXXXXXXXX

void test_hash()
{
    printf("Test 1. hasovaci funkce (hash): \n");
    if (hash(10) != (10 % SIZE)){
        printf("NOK - nekorektni hasovani.\n");
        printf("Vase hodnota %d != %d\n", hash(10), SIZE % 10);
        return;
    }
    if (hash(5323) != (5323 % SIZE)){
        printf("NOK - nekorektni hasovani.\n");
        printf("Vase hodnota %d != %d\n", hash(5323), 5323 % SIZE);
        return;
    }
    if (hash(65321) != (65321 % SIZE)){
        printf("NOK - nekorektni hasovani.\n");
        printf("Vase hodnota %d != %d\n", hash(65321), 65321 % SIZE);
        return;
    }
    printf("OK\n");
}

void freeTable(HashTable *table)
{
    for (int i = 0; i < SIZE; i++) {
        Node *n = table->array[i]->first;
        while (n != NULL) {
            Node *next = n->next;
            free(n->pair);
            free(n);
            n = next;
        }
        free(table->array[i]);
    }
    free(table);
}

void test_insert(){
    printf("Test 2. vkladani do tabulky (insert):\n");
    HashTable *t = hashTableInit();
    int key = SIZE - 1;
    char value = 'A';
    insertHashtable(t, key, value);
    if(t->array[key % SIZE]->first == NULL){
        printf("NOK - nekorektni vkladani do tabulky\n");
        printf("V tabulce je po vlozeni NULL\n");
        freeTable(t);
        return;
    }
    if(t->array[key % SIZE]->first->pair->data != value){
        printf("NOK - nekorektni vkladani do tabulky\n");
        printf("Na pozici hash(%d) se na prvni pozici v seznamu nenachazi '%c'\n", key, value);
        printf("Ve vasi tabulce se na pozici hash(%d) nachazi '%c'\n", key, t->array[key % SIZE]->first->pair->data);
        freeTable(t);
        return;
    }
    key = SIZE * 2;
    value = 'B';
    insertHashtable(t, key, value);
    if(t->array[key % SIZE]->first == NULL){
        printf("NOK - nekorektni vkladani do tabulky\n");
        printf("V tabulce je po vlozeni NULL\n");
        freeTable(t);
        return;
    }
    if(t->array[key % SIZE]->first->pair->data != value){
        printf("NOK - nekorektni vkladani do tabulky");
        printf("Na pozici hash(%d) se na prvni pozici v seznamu nenachazi '%c'",key, value);
        printf("Ve vasi tabulce se na pozici hash(%d) nachazi '%c'\n", key, t->array[key % SIZE]->first->pair->data);
        freeTable(t);
        return;
    }
    key = 2 * SIZE - 1;
    value = 'C';
    insertHashtable(t, key, value);
    if(t->array[key % SIZE]->first == NULL){
        printf("NOK - nekorektni vkladani do tabulky\n");
        printf("Ve vasi tabulce je po vlozeni NULL\n");
        freeTable(t);
        return;
    }
    if(t->array[key % SIZE]->first->next == NULL){
        printf("NOK - nekorektni vkladani do tabulky\n");
        printf("Ve vasi tabulce je po vlozeni NULL\n");
        freeTable(t);
        return;
    }
    if(t->array[key % SIZE]->first->next->pair->data != value){
        printf("NOK - nekorektni vkladani do tabulky\n");
        printf("Na pozici hash(%d) se na prvni pozici v seznamu nenachazi '%c'\n",key, value);
        printf("Ve vasi tabulce se na pozici hash(%d) nachazi '%c'\n", key, t->array[key % SIZE]->first->next->pair->data);
        freeTable(t);
        return;
    }
    key = 0;
    value = 'D';
    insertHashtable(t, key, value);
    if(t->array[key % SIZE]->last == NULL){
        printf("NOK - nekorektni vkladani do tabulky\n");
        printf("Ve vasi tabulce je po vlozeni NULL\n");
        freeTable(t);
        return;
    }
    if(t->array[key % SIZE]->last->pair->data != value){
        printf("NOK - nekorektni vkladani do tabulky\n");
        printf("Na pozici hash(%d) se na prvni pozici v seznamu nenachazi '%c'\n",key, value);
        printf("Ve vasi tabulce se na pozici hash(%d) nachazi '%c'\n", key, t->array[key % SIZE]->last->pair->data);
        freeTable(t);
        return;
    }
    printf("OK\n");
    freeTable(t);
}

HashTable* init_test_table()
{
    HashTable *t = hashTableInit();
    Pair * p1 = malloc(sizeof(Pair));
    p1->key = 0;
    p1->data = 'A';
    Pair * p2 = malloc(sizeof(Pair));
    p2->key = SIZE;
    p2->data = 'B';
    Pair * p3 = malloc(sizeof(Pair));
    p3->key = 1;
    p3->data = 'C';
    Pair * p4 = malloc(sizeof(Pair));
    p4->key = 2 * SIZE;
    p4->data = 'D';
    Pair * p5 = malloc(sizeof(Pair));
    p5->key = 2 * SIZE - 2;
    p5->data = 'E';

    listInsert(t->array[0], p1);
    listInsert(t->array[0], p2);
    listInsert(t->array[0], p4);
    listInsert(t->array[1], p3);
    listInsert(t->array[SIZE - 2], p5);
    return t;
}

void test_get(){
    printf("Test 3. hledani v tabulce (get):\n");
    HashTable *t = init_test_table();

    char res = getHashtable(t, 0);
    char correct = 'A';
    if(res != correct){
        printf("NOK - nekorektni hledani v tabulce\n");
        printf("Tabulka vraci %c != %c\n", res, correct);
        freeTable(t);
        return;
    }
    res = getHashtable(t, SIZE);
    correct = 'B';
    if(res != correct){
        printf("NOK - nekorektni hledani v tabulce\n");
        printf("Tabulka vraci %c != %c\n", res, correct);
        freeTable(t);
        return;
    }
    res = getHashtable(t, 1);
    correct = 'C';
    if(res != correct){
        printf("NOK - nekorektni hledani v tabulce\n");
        printf("Tabulka vraci %c != %c\n", res, correct);
        freeTable(t);
        return;
    }
    res = getHashtable(t, 2 * SIZE);
    correct = 'D';
    if(res != correct){
        printf("NOK - nekorektni hledani v tabulce\n");
        printf("Tabulka vraci %c != %c\n", res, correct);
        freeTable(t);
        return;
    }
    res = getHashtable(t, 2 * SIZE - 2);
    correct = 'E';
    if(res != correct){
        printf("NOK - nekorektni hledani v tabulce\n");
        printf("Tabulka vraci %c != %c\n", res, correct);
        freeTable(t);
        return;
    }
    res = getHashtable(t, 3 * SIZE - 2);
    correct = '\0';
    if(res != correct){
        printf("NOK - nekorektni hledani v tabulce\n");
        printf("Tabulka vraci %c != %c\n", res, correct);
        freeTable(t);
        return;
    }
    res = getHashtable(t, SIZE - 3);
    correct = '\0';
    if(res != correct){
        printf("NOK - nekorektni hledani v tabulce\n");
        printf("Tabulka vraci %c != %c\n", res, correct);
        freeTable(t);
        return;
    }
    printf("OK\n");
    freeTable(t);
}

void test_remove(){
    printf("Test 4. odstranovani z tabulky (remove):\n");
    HashTable *t = init_test_table();

    int key = 1;
    removeHashtable(t, key);
    if (t->array[key % SIZE]->first != NULL){
        printf("NOK - nekorektni odebirani prvku s klicem %d\n", key);
        freeTable(t);
        return;
    }
    key = 2 * SIZE - 2;
    removeHashtable(t, key);
    if (t->array[key % SIZE]->first != NULL){
        printf("NOK - nekorektni odebirani prvku s klicem %d\n", key);
        freeTable(t);
        return;
    }
    key = 0;
    removeHashtable(t, key);
    if ((t->array[key % SIZE]->first == NULL) ||
        (t->array[key % SIZE]->first->pair->data != 'B')){
        printf("NOK - nekorektni odebirani prvku s klicem %d\n", key);
        freeTable(t);
        return;
    }
    key = 2 * SIZE;
    removeHashtable(t, key);
    if ((t->array[key % SIZE]->first == NULL) ||
        (t->array[key % SIZE]->first->pair->data != 'B')){
        printf("NOK - nekorektni odebirani prvku s klicem %d\n", key);
        freeTable(t);
        return;
    }
    key = SIZE;
    removeHashtable(t, key);
    if (t->array[key % SIZE]->first != NULL){
        printf("NOK - nekorektni odebirani prvku s klicem %d\n", key);
        freeTable(t);
        return;
    }
    printf("OK\n");
    freeTable(t);
}

bool sameKeys(int * k1, int *k2, int n) {
    bool r = true;
    for(int i = 0; i < n; i++) {
        r = (k1[i] == k2[i]) && r;
    }
    return r;
}

void printArrayInt(int *a, int n)
{
    printf("[");
    for (int i =0; i < n - 1; i++) {
        printf("%d, ",a[i]);
    }
    printf("%d]",a[n - 1]);
}

void test_keys(){
    printf("Test 5. seznam klicu (keys):\n");
    HashTable *t = init_test_table();
    int *k = keysHashtable(t);

    int res[5] = {0, SIZE, 2 * SIZE, 1, 2 * SIZE - 2};
    if (!sameKeys(k, res, 5)){
        printf("NOK - nekorektni vypis klicu ");
        printArrayInt(res, 5); printf("\n");
        printf("Vas vystup: ");
        printArrayInt(k, 5); printf("\n");
        free(k);
        freeTable(t);
        return;
    }

    Pair *p = malloc(sizeof(Pair));
    p->key = SIZE / 2;
    p->data = 'G';
    listInsert(t->array[SIZE/2], p);
    free(k);
    k = keysHashtable(t);
    int res2[6] = {0, SIZE, 2 * SIZE, 1, SIZE/2,2 * SIZE - 2};
    if (!sameKeys(k, res2, 6)){
        printf("NOK - nekorektni vypis klicu ");
        printArrayInt(res2, 6); printf("\n");
        printf("Vas vystup: ");
        printArrayInt(k, 6); printf("\n");
        free(k);
        freeTable(t);
        return;
    }

    printf("OK\n");
    free(k);
    freeTable(t);
}

bool sameValues(char * k1, char *k2, int n) {
    bool r = true;
    for(int i = 0; i < n; i++) {
        r = (k1[i] == k2[i]) && r;
    }
    return r;
}

void printArrayChar(char *a, int n)
{
    printf("[");
    for (int i =0; i < n - 1; i++) {
        printf("%c, ",a[i]);
    }
    printf("%c]",a[n - 1]);
}

void test_values(){
    printf("Test 6. seznam hodnot (value):\n");
    HashTable *t = init_test_table();
    char *k = valuesHashtable(t);
    char res[5] = {'A', 'B', 'D', 'C', 'E'};
    if (!sameValues(k, res, 5)){
        printf("NOK - nekorektni vypis klicu ");
        printArrayChar(res, 5); printf("\n");
        printf("Vas vystup: ");
        printArrayChar(k, 5); printf("\n");
        free(k);
        freeTable(t);
        return;
    }

    Pair *p = malloc(sizeof(Pair));
    p->key = SIZE / 2;
    p->data = 'G';
    listInsert(t->array[SIZE/2], p);
    free(k);
    k = valuesHashtable(t);
    char res2[6] = {'A', 'B', 'D', 'C', 'G', 'E'};
    if (!sameValues(k, res2, 6)){
        printf("NOK - nekorektni vypis klicu ");
        printArrayChar(res2, 6); printf("\n");
        printf("Vas vystup: ");
        printArrayChar(k, 6); printf("\n");
        free(k);
        freeTable(t);
        return;
    }
    printf("OK\n");
    free(k);
    freeTable(t);
}

int main(void)
{
    test_hash();
    printf("\n");
    test_insert();
    printf("\n");
    test_get();
    printf("\n");
    test_remove();
    printf("\n");
    test_keys();
    printf("\n");
    test_values();
    printf("\n");
    return 0;
}
