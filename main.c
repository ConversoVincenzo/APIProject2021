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

node *listBuffer;
scoreRanking *firstPosition;
scoreRanking *lastPosition;
heap *minHeap;
int rankLength = 0;

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
    minHeap->position[i] = max;
    minHeap->position[max] = i;
    heap_node* temp = list[max];
    list[max] = list[i];
    list[i] = temp;
}
void minHeapify(int i, heap *hp){
    int left = Left(i);
    int right = Right(i);
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

int DijkstraQueue(){
    int distance[listBuffer->size];
    int temp;
    int vertexSrc = 0;
    heap_node *heapNode;
    Create_Min_Heap(listBuffer->size);
    for(int i = 0; i < listBuffer->size; i++){
        distance[i] = INT_MAX;
        Insert(i,distance[i]);
        minHeap->position[i] = i;
    }
    Insert(vertexSrc,distance[vertexSrc]);
    minHeap->position[vertexSrc] = vertexSrc;
    distance[vertexSrc] = 0;
    decreaseKey(vertexSrc,distance[vertexSrc]);
    minHeap->size = listBuffer->size;
    while(minHeap->size != 0){
        heapNode = extractMin();
        temp = heapNode->vertex;
        proximity_list *list = listBuffer->list[temp];
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
    for (int i = 0; i < listBuffer->size; i++) {
        if (distance[i] != INT_MAX) {
            score = score + distance[i];
        }
    }
    return score;
};


void Insert_Node(proximity_list *num,int ind1){
    proximity_list *prev,*next;
    prev = listBuffer->list[ind1];
    while(prev!=NULL){
        next = prev->next;
        prev->next = NULL;
        prev->prev = NULL;
        free(prev);
        prev = next;
    }
    listBuffer->list[ind1] = num;
    listBuffer->size++;
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
    int length;
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
            temp->prev = prev;
            temp->next = NULL;
            prev = temp;
        }
        number++;
    }
    return head;
}

int main() {
    firstPosition = NULL;
    lastPosition = firstPosition;
    listBuffer = malloc(sizeof (node));
    listBuffer->size = 0;
    int size;
    int d,k,rowLength,numOfMatrixProc;
    int index;
    int *i;
    bool isAFunctionActive = false;
    size = N;
    i = malloc(sizeof(int));
    *i = 0;
    d = 0;
    k = 0;
    numOfMatrixProc = 0;
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
            listBuffer->list = malloc(d*sizeof(proximity_list));
            if (d == 0){
                return 0;
            } else {
                size = (d*10*sizeof(char));
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
            Insert_Node(RowAssembler(SecondPointer, key),key);
            rowLength--;
            if (rowLength == 0){
                Add_Graph(index,k);
                numOfMatrixProc++;
                listBuffer->size = 0;
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