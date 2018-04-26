#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "mpi.h"

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
struct Celda** inicializarMatriz(unsigned, unsigned);
void CompletarMatrizMPI(char* string1, char*, struct Celda**, int);
void CalcularSubMatrizMPI(struct Celda**, char*, char*, int,int);
void CalcularCasilla(unsigned, unsigned, bool, struct Celda**,bool,int,int,int);  
unsigned GetRuta(struct Celda**,unsigned,unsigned);
int AuxGetRuta(struct Celda**, unsigned, unsigned, int, unsigned*);
void print(struct Celda**, char*);

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
  }
  }

/**
 * Main Lleva control del orden de ejecucion de las funciones del algoritmo y la entrada de parametros
 * @author Lidia y Nacho
 * @date 8/2/2018
 * @date 24/4/2018 Rehecho unicamente para MPI
 * @param Nombre o ruta de Fichero1 (Necesario)
 * @param Nombre o ruta de Fichero2 (Necesario)
 * @param Tamano de cadena de fichero1 (Opcional o ambos ficheros si falta siguiente parametro)
 * @param Tamano de cadena de Fichero2 (Opcional)
 * @param Inicio de cadena de Fichero1 (Opcional o ambos ficheros si falta siguiente parametro)
 * @param Inicio de cadena de Fichero2 (Opcional)
 */
