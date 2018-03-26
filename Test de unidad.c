#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <omp.h>

/*Struct con el valor de la celda y las direcciones
  Si las direcciones son verdadero, entonces ha heredado de esa direccion
*/
struct Celda
{
    int score;
    short dir;
};

char* CargarFichero(char*,unsigned,unsigned);
struct Celda** inicializarMatriz(unsigned, unsigned, unsigned);
void CompletarMatrizSecuencial(char*,char*,struct Celda**);
void CompletarMatrizOmp(char*,char*,struct Celda**,unsigned);
unsigned* AsignarVector(unsigned,unsigned);
void CalcularSubMatriz(struct Celda**,unsigned,unsigned,char*,char*,unsigned*,unsigned);
void CalcularCasilla(unsigned, unsigned, bool, struct Celda**);  
unsigned GetRuta(struct Celda**,unsigned,unsigned);
int AuxGetRuta(struct Celda**, unsigned, unsigned, int, unsigned*);
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
	char* string1=CargarFichero("UT1.fa",1,0);
	printf("Cadena 1 leida\n");
    char* string2=CargarFichero("UT2.fa",1,0);
    printf("Cadena 2 leida\n");
    if(strlen(string1)==0 || strlen(string2)==0)
    {
        printf("Una cadena esta vacia\n");
        exit(2);
    }
        
    struct Celda **Matriz;
    Matriz=inicializarMatriz(strlen(string1),strlen(string2),2);
    printf("Matriz iniciada\n");
    
    //CompletarMatrizSecuencial(string1,string2,Matriz); 
    
    CompletarMatrizOmp(string1,string2,Matriz,1); 
    
    int i,j;
    
    printf("\n");
    for(i=0;i<=7;i++) {
        for(j=0;j<=7;j++)
            printf("%d ", Matriz[i][j].dir);
         printf("\n");
        }
        printf("\n");
        for(i=0;i<=7;i++) {
        for(j=0;j<=7;j++)
            printf("%d ", Matriz[i][j].score);
         printf("\n");
        }
        printf("\n");
    
    
    
    printf("Matriz completada\n");
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
    tamano*=100;
    inicio*=100;
    FILE *archivo;
 	unsigned i;
 	char caracteres[100];
 	char *cadena=malloc(tamano);   
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
        i=tamano;
 	    while (feof(archivo) == 0 && strlen(cadena)<i) //Hasta fin de archivo o memoria
 	    {
 		fgets(caracteres,1000,archivo);
 		strcat(cadena, caracteres);
 	    }
    }
        Mayus(cadena);
	return cadena;
}


// inicializarMatriz funcion que crea la matriz de tamano r c e inicializa la primera fila y columna con valores negativos descendentes.
// @author Sara
// @date 12/2/2018
// @param unsigned r rows
// @param unsigned c cols
// @param unsigned m modo de ejecucion
// @return arr matriz con los valores negativos
struct Celda** inicializarMatriz(unsigned r, unsigned c, unsigned m)
{
    unsigned i;
    struct Celda **arr =(struct Celda **)malloc((r+1)* sizeof(struct Celda *));
    struct Celda *mem= (struct Celda *) malloc((r+1)*(c+1)*sizeof(struct Celda));
    for (i=0;i<=r;++i)
           arr[i]= mem + i*(c+1);
    //Casos base posicion:  r = 0, c = 0
    arr[0][0].score = 0;
    arr[0][0].dir = 0;
    switch(m)
    {
        case 2: //Omp
        {
            #pragma omp parallel sections shared(arr) private (i)
            {
                #pragma omp section
                {
                    for(i = 1 ; i<=r; i++)
                    {
                        arr[i][0].score = -i;
                        arr[i][0].dir=0;
                    }
                }
                #pragma omp section
                {
                    for(i = 1 ; i<=c; i++)
                    {
                        arr[0][i].score = -i;
                        arr[0][i].dir=0;
                    }
                }
            }
        }
        break;
        default:
        {
            for(i = 1 ; i<=r; i++)
            {
                arr[i][0].score = -i;
                arr[i][0].dir=0;
            }
            for(i = 1 ; i<=c; i++)
            {
                arr[0][i].score = -i;
                arr[0][i].dir=0;
            }
        }
        break;
    }
    
