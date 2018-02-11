#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

struct Celda
{
    int score;
    bool diag;
    bool arriba;
    bool lateral;
    bool contado;
};

int GetRuta(struct Celda**, int, int);
unsigned max(unsigned arg1, unsigned arg2)
{
    if(arg1>arg2){
        if(arg1==0)
        return 1;
        return arg1;
    }
    else
        if(arg2==0)
        return 1;
        return arg2;
}

//TEST DE GET RUTA
int main( ) 
{ 
    unsigned r=6,c=6;
    struct Celda **arr = (struct Celda **)malloc(r*c * sizeof(struct Celda));
    unsigned i,j;
    
    for (i = 0; i < r; ++i)
        for (j = 0; i < c; ++i)
        arr[i] = (struct Celda *)malloc(c * sizeof(struct Celda));
    
    for (i = 0; i < r; ++i)
    {
        arr[i][0].arriba=0;
        arr[i][0].lateral=0;
        arr[i][0].diag=0;
        arr[0][j].arriba=0;
        arr[0][j].lateral=0;
        arr[0][j].diag=0;
    }
    for (i = 1; i < r; ++i)
        for (j = 1; i < c; ++i)
        {
        arr[i][j].arriba=1;
        arr[i][j].lateral=1;
        arr[i][j].diag=1;
        }
    int result=0GetRuta(arr,5,5);
    printf("El resultado deberia ser 5\n");
    printf("El resultado es %d",result);
    return 0;
}


/**
 * GetRuta funcion que cuenta cuantas veces los alineamiento pasa por una diagonal distinta
 * @author Paul
 * @date 8/2/2018
 * @param matrix Matriz de structs sobre la que se opera. In/Out
 * @param row Posicion en la fila de la celda que comprobaremos
 * @param col Posicion en la columna de la celda que comprobaremos
 */
int GetRuta(struct Celda** matrix, int row, int col)
{	
	int diag = 0;
	int i = row;
	int j = col;
	
	if(matrix[i][j].lateral)
		diag += GetRuta(matrix, i - 1, j);
	if(matrix[i][j].arriba)
		diag += GetRuta(matrix, i, j - 1);
	if(matrix[i][j].diag)
	{
		if(!matrix[i][j].contado)
			diag += 1;
		diag += GetRuta(matrix, i - 1, j - 1);
		
	}
		
    return diag;
}