int main( int argc, char *argv[] ) 
{ 
    char* string1;
    char* string2;
    struct Celda **Matriz;
                                                                                                    //printf("Inicio\n");
    MPI_Init(&argc,&argv);
    int rank,x,y;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
                                                                                                    //printf("Rank %d\n",rank);
    unsigned T1,T2,I1,I2, sobrecarga;
    char* nombre1;
    char* nombre2;
    bool lecturaCorrecta=1;
    struct timeval t1,t2,t3,t4;
            double total;
    if(argc>=4)
    {
            sobrecarga=atoi(argv[1]);
            nombre1=argv[2];
            nombre2=argv[3];
            switch(argc)
            {
                case 4: //Ambos desde el principio hasta lo que coja
                    T1=1000;T2=1000;I1=0;I2=0;
                    break;
                case 5: //Ambos con Tamano arg[4]
                    T1=atoi(argv[4]);T2=T1;I1=0;I2=0;
                    break;
                case 6: //Cada uno con Tamano arg[4] y arg[5]
                    T1=atoi(argv[4]);T2=atoi(argv[5]);I1=0;I2=0;
                    break;
                case 7: //Cada uno con Tamano arg[4] y arg[5] empezando desde arg[6]
                    T1=atoi(argv[4]);T2=atoi(argv[5]);I1=atoi(argv[6]);I2=I1;
                    break;
                case 8: //Cada uno con Tamano arg[4] y arg[5] empezando desde arg[6] y arg[7]
                    T1=atoi(argv[4]);T2=atoi(argv[5]);I1=atoi(argv[6]);I2=atoi(argv[7]);
                    break;
                default: //Instrucciones de uso
                    ImprimirInstruccionesDeUso();
                    lecturaCorrecta=0;
                break;
            }
    }
    else
    {
    ImprimirInstruccionesDeUso();
    lecturaCorrecta=0;    
    }
        
    
    if(lecturaCorrecta==1)
    {
        if(rank==0)
        {
            printf("%s comparado con %s\n",nombre1,nombre2);
            printf("Tamanos: %d %d\n",T1*100,T2*100);
            printf("Puntos de inicio: %d %d\n",I1*100,I2*100);
            
    
            gettimeofday(&t1, NULL);
            
            string1=CargarFichero(nombre1,T1,I1);
            string2=CargarFichero(nombre2,T2,I2);   
    
            if(strlen(string1)==0 || strlen(string2)==0)
            {
                printf("Una cadena esta vacia");
                exit(2);
            }
            Matriz=inicializarMatriz(strlen(string1),strlen(string2));
            gettimeofday(&t2, NULL);

        }
        else
        {
            string1=CargarFichero(nombre1,T1,I1);
            if(strlen(string1)==0)
            {
                printf("Una cadena esta vacia");
                exit(2);
            }
        }
           
	if(rank==0)print(Matriz,"PreCalculo");  
                                                                               //printf("Rank: %d.Inicio\n",rank);
        CompletarMatrizMPI(string1,string2,Matriz,sobrecarga);

	if(rank==0)print(Matriz,"Salida");

        if(rank==0){
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
    }
    else
    {
        exit(3);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Fin P%d\n",rank);
    MPI_Finalize();
    exit(0);
    return 0;
}

/**
 * ImprimirInstruccionesDeUso imprime la informacion de uso de este programa, asi como los autores.
 * @author Nacho
 * @date 3/03/2018
 * @date 24/4/2018 Adaptado a MPI
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
    printf("-Ejecucion con MPI, segundo argumento 2, requiere opcion de compilado -fopenmp\n"); 
    printf("3 argumentos:\n  Bloques_Por_Hilo Fichero_1 Fichero_2\n");        
    printf("4 argumentos:\n  Bloques_Por_Hilo Fichero_1 Fichero_2 Tamano_maximo\n");       
    printf("5 argumentos:\n  Bloques_Por_Hilo Fichero_1 Fichero_2 TamanoMax_Cadena1 TamanoMax_Cadena2\n");  
    printf("6 argumentos:\n  Bloques_Por_Hilo Fichero_1 Fichero_2 TamanoMax_Cadena1 TamanoMax_Cadena2 Inicio_Cadenas\n");  
    printf("7 argumentos:\n  Bloques_Por_Hilo Fichero_1 Fichero_2 TamanoMax_Cadena1 TamanoMax_Cadena2 Inicio_C1 Inicio_C2\n");  

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
        for(i=0;(i<inicio)&&(feof(archivo) == 0);i++)
        {
            fgets(caracteres,100,archivo);
        }
        i=tamano;
 	    while (feof(archivo) == 0) //Hasta fin de archivo o memoria
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
    struct Celda **arr =(struct Celda **)malloc((r+1)* sizeof(struct Celda *));
    struct Celda *mem= (struct Celda *) malloc((r+1)*(c+1)*sizeof(struct Celda));
    for (i=0;i<=r;++i)
           arr[i]= mem + i*(c+1);
    //Casos base posicion:  r = 0, c = 0
    arr[0][0].score = 0;
    arr[0][0].dir = 0;
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
    return arr;
}


void CompletarMatrizMPI(char* string1, char* string2, struct Celda** Matriz, int sobrecarga)
{  
    int P,id,i,j,r,l;
	MPI_Barrier(MPI_COMM_WORLD);

    MPI_Comm_size(MPI_COMM_WORLD,&P);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);




    //Crear datos locales
                                                                                //printf("Rank: %d.Crear datos locales\n",id);                                        
    if(id==0)
    {
        r=(strlen(string1)+1);
        l=strlen(string2);
    }
    MPI_Bcast(&r, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&l, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int k=l%P;
    int num=((l-k)/P);
    struct Celda **matriz_l =(struct Celda **)malloc((r)* sizeof(struct Celda *));
    char *string2_l;
    if(id!=0){
        string2_l=malloc(num);
        struct Celda *mem= (struct Celda *) malloc(r*num*sizeof(struct Celda));
        for (i=0;i<=r;++i)
           matriz_l[i]= mem + i*(num);
    }
    else{
        string2_l=malloc(num+(l%P));  
        struct Celda *mem= (struct Celda *) malloc(r*(num+(l%P)+1)*sizeof(struct Celda));
        for (i=0;i<=r;++i)
           matriz_l[i]= mem + i*(num+(l%P)+1);
    }
    //Crear tipo para Struct
    // Origen de c�digo: https://stackoverflow.com/questions/9864510/struct-serialization-in-c-and-transfer-over-mpi
                                                                                //printf("Rank: %d.Crear struct MPI\n",id);  
    const int nitems=2;
    int          blocklengths[2] = {1,1};
    MPI_Datatype types[2] = {MPI_INT, MPI_CHAR};
    MPI_Datatype mpi_celda;
    MPI_Aint     offsets[2];
    offsets[0] = offsetof(struct Celda, score);
    offsets[1] = offsetof(struct Celda, dir);
    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_celda);
    MPI_Type_commit(&mpi_celda);
    
    //Enviar a los procesos
                                                                                //printf("Rank: %d.Enviando\n",id);  //ORIGEN DE ERROR
                                                                                //printf("Rank: %d: %d==%d\n",id,r-1,strlen(string1));
    //MPI_Bcast(string1, r-1, MPI_CHAR, 0, MPI_COMM_WORLD);
                                                                                //printf("Rank: %d.Bcast\n",id);
    if(id==0)
    {
        MPI_Scatter(&string2[k], num, MPI_CHAR, &string2_l[k], num,MPI_CHAR, 0, MPI_COMM_WORLD);
        MPI_Scatter(&Matriz[0][k], num, mpi_celda, &matriz_l[0][k], num,mpi_celda, 0, MPI_COMM_WORLD);
        for(i=0;i<k;i++)
        {
            string2_l[i]=string2[i];
            matriz_l[0][i]=Matriz[0][i];
        }
    }
    else
    {
        
        MPI_Scatter(&string2[k], num, MPI_CHAR, &string2_l[0], num,MPI_CHAR, 0, MPI_COMM_WORLD);
                                                                                    //printf("Rank: %d.SCatter1\n",id);
        MPI_Scatter(&Matriz[0][k], num, mpi_celda, &matriz_l[0][0], num,mpi_celda, 0, MPI_COMM_WORLD);
                                                                                    //printf("Rank: %d.Allsended\n",id);
    }    
        
    //Ejecutar c�digo
    CalcularSubMatrizMPI(matriz_l,string1,string2_l,id,P-1);
    //Recolectar de los procesos
                                                                                    //printf("Rank: %d.Recolectando procesos\n",id);  
    if(id==0)
    {
        for(j=1;j<r-1;j++)
            MPI_Gather(&matriz_l[j][k], num, mpi_celda, &Matriz[j][k], num, mpi_celda, 0, MPI_COMM_WORLD);
            for(i=0;i<k;i++)
            {
                matriz_l[j][i]=Matriz[j][i];
            }
    }
    else
    {
        for(j=1;j<r-1;j++)
            MPI_Gather(&matriz_l[j][k], num, mpi_celda, &Matriz[j][0], num, mpi_celda, 0, MPI_COMM_WORLD);
    }
                                                                                //printf("Rank: %d.Fin MPI\n",id);  
    //FinalizarMPI
    MPI_Barrier(MPI_COMM_WORLD);
                                                                                //printf("Rank: %d.Barrera superada\n",id);  
                                                                                //printf("Rank: %d.Fin Funcion\n",id);  
}


/**
 * CalcularSubMatrizMPI rellena una submatriz, de manera completa.
 * En el caso 0 con tama�o N se deben pasar como string2 un string de tama�o N-1.
 * En el resto de casos se deben pasar como string2 un string de tama�o N. Adem�s recibe del anterior hilo los datos necesarios para el c�lculo.
 * En todos los casos salvo el ultimo, envia los datos necesarios para el calculo al siguiente hilo
 * @author Nacho
 * @date 17/04/2018
 * @param matrix Submatriz sobre la que se opera
 * @param tam ancho de matriz (columnas)
 * @param string1 Subcadena de texto 1 (filas)
 * @param string2 Subcadena de texto 2 (columnas)
 * @param id Identificador del hilo que  esta ejecutando esta funcion
 * @param maxId Maximo identificador de hilo
 */
void CalcularSubMatrizMPI(struct Celda** matrix, char* string1, char* string2, int id,int maxId)
{
                                                                                //printf("Rank: %d.Comienza calculo submatriz\n",id);
    MPI_Status status;
	int size1=strlen(string1); //Tama�o dependiente del char* recibido, filas
	int tam;
	if(id>0)
		tam=(strlen(string2))/maxId;   //Tama�o dependiente del char* recibido, columnas
	else
		tam=1+((strlen(string2))/maxId)+(strlen(string2)%maxId);
	int i;
	int j;
	int valor1,valor2=0;
	valor1=matrix[0][0].score;
	for( i = 1; i <= size1; ++i)   //PARA CADA FILA
	{
	    if(id!=0)
	    {
	        	                                                                                                                   //printf("Rank: %d.Fila %d: valor: %d\n",id,i,valor2);    //PETA EN LA 1
	      valor2=valor1; //Desplazar buffer de datos
	                                                                                                                               //printf("Rank: %d.Buffer desplazado",id); 
	      MPI_Recv(&valor1,1,MPI_INT,id-1,i,MPI_COMM_WORLD,&status);   //Recibir dato para nueva linea
	      printf("%d",id);                                                                                                                         //printf("Rank: %d. Calcular casilla 0",id);
	      CalcularCasilla(i, 1, string1[i] == string2[0] || string1[i] == 'N' || string2[0] == 'N', matrix,true,valor1,valor2,id);
		  for( j = 2; j < tam; ++j)
		  {
		    printf("%d",id);        	                                                                                                                   //printf("Rank: %d. Calcular casilla %d",id,j);
		    CalcularCasilla(i, j, string1[i] == string2[j] || string1[i] == 'N' || string2[j] == 'N', matrix,false,0,0,id);
		  }
		  if(id!=maxId)                                                //Si no es el ultimo
	        MPI_Send(&matrix[i][tam].score,1,MPI_INT,id+1,i,MPI_COMM_WORLD);  //Enviar dato a siguiente procesador
	   }
	   else
	   {
	printf("0");		
	    for( j = 1; j < tam; ++j)
		  {
			printf(".");
		    CalcularCasilla(i, j, string1[i-1] == string2[j-1] || string1[i-1] == 'N' || string2[j-1] == 'N', matrix,false,0,0,id);
		  }
		  if(id!=maxId)                                                //Si no es el ultimo
		    MPI_Send(&matrix[i][tam].score,1,MPI_INT,id+1,i,MPI_COMM_WORLD);  //Enviar dato a siguiente procesador
       }
	}
	                                                                            //printf("Rank: %d.Fin calculo submatriz\n",id);
    return;
}


/**
 * CalcularCasilla funcion que calcula el contenido de una casillo de la matriz para el algoritmo Needleman-Wunsch
 * @author Nacho
 * @date 17/4/2018
 * @param i Indice de fila (No puede ser 0)
 * @param j Indice de columna (No puede ser 0)
 * @param igual Comparativa (Char==Char)
 * @param matrix Matriz de structs sobre la que se opera. In/Out
 * @param MPIrec booleano sobre si debe utilizar valores pasados por argumentos (true) o de la matriz (false)
 * @param MPIData1 Valor a la izquierda del calculado si MPIrec
 * @param MPIData2 Valor a la diagonal del calculado si MPIrec
 */
void CalcularCasilla(unsigned i, unsigned j, bool igual, struct Celda **matrix,bool MPIrec,int MPIData1, int MPIData2, int id)
{
    int A,B,C;
    if(!MPIrec){
    // Constantes Match 2, -2 dismatch, -1 Hueco
        A = matrix[i-1][j].score - 1;
        B = matrix[i][j-1].score - 1;
        C = matrix[i-1][j-1].score+ (igual*4)-2; //C= arg + (argB*(Match-Fallo))+Fallo
    }
    else
    {
        A = MPIData1 - 1;
        B = matrix[i][j-1].score - 1;
        C = MPIData2+ (igual*4)-2; //C= arg + (argB*(Match-Fallo))+Fallo
	
    }
    
    //Calculo de la direccion como un array de booleanos
    int D=0;
    D += (A>=B && A>=C)<<1;    //Vertical en 1a posicion
    D += (B>=A && B>=C); //Horizontal en 2a posicion
    D += (C>=B && C>=A)<<2; //Diagonal en 3a posicion
    matrix[i][j].dir = D;
    matrix[i][j].score=maxI(maxI(A,B),C);
if(!MPIrec)
	printf("i:%d j:%d id:%d horz:%d diag:%d arriba:%d res:%d\n",i,j,id,matrix[i-1][j],matrix[i-1][j-1], matrix[i][j-1].score,matrix[i][j].score);
else
	printf("i:%d j:%d id:%d horz:%d diag:%d arriba:%d res:%d\n",i,j,id,MPIData1,MPIData2, matrix[i][j-1].score,matrix[i][j].score);
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
    //Poda, impide repetici�n de celdas si ya ha calculado el camino
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
		    //En las diagonales se a�ade distancia si existen
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


void print(struct Celda** matrix,char* titulo)
{
int x;
int y;
	printf("%s",titulo);
	printf("\n");
        for(x=0;x<=7;x++) {
        for(y=0;y<=7;y++)
            printf("%d ", matrix[x][y].score);
         printf("\n");
        }
        printf("\n");

	printf("\n");
        for(x=0;x<=7;x++) {
        for(y=0;y<=7;y++)
            printf("%d ", matrix[x][y].dir);
         printf("\n");
        }
        printf("\n");

}