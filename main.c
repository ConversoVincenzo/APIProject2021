#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 1025

typedef struct Point{
    int key;
    int *Vector_Pointer;
    int d;
    char color;
    struct Point *father;
    struct Point *dx;
    struct Point *sx;
}node;

node *root;
node *nil;

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
void RB_Insert_FIXUP(node *z){
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
                    RB_Insert_FIXUP(x->father);
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
                    RB_Insert_FIXUP(x->father);
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
    RB_Insert_FIXUP(z);
}
void NodeCreate(int key){
    node *z;
    z=malloc(sizeof(node));
    z->key = key;
    z->Vector_Pointer=NULL;
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
node *Tree_Succ(node *x){
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
        y = Tree_Succ(z);
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
node  *Ricerca(int key){
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
void Insert_Node(int *vet,int ind1,int len){
    node *x;
    x=Ricerca(ind1);
}

int main() {
    nil = malloc(sizeof(node));
    nil->color = 'B';
    nil->father = nil;
    nil->dx = nil;
    nil->sx = nil;
    root = nil;
    char vet[N];
    char *Pointer;
    while (1) {
        fgets(vet, N, stdin);
    }
}