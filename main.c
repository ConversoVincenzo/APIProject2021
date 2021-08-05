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
heap_node *reset;
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
    minHeap = malloc(sizeof (heap*));
    minHeap->nodeList = malloc(nodeListLength * sizeof(heap_node*));
    minHeap->size = 0;
    minHeap->position = malloc(nodeListLength * sizeof(int));
}

void Insert(int index,int i,int distance){
    if(index==0){
        minHeap->nodeList[i] = malloc(sizeof (heap_node));
        if(i==0){
            reset = minHeap->nodeList[i];
        }
    }
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
    if (minHeap->size<1)
        return NULL;
    heap_node *heapRoot = minHeap->nodeList[0];
    heap_node *heapLeaf = minHeap->nodeList[minHeap->size - 1];
    minHeap->nodeList[0] = heapLeaf;
    minHeap->position[heapLeaf->vertex] = 0;
    minHeap->position[heapRoot->vertex] = minHeap->size - 1;
    minHeap->size = minHeap->size - 1;
    minHeapify(0);
    return heapRoot;
}

int DijkstraQueue(int index){
    int distance[ArrayBuffer->size];
    int temp;
    int vertexSrc = 0;
    heap_node *heapNode;
    minHeap->size = 0;
    heap_node *flag = reset;
    for(int i = 0; i < ArrayBuffer->size; i++){
        distance[i] = INT_MAX;
        if(index != 0){
            minHeap->nodeList[i] = flag;
            flag = flag + 4;
        }
        Insert(index,i,distance[i]);
        minHeap->position[i] = i;
    }
    distance[vertexSrc] = 0;
    minHeap->size = ArrayBuffer->size;
    while(minHeap->size != 0){
        heapNode = extractMin();
        temp = heapNode->vertex;
        if(temp == 0){
            reset = heapNode;
        }
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

void Insert_Node(proximity_list *num,int key){
    proximity_list *prev,*next;
    prev = ArrayBuffer->list[key];
    while(prev!=NULL){
        next = prev->next;
        prev->next = NULL;
        free(prev);
        prev = next;
    }
    ArrayBuffer->list[key] = num;
}
void add_score(int score,int ID,int k){
    scoreRanking *s,*r;
    if(firstPosition == NULL){
        firstPosition = malloc(sizeof (scoreRanking*));
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

void Insert_List(char *Pointer, int vertex) {
    proximity_list *head = ArrayBuffer->list[vertex];
    proximity_list *prev, *next, *temp;
    prev = NULL;
    next = NULL;
    bool flag = 0;
    int length;
    int number = 0;
    if (head == NULL) {
        while(strlen(Pointer) != 0){
            length = StringToNumber(&Pointer);
            if (length != 0 && vertex != number && number != 0) {
                flag = 1;
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
        if(!flag){
            head = NULL;
        }
        ArrayBuffer->list[vertex] = head;
    } else {
        while (head!=NULL){
            length = StringToNumber(&Pointer);
            if (length != 0 && vertex != number && number != 0) {
                flag = 1;
            }
            number++;
            if(strlen(Pointer)!=0)
                return;
        }
        if(!flag){
            head = NULL;
        }
    }
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
    ArrayBuffer = malloc(sizeof(node*));
    firstPosition = NULL;
    lastPosition = firstPosition;
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
            d = StringToNumber(&SecondPointer);
            k = StringToNumber(&SecondPointer);
            if (d == 0){
                return 0;
            } else {
                sizeLine= (d*10);
                ArrayBuffer->size = d;
                ArrayBuffer->list = malloc(d*sizeof(proximity_list*));
                Create_Min_Heap(ArrayBuffer->size);
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
            Insert_List(SecondPointer, key);
            rowLength--;
            if (rowLength == 0){
                Add_Graph(index,k);
                numOfMatrixProc++;
                isAFunctionActive = false;
            }
        } else if (Pointer[*i] == 84){
            TopK(k,numOfMatrixProc);
            printf("\n");
        }
    }
}