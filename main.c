#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N (sizeof(char )*23)

typedef struct Point{
    int key;
    int *num;
    char color;
    struct Point *father;
    struct Point *dx;
    struct Point *sx;
}node;

typedef struct temp{
    int pathLength;
    int ID;
    struct temp* next;
    struct temp* prev;
}scoreRanking;

node *root;
node *nil;
scoreRanking *firstPosition;
scoreRanking *lastPosition;
int rankLength = 0;

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
void NodeCreate(int key, int *num){
    node *z;
    z=malloc(sizeof(node));
    z->key = key;
    z->num = num;
    RB_Insert(z);
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
void Insert_Node(int *num,int ind1){
    node *x;
    x = Searching(ind1);
    x->num = num;
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

int StringToNumberConverter(int *i,const char* vet){
    int temp = 0;
    if(!(vet[*i]<=57 && vet[*i]>= 48)){
        (*i)++;
    }
    while (vet[*i]<=57 && vet[*i]>= 48){
        if(temp == 0){
            temp = vet[*i] - 48;
        } else{
            temp = temp*10 + (vet[*i] - 48);
        }
        (*i)++;
    }
    return temp;
}

int *RowAssembler(char* Pointer, int d){
    int *temp = malloc(sizeof(int) *d);
    for(int number = 0; number<d; number++){
        temp[number] = strtoul(Pointer,&Pointer,10);
    }
    return temp;
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
    unsigned long int size;
    int d,k,rowLength;
    int index;
    int *i;
    int isAFunctionActive = 0;
    size = N;
    i = malloc(sizeof(int));
    *i = 0;
    d = 0;
    k = 0;
    index = -1;
    char *Pointer = malloc(N);
    while (1) {
        fgets(Pointer, size, stdin);
        if(d == 0 || k==0){
            d = strtoul(Pointer,&Pointer,10);
            k = strtoul(Pointer,&Pointer,10);
            if (d == 0){
                return 0;
            } else {
                size = (11*(d*sizeof(int)));
                Pointer = realloc(Pointer, size);
            }
        }
        if (!isAFunctionActive && Pointer[*i]==65){
            isAFunctionActive = 1;
            rowLength = d;
            index++;
        } else if(isAFunctionActive && rowLength>0){
            if (index == 0) {
                NodeCreate(-1 * (rowLength - d), RowAssembler(Pointer, d));
            } else
                Insert_Node(RowAssembler(Pointer, d),-1 * (rowLength - d));
            rowLength--;
            if (rowLength == 0){
                Add_Graph(index,k);
                isAFunctionActive = 0;
            }
        } else if (Pointer[*i] == 84){
            TopK(k);
            return 0;
        }
    }
}