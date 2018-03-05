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
    char dir; //Array de booleanos
};

void ImprimirInstruccionesDeUso();
char* CargarFichero(char*,unsigned,unsigned);
struct Celda** inicializarMatriz(unsigned, unsigned,unsigned);
void CompletarMatrizSecuencial(char*,char*,struct Celda**);
void CompletarMatrizOmp(char*,char*,struct Celda**,unsigned);
unsigned* AsignarVector(unsigned,unsigned);
void CalcularSubMatriz(struct Celda**,unsigned,unsigned,char*,char*,unsigned*,unsigned);
void CalcularCasilla(unsigned, unsigned, bool, struct Celda**);  
unsigned GetRuta(struct Celda**,unsigned,unsigned);
int AuxGetRuta(struct Celda**, unsigned, unsigned, int, unsigned*);

//Funciones auxiliares

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
 * @date 3/3/2018 añadido OMP
 * @param Nombre o ruta de Fichero1 (Necesario)
 * @param Nombre o ruta de Fichero2 (Necesario)
 * @param Tamano de cadena de fichero1 (Opcional o ambos ficheros si falta siguiente parametro)
 * @param Tamano de cadena de Fichero2 (Opcional)
 * @param Inicio de cadena de Fichero1 (Opcional o ambos ficheros si falta siguiente parametro)
 * @param Inicio de cadena de Fichero2 (Opcional)
 */