    return arr;
}


/**
 * CompletarMatrizSecuencial funcion que calcula el algoritmo Needleman-Wunsch para una matriz
 * @author Nacho
 * @date 7/2/2018
 * @param string1 Cadena de texto 1
 * @param string2 Cadena de texto 2
 * @param matrix Matriz de Celdas, su tamaño debe ser el de las cadenas de texto +1
 */
 
void CompletarMatrizSecuencial(char* string1,char* string2,struct Celda** matrix)
{
    
    unsigned i;
    unsigned j;
    unsigned size1=strlen(string1);
    unsigned size2=strlen(string2);
    for(i=1;i<=size1;++i)
        for(j=1;j<=size2;++j)
            //El argumento de calcular casilla es cierto si ambos strings coinciden o uno de ellos es N
            //Recordar que el tamano de la matriz es 1 mayor que los strings, y estos se alinean con el final.
            {
            CalcularCasilla(i, j, (string1[i-1]==string2[j-1]||string1[i-1]=='N'||string2[j-1]=='N'), matrix);
            }
    return;
}

/**
 * CompletarMatriz funcion que gestiona los hilos para realizar el algoritmo Needleman-Wunsch en multiples procesadores
 * @author Nacho
 * @date 28/2/2018
 * @param string1 Cadena de texto 1
 * @param string2 Cadena de texto 2
 * @param matrix Matriz de Celdas, su tamano debe ser el de las cadenas de texto +1
 * @param sobrecarga Cantidad de bloques que realiza un hilo. A mayor cantidad menores tiempos, pero su valor maximo deber�a ser la longitud del string.
 */
void CompletarMatrizOmp(char* string1,char* string2,struct Celda** matrix, unsigned sobrecarga)
{
    unsigned i;
    unsigned p=omp_get_max_threads();
    unsigned *posiciones=AsignarVector(strlen(string2),p*sobrecarga);
    unsigned *locks =(unsigned *)malloc(p*sobrecarga* sizeof(unsigned));
    for (i = 0; i < p*sobrecarga; ++i)
    {
        locks[i] = 0;
    }
    
    #pragma omp parallel private(i) shared(matrix,posiciones,string1,string2,locks,p,sobrecarga)
    {
        unsigned id=omp_get_thread_num();
        for(i=0;i<sobrecarga;i++)
            if(id==0&&i==0)
                CalcularSubMatriz (matrix, 1,posiciones[0],string1,string2,locks,0);
            else
                CalcularSubMatriz (matrix, posiciones[id-1+p*i],posiciones[id+p*i],string1,string2,locks,(id+p*i));    
    }
}

/**
 * AsignarVector funcion que calcula las posiciones finales de cada bloque
 * @author Lidia
 * @date 4/3/2018
 * @param tamano Tamano del ancho de la matriz
 * @param p Numero de procesadores
 * @return final Vector dinamico de posiciones
 */
unsigned* AsignarVector(unsigned tamano,unsigned p)
{
	int a,l, i;
	unsigned *final =(unsigned *)malloc(p*sizeof(unsigned));
	l=tamano/p;
	//Garantizar un minimo avance
	if(l==0)
	   l=1;
	//Asignacion
	a=l;
	for(i=0;i<p-1;++i)
	{
		final[i]=a;
		a+=l;
	}
	final[p-1]=tamano;
	//Para evitar accidentes
	for(i=0;i<p;++i)
	{
		if(final[i]>tamano)
		  final[i]=tamano;
	}
    return final;
}
/**
 * CalcularSubMatriz rellena la matriz a partes, entre dos columnas objetivo
 * @author Paul
 * @date 01/03/2018
 * @param matrix Matriz sobre la que se opera
 * @param c1 indice de la columna que calcularemos
 * @param c2 indice de la columna que calcularemos
 * @param string1 Cadena de texto
 * @param string2 Cadena de texto
 * @param locks Array de cerrojos
 * @param id Identificador del hilo que  esta ejecutando esta funcion
 */
