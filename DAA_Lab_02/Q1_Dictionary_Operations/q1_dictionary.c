/*
 * DAA Lab-02, Q1: Dictionary Operations
 * -------------------------------------
 * Implements Search, Insert, Delete, Max, Min, Predecessor, Successor
 * over six underlying structures:
 *   1. Unsorted array
 *   2. Sorted array
 *   3. Singly linked unsorted list
 *   4. Singly linked sorted list
 *   5. Doubly linked unsorted list
 *   6. Doubly linked sorted list
 *
 * Benchmarks each operation's running time as n grows, writes CSV:
 *   structure,operation,n,avg_time_ns
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXN 40000
#define NEG_INF (-2147483647)
#define POS_INF (2147483647)

/* ============================================================
   1. UNSORTED ARRAY
   ============================================================ */
typedef struct { int data[MAXN]; int size; } UArr;
void uarr_init(UArr *a){ a->size = 0; }
void uarr_insert(UArr *a, int x){ a->data[a->size++] = x; }
int  uarr_search(UArr *a, int k){
    for (int i = 0; i < a->size; i++) if (a->data[i] == k) return i;
    return -1;
}
void uarr_delete_idx(UArr *a, int idx){          /* O(1): swap with last */
    a->data[idx] = a->data[a->size - 1];
    a->size--;
}
int uarr_max(UArr *a){ int m = a->data[0]; for (int i=1;i<a->size;i++) if (a->data[i]>m) m=a->data[i]; return m; }
int uarr_min(UArr *a){ int m = a->data[0]; for (int i=1;i<a->size;i++) if (a->data[i]<m) m=a->data[i]; return m; }
int uarr_pred(UArr *a, int k){ int best=NEG_INF; for(int i=0;i<a->size;i++) if(a->data[i]<k && a->data[i]>best) best=a->data[i]; return best; }
int uarr_succ(UArr *a, int k){ int best=POS_INF; for(int i=0;i<a->size;i++) if(a->data[i]>k && a->data[i]<best) best=a->data[i]; return best; }

/* ============================================================
   2. SORTED ARRAY
   ============================================================ */
typedef struct { int data[MAXN]; int size; } SArr;
void sarr_init(SArr *a){ a->size = 0; }
int sarr_lower_bound(SArr *a, int k){
    int lo=0, hi=a->size;
    while (lo < hi){ int mid=(lo+hi)/2; if (a->data[mid]<k) lo=mid+1; else hi=mid; }
    return lo;
}
void sarr_insert(SArr *a, int x){
    int pos = sarr_lower_bound(a, x);
    for (int i = a->size; i > pos; i--) a->data[i] = a->data[i-1];
    a->data[pos] = x; a->size++;
}
int sarr_search(SArr *a, int k){
    int pos = sarr_lower_bound(a, k);
    if (pos < a->size && a->data[pos] == k) return pos;
    return -1;
}
void sarr_delete_idx(SArr *a, int idx){          /* O(n): shift left */
    for (int i = idx; i < a->size-1; i++) a->data[i] = a->data[i+1];
    a->size--;
}
int sarr_max(SArr *a){ return a->data[a->size-1]; }
int sarr_min(SArr *a){ return a->data[0]; }
int sarr_pred(SArr *a, int k){ int pos=sarr_lower_bound(a,k); if(pos==0) return NEG_INF; return a->data[pos-1]; }
int sarr_succ(SArr *a, int k){
    int pos = sarr_lower_bound(a, k);
    if (pos < a->size && a->data[pos] == k) pos++;
    if (pos >= a->size) return POS_INF;
    return a->data[pos];
}

/* ============================================================
   3 & 4. SINGLY LINKED LIST (unsorted / sorted share SNode)
   ============================================================ */
typedef struct SNode { int key; struct SNode *next; } SNode;

