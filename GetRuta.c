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
};

unsigned GetRuta(struct Celda**, unsigned, unsigned);
void AuxGetRuta(struct Celda**, unsigned, unsigned, unsigned, unsigned *);

int maxU(int arg1, int arg2)
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
        arr[0][i].arriba=0;
        arr[0][i].lateral=0;
        arr[0][i].diag=0;
    }
    for (i = 1; i < r; ++i)
        for (j = 1; j < c; ++j)
        {
        arr[i][j].arriba=1;
        arr[i][j].lateral=1;
        arr[i][j].diag=1;
        }
    int result=GetRuta(arr,5,5);
    printf("El resultado deberia ser 5\n");
    printf("El resultado es %d \n",result);
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
unsigned GetRuta(struct Celda** matrix, unsigned i, unsigned j)
{
	unsigned maximo = 0;
	
    unsigned x,y;
	for(x=0;x<=i;x++)
	   for(y=0;y<=i;y++)
	       matrix[x][y].score=0;
	
	AuxGetRuta(matrix, i, j, 0, &maximo);
	
	return maximo;
}

void AuxGetRuta(struct Celda** matrix, unsigned i, unsigned j, unsigned cont, unsigned *maximo)
{
    //printf("Valores actuales: %d %d val %d cont %d   max %d\n", i,j,matrix[i][j].score,cont, *maximo);
    //system("PAUSE");
    
    if(cont<matrix[i][j].score || ((cont+i<maximo || cont+j<maximo)&&(*maximo>0)))
    {
        printf("Valores podados: %d %d val %d cont %d   max %d\n", i,j,matrix[i][j].score,cont, *maximo);
        return;
    }
    
    matrix[i][j].score=cont;
    printf("Valores actuales: %d %d val %d cont %d   max %d\n", i,j,matrix[i][j].score,cont, *maximo);
	if(i == 0 || j == 0)
	{
		*maximo = maxU(cont, *maximo);
	}
	else 
	{
        
	    if(matrix[i][j].diag)
		{
			AuxGetRuta(matrix, i - 1, j - 1, cont + 1, maximo);
		}
		if(matrix[i][j].lateral)
        {
			AuxGetRuta(matrix, i - 1, j, cont, maximo);
        }
		if(matrix[i][j].arriba)
        {
			AuxGetRuta(matrix, i, j - 1, cont, maximo);
        }
	}
	
    return;
}
