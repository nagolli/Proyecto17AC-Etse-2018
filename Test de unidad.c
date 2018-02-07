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

int main()
{ 
    char* string1=CargarFichero("UT1.fa");
    char* string2=CargarFichero("UT2.fa");
    struct Celda **Matriz;
    Matriz=InicioMatriz(strlen(string1),strlen(string2));
    CompletarMatriz(string1,string2,Matriz);
    int resultado= GetRuta(Matriz);
    
    return resultado /*== resultadoNumerico estandar*/;
}