void CalcularSubMatriz(struct Celda** matrix, unsigned c1, unsigned c2, char* string1, char* string2, unsigned* locks, unsigned id)
{
	int tiempo = 500;
	int dormir = 0;
	unsigned size1=strlen(string1);
	int i;
	int j;
	
	for( i = 1; i <= size1; ++i)
	{
		for( j = c1; j <= c2; ++j)
		{
			while( id > 0 && locks[id - 1] <= locks[id] )
			{
				dormir = usleep(tiempo);
			}
			
			CalcularCasilla(i, j, string1[i - 1] == string2[j-1] || string1[i - 1] == 'N' || string2[j-1] == 'N', matrix);
		}
		locks[id]++;
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
    int A = matrix[i-1][j].score - 1;
    int B = matrix[i][j-1].score - 1;
    int C = matrix[i-1][j-1].score+ (igual*4)-2; //C= arg + (argB*(Match-Fallo))+Fallo
    
    //Calculo de la direcci�n como un array de booleanos
    int D=0;
    D += (A>=B && A>=C)<<1;    //Vertical en 1a posicion
    D += (B>=A && B>=C); //Horizontal en 2a posicion
    D += (C>=B && C>=A)<<2; //Diagonal en 3a posicion
    matrix[i][j].dir = D;
    
    matrix[i][j].score=maxI(maxI(A,B),C);
    
}

/**
 * GetRuta Funcion de backtraking para saber cual es el mayor indice de coincidencias. Aumenta la cuenta si encuentra diagonales.
 * @author Paul
 * @author Nacho en la optimizaci�n
 * @date 12/2/2018
 * @date 14/2/2018 en optimizacion
 * @param matrix Matriz de structs sobre la que se opera. In/Out
 * @param i Indice de fila inicial (Debe ser la ultima)
 * @param j Indice de columna inicial (Debe ser la ultima)
 */
unsigned GetRuta(struct Celda** matrix, unsigned i, unsigned j)
{
	unsigned maximo = 0;
	unsigned x,y;
	for(x=0;x<=i;x++)
	   for(y=0;y<=j;y++)
	       matrix[x][y].score=-1;
	
	/*
	printf("\n");
        for(x=0;x<=7;x++) {
        for(y=0;y<=7;y++)
            printf("%d ", matrix[x][y].score);
         printf("\n");
        }
        printf("\n");
	*/
	AuxGetRuta(matrix, i, j, 0, &maximo);
	/*
	printf("\n");
        for(x=0;x<=7;x++) {
        for(y=0;y<=7;y++)
            printf("%d ", matrix[x][y].score);
         printf("\n");
        }
        printf("\n");
	*/
	
	return maximo;
}

int AuxGetRuta(struct Celda** matrix, unsigned i, unsigned j, int cont, unsigned *maximo)
{
    //printf("%d %d %d %d %d %d ",i,j,matrix[i][j].score,matrix[i][j].dir,cont,*maximo);
    
    
    int A=-1,B=-1,C=-1;
    if(matrix[i][j].score>=0)
    {
        //printf("Poda por score>0\n");
        return matrix[i][j].score;
    }
    if((cont+i<*maximo || cont+j<*maximo)&&(*maximo>0))
    {
        matrix[i][j].score=0;
        //printf("Poda por lejania\n");
        return 0;
    }
	if(i == 0 || j == 0)
	{
		*maximo = maxI(cont, *maximo);
		matrix[i][j].score=0;
		//printf("Guardando Maximo\n");
		return 0;
	}
	else 
	{
	    //printf("Procesando\n");
	    if(matrix[i][j].dir>3)
		{
		    //printf("%d %d diagonal\n",i,j);
			A=AuxGetRuta(matrix, i - 1, j - 1, cont + 1, maximo)+1;
		}
		if(matrix[i][j].dir==2||matrix[i][j].dir==3||matrix[i][j].dir==6||matrix[i][j].dir==7)
        {
            //printf("%d %d arriba\n",i,j);
			B=AuxGetRuta(matrix, i - 1, j, cont, maximo);
        }
		if(matrix[i][j].dir==1||matrix[i][j].dir==3||matrix[i][j].dir==5||matrix[i][j].dir==7)
        {
            //printf("%d %d horiz\n",i,j);
			C=AuxGetRuta(matrix, i, j - 1, cont, maximo);
        }
	}
	
	matrix[i][j].score=maxI(maxI(A,B),C);
    return matrix[i][j].score;
}
