#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define N (sizeof(char )*23)
typedef struct temp{
    int pathLength;
    int ID;
    struct temp* next;
    struct temp* prev;
}scoreRanking;

typedef struct temp1{
    int destinationVertex;
    int length;
    struct temp1 *next;
    struct temp1 *prev;
}proximity_list;

typedef struct temp2{
    int vertex;
    int distance;
}heap_node;

typedef struct  temp3{
    int *position;
    int size;
    int length;
    heap_node **nodeList;
}heap;

typedef struct Point{
    int key;
    int size;
    proximity_list *list;
    char color;
    struct Point *father;
    struct Point *dx;
    struct Point *sx;
}node;

node* root;
node *nil;
scoreRanking *firstPosition;
scoreRanking *lastPosition;
heap *minHeap;
int rankLength = 0;

typedef struct tree{
    int size;
    node* root;
}tree;

tree* RB_Tree;

void left_rotate(node *x){
    node *y;
    y=x->dx;
    x->dx = y->sx;
    if (y->sx!=nil){
        y->sx->father = x;
    }
    y->father = x->father;
    if( x->father == nil){
        root = y;
    }else if(x==x->father->sx){
        x->father->sx = y;
    } else{
        x->father->dx = y;
    }
    y->sx = x;
    x->father = y;
}
void right_rotate(node *x){
    node *y;
    y=x->sx;
    x->sx = y->dx;
    if (y->dx!=nil){
        y->dx->father = x;
    }
    y->father = x->father;
    if( x->father == nil){
        root = y;
    }else if(x==x->father->dx){
        x->father->dx = y;
    } else{
        x->father->sx = y;
    }
    y->dx = x;
    x->father = y;
}
void RB_Insert_Fixup(node *z){
    node  *x;
    node *y;
    if(z==root){
        root->color = 'B';
    } else{
        x = z->father;
        if(x->color == 'R'){
            if(x==x->father->sx){
                y = x->father->dx;
                if(y->color == 'R'){
                    x->color = 'B';
                    y->color = 'B';
                    x->father->color = 'R';
                    RB_Insert_Fixup(x->father);
                } else {
                    if(z == x->dx){
                        z = x;
                        left_rotate(z);
                        x = z->father;
                    }
                    x->color= 'B';
                    x->father->color = 'R';
                    right_rotate(x->father);}
            } else{
                y= x->father->sx;
                if (y->color == 'R'){
                    x->color = 'B';
                    y->color = 'B';
                    x->father->color = 'R';
                    RB_Insert_Fixup(x->father);
                }else {
                    if(z == x->sx){
                        z= x;
                        right_rotate(z);
                        x= z->father;
                    }
                    x->color = 'B';
                    x->father->color = 'R';
                    left_rotate(x->father);
                }
            }
        }
    }
}
void RB_Insert(node *z){
    node *x;
    node *y;
    x = root;
    y = nil;
    while (x!=nil){
        y = x;
        if (z->key < x->key){
            x = x->sx;
        } else{
            x= x->dx;
        }
        z->father = y;
    }
    if (y == nil){
        root = z;
        root->father = nil;
    }else if (z->key < y->key){
        y->sx = z;
    }else {
        y->dx = z;
    }
    z->sx = nil;
    z->dx = nil;
    z->color = 'R';
    RB_Insert_Fixup(z);
}
void NodeCreate(int key, proximity_list *num){
    node *z;
    z=malloc(sizeof(node));
    z->key = key;
    z->list = num;
    RB_Insert(z);
    RB_Tree->size++;
    RB_Tree->root = root;
}
void RB_Delete_Fixup(node *x){
    node *w;
    if (x->color == 'R' || x->father == nil){
        x->color = 'B';
    } else if (x == x->father->sx){
        w = x->father->dx;
        if (w->color == 'R'){
            w->color='B';
            x->father->color='R';
            left_rotate(x->father);
            w=x->father->dx;
        }
        if (w->sx->color == 'B' && w->dx->color == 'B'){
            w->color = 'R';
            RB_Delete_Fixup(x->father);
        } else{
            if (w->dx->color == 'B'){
                w->sx->color = 'B';
                w->color = 'R';
                right_rotate(w);
                w=x->father->dx;
            }
            w->color = x->father->color;
            x->father->color = 'B';
            w->dx->color = 'B';
            left_rotate(x->father);
        }
    } else{
        w = x->father->sx;
        if (w->color == 'R'){
            w->color='B';
            x->father->color='R';
            right_rotate(x->father);
            w=x->father->sx;
        }
        if (w->dx->color == 'B' && w->sx->color == 'B'){
            w->color = 'R';
            RB_Delete_Fixup(x->father);
        } else{
            if (w->sx->color == 'B'){
                w->dx->color = 'B';
                w->color = 'R';
                left_rotate(w);
                w=x->father->sx;
            }
            w->color = x->father->color;
            x->father->color = 'B';
            w->sx->color = 'B';
            right_rotate(x->father);
        }

    }
}
node *Tree_Min(node *x){
    while (x->sx!=nil){
        x = x->sx;
    }
    return x;
}
node *Tree_Next(node *x){
    node  *y;
    if (x->dx != nil){
        return Tree_Min(x->dx);
    }
    y = x->father;
    while (y!=nil && x == y->dx){
        x = y;
        y = y->father;
    }
    return y;
}
node *RB_Delete(node *z){
    node *x;
    node *y;
    if ( z->sx == nil || z->dx == nil){
        y = z;
    } else{
        y = Tree_Next(z);
    }
    if (y->sx!= nil){
        x = y->sx;
    } else{
        x= y->dx;
    }
    x->father = y->father ;
    if(y->father == nil){
        root = x;
    } else if( y == y->father->sx){
        y->father->sx = x;
    } else {
        y->father->dx = x;
    }
    if (y!=z){
        z->key = y->key;
    }
    if (y->color == 'B'){
        RB_Delete_Fixup(x);
    }
    return y;
}
node  *Searching(int key){
    node  *x = root;
    while (x != nil && key != x->key){
        if (x->key>key){
            x=x->sx;
        } else{
            x=x->dx;
        }
    }
    return x;

}

void swap(heap_node **list,int i, int max){
    heap_node* temp = list[max];
    list[max] = list[i];
    list[i] = temp;
}
void minHeapify(int i, heap *hp){
    int left = 2*i;
    int right = 2*i + 1;
    int posMax;
    if(left <= hp->size && hp->nodeList[left]->distance<hp->nodeList[i]->distance){
        posMax = left;
    } else
        posMax = i;
    if(right <= hp->size && hp->nodeList[right]->distance<hp->nodeList[posMax]->distance){
        posMax = right;
    }
    if(posMax!=i){
        swap(hp->nodeList,i,posMax);
        minHeapify(posMax,hp);
    }
}
void Create_Min_Heap(int nodeListLength){
    minHeap = malloc(sizeof (heap));
    minHeap->nodeList = malloc(nodeListLength* sizeof(heap_node));
    minHeap->length = nodeListLength;
    minHeap->size = 0;
}
heap_node *Min(){
    return minHeap->nodeList[0];
}
heap_node *Delete_Min() {
    if (minHeap->size < 1) {
        return NULL;
    }
    heap_node *min = minHeap->nodeList[0];
    minHeap->size = minHeap->size - 1;
    minHeap->nodeList[0] = minHeap->nodeList[minHeap->size];
    minHeapify(0,minHeap);
    return min;
}
int Parent(int i){
    return (i - 1) / 2;
};
void Insert(int key){
    int temp = minHeap->size + 1;
    minHeap->size = temp;
    int i = minHeap->size - 1;
    while (i>0 && minHeap->nodeList[Parent(i)] > minHeap->nodeList[i]) {
        swap(minHeap->nodeList, Parent(i), i);
        i = Parent(i);
    }
}

void decreaseKey(int vert, int dist) {
    int i = minHeap->position[vert];
    minHeap->nodeList[i]->distance = dist;
    while (i && minHeap->nodeList[i]->distance < minHeap->nodeList[Parent(i)]->distance) {
        minHeap->position[minHeap->nodeList[i]->vertex] = Parent(i);
        minHeap->position[minHeap->nodeList[Parent(i)]->vertex] = i;
        swap(minHeap->nodeList,i, Parent(i));
        i = Parent(i);
    }
}

heap_node *extractMin(){
    if (minHeap->size<1)
        return NULL;
    heap_node *heapRoot = minHeap->nodeList[0];
    heap_node *heapLeaf = minHeap->nodeList[minHeap->size - 1];
    minHeap->nodeList[0] = heapLeaf;
    minHeap->position[heapLeaf->vertex] = 0;
    minHeap->position[heapRoot->vertex] = minHeap->size-1;
    minHeap->size = minHeap->size - 1;
    minHeapify(0,minHeap);
    return heapRoot;
}

void DijkstraQueue(int vertexSrc){
    int distance[RB_Tree->size];
    Create_Min_Heap(RB_Tree->size);

};

void Insert_NodeV1(proximity_list *num, int key) {
    node *x;
    x = Searching(key);
    proximity_list *s, *r;
    s = x->list;
    while (s != NULL && num->destinationVertex > s->destinationVertex) {
        r = s;
        s = s->next;
        s->prev = NULL;
        free(r);
    }
    if (s == NULL) {
        x->list = num;
        return;
    }
    x->list = num;
    while (num != NULL && num->destinationVertex < s->destinationVertex) {
        r = num;
        num = num->next;
        r->next = s;
        if (s->prev == NULL) {
            s->prev = r;
            r->prev = NULL;
        } else {
            s->prev->next = r;
            r->prev = s->prev;
            s->prev = r;
        }
    }
    if (num == NULL) {
        while (s != NULL) {
            r = s;
            s = s->next;
            free(r);
        }
        return;
    }
    while (num != NULL && s->next != NULL) {
        if (num->destinationVertex > s->destinationVertex) {
            r = s;
            s = s->next;
            s->prev = r->prev;
            free(r);
        } else if (num->destinationVertex == s->destinationVertex) {
            s->length = num->length;
            r = num;
            num = num->next;
            s = s->next;
            free(r);
        } else {
            r = num;
            num = num->next;
            r->next = s;
            r->prev = s->prev;
            s->prev->next = r;
            s->prev = r;
        }
    }
    while (num != NULL) {
        s->next = num;
        num->prev = s;
    }
    while (s != NULL) {
        r = s;
        s = s->next;
        free(r);
    }
}
void Insert_Node(proximity_list *num,int ind1){
    node *x;
    x = Searching(ind1);
    proximity_list *prev,*next;
    prev = x->list;
    while(prev!=NULL){
        next = prev->next;
        prev->next = NULL;
        prev->prev = NULL;
        free(prev);
        prev = next;
    }
    x->list = num;
}
void add_score(int score,int ID,int k){
    scoreRanking *s,*r;
    if(firstPosition == NULL){
        firstPosition = malloc(sizeof (scoreRanking));
        firstPosition->ID = ID;
        firstPosition->pathLength = score;
        firstPosition->next = NULL;
        firstPosition->prev = NULL;
        lastPosition = firstPosition;
        rankLength++;
        return;
    } else if (firstPosition->pathLength<score){
        s = firstPosition;
        firstPosition = malloc(sizeof (scoreRanking));
        firstPosition->ID = ID;
        firstPosition->pathLength = score;
        firstPosition->next = s;
        firstPosition->prev = NULL;
        s->prev = firstPosition;
        if(rankLength == k){
            r = lastPosition->prev;
            r->next = NULL;
            free(lastPosition);
            lastPosition = r;
        } else
            rankLength++;
    } else if(lastPosition->pathLength >= score){
        if(k==rankLength){
            return;
        } else{
            s=lastPosition;
            s->next= malloc(sizeof(scoreRanking));
            s->next->ID = ID;
            s->next->pathLength = score;
            s->next->next = NULL;
            s->next->prev = lastPosition;
            lastPosition = s;
            rankLength++;
            return;
        }
    } else {
        s = firstPosition;
        while (s->next != NULL) {
            if (s->next->pathLength < score) {
                r = s->next;
                s->next = malloc(sizeof(scoreRanking));
                s->next->pathLength = score;
                s->next->ID = ID;
                s->next->next = r;
                r->prev = s->next->next;
                if(k==rankLength){
                    r = lastPosition->prev;
                    r->next = NULL;
                    free(lastPosition);
                    lastPosition = r;
                } else
                    rankLength++;
                return;
            }
            s = s->next;
        }
    }
}

void Add_Graph(int index,int k){
    int score = 0;
    add_score(score,index,k);
};

/**
 * read all the graphs from the one with ID 0 and
 * @param length
 */
void TopK(int length){
    printf("ciaone");
};

proximity_list *RowAssembler(char* Pointer, int d,int vertex){
    proximity_list *temp,*prev;
    proximity_list *head = NULL;
    prev = NULL;
    int length;
    for(int number = 0; number<d; number++){
        length = strtoul(Pointer, &Pointer, 10);
        if (length != 0 && vertex < number) {
            temp = malloc(sizeof(proximity_list));
            if(prev == NULL){
                head = temp;
            } else
                prev->next = temp;
            temp->length = length;
            temp->destinationVertex = number;
            temp->prev = prev;
            temp->next = NULL;
            prev = temp;
        }
    }
    return head;
}

int main() {
    nil = malloc(sizeof(node));
    nil->color = 'B';
    nil->father = nil;
    nil->dx = nil;
    nil->sx = nil;
    firstPosition = NULL;
    lastPosition = firstPosition;
    root = nil;
    RB_Tree->size = 0;
    RB_Tree->root = root;
    int size;
    int d,k,rowLength;
    int index;
    int *i;
    bool isAFunctionActive = false;
    size = N;
    i = malloc(sizeof(int));
    *i = 0;
    d = 0;
    k = 0;
    index = -1;
    char *Pointer = malloc(N);
    char *SecondPointer = NULL;
    while (1) {
        if(fgets(Pointer, size, stdin) == NULL)
            return 0;
        SecondPointer = Pointer;
        if(d == 0 || k==0){
            d = strtoul(SecondPointer,&SecondPointer,10);
            k = strtoul(SecondPointer,&SecondPointer,10);
            if (d == 0){
                return 0;
            } else {
                size = (d*sizeof(int));
                Pointer = realloc(Pointer, size);
            }
            (*i) = 0;
        }
        if (!isAFunctionActive && Pointer[*i]==65){
            isAFunctionActive = true;
            rowLength = d;
            index++;
        } else if(isAFunctionActive && rowLength>0){
            int key = -1 * (rowLength - d);
            if (index == 0) {
                NodeCreate(key, RowAssembler(SecondPointer, d, key));
            } else
                Insert_Node(RowAssembler(SecondPointer, d, key),key);
            rowLength--;
            if (rowLength == 0){
                Add_Graph(index,k);
                RB_Tree->size = 0;
                isAFunctionActive = false;
            }
        } else if (Pointer[*i] == 84){
            TopK(k);
        }
    }
}