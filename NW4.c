#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int maxI(int arg1, int arg2)
{
    if(arg1>arg2){
        return arg1;
    }
    else
        return arg2;
}

struct Celda
{
    int score;
    short dir;
};
void CalcularCasilla(unsigned, unsigned, bool, struct Celda**);    
/*
//Copia seguridad C++
void CalcularCasilla(unsigned i, unsigned j, bool igual, Matriz& matrix)
{
    // Constantes Match 2, -2 dismatch, -1 Hueco
    int A = matrix[i-1][j].score - 1;
    int B = matrix[i][j-1].score - 1;
    int C = matrix[i-1][j-1].score+ (igual*4)-2; //C= arg + (argB*(Match-Fallo))+Fallo
    
    matrix[i][j].lateral = (A>=B && A>=C);
    matrix[i][j].arriba  = (B>=A && B>=C);
    matrix[i][j].diag    = (C>=B && C>=A);
    
    matrix[i][j].score=((A*matrix[i][j].lateral)+(B*matrix[i][j].arriba)+(C*matrix[i][j].diag)) / (matrix[i][j].arriba+matrix[i][j].lateral+matrix[i][j].diag);
}
*/

/**
 * CalcularCasilla funcion que calcula el contenido de una casillo de la matriz para el algoritmo Needleman-Wunsch
 * @author Nacho
 * @date 6/2/2018
 * @param i Indice de fila (No puede ser 0)
 * @param j Indice de columna (No puede ser 0)
 * @param igual Comparativa (Char==Char)
 * @param matrix Matriz de structs sobre la que se opera. In/Out
 */
void CalcularCasilla(unsigned i, unsigned j, bool igual, struct Celda **matrix)
{
    // Constantes Match 2, -2 dismatch, -1 Hueco
    int A = matrix[i-1][j].score - 1;
    int B = matrix[i][j-1].score - 1;
    int C = matrix[i-1][j-1].score+ (igual*4)-2; //C= arg + (argB*(Match-Fallo))+Fallo
    
    //Calculo de la dirección como un array de booleanos
    int D=0;
    D += (A>=B && A>=C)<<1;    //Vertical en 1a posicion
    D += (B>=A && B>=C); //Horizontal en 2a posicion
    D += (C>=B && C>=A)<<2; //Diagonal en 3a posicion
    matrix[i][j].dir = D;
    
    matrix[i][j].score=maxI(maxI(A,B),C);
    
}

//Prueba de funcion calcular casilla
int main()
{
    unsigned r=2,c=2;
    struct Celda **arr = (struct Celda **)malloc(r*c * sizeof(struct Celda));
    unsigned i;
    for (i = 0; i < r; ++i)
        arr[i] = (struct Celda *)malloc(c * sizeof(struct Celda));
    
    (arr[0][0]).score = 1;
    (arr[1][0]).score = 2;
    (arr[0][1]).score = 2;
    
    CalcularCasilla(1,1,true,arr);
    
    printf("%d ", arr[0][0].score);
    printf("%d ", arr[1][0].score);
    printf("%d ", arr[0][1].score);
    printf("%d ", arr[1][1].score);
    printf("%d ", arr[1][1].dir);
    if((arr[1][1].dir&2)==1)
        printf("--");
    if(arr[1][1].dir&1==1)
        printf("||");
    if(arr[1][1].dir>3)
        printf("//");
    return 0;
}
/*
int main()
{
    time_t t1, t2;
    unsigned r=2,c=2;
    struct Celda **arr = (struct Celda **)malloc(r*c * sizeof(struct Celda));
    unsigned i;
    for (i = 0; i < r; ++i)
        arr[i] = (struct Celda *)malloc(c * sizeof(struct Celda));
    srand(time(NULL));
    t1 = time(0);
    for(i=0;i<100000;i++){
    (arr[0][0]).score = rand()%20-10;
    (arr[1][0]).score = rand()%20-10;
    (arr[0][1]).score = rand()%20-10;
    
    CalcularCasilla(1,1,rand()%2,arr);
    }
    t2 = time(0);
    printf("tiempo:       %d\n", t2 - t1);
    return 0;
}
*/
