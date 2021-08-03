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
    heap_node **nodeList;
}heap;

typedef struct Point{
    int size;
    proximity_list **list;
}node;

node *ArrayBuffer;
scoreRanking *firstPosition;
scoreRanking *lastPosition;
heap *minHeap;
int rankLength = 0;
int StringToNumber(char** Pointer);



int Parent(int i){
    return (i - 1) / 2;
};

int Left(int i){
    return i*2 +1;
}

int Right(int i){
    return i*2 +2;
}
void swap(int i, int max){
    heap_node **list = minHeap->nodeList;
    minHeap->position[list[i]->vertex] = max;
    minHeap->position[list[max]->vertex] = i;
    int vert= list[max]->vertex;
    int dist= list[max]->distance;
    list[max]->vertex = list[i]->vertex;
    list[max]->distance = list[i]->distance;
    list[i]->distance = dist;
    list[i]->vertex = vert;
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
        swap(i,posMax);
        minHeapify(posMax);
    }
}
void resetHeap(){
    minHeap->size = 0;
}

void Create_Min_Heap(int nodeListLength){
    minHeap = malloc(sizeof (heap));
    minHeap->nodeList = malloc(nodeListLength* sizeof(heap_node*));
    minHeap->size = 0;
    minHeap->position = malloc(nodeListLength *sizeof(int));
}

void decreaseKey(int vert, int dist) {
    int i = minHeap->position[vert];
    minHeap->nodeList[i]->distance = dist;
    while (i!=0 && minHeap->nodeList[i]->distance < minHeap->nodeList[Parent(i)]->distance) {
        swap(i, Parent(i));
        i = Parent(i);
    }
}

heap_node *extractMin(){
    if (minHeap->size<1)
        return NULL;
    int vertRoot = minHeap->nodeList[0]->vertex;
    int distRoot = minHeap->nodeList[0]->distance;
    heap_node *heapLeaf = minHeap->nodeList[minHeap->size - 1];
    minHeap->nodeList[0]->vertex = heapLeaf->vertex;
    minHeap->nodeList[0]->distance = heapLeaf->distance;
    minHeap->position[heapLeaf->vertex] = 0;
    heapLeaf->distance = distRoot;
    heapLeaf->vertex = vertRoot;
    minHeap->position[vertRoot] = minHeap->size - 1;
    heap_node *heapNode = minHeap->nodeList[minHeap->size - 1];
    for(int i = 0; i<minHeap->size; i++){
        if (distRoot > minHeap->nodeList[i]->distance){
            printf("hai cannato");
        }
    }
    minHeap->size = minHeap->size - 1;
    minHeapify(0);
    return heapNode;
}

int DijkstraQueue(int index){
    int distance[ArrayBuffer->size];
    int temp;
    heap_node *heapNode;
    resetHeap();
    for(int i = 0; i < ArrayBuffer->size; i++){
        distance[i] = INT_MAX;
        if(index == 0){
            minHeap->nodeList[i] = malloc(sizeof (heap_node));
        }
        minHeap->nodeList[i]->vertex = i;
        minHeap->nodeList[i]->distance = distance[i];
        minHeap->position[i] = i;
    }
    distance[0] = 0;
    minHeap->size = ArrayBuffer->size;
    while(minHeap->size != 0){
        heapNode = extractMin();
        temp = heapNode->vertex;
        proximity_list *list = ArrayBuffer->list[temp];
        int vTemp;
        while(list != NULL){
            vTemp = list->destinationVertex;
            if (minHeap->position[vTemp] < minHeap->size && distance[temp] < INT_MAX && list->length + distance[temp] < distance[vTemp]){
                distance[vTemp] = distance[temp] + list->length;
                decreaseKey(vTemp,distance[vTemp]);
            }
            list = list->next;
        }
    }
    int score = 0;
    for (int i = 0; i < ArrayBuffer->size; i++) {
        if (distance[i] != INT_MAX) {
            score = score + distance[i];
        }
    }
    return score;
};

void Insert_NodeV1(proximity_list *num, int key) {
    proximity_list *x;
    x = ArrayBuffer->list[key];
    proximity_list *s, *r;
    s = x;
    while (s != NULL && num->destinationVertex > s->destinationVertex) {
        r = s;
        s = s->next;
        s->prev = NULL;
        free(r);
    }
    if (s == NULL) {
        x = num;
        return;
    }
    x = num;
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
        num = num->next;
    }
    while (s != NULL) {
        r = s;
        s = s->next;
        free(r);
    }
}

void Insert_Node(proximity_list *num,int ind1){
    proximity_list *prev,*next;
    prev = ArrayBuffer->list[ind1];
    while(prev!=NULL){
        next = prev->next;
        prev->next = NULL;
        free(prev);
        prev = next;
    }
    ArrayBuffer->list[ind1] = num;
    ArrayBuffer->size++;
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
        return;
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
    int score = DijkstraQueue(index);
    add_score(score,index,k);
};

/**
 * read all the graphs from the one with ID 0 and
 * @param length
 */
void TopK(int k,int length){
    scoreRanking *head_temp = firstPosition;
    if(length>k){
        length = k;
    }
    int temp = length - 1;
    for (int i = 0; i < length; i++) {
        if (i == temp){
            printf("%d", head_temp->ID);
            return;
        } else {
            printf("%d ", head_temp->ID);
        }
        head_temp = head_temp->next;
    }
};

void CreateNode(int size){
    ArrayBuffer = malloc(sizeof(node));
    ArrayBuffer->size = 0;
    ArrayBuffer->list = malloc(size*sizeof(proximity_list));
}

proximity_list *RowAssembler(char* Pointer,int vertex){
    proximity_list *temp,*prev;
    proximity_list *head = NULL;
    prev = NULL;
    int length;
    int number = 0;
    while(strlen(Pointer) != 0){
        length = StringToNumber(&Pointer);
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

int StringToNumber(char **Pointer){
    int temp = 0;
    while (*Pointer[0]<=57 && *Pointer[0]>= 48){
        if(temp == 0){
            temp = *Pointer[0] - 48;
        } else{
            temp = temp*10 + (*Pointer[0] - 48);
        }
        *Pointer = *Pointer + sizeof (char );
    }
    *Pointer = *Pointer + sizeof (char );
    return temp;
}

int main() {
    firstPosition = NULL;
    lastPosition = firstPosition;
    int sizeLine;
    int d,k,rowLength,numOfMatrixProc;
    int index;
    bool isAFunctionActive = false;
    sizeLine = N;
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
            d = StringToNumber(&SecondPointer);
            k = StringToNumber(&SecondPointer);
            if (d == 0){
                return 0;
            } else {
                sizeLine= (d*10);
                CreateNode(d);
                Create_Min_Heap(d);
                Pointer = realloc(Pointer, sizeLine);
            }
        }
        if (!isAFunctionActive && Pointer[0]==65){
            isAFunctionActive = true;
            rowLength = d;
            index++;
        } else if(isAFunctionActive && rowLength>0){
            int key = -1 * (rowLength - d);
            Insert_Node(RowAssembler(SecondPointer, key),key);
            rowLength--;
            if (rowLength == 0){
                Add_Graph(index,k);
                numOfMatrixProc++;
                ArrayBuffer->size = 0;
                isAFunctionActive = false;
            }
        } else if (Pointer[0] == 84){
            TopK(k,numOfMatrixProc);
            printf("\n");
        }
    }
}