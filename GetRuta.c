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
char* CargarFichero(char*);
struct Celda** InicioMatriz(unsigned,unsigned);
void CompletarMatriz(char*,char*,struct Celda**);
void CalcularCasilla(unsigned, unsigned, bool, struct Celda**);  
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
void Mayus(char * temp) {
  char * name;
  name = strtok(temp,":");
  char *s = name;
  while (*s) {
    *s = toupper((unsigned char) *s);
    s++;
  }}

int main( int argc, char *argv[] ) 
{ 
    if(argc == 2)
    {
		time_t t1, t2, t3, t4;
		t1 = time(0);
		char* string1=CargarFichero(argv[1]);
		char* string2=CargarFichero(argv[2]);
		int row = strlen(string1);
		int col = strlen(string2);
		struct Celda **Matriz;
		Matriz=InicioMatriz(row - 1,col - 1);
		t2 = time(0);
		CompletarMatriz(string1,string2,Matriz);
		t3 = time(0);
		int resultado= GetRuta(Matriz, row, col);
		t4 = time(0);
		printf("Inicializado:       %d\n", t2 - t1);
		printf("Creacion de matriz: %d\n", t3 - t2);
		printf("Backtracking:       %d\n", t4 - t3);
		printf("Total:              %d\n", t4 - t1);
		printf("Coincidencia(porc): %d\n", 100*resultado/max(strlen(string1),strlen(string2)));
    }
    else
    {
        printf("Requiere 2 argumentos\n");
        exit(3);
    }
    return 0;
}

/**
 * CargarFichero funcion que guarda en un string el contenido de un fichero .fasta
 * @author Lidia y Nacho
 * @date 8/2/2018
 * @param NombreFichero nombre del fichero, incluida extension y ruta relativa
 * @out string con el contenido en mayusculas
 */
char* CargarFichero(char* NombreFichero)
{
    FILE *archivo;
 	
 	char caracteres[1000];
 	char *cadena=malloc(100000000);   //L�mite de 10 millones de caracteres
 	strcpy (cadena, ""); 
 	archivo = fopen(NombreFichero,"r");
 	unsigned cont=0;
 	
 	if (archivo == NULL)
 		exit(1);
 	else
	{
        fgets(caracteres,1000,archivo); //Primera linea
 	    while (feof(archivo) == 0 && strlen(cadena)<90000000) //Hasta fin de archivo o memoria
 	    {
			fgets(caracteres,1000,archivo);
			strcat(cadena, caracteres);
 	    }
	}
	Mayus(cadena);
	
	return cadena;
}

struct Celda** InicioMatriz(unsigned FillMe0,unsigned FillMe1)
{
    return  (struct Celda **)malloc(3*3 * sizeof(struct Celda *));
}


/**
 * CompletarMatriz funcion que calcula el algoritmo Needleman-Wunsch para una matriz
 * @author Nacho
 * @date 7/2/2018
 * @param string1 Cadena de texto 1
 * @param string2 Cadena de texto 2
 * @param matrix Matriz de Celdas, su tama�o debe ser el de las cadenas de texto +1
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
 * @param matrix Matriz de Celdas, su tama�o debe ser el de las cadenas de texto +1
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

