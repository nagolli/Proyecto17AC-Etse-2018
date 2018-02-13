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

char* CargarFichero(char*);
struct Celda** InicioMatriz(unsigned,unsigned);
struct Celda** inicializarMatriz(unsigned, unsigned);
void CompletarMatriz(char*,char*,struct Celda**);
void CalcularCasilla(unsigned, unsigned, bool, struct Celda**);  
int GetRuta(struct Celda**,unsigned,unsigned);
void AuxGetRuta(struct Celda**, int, int, int, int*)
/*Maximo entre dos valores*/
unsigned max(unsigned arg1, unsigned arg2)
{
    if(arg1>arg2){
        return arg1;
    }
    else
        return arg2;
}
int max(int arg1, int arg2)
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

int main( int argc, char *argv[] ) 
{ 
    if(argc == 3)
    {
        time_t t1, t2, t3, t4;
        t1 = time(0);
        char* string1=CargarFichero(argv[1]);
        char* string2=CargarFichero(argv[2]);
        struct Celda **Matriz;
        Matriz=InicioMatriz(strlen(string1),strlen(string2));
        t2 = time(0);
        CompletarMatriz(string1,string2,Matriz);
        t3 = time(0);
        int resultado= GetRuta(Matriz);
        t4 = time(0);
        printf("Inicializado:       %d\n", t2 - t1);
        printf("Creacion de matriz: %d\n", t3 - t2);
        printf("Backtracking:       %d\n", t4 - t3);
        printf("Total:              %d\n", t4 - t1);
        printf("Coincidencia(porc): %d\n", 100*resultado/max(max(strlen(string1),strlen(string2)),1));
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
 	char *cadena=malloc(100000000);   //Límite de 10 millones de caracteres
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
    unsigned i;
    struct Celda **arr =  (struct Celda **)malloc(50*50 * sizeof(struct Celda));
    for (i = 0; i < 50; ++i)
        arr[i] = (struct Celda *)malloc(50 * sizeof(struct Celda));
    return arr;
}

/**
 * inicializarMatriz funcion que crea la matriz de tamaño r c e inicializa la primera fila y columna con valores negativos descendentes.
 * @author Sara
 * @date 12/2/2018
 * @param unsigned r rows
 * @param unsigned c cols
 * @out arr matriz con los valores negativos
 */
struct Celda** inicializarMatriz(unsigned r, unsigned c)
{
    struct Celda **arr =(struct Celda **)malloc(r*c* sizeof(struct Celda));
    int negativo = -2;
    
    //Casos base posicion:  r = 0, c = 0
    //                      r = 1, c = 0
    //                      r = 0, c = 1
    arr[0][0] = 0;
    arr[1][0] = -2;
    arr[0][1] = -2;
    
    for(unsigned i = 2; i<r; i++)
    {
        for(unsigned j = 2; j<c; j++)
        {
            arr[i][0] = negativo*2;
            arr[0][j] = negativo*2;
            negativo *= 2;
        }
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
    if((matrix[i][j].arriba+matrix[i][j].lateral+matrix[i][j].diag)==0)
    //Calculo del valor a partir de los valores y direcciones
    
    //Version sin IF
    //Sumatorio (valor x Direccion) / Suma de direcciones
    matrix[i][j].score=((A* matrix[i][j].lateral)+(B* matrix[i][j].arriba)+(C* matrix[i][j].diag)) / (matrix[i][j].arriba+matrix[i][j].lateral+matrix[i][j].diag);
    
    //Version con IF
    /*
    matrix[i][j].score=max(max(A,B),C);
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
		*maximo = max(cont, *maximo);
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
