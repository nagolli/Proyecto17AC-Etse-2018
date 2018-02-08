#include <stdio.h>
#include <stdlib.h>
 

void Mayus(char * temp) {
  char * name;
  name = strtok(temp,":");
  char *s = name;
  while (*s) {
    *s = toupper((unsigned char) *s);
    s++;
  }}

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
 
int main()
{
    char* texto=CargarFichero("chr22_random.fa");
 	printf("%s\n",texto);
 	printf("%d\n",strlen(texto));
 	return 0;
}
