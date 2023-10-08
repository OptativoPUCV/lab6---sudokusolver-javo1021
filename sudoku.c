#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){

  int filaAux[9][10] = {0}, columnaAux[9][10] = {0}, subMatrizAux[9][10] = {0};

  for(int fila = 0; fila < 9 ; fila++){
    for(int columna = 0; columna < 9 ; columna ++){
      int num = n->sudo[fila][columna];
      if(num != 0){
        if(filaAux[fila][num] == 1){
          return 0;
        } else{
          filaAux[fila][num] = 1;
        }

        if(columnaAux[columna][num] == 1){
          return 0;
        } else{
          columnaAux[columna][num] = 1;
        }
        int subMatriz_i = 3 * (fila/3) + (columna / 3);
        if(subMatrizAux[subMatriz_i][num] == 1){
          return 0;
        } else{
          subMatrizAux[fila][num] = 1;
        }
        
      }
    }
  }
  return 1;
}


List* get_adj_nodes(Node* n){
  List* list=createList();
  int filaVacia = -1;
  int columnaVacia = -1;

  for(int fila = 0; fila < 9; fila++){
    for(int columna = 0; columna < 9; columna++){
      if(n->sudo[fila][columna] == 0){
        filaVacia = fila;
        columnaVacia = columna;
        break;
      }
    }
    if(filaVacia != -1){
      break;
    }
  }
    if(filaVacia != -1 && columnaVacia != -1){
      for(int num = 1 ; num <= 9; num++){
        Node* new_node = copy(n);
        new_node->sudo[filaVacia][columnaVacia] = num;
        
        if(is_valid(new_node)){
          pushBack(list,new_node);
        }else{
          free(new_node);
        }
      }
    }
  
  return list;
}


int is_final(Node* n){
  for(int fila = 0; fila < 9 ; fila++){
    for(int columna = 0; columna < 9 ; columna++){
      if(n->sudo[fila][columna] == 0) return 0;
    }
  }
  return 1;
}

Node* DFS(Node* initial, int* cont){
  
  Stack* S = createStack();
  push(S, initial);
  *cont = 0;
  
  while(!is_empty(S)){
    (*cont)++;

    Node* NodoTop = top(S);
    pop(S);

    int finall = 1;
    for(int fila = 0; fila < 9 && finall; fila++){
      for(int columna = 0 ; columna < 9 && finall; columna++){
        if(NodoTop->sudo[fila][columna] == 0){
          finall = 0;
        }
      }
    }
    if(finall){
      return NodoTop;
    }
    //Si no es final hay que buscar adyacentes
    List* nodosAD = get_adj_nodes(NodoTop);
    Node* NodoAd = first(nodosAD);

    while(NodoAd){
      push(S, NodoAd);
      NodoAd = next(nodosAD);
    }
    free(NodoTop);
  }
  return NULL;
}




int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}