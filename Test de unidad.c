#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/*Struct con el valor de la celda y las direcciones
  Si las direcciones son verdadero, entonces ha heredado de esa direccion
*/
struct Celda
{
    int score;
    bool diag;
    bool arriba;
    bool lateral;
};

char* CargarFichero(char*,unsigned,unsigned);
struct Celda** inicializarMatriz(unsigned, unsigned);
void CompletarMatriz(char*,char*,struct Celda**);
void CalcularCasilla(unsigned, unsigned, bool, struct Celda**);  
int GetRuta(struct Celda**,unsigned,unsigned);
void AuxGetRuta(struct Celda**, unsigned, unsigned, unsigned, unsigned*);
/*Maximo entre dos valores*/
unsigned maxU(unsigned arg1, unsigned arg2)
{
    if(arg1>arg2){
        return arg1;
    }
    else
        return arg2;
}
int maxI(int arg1, int arg2)
{
    if(arg1>arg2){
        return arg1;
    }
    else
        return arg2;
}
/*Funcion para pasar a mayuscula una cadena. Obtenida de:
https://stackoverflow.com/questions/35181913/converting-char-to-uppercase-in-c
*/
void Mayus(char * temp) {
  char * name;
  name = strtok(temp,":");
  char *s = name;
  while (*s) {
    *s = toupper((unsigned char) *s);
    s++;
  }}

int main()
{ 
    char* string1=CargarFichero("UT1.fa",1000,0);
    char* string2=CargarFichero("UT2.fa",1000,0);
    struct Celda **Matriz;
    Matriz=inicializarMatriz(strlen(string1),strlen(string2));
    
    CompletarMatriz(string1,string2,Matriz);
    
    int resultado= GetRuta(Matriz,strlen(string1),strlen(string2));
    
    printf("Resultado, si es un 1 es un exito: %d", resultado == 5);
    
    return 0;
}

/**
 * CargarFichero funcion que guarda en un string el contenido de un fichero .fasta
 * @author Lidia y Nacho
 * @date 8/2/2018
 * @param NombreFichero nombre del fichero, incluida extension y ruta relativa
 * @out string con el contenido en mayusculas
 */
char* CargarFichero(char* NombreFichero,unsigned tamano,unsigned inicio)
{
    FILE *archivo;
 	unsigned i;
 	char caracteres[1000];
 	char *cadena=malloc(1000*tamano);   //Limite de 10 millones de caracteres
 	strcpy (cadena, ""); 
 	archivo = fopen(NombreFichero,"r");
 	
 	if (archivo == NULL)
 	{
 	    printf("%s no existe",NombreFichero);
 		exit(1);
 	}
 	else
        {
        fgets(caracteres,1000,archivo); //Primera linea
        for(i=0;i<inicio;i++)
            fgets(caracteres,1000,archivo);
        i=1000*tamano;
 	    while (feof(archivo) == 0 && strlen(cadena)<i) //Hasta fin de archivo o memoria
 	    {
 		fgets(caracteres,1000,archivo);
 		strcat(cadena, caracteres);
 	    }
        }
        Mayus(cadena);
	return cadena;
}


// inicializarMatriz funcion que crea la matriz de tamaño r c e inicializa la primera fila y columna con valores negativos descendentes.
// @author Sara
// @date 12/2/2018
// @param unsigned r rows
// @param unsigned c cols
// @return arr matriz con los valores negativos
struct Celda** inicializarMatriz(unsigned r, unsigned c)
{
    unsigned i;
    struct Celda **arr =(struct Celda **)malloc(r*c* sizeof(struct Celda));
    for (i = 0; i <= r; ++i)
        arr[i] = (struct Celda *)malloc(c * sizeof(struct Celda));
    
    //Casos base posicion:  r = 0, c = 0
    arr[0][0].score = 0;
    arr[0][0].lateral = 0;
    arr[0][0].arriba = 0;
    arr[0][0].diag = 0;
    
    for(i = 1 ; i<=r; i++)
    {
        arr[i][0].score = -i;
        arr[i][0].lateral = 0;
        arr[i][0].arriba = 0;
        arr[i][0].diag = 0;
    }
    
    for(i = 1 ; i<=c; i++)
    {
        arr[0][i].score = -i;
        arr[0][i].lateral = 0;
        arr[0][i].arriba = 0;
        arr[0][i].diag = 0;
    }
    
    return arr;
}


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
            //El argumento de calcular casilla es cierto si ambos strings coinciden o uno de ellos es N
            //Recordar que el tamano de la matriz es 1 mayor que los strings, y estos se alinean con el final.
            {
            CalcularCasilla(i, j, (string1[i-1]==string2[j-1]||string1[i-1]=='N'||string2[j-1]=='N'), matrix);
            }
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
    
    //Version sin IFs
    //Calculo de los 3 valores
    
    int A = matrix[i-1][j].score - 1;
    int B = matrix[i][j-1].score - 1;
    int C = matrix[i-1][j-1].score+ (igual*4)-2; //C= arg + (argB*(Match-Fallo))+Fallo
    
    
    
    //Busqueda de que caminos maximizan
    
    matrix[i][j].lateral = (A>=B && A>=C);
    matrix[i][j].arriba  = (B>=A && B>=C);
    matrix[i][j].diag    = (C>=B && C>=A);
    //Calculo del valor a partir de los valores y direcciones
    
    //Version sin IF
    //Sumatorio (valor x Direccion) / Suma de direcciones
    matrix[i][j].score=((A* matrix[i][j].lateral)+(B* matrix[i][j].arriba)+(C* matrix[i][j].diag)) / (matrix[i][j].arriba+matrix[i][j].lateral+matrix[i][j].diag);
    
    
    //Version con IF
    /*
    matrix[i][j].score=maxI(maxI(A,B),C);
    */
}

/**
 * GetRuta Funcion de backtraking para saber cual es el mayor indice de coincidencias. Aumenta la cuenta si encuentra diagonales.
 * @author Paul
 * @date 12/2/2018
 * @param matrix Matriz de structs sobre la que se opera. In/Out
 * @param i Indice de fila inicial (Debe ser la ultima)
 * @param j Indice de columna inicial (Debe ser la ultima)
 */
int GetRuta(struct Celda** matrix, unsigned i, unsigned j)
{
	unsigned maximo = 0;
	
	AuxGetRuta(matrix, i, j, 0, &maximo);
	
	return maximo;
}

void AuxGetRuta(struct Celda** matrix, unsigned i, unsigned j, unsigned cont, unsigned *maximo)
{ 
	if(i == 0 || j == 0)
	{
		*maximo = maxU(cont, *maximo);
	}
	else
	{
		if(matrix[i][j].lateral)
        {
			AuxGetRuta(matrix, i - 1, j, cont, maximo);
        }
		if(matrix[i][j].arriba)
        {
			AuxGetRuta(matrix, i, j - 1, cont, maximo);
        }
		if(matrix[i][j].diag)
		{
			AuxGetRuta(matrix, i - 1, j - 1, cont + 1, maximo);
		}
	}
	
    return;
}
