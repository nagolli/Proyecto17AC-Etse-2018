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
char* CargarFichero(char*);
struct Celda** InicioMatriz(unsigned,unsigned);
void CompletarMatriz(char*,char*,struct Celda**);
void CalcularCasilla(unsigned, unsigned, bool, struct Celda**);  
int GetRuta(struct Celda**);

int main( int argc, char *argv[] ) 
{ 
    if(argc == 2)
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
    }
    else
    {
        printf("Requiere 2 argumentos\n");
        exit(3);
    }
    return 0;
}

//FILL ME!

char* CargarFichero(char* FillMe)
{
    return "Feed me";
}
struct Celda** InicioMatriz(unsigned FillMe0,unsigned FillMe1)
{
    return NULL;
}
void CompletarMatriz(char* FillMe0 ,char* FillMe1,struct Celda** FillM2)
{
    
}

int GetRuta(struct Celda** FillMe)
{
    return 0;
}
