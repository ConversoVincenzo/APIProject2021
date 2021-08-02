#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define N (sizeof(char )*23)

typedef struct temp{
    int pathLength;
    int ID;
    struct temp* next;
    struct temp* prev;
}scoreRanking;

typedef struct temp1{
    int destinationVertex;
    unsigned long int length;
    struct temp1 *next;
}proximity_list;

typedef struct temp2{
    int vertex;
    int distance;
}heap_node;

typedef struct  temp3{
    int *position;
    int size;
    heap_node **nodeList;
}heap;

typedef struct Point{
    int key;
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
int size;

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
    size++;
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

int Parent(int i){
    return (i - 1) / 2;
};

int Left(int i){
    return i*2 +1;
}

int Right(int i){
    return i*2 +2;
}
void swap(heap_node **list,int i, int max){
    heap_node* temp = list[max];
    list[max] = list[i];
    list[i] = temp;
}
void minHeapify(int i){
    int left = Left(i);
    int right = Right(i);
    int posMax;
    if(left <= minHeap->size && minHeap->nodeList[left]->distance<minHeap->nodeList[i]->distance){
        posMax = left;
    } else
        posMax = i;
    if(right <= minHeap->size && minHeap->nodeList[right]->distance<minHeap->nodeList[posMax]->distance){
        posMax = right;
    }
    if(posMax!=i){
        minHeap->position[minHeap->nodeList[posMax]->vertex] = i;
        minHeap->position[minHeap->nodeList[i]->vertex] = posMax;
        swap(minHeap->nodeList,i,posMax);
        minHeapify(posMax);
    }
}
void Create_Min_Heap(int nodeListLength){
    minHeap = malloc(sizeof (heap));
    minHeap->nodeList = malloc(nodeListLength* sizeof(heap_node));
    minHeap->size = 0;
    minHeap->position = malloc(nodeListLength *sizeof(int));
}

void Insert(int i,int distance){
    minHeap->nodeList[i] = malloc(sizeof (heap_node));
    minHeap->nodeList[i]->vertex = i;
    minHeap->nodeList[i]->distance = distance;
}

void decreaseKey(int vert, int dist) {
    int i = minHeap->position[vert];
    minHeap->nodeList[i]->distance = dist;
    while (i!=0 && minHeap->nodeList[i]->distance < minHeap->nodeList[Parent(i)]->distance) {
        minHeap->position[minHeap->nodeList[i]->vertex] = Parent(i);
        minHeap->position[minHeap->nodeList[Parent(i)]->vertex] = i;
        swap(minHeap->nodeList,i, Parent(i));
        i = Parent(i);
    }
}

heap_node *extractMin(){
    int temp2,v2;
    temp2 = INT_MAX;
    if (minHeap->size<1)
        return NULL;
    heap_node *heapRoot = minHeap->nodeList[0];
    heap_node *heapLeaf = minHeap->nodeList[minHeap->size - 1];
    for (int i = 0; i < minHeap->size; i++) {
        if ( temp2 > minHeap->nodeList[i]->distance){
            temp2 = minHeap->nodeList[i]->distance;
            v2 = minHeap->nodeList[i]->vertex;
        }
    }
    minHeap->nodeList[0] = heapLeaf;
    if(heapRoot->distance!=temp2 && heapRoot->vertex!=v2){
        printf("hai cannato \n");
    }
    minHeap->position[heapLeaf->vertex] = 0;
    minHeap->position[heapRoot->vertex] = minHeap->size - 1;
    minHeap->size = minHeap->size - 1;
    minHeapify(0);
    return heapRoot;
}

int DijkstraQueue(){
    int distance[size];
    int temp;
    int vertexSrc = 0;
    heap_node *heapNode;
    Create_Min_Heap(size);
    for(int i = 0; i < size; i++){
        distance[i] = INT_MAX;
        Insert(i,distance[i]);
        minHeap->position[i] = i;
    }
    Insert(vertexSrc,distance[vertexSrc]);
    minHeap->position[vertexSrc] = vertexSrc;
    distance[vertexSrc] = 0;
    decreaseKey(vertexSrc,distance[vertexSrc]);
    minHeap->size = size;
    while(minHeap->size != 0){
        heapNode = extractMin();
        temp = heapNode->vertex;
        proximity_list *list = Searching(temp)->list;
        int vTemp;
        while(list != NULL){
            vTemp = list->destinationVertex;
            if (minHeap->position[vTemp] < minHeap->size && distance[temp] < INT_MAX && list->length + distance[temp] < distance[vTemp]){
                distance[vTemp] = distance[temp] + list->length;
                decreaseKey(vTemp,distance[vTemp]);
            }
            list = list->next;
        }
        free(heapNode);
    }
    int score = 0;
    for (int i = 0; i < size; i++) {
        if (distance[i] != INT_MAX) {
            score = score + distance[i];
        }
    }
    return score;
};

void Insert_Node(proximity_list *num,int ind1){
    node *x;
    x = Searching(ind1);
    proximity_list *prev,*next;
    prev = x->list;
    while(prev!=NULL){
        next = prev->next;
        prev->next = NULL;
        free(prev);
        prev = next;
    }
    x->list = num;
    size++;
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
    } else if (firstPosition->pathLength>score){
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
    } else if(lastPosition->pathLength <= score){
        if(k==rankLength){
            return;
        } else{
            s=lastPosition;
            s->next= malloc(sizeof(scoreRanking));
            s->next->ID = ID;
            s->next->pathLength = score;
            s->next->next = NULL;
            s->next->prev = lastPosition;
            lastPosition = s->next;
            rankLength++;
            return;
        }
    } else {
        s = firstPosition;
        while (s->next != NULL) {
            if (s->next->pathLength > score) {
                r = s->next;
                s->next = malloc(sizeof(scoreRanking));
                s->next->prev = r->prev;
                s->next->pathLength = score;
                s->next->ID = ID;
                s->next->next = r;
                r->prev = s->next;
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
    int score = DijkstraQueue();
    add_score(score,index,k);
};

/**
 * read all the graphs from the one with ID 0 and
 * @param length
 */
void TopK(int k,int length){
    scoreRanking *head_temp = firstPosition;
    scoreRanking *tail_temp = lastPosition;
    if(length>k){
        length = k;
    }
    int buffer[length];
    int temp = length;
    for (int i = 0; i < length/2; i++) {
        buffer[i] = head_temp->ID;
        temp--;
        buffer[temp] = tail_temp->ID;
        head_temp = head_temp->next;
        tail_temp = tail_temp->prev;
    }
    if(length%2 != 0){
        buffer[length/2] = tail_temp->ID;
    }
    temp = length - 1;
    for (int i = 0; i < length; i++) {
        if (i == temp){
            printf("%d", buffer[i]);
            return;
        } else {
            printf("%d ", buffer[i]);
        }
    }
};

proximity_list *RowAssembler(char* Pointer,int vertex){
    proximity_list *temp,*prev;
    proximity_list *head = NULL;
    prev = NULL;
    unsigned long int length;
    int number = 0;
    while(strlen(Pointer) != 0){
        length = strtoul(Pointer, &Pointer, 10);
        Pointer = Pointer + sizeof (char );
        if (length != 0 && vertex != number && number != 0) {
            temp = malloc(sizeof(proximity_list));
            if(prev == NULL){
                head = temp;
            } else
                prev->next = temp;
            temp->length = length;
            temp->destinationVertex = number;
            temp->next = NULL;
            prev = temp;
        }
        number++;
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
    size = 0;
    int sizeLine;
    int d,k,rowLength,numOfMatrixProc;
    int index;
    int *i;
    bool isAFunctionActive = false;
    sizeLine = N;
    i = malloc(sizeof(int));
    *i = 0;
    d = 0;
    k = 0;
    numOfMatrixProc = 0;
    index = -1;
    char *Pointer = malloc(N);
    char *SecondPointer = NULL;
    while (1) {
        if(fgets(Pointer, sizeLine, stdin) == NULL)
            return 0;
        SecondPointer = Pointer;
        if(d == 0 || k==0){
            d = strtoul(SecondPointer,&SecondPointer,10);
            k = strtoul(SecondPointer,&SecondPointer,10);
            if (d == 0){
                return 0;
            } else {
                sizeLine= (d*10*sizeof(char));
                Pointer = realloc(Pointer, sizeLine);
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
                NodeCreate(key, RowAssembler(SecondPointer, key));
            } else
                Insert_Node(RowAssembler(SecondPointer, key),key);
            rowLength--;
            if (rowLength == 0){
                Add_Graph(index,k);
                numOfMatrixProc++;
                size = 0;
                free(minHeap);
                minHeap = NULL;
                isAFunctionActive = false;
            }
        } else if (Pointer[*i] == 84){
            TopK(k,numOfMatrixProc);
            printf("\n");
        }
    }
}