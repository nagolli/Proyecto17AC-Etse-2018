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
char* CargarFichero(char*);
struct Celda** InicioMatriz(unsigned,unsigned);
void CompletarMatriz(char*,char*,struct Celda**);
void CalcularCasilla(unsigned, unsigned, bool, struct Celda**);  
int GetRuta(struct Celda**);
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
    char* string1=CargarFichero("UT1.fa");
    char* string2=CargarFichero("UT2.fa");
    struct Celda **Matriz;
    Matriz=InicioMatriz(strlen(string1),strlen(string2));
    CompletarMatriz(string1,string2,Matriz);
    int resultado= GetRuta(Matriz);
    
    return resultado == 5;
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
