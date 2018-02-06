#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Celda
{
    int score;
    bool diag;
    bool arriba;
    bool lateral;
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
    
    matrix[i][j].lateral = (A>=B && A>=C);
    matrix[i][j].arriba  = (B>=A && B>=C);
    matrix[i][j].diag    = (C>=B && C>=A);
    
    matrix[i][j].score=((A* matrix[i][j].lateral)+(B* matrix[i][j].arriba)+(C* matrix[i][j].diag)) / (matrix[i][j].arriba+matrix[i][j].lateral+matrix[i][j].diag);
}

//Prueba de funcion calcular casilla
int main()
{
    unsigned r=2,c=0;
    struct Celda **arr = (struct Celda **)malloc(20 * sizeof(struct Celda *));
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
    if(arr[1][1].lateral)
        printf("--");
    if(arr[1][1].arriba)
        printf("||");
    if(arr[1][1].diag)
        printf("//");
    return 0;
}