int main( int argc, char *argv[] ) 
{ 
    unsigned T1,T2,I1,I2, sobrecarga, modo;
    char* nombre1;
    char* nombre2;
    bool lecturaCorrecta=1;
    switch(atoi(argv[1]))
    {
        case 2: //OpenMP
        {
            modo=2;
            sobrecarga=atoi(argv[2]);
            nombre1=argv[3];
            nombre2=argv[4];
            switch(argc)
            {
                case 5: //Ambos desde el principio hasta lo que coja
                    T1=1000;T2=1000;I1=0;I2=0;
                    break;
                case 6: //Ambos con Tamano arg[5]
                    T1=atoi(argv[5]);T2=T1;I1=0;I2=0;
                    break;
                case 7: //Cada uno con Tamano arg[5] y arg[6]
                    T1=atoi(argv[5]);T2=atoi(argv[6]);I1=0;I2=0;
                    break;
                case 8: //Cada uno con Tamano arg[5] y arg[6] empezando desde arg[7]
                    T1=atoi(argv[5]);T2=atoi(argv[6]);I1=atoi(argv[7]);I2=I1;
                    break;
                case 9: //Cada uno con Tamano arg[5] y arg[6] empezando desde arg[7] y arg[8]
                    T1=atoi(argv[5]);T2=atoi(argv[6]);I1=atoi(argv[7]);I2=atoi(argv[8]);
                    break;
                default: //Instrucciones de uso
                    ImprimirInstruccionesDeUso();
                    lecturaCorrecta=0;
                break;
            }
        }
        break;
        case 3: //MPI
        {
            
        }   
        break;

        case 1:
        {
            modo=1;
            nombre1=argv[2];
            nombre2=argv[3];
            switch(argc)
            {
                case 4: //Ambos desde el principio hasta lo que coja
                    T1=1000;T2=1000;I1=0;I2=0;
                break;
                case 5: //Ambos con Tamano arg[3]
                    T1=atoi(argv[4]);T2=T1;I1=0;I2=0;
                break;
                case 6: //Cada uno con Tamano arg[3] y arg[4]
                    T1=atoi(argv[4]);T2=atoi(argv[5]);I1=0;I2=0;
                break;
                case 7: //Cada uno con Tamano arg[3] y arg[4] empezando desde arg[5]
                    T1=atoi(argv[4]);T2=atoi(argv[5]);I1=atoi(argv[6]);I2=I1;
                break;
                case 8: //Cada uno con Tamano arg[3] y arg[4] empezando desde arg[5] y arg[6]
                    T1=atoi(argv[4]);T2=atoi(argv[5]);I1=atoi(argv[6]);I2=atoi(argv[7]);
                break;
                default: //Instrucciones de uso
                ImprimirInstruccionesDeUso();
                lecturaCorrecta=0;
            }
        }
        break;
        default:
        {
            lecturaCorrecta=0;
            ImprimirInstruccionesDeUso();
        }
        break;
    }
    
    if(lecturaCorrecta==1)
    {
        printf("%s comparado con %s\n",nombre1,nombre2);
        printf("Tamanos: %d %d\n",T1*100,T2*100);
        printf("Puntos de inicio: %d %d\n",I1*100,I2*100);
        struct timeval t1,t2,t3,t4;
        double total;

        gettimeofday(&t1, NULL);
        
        char* string1;
        char* string2;
        switch(modo)
        {
            case 1: //Secuencial
            {
                string1=CargarFichero(nombre1,T1,I1);
                string2=CargarFichero(nombre2,T2,I2);   
            }
            break;
            
            
            default:
            {
                exit(4);
            }
            break;
                
        }
        
        struct Celda **Matriz;
        if(strlen(string1)==0 || strlen(string2)==0)
        {
            printf("Una cadena esta vacia");
            exit(2);
        }
        Matriz=inicializarMatriz(strlen(string1),strlen(string2),modo);
        gettimeofday(&t2, NULL);
        if(modo==1)
            CompletarMatrizSecuencial(string1,string2,Matriz);
        if(modo==2)
            CompletarMatrizOmp(string1,string2,Matriz,sobrecarga);
        gettimeofday(&t3, NULL);
        int resultado= GetRuta(Matriz,strlen(string1),strlen(string2));
        
        gettimeofday(&t4, NULL);
        
        total = ((t2.tv_sec * 1000000 + t2.tv_usec)-(t1.tv_sec * 1000000 + t1.tv_usec));
        printf("Inicializado:       %lf\n", total );
        
        total = ((t3.tv_sec * 1000000 + t3.tv_usec)-(t2.tv_sec * 1000000 + t2.tv_usec));
        printf("Creacion de matriz: %lf\n", total );
        
        total = ((t4.tv_sec * 1000000 + t4.tv_usec)-(t3.tv_sec * 1000000 + t3.tv_usec));
        printf("Backtracking:       %lf\n", total ) ;
        
        total = ((t4.tv_sec * 1000000 + t4.tv_usec)-(t1.tv_sec * 1000000 + t1.tv_usec));
        printf("Total:              %lf\n", total );
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
 * ImprimirInstruccionesDeUso imprime la informacion de uso de este programa, asi como los autores.
 * @author Nacho
 * @date 3/03/2018
 */
void ImprimirInstruccionesDeUso()
{
    printf("Programa para el calculo de porcentaje de similitud de dos cadenas de Adn\n");  
    printf("Autores:\n");  
    printf("\tIgnacio Gomis Lli\n");  
    printf("\tLidia Montero Egidos\n");  
    printf("\tSara Monzo Bravo\n");  
    printf("\tPaul Vargas Hurtado\n");  
    printf("Asignatura: Arquitectura de procesadores, 3ro Ingenieria Informatica\n");  
    printf("Profesorado:\n");  
    printf("\t Jose Manuel Claver Iborra\n");  
    printf("\t Adria Gimenez Pastor\n");  
    printf("Universidad de Valencia, ETSE\n");  
    printf("Curso 2017/2018\n");  
    printf("\n");  
    printf("Instrucciones de uso:\n");  
    printf("-Los ficheros deben estar escritos en formato fasta\n");  
    printf("-Los argumentos de tamano tienen un escalado 1:100, es decir, en caso de que\n"); 
    printf("se introduzca un 1 nos referimos a la posicion 100 del fichero\n");  
    printf("\n");  
    printf("Existen distintos modos de ejecucion, se determina cual se emplea segun el\n"); 
    printf("primer argumento\n");  
    printf("-Ejecucion secuencial, primer argumento 1\n");  
    printf("3 argumentos:\n  1 Bloques_Por_Hilo Fichero_1 Fichero_2\n");     
    printf("4 argumentos:\n  1 Fichero_1 Fichero_2 Tamano_maximo\n");       
    printf("5 argumentos:\n  1 Fichero_1 Fichero_2 TamanoMax_Cadena1 TamanoMax_Cadena2\n");  
    printf("6 argumentos:\n  1 Fichero_1 Fichero_2 TamanoMax_Cadena1 TamanoMax_Cadena2 Inicio_Cadenas\n");  
    printf("7 argumentos:\n  1 Fichero_1 Fichero_2 TamanoMax_Cadena1 TamanoMax_Cadena2 Inicio_C1 Inicio_C2\n");  
    printf("\n");  
    printf("-Ejecucion con OMP, segundo argumento 2, requiere opcion de compilado -fopenmp\n"); 
    printf("4 argumentos:\n  2 Bloques_Por_Hilo Fichero_1 Fichero_2\n");        
    printf("5 argumentos:\n  2 Bloques_Por_Hilo Fichero_1 Fichero_2 Tamano_maximo\n");       
    printf("6 argumentos:\n  2 Bloques_Por_Hilo Fichero_1 Fichero_2 TamanoMax_Cadena1 TamanoMax_Cadena2\n");  
    printf("7 argumentos:\n  2 Bloques_Por_Hilo Fichero_1 Fichero_2 TamanoMax_Cadena1 TamanoMax_Cadena2 Inicio_Cadenas\n");  
    printf("8 argumentos:\n  2 Bloques_Por_Hilo Fichero_1 Fichero_2 TamanoMax_Cadena1 TamanoMax_Cadena2 Inicio_C1 Inicio_C2\n");  

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
 	char *cadena=malloc(tamano);   //<--Origen error
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
struct Celda** inicializarMatriz(unsigned r, unsigned c, unsigned m)
{
    unsigned i;
    struct Celda **arr =(struct Celda **)malloc(r*c* sizeof(struct Celda));
    for (i = 0; i <= r; ++i)
        arr[i] = (struct Celda *)malloc(c * sizeof(struct Celda));
    //Casos base posicion:  r = 0, c = 0
    arr[0][0].score = 0;
    arr[0][0].dir = 0;
    switch(m)
    {
        case 1: //Secuencial
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
        case 2: //Omp
        {
            
        }
        break;
        default:
        {
            exit(4);
        }
        break;
    }
    
    return arr;
}


/**
 * CompletarMatriz funcion que calcula el algoritmo Needleman-Wunsch para una matriz
 * @author Nacho
 * @date 7/2/2018
 * @param string1 Cadena de texto 1
 * @param string2 Cadena de texto 2
 * @param matrix Matriz de Celdas, su tamano debe ser el de las cadenas de texto +1
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
 * @param sobrecarga Cantidad de bloques que realiza un hilo. A mayor cantidad menores tiempos, pero su valor maximo deberï¿½a ser la longitud del string.
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
	unsigned a,l, i;
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
	unsigned size1=strlen(string1);
	unsigned i;
	unsigned j;
	
	for( i = 1; i <= size1; ++i)
	{
		for( j = c1; j <= c2; ++j)
		{
			while( id > 0 && locks[id - 1] <= locks[id] )
			{
				usleep(tiempo);
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
    
    //Calculo de la dirección como un array de booleanos
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
	//Reinicializado de los scores, ahora mediremos la distancia a la esquina 0,0
	//El valor -1 representa dato desconocido
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
    //Poda, impide repetición de celdas si ya ha calculado el camino
    if(matrix[i][j].score>=0)
    {
        return matrix[i][j].score;
    }
    //Poda, impide recorrer un nuevo camino si no hay posibilidad de superar la mejor marca
    if((cont+i<*maximo || cont+j<*maximo)&&(*maximo>0))
    {
        matrix[i][j].score=0;
        return 0;
    }
    //Caso base
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
		    //En las diagonales se añade distancia si existen
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
