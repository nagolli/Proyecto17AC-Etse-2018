#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

struct Celda
{
    int score;
    short dir;
};

unsigned GetRuta(struct Celda**, unsigned, unsigned);
int AuxGetRuta(struct Celda**, unsigned, unsigned, int, unsigned *);
int maxI(int arg1, int arg2)
{
    if(arg1>arg2){
        return arg1;
    }
    else
        return arg2;
}

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
        arr[i][0].dir=0;
        arr[0][i].dir=0;
    }
    for (i = 1; i < r; ++i)
        for (j = 1; j < c; ++j)
        {
        arr[i][j].dir=7;
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
	   for(y=0;y<=j;y++)
	       matrix[x][y].score=-1;
	
	AuxGetRuta(matrix, i, j, 0, &maximo);
	
	return maximo;
}

int AuxGetRuta(struct Celda** matrix, unsigned i, unsigned j, int cont, unsigned *maximo)
{    
    
    int A=-1,B=-1,C=-1;
    if(matrix[i][j].score>=0)
    {
        return matrix[i][j].score;
    }
    if((cont+i<*maximo || cont+j<*maximo)&&(*maximo>0))
    {
        matrix[i][j].score=0;
        return 0;
    }
	if(i == 0 || j == 0)
	{
		*maximo = maxI(cont, *maximo);
		matrix[i][j].score=0;;
		return 0;
	}
	else 
	{
	    if(matrix[i][j].dir>3)
		{
			A=AuxGetRuta(matrix, i - 1, j - 1, cont + 1, maximo)+1;
		}
		if(matrix[i][j].dir==2||matrix[i][j].dir==3||matrix[i][j].dir==6||matrix[i][j].dir==7)
        {
			B=AuxGetRuta(matrix, i - 1, j, cont, maximo);
        }
		if(matrix[i][j].dir==1||matrix[i][j].dir==3||matrix[i][j].dir==5||matrix[i][j].dir==7)
        {
			C=AuxGetRuta(matrix, i, j - 1, cont, maximo);
        }
	}
	
	matrix[i][j].score=maxI(maxI(A,B),C);
    return matrix[i][j].score;
}
