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
unsigned GetRuta(struct Celda**,unsigned,unsigned);
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

/**
 * Main Lleva control del orden de ejecucion de las funciones del algoritmo y la entrada de parametros
 * @author Lidia y Nacho
 * @date 8/2/2018
 * @param Nombre o ruta de Fichero1 (Necesario)
 * @param Nombre o ruta de Fichero2 (Necesario)
 * @param Tamano de cadena de fichero1 (Opcional o ambos ficheros si falta siguiente parametro)
 * @param Tamano de cadena de Fichero2 (Opcional)
 * @param Inicio de cadena de Fichero1 (Opcional o ambos ficheros si falta siguiente parametro)
 * @param Inicio de cadena de Fichero2 (Opcional)
 */
int main( int argc, char *argv[] ) 
{ 
    unsigned T1,T2,I1,I2;
    switch(argc)
    {
        case 3: //Ambos desde el principio hasta lo que coja
            T1=100;T2=100;I1=0;I2=0;
            
        break;
        case 4: //Ambos con Tamaño arg[3]
            T1=atoi(argv[3]);T2=T1;I1=0;I2=0;
        break;
        case 5: //Cada uno con Tamaño arg[3] y arg[4]
            T1=atoi(argv[3]);T2=atoi(argv[4]);I1=0;I2=0;
        break;
        case 6: //Cada uno con Tamaño arg[3] y arg[4] empezando desde arg[5]
            T1=atoi(argv[3]);T2=atoi(argv[4]);I1=atoi(argv[5]);I2=I1;
        break;
        case 7: //Cada uno con Tamaño arg[3] y arg[4] empezando desde arg[5] y arg[6]
            T1=atoi(argv[3]);T2=atoi(argv[4]);I1=atoi(argv[5]);I2=atoi(argv[6]);
        break;
        default: //Instrucciones de uso
        printf("Error en introduccion de datos:\n Se pueden introducir entre 2 y 6 argumentos:\n2 argumentos:\n   Fichero_1 Fichero_2\n");
        printf("3 argumentos:\n   Fichero_1 Fichero_2 Tamano_maximo\n");       
        printf("4 argumentos:\n   Fichero_1 Fichero_2 TamanoMax_Cadena1 TamanoMax_Cadena2\n");  
        printf("5 argumentos:\n   Fichero_1 Fichero_2 TamanoMax_Cadena1 TamanoMax_Cadena2 Inicio_Cadenas\n");  
        printf("6 argumentos:\n   Fichero_1 Fichero_2 TamanoMax_Cadena1 TamanoMax_Cadena2 Inicio_C1 Inicio_C2\n");  
        printf("Tamano e inicio escalados: 1:100\n");
    }

    if(argc >= 3 && argc <=7)
    {
        printf("%s comparado con %s\n",argv[1],argv[2]);
        printf("Tamanos: %d %d\n",T1*100,T2*100);
        printf("Puntos de inicio: %d %d\n",I1*100,I2*100);
        //time_t t1, t2, t3, t4;
        //t1 = time(0);
        char* string1=CargarFichero(argv[1],T1,I1);
        char* string2=CargarFichero(argv[2],T2,I2);
        //                                                                    printf("Fin construccion cadenas\n");
        struct Celda **Matriz;
        if(strlen(string1)==0 || strlen(string2)==0)
        {
            printf("Una cadena esta vacia");
            exit(2);
        }
                                                                            printf("Lectura correcta \n");
        Matriz=inicializarMatriz(strlen(string1),strlen(string2));
        //t2 = time(0);
        //printf("Inicializado:       %d\n", t2 - t1);
                                                                            printf("Matriz iniciada\n");
        CompletarMatriz(string1,string2,Matriz);
        //t3 = time(0);
        //printf("Creacion de matriz: %d\n", t3 - t2);
                                                                            printf("Matriz completa\n");
        int resultado= GetRuta(Matriz,strlen(string1),strlen(string2));
                                                                            printf("Ruta calculada\n");
        //t4 = time(0);
        //printf("Backtracking:       %d\n", t4 - t3);

        //printf("Total:              %d\n", t4 - t1);
        printf("Coincidencia(porc): %d\n", 100*resultado/maxU(strlen(string1),strlen(string2)));
    }
    else
    {
        exit(3);
    }
    printf("Fin");
    exit(0);
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
        fgets(caracteres,100,archivo); //Primera linea
        for(i=0;i<inicio;i++)
            fgets(caracteres,100,archivo);
        i=tamano;
 	    while (feof(archivo) == 0 && strlen(cadena)<i) //Hasta fin de archivo o memoria
 	    {
 		fgets(caracteres,100,archivo);
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
 * @param matrix Matriz de Celdas, su tamaÃ±o debe ser el de las cadenas de texto +1
 */
void CompletarMatriz(char* string1,char* string2,struct Celda** matrix)
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
 * @author Nacho en la optimización
 * @date 12/2/2018
 * @date 14/2/2018 en optimizacion
 * @param matrix Matriz de structs sobre la que se opera. In/Out
 * @param i Indice de fila inicial (Debe ser la ultima)
 * @param j Indice de columna inicial (Debe ser la ultima)
 */
unsigned GetRuta(struct Celda** matrix, unsigned i, unsigned j)
{
	unsigned maximo = 0;
	
	AuxGetRuta(matrix, i, j, 0, &maximo);
	
	return maximo;
}

void AuxGetRuta(struct Celda** matrix, unsigned i, unsigned j, unsigned cont, unsigned *maximo)
{
    //Comprobante de poda
    //Si el algoritmo no puede conseguir tantas diagonales como el mejor valora partir del punto actual, corta.
    if(((cont+i<maximo || cont+j<maximo)&&(*maximo>0)))
    {
        return;
    }
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