/* ---- 3. unsorted ---- */
typedef struct { SNode *head; } USList;
void uslist_init(USList *l){ l->head = NULL; }
SNode* uslist_insert(USList *l, int x){
    SNode *n = malloc(sizeof(SNode));
    n->key = x; n->next = l->head; l->head = n;
    return n;
}
SNode* uslist_search(USList *l, int k){
    SNode *p = l->head;
    while (p){ if (p->key == k) return p; p = p->next; }
    return NULL;
}
void uslist_delete(USList *l, SNode *target){    /* O(n): must find prev */
    if (l->head == target){ l->head = target->next; free(target); return; }
    SNode *p = l->head;
    while (p && p->next != target) p = p->next;
    if (p){ p->next = target->next; free(target); }
}
int uslist_max(USList *l){ int m=l->head->key; SNode*p=l->head->next; while(p){ if(p->key>m)m=p->key; p=p->next;} return m; }
int uslist_min(USList *l){ int m=l->head->key; SNode*p=l->head->next; while(p){ if(p->key<m)m=p->key; p=p->next;} return m; }
int uslist_pred(USList *l, int k){ int best=NEG_INF; SNode*p=l->head; while(p){ if(p->key<k&&p->key>best)best=p->key; p=p->next;} return best; }
int uslist_succ(USList *l, int k){ int best=POS_INF; SNode*p=l->head; while(p){ if(p->key>k&&p->key<best)best=p->key; p=p->next;} return best; }

/* ---- 4. sorted ---- */
typedef struct { SNode *head; } SSList;
void sslist_init(SSList *l){ l->head = NULL; }
SNode* sslist_insert(SSList *l, int x){
    SNode *n = malloc(sizeof(SNode)); n->key = x;
    if (!l->head || l->head->key >= x){ n->next = l->head; l->head = n; return n; }
    SNode *p = l->head;
    while (p->next && p->next->key < x) p = p->next;
    n->next = p->next; p->next = n;
    return n;
}
SNode* sslist_search(SSList *l, int k){
    SNode *p = l->head;
    while (p && p->key < k) p = p->next;
    if (p && p->key == k) return p;
    return NULL;
}
void sslist_delete(SSList *l, SNode *target){    /* O(n): must find prev */
    if (l->head == target){ l->head = target->next; free(target); return; }
    SNode *p = l->head;
    while (p && p->next != target) p = p->next;
    if (p){ p->next = target->next; free(target); }
}
int sslist_min(SSList *l){ return l->head->key; }
int sslist_max(SSList *l){ SNode *p=l->head; while(p->next) p=p->next; return p->key; }
int sslist_pred(SSList *l, int k){
    SNode *p = l->head; int best = NEG_INF;
    while (p && p->key < k){ best = p->key; p = p->next; }
    return best;
}
int sslist_succ(SSList *l, int k){
    SNode *p = l->head;
    while (p && p->key <= k) p = p->next;
    return p ? p->key : POS_INF;
}

/* ============================================================
   5 & 6. DOUBLY LINKED LIST (unsorted / sorted share DNode)
   ============================================================ */
typedef struct DNode { int key; struct DNode *prev, *next; } DNode;

/* ---- 5. unsorted ---- */
typedef struct { DNode *head, *tail; } UDList;
void udlist_init(UDList *l){ l->head = l->tail = NULL; }
DNode* udlist_insert(UDList *l, int x){
    DNode *n = malloc(sizeof(DNode)); n->key = x; n->prev = NULL; n->next = l->head;
    if (l->head) l->head->prev = n;
    l->head = n;
    if (!l->tail) l->tail = n;
    return n;
}
DNode* udlist_search(UDList *l, int k){
    DNode *p = l->head;
    while (p){ if (p->key == k) return p; p = p->next; }
    return NULL;
}
void udlist_delete(UDList *l, DNode *n){         /* O(1): direct unlink */
    if (n->prev) n->prev->next = n->next; else l->head = n->next;
    if (n->next) n->next->prev = n->prev; else l->tail = n->prev;
    free(n);
}
int udlist_max(UDList *l){ int m=l->head->key; DNode*p=l->head->next; while(p){ if(p->key>m)m=p->key; p=p->next;} return m; }
int udlist_min(UDList *l){ int m=l->head->key; DNode*p=l->head->next; while(p){ if(p->key<m)m=p->key; p=p->next;} return m; }
int udlist_pred(UDList *l, int k){ int best=NEG_INF; DNode*p=l->head; while(p){ if(p->key<k&&p->key>best)best=p->key; p=p->next;} return best; }
int udlist_succ(UDList *l, int k){ int best=POS_INF; DNode*p=l->head; while(p){ if(p->key>k&&p->key<best)best=p->key; p=p->next;} return best; }

/* ---- 6. sorted ---- */
typedef struct { DNode *head, *tail; } SDList;
void sdlist_init(SDList *l){ l->head = l->tail = NULL; }
DNode* sdlist_insert(SDList *l, int x){
    DNode *n = malloc(sizeof(DNode)); n->key = x;
    if (!l->head || l->head->key >= x){
        n->prev = NULL; n->next = l->head;
        if (l->head) l->head->prev = n;
        l->head = n;
        if (!l->tail) l->tail = n;
        return n;
    }
    DNode *p = l->head;
    while (p->next && p->next->key < x) p = p->next;
    n->next = p->next; n->prev = p;
    if (p->next) p->next->prev = n; else l->tail = n;
    p->next = n;
    return n;
}
DNode* sdlist_search(SDList *l, int k){
    DNode *p = l->head;
    while (p && p->key < k) p = p->next;
    if (p && p->key == k) return p;
    return NULL;
}
void sdlist_delete(SDList *l, DNode *n){         /* O(1): direct unlink */
    if (n->prev) n->prev->next = n->next; else l->head = n->next;
    if (n->next) n->next->prev = n->prev; else l->tail = n->prev;
    free(n);
}
int sdlist_min(SDList *l){ return l->head->key; }
int sdlist_max(SDList *l){ return l->tail->key; }
int sdlist_pred_node(DNode *n){ return n->prev ? n->prev->key : NEG_INF; }
int sdlist_succ_node(DNode *n){ return n->next ? n->next->key : POS_INF; }

/* ============================================================
   BENCHMARK HARNESS
   ============================================================ */
static double now_ns(clock_t start, clock_t end, long reps){
    double secs = (double)(end - start) / CLOCKS_PER_SEC;
    return (secs * 1e9) / (double)reps;
}

void shuffle(int *arr, int n){
    for (int i = n-1; i > 0; i--){
        int j = rand() % (i+1);
        int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
    }
}

int main(void){
    srand(42);
    int sizes[] = {500, 1000, 2000, 4000, 8000, 16000};
    int nsizes = sizeof(sizes)/sizeof(sizes[0]);
    const int SEARCH_REPS = 200, MM_REPS = 500, PS_REPS = 200, IO_REPS = 200;

    FILE *fp = fopen("q1_dictionary_benchmark.csv", "w");
    fprintf(fp, "structure,operation,n,avg_time_ns\n");

    for (int s = 0; s < nsizes; s++){
        int n = sizes[s];
        /* pool of 2n distinct values; first n = initial keys, rest = "new" keys for insert */
        int *pool = malloc(sizeof(int) * 2 * n);
        for (int i = 0; i < 2*n; i++) pool[i] = i;
        shuffle(pool, 2*n);
        int *initial = pool;          /* first n */
        int *fresh = pool + n;        /* next n, guaranteed not yet inserted */

        clock_t t0, t1;

        /* ---------- 1. UNSORTED ARRAY ---------- */
        {
            UArr a; uarr_init(&a);
            for (int i = 0; i < n; i++) uarr_insert(&a, initial[i]);

            t0 = clock();
            for (int r = 0; r < SEARCH_REPS; r++) uarr_search(&a, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "UnsortedArray,Search,%d,%.2f\n", n, now_ns(t0,t1,SEARCH_REPS));

            t0 = clock();
            for (int r = 0; r < MM_REPS; r++) uarr_max(&a);
            t1 = clock();
            fprintf(fp, "UnsortedArray,Max,%d,%.2f\n", n, now_ns(t0,t1,MM_REPS));

            t0 = clock();
            for (int r = 0; r < MM_REPS; r++) uarr_min(&a);
            t1 = clock();
            fprintf(fp, "UnsortedArray,Min,%d,%.2f\n", n, now_ns(t0,t1,MM_REPS));

            t0 = clock();
            for (int r = 0; r < PS_REPS; r++) uarr_pred(&a, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "UnsortedArray,Predecessor,%d,%.2f\n", n, now_ns(t0,t1,PS_REPS));

            t0 = clock();
            for (int r = 0; r < PS_REPS; r++) uarr_succ(&a, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "UnsortedArray,Successor,%d,%.2f\n", n, now_ns(t0,t1,PS_REPS));

            t0 = clock();
            for (int r = 0; r < IO_REPS; r++) uarr_insert(&a, fresh[r]);
            t1 = clock();
            fprintf(fp, "UnsortedArray,Insert,%d,%.2f\n", n, now_ns(t0,t1,IO_REPS));

            t0 = clock();
            for (int r = 0; r < IO_REPS; r++){
                int idx = uarr_search(&a, initial[rand()%n]);
                if (idx >= 0) uarr_delete_idx(&a, idx);
            }
            t1 = clock();
            fprintf(fp, "UnsortedArray,Delete,%d,%.2f\n", n, now_ns(t0,t1,IO_REPS));
        }

        /* ---------- 2. SORTED ARRAY ---------- */
        {
            SArr a; sarr_init(&a);
            for (int i = 0; i < n; i++) sarr_insert(&a, initial[i]);

            t0 = clock();
            for (int r = 0; r < SEARCH_REPS; r++) sarr_search(&a, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "SortedArray,Search,%d,%.2f\n", n, now_ns(t0,t1,SEARCH_REPS));

            t0 = clock();
            for (int r = 0; r < MM_REPS; r++) sarr_max(&a);
            t1 = clock();
            fprintf(fp, "SortedArray,Max,%d,%.2f\n", n, now_ns(t0,t1,MM_REPS));

            t0 = clock();
            for (int r = 0; r < MM_REPS; r++) sarr_min(&a);
            t1 = clock();
            fprintf(fp, "SortedArray,Min,%d,%.2f\n", n, now_ns(t0,t1,MM_REPS));

            t0 = clock();
            for (int r = 0; r < PS_REPS; r++) sarr_pred(&a, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "SortedArray,Predecessor,%d,%.2f\n", n, now_ns(t0,t1,PS_REPS));

            t0 = clock();
            for (int r = 0; r < PS_REPS; r++) sarr_succ(&a, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "SortedArray,Successor,%d,%.2f\n", n, now_ns(t0,t1,PS_REPS));

            t0 = clock();
            for (int r = 0; r < IO_REPS; r++) sarr_insert(&a, fresh[r]);
            t1 = clock();
            fprintf(fp, "SortedArray,Insert,%d,%.2f\n", n, now_ns(t0,t1,IO_REPS));

            t0 = clock();
            for (int r = 0; r < IO_REPS; r++){
                int idx = sarr_search(&a, initial[rand()%n]);
                if (idx >= 0) sarr_delete_idx(&a, idx);
            }
            t1 = clock();
            fprintf(fp, "SortedArray,Delete,%d,%.2f\n", n, now_ns(t0,t1,IO_REPS));
        }

        /* ---------- 3. SINGLY LINKED UNSORTED ---------- */
        {
            USList l; uslist_init(&l);
            for (int i = 0; i < n; i++) uslist_insert(&l, initial[i]);

            t0 = clock();
            for (int r = 0; r < SEARCH_REPS; r++) uslist_search(&l, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "SinglyLinkedUnsorted,Search,%d,%.2f\n", n, now_ns(t0,t1,SEARCH_REPS));

            t0 = clock();
            for (int r = 0; r < MM_REPS; r++) uslist_max(&l);
            t1 = clock();
            fprintf(fp, "SinglyLinkedUnsorted,Max,%d,%.2f\n", n, now_ns(t0,t1,MM_REPS));

            t0 = clock();
            for (int r = 0; r < MM_REPS; r++) uslist_min(&l);
            t1 = clock();
            fprintf(fp, "SinglyLinkedUnsorted,Min,%d,%.2f\n", n, now_ns(t0,t1,MM_REPS));

            t0 = clock();
            for (int r = 0; r < PS_REPS; r++) uslist_pred(&l, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "SinglyLinkedUnsorted,Predecessor,%d,%.2f\n", n, now_ns(t0,t1,PS_REPS));

            t0 = clock();
            for (int r = 0; r < PS_REPS; r++) uslist_succ(&l, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "SinglyLinkedUnsorted,Successor,%d,%.2f\n", n, now_ns(t0,t1,PS_REPS));

            t0 = clock();
            for (int r = 0; r < IO_REPS; r++) uslist_insert(&l, fresh[r]);
            t1 = clock();
            fprintf(fp, "SinglyLinkedUnsorted,Insert,%d,%.2f\n", n, now_ns(t0,t1,IO_REPS));

            t0 = clock();
            for (int r = 0; r < IO_REPS; r++){
                SNode *tgt = uslist_search(&l, initial[rand()%n]);
                if (tgt) uslist_delete(&l, tgt);
            }
            t1 = clock();
            fprintf(fp, "SinglyLinkedUnsorted,Delete,%d,%.2f\n", n, now_ns(t0,t1,IO_REPS));
        }

        /* ---------- 4. SINGLY LINKED SORTED ---------- */
        {
            SSList l; sslist_init(&l);
            for (int i = 0; i < n; i++) sslist_insert(&l, initial[i]);

            t0 = clock();
            for (int r = 0; r < SEARCH_REPS; r++) sslist_search(&l, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "SinglyLinkedSorted,Search,%d,%.2f\n", n, now_ns(t0,t1,SEARCH_REPS));

            t0 = clock();
            for (int r = 0; r < MM_REPS; r++) sslist_max(&l);
            t1 = clock();
            fprintf(fp, "SinglyLinkedSorted,Max,%d,%.2f\n", n, now_ns(t0,t1,MM_REPS));

            t0 = clock();
            for (int r = 0; r < MM_REPS; r++) sslist_min(&l);
            t1 = clock();
            fprintf(fp, "SinglyLinkedSorted,Min,%d,%.2f\n", n, now_ns(t0,t1,MM_REPS));

            t0 = clock();
            for (int r = 0; r < PS_REPS; r++) sslist_pred(&l, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "SinglyLinkedSorted,Predecessor,%d,%.2f\n", n, now_ns(t0,t1,PS_REPS));

            t0 = clock();
            for (int r = 0; r < PS_REPS; r++) sslist_succ(&l, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "SinglyLinkedSorted,Successor,%d,%.2f\n", n, now_ns(t0,t1,PS_REPS));

            t0 = clock();
            for (int r = 0; r < IO_REPS; r++) sslist_insert(&l, fresh[r]);
            t1 = clock();
            fprintf(fp, "SinglyLinkedSorted,Insert,%d,%.2f\n", n, now_ns(t0,t1,IO_REPS));

            t0 = clock();
            for (int r = 0; r < IO_REPS; r++){
                SNode *tgt = sslist_search(&l, initial[rand()%n]);
                if (tgt) sslist_delete(&l, tgt);
            }
            t1 = clock();
            fprintf(fp, "SinglyLinkedSorted,Delete,%d,%.2f\n", n, now_ns(t0,t1,IO_REPS));
        }

        /* ---------- 5. DOUBLY LINKED UNSORTED ---------- */
        {
            UDList l; udlist_init(&l);
            for (int i = 0; i < n; i++) udlist_insert(&l, initial[i]);

            t0 = clock();
            for (int r = 0; r < SEARCH_REPS; r++) udlist_search(&l, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "DoublyLinkedUnsorted,Search,%d,%.2f\n", n, now_ns(t0,t1,SEARCH_REPS));

            t0 = clock();
            for (int r = 0; r < MM_REPS; r++) udlist_max(&l);
            t1 = clock();
            fprintf(fp, "DoublyLinkedUnsorted,Max,%d,%.2f\n", n, now_ns(t0,t1,MM_REPS));

            t0 = clock();
            for (int r = 0; r < MM_REPS; r++) udlist_min(&l);
            t1 = clock();
            fprintf(fp, "DoublyLinkedUnsorted,Min,%d,%.2f\n", n, now_ns(t0,t1,MM_REPS));

            t0 = clock();
            for (int r = 0; r < PS_REPS; r++) udlist_pred(&l, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "DoublyLinkedUnsorted,Predecessor,%d,%.2f\n", n, now_ns(t0,t1,PS_REPS));

            t0 = clock();
            for (int r = 0; r < PS_REPS; r++) udlist_succ(&l, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "DoublyLinkedUnsorted,Successor,%d,%.2f\n", n, now_ns(t0,t1,PS_REPS));

            t0 = clock();
            for (int r = 0; r < IO_REPS; r++) udlist_insert(&l, fresh[r]);
            t1 = clock();
            fprintf(fp, "DoublyLinkedUnsorted,Insert,%d,%.2f\n", n, now_ns(t0,t1,IO_REPS));

            t0 = clock();
            for (int r = 0; r < IO_REPS; r++){
                DNode *tgt = udlist_search(&l, initial[rand()%n]);
                if (tgt) udlist_delete(&l, tgt);
            }
            t1 = clock();
            fprintf(fp, "DoublyLinkedUnsorted,Delete,%d,%.2f\n", n, now_ns(t0,t1,IO_REPS));
        }

        /* ---------- 6. DOUBLY LINKED SORTED ---------- */
        {
            SDList l; sdlist_init(&l);
            for (int i = 0; i < n; i++) sdlist_insert(&l, initial[i]);

            t0 = clock();
            for (int r = 0; r < SEARCH_REPS; r++) sdlist_search(&l, initial[rand()%n]);
            t1 = clock();
            fprintf(fp, "DoublyLinkedSorted,Search,%d,%.2f\n", n, now_ns(t0,t1,SEARCH_REPS));

            t0 = clock();
            for (int r = 0; r < MM_REPS; r++) sdlist_max(&l);
            t1 = clock();
            fprintf(fp, "DoublyLinkedSorted,Max,%d,%.2f\n", n, now_ns(t0,t1,MM_REPS));

            t0 = clock();
            for (int r = 0; r < MM_REPS; r++) sdlist_min(&l);
            t1 = clock();
            fprintf(fp, "DoublyLinkedSorted,Min,%d,%.2f\n", n, now_ns(t0,t1,MM_REPS));

            t0 = clock();
            for (int r = 0; r < PS_REPS; r++){
                DNode *node = sdlist_search(&l, initial[rand()%n]);
                if (node) sdlist_pred_node(node);
            }
            t1 = clock();
            fprintf(fp, "DoublyLinkedSorted,Predecessor,%d,%.2f\n", n, now_ns(t0,t1,PS_REPS));

            t0 = clock();
            for (int r = 0; r < PS_REPS; r++){
                DNode *node = sdlist_search(&l, initial[rand()%n]);
                if (node) sdlist_succ_node(node);
            }
            t1 = clock();
            fprintf(fp, "DoublyLinkedSorted,Successor,%d,%.2f\n", n, now_ns(t0,t1,PS_REPS));

            t0 = clock();
            for (int r = 0; r < IO_REPS; r++) sdlist_insert(&l, fresh[r]);
            t1 = clock();
            fprintf(fp, "DoublyLinkedSorted,Insert,%d,%.2f\n", n, now_ns(t0,t1,IO_REPS));

            t0 = clock();
            for (int r = 0; r < IO_REPS; r++){
                DNode *tgt = sdlist_search(&l, initial[rand()%n]);
                if (tgt) sdlist_delete(&l, tgt);
            }
            t1 = clock();
            fprintf(fp, "DoublyLinkedSorted,Delete,%d,%.2f\n", n, now_ns(t0,t1,IO_REPS));
        }

        free(pool);
        printf("Completed n = %d\n", n);
    }

    fclose(fp);
    printf("Done. Wrote q1_dictionary_benchmark.csv\n");
    return 0;
}
