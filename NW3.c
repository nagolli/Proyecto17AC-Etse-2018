#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Celda
{
    int score;
    bool diag;
    bool arriba;
    bool lateral;
};

void CalcularCasilla(unsigned, unsigned, bool, struct Celda**);    
void CompletarMatriz(char*,char*,struct Celda**);
void CompletarFila(char*,char*,struct Celda**, unsigned);

/**
 * CompletarMatriz funcion que calcula el algoritmo Needleman-Wunsch para una matriz
 * @author Nacho
 * @date 7/2/2018
 * @param string1 Cadena de texto 1
 * @param string2 Cadena de texto 2
 * @param matrix Matriz de Celdas, su tamaño debe ser el de las cadenas de texto +1
 */
void CompletarMatriz(char* string1,char* string2,struct Celda** matrix)
{
    unsigned i;
    unsigned j;
    unsigned size1=strlen(string1);
    unsigned size2=strlen(string2);
    for(i=1;i<=size1;i++)
        for(j=1;j<=size2;j++)
            CalcularCasilla(i, j, (string1[i-1]==string2[j-1]||string1[i-1]=='N'||string2[j-1]=='N'), matrix);
    return;
}

/**
 * CompletarFila funcion que calcula el algoritmo Needleman-Wunsch para una fila, preparado para paralelizar una matriz, que en ese caso se pasa i=1
 * @author Nacho
 * @date 7/2/2018
 * @param string1 Cadena de texto 1
 * @param string2 Cadena de texto 2
 * @param matrix Matriz de Celdas, su tamaño debe ser el de las cadenas de texto +1
 * @param i indice de fila a rellenar
 */
void CompletarFila(char* string1,char* string2,struct Celda** matrix, unsigned i)//Para empezar de 0 dar i=1
{
    unsigned j;
    unsigned size1=strlen(string1);
    unsigned size2=strlen(string2);
        CalcularCasilla(i, 1, (string1[i-1]==string2[0]||string1[i-1]=='N'||string2[0]=='N'), matrix);
        if(i<=size1)
        {
            //Lanzar siguiente fila
        }
        for(j=2;j<=size2;j++)
            CalcularCasilla(i, j, (string1[i-1]==string2[j-1]||string1[i-1]=='N'||string2[j-1]=='N'), matrix);
    return;
}


//DE PRUEBA
void CalcularCasilla(unsigned i, unsigned j, bool s, struct Celda** arr)
{
    arr[0][0].score += 1;
}

int main()
{
    unsigned r=50,c=50;
    struct Celda **arr = (struct Celda **)malloc(r*c * sizeof(struct Celda));
    unsigned i;
    for (i = 0; i < r; ++i)
        arr[i] = (struct Celda *)malloc(c * sizeof(struct Celda));
    
    char* string1= "aaappp";
    char* string2= "xxxppp";
    arr[0][0].score = 0;
    CompletarMatriz(string1,string2,arr);
    
    return arr[0][0].score;
}
