#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_BUFFER 200
#define MAX_STRING 100
/*El programa comienza solicitando que se ingrese por teclado una cantidad de personas (N), continua pidiendo hasta
que la cantidad de personas ingresadas sea mayor a 0 y menor o igual al total de personas en el archivo de entrada.

Luego, genera N numeros aleatorios sin repeir que representaran el numero de la linea de la persona a leer,
 y luego escribir en el archivo de salida, y los almacena en un arreglo.

Despues, arma un arreglo de strings con las localidades del archivo codigoLocalidades donde la posicion 
en la que se almacenan representa el codigo-1 de la respectiva localidad.

Finalmente, comienza a leer las personas que correspenden segun el numero aleatorio, almacena sus
datos en variables temporales y luego los escribe en el archivo de salida como corresponde.*/

int cant_lineas (char *nombre) {
    FILE *archivo;
    archivo = fopen(nombre,"r");
    int lineas = 0;
    for (char c = fgetc(archivo); c != EOF; c = fgetc(archivo)) {
        if (c == '\n')
            lineas++;
    }
    fclose(archivo);
    return lineas;
}

void swap (int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int comparar (const void *a, const void *b) {
    int n = *(int*)a, m = *(int*)b;
    return n - m;
}

int* crear_array_random(int cantPersonas, int totalPersonas) {
    int *arrayIndex = malloc(sizeof(int)*totalPersonas);
    int *numsRandom = malloc(sizeof(int)*cantPersonas);
    int nRandom, ultimo = totalPersonas;
    
    for(int i = 0; i < totalPersonas; ++i)
        arrayIndex[i] = i;

    srand((unsigned int)time(NULL));
    for(int i = 0; i < cantPersonas; ++i){
        nRandom = rand() % ultimo;
        numsRandom[i] = arrayIndex[nRandom];
        swap(&arrayIndex[nRandom], &arrayIndex[ultimo-1]);
        ultimo--;
    }
    qsort(numsRandom, cantPersonas, sizeof(int), comparar);
    free(arrayIndex);

    return numsRandom;
}

void eliminar_espacios(char *array) {
    int i = 0;
    while (array[i]!='\0'){
      i++;
    }
    while(array[i]!=' '){
      i--;
    }
    while(array[i]==' '){
      i--;
    }
    array[i+1]='\0';
}

char** crear_array_localidades(char *fLocalidades, int cantLocalidades) {
    FILE *fileLocalidades;
    fileLocalidades = fopen(fLocalidades, "r");
    char **localidades = malloc (sizeof(char*)*cantLocalidades);
    char basura[MAX_STRING], buffer[MAX_BUFFER];
    for (int i = 0; i < cantLocalidades; ++i){
        fscanf(fileLocalidades,"%[^,],%[^\n]",basura, buffer);
        eliminar_espacios(buffer);
        localidades[i] = malloc(sizeof(char)*(strlen(buffer)+1));
        strcpy(localidades[i], buffer);
    }

    fclose(fileLocalidades);
    return localidades;
}

void liberar_memoria(char **array, int largo) {
    for(int i = 0; i < largo; ++i)
        free(array[i]);
}

char genero (int codigo) {
    char caracter;
    switch (codigo) {
    case 1:
        caracter = 'M';
        break;
    case 2:
        caracter = 'F';
        break; 
    default:
        break;
    }
    return caracter;
}

char interes (int codigo) {
    char caracter;
    switch (codigo) {
    case 1:
        caracter = 'F';
        break;
    case 2:
        caracter = 'M';
        break;
    case 3:
        caracter = 'A';
        break;
    case 4:
        caracter = 'N';
        break; 
    default:
        break;
    }
    return caracter;
}

void generar_personas(int cantPersonas, int totalPersonas, int cantLocalidades, char *fEntrada, char *fLocalidades, char *fSalida){
    FILE *filePersonas,*fileSalida;
    char buffer[MAX_BUFFER], nombre[MAX_STRING], apellido[MAX_STRING];
    int codigo, sexo, sexualidad, edad;

    int *arrayRandom = crear_array_random(cantPersonas, totalPersonas);
    char **localidades = crear_array_localidades(fLocalidades, cantLocalidades);

    filePersonas =fopen(fEntrada,"r");
    fileSalida =fopen(fSalida,"w");
    int cantElegidas = 0;
    for(int linea = 0; linea < totalPersonas && cantElegidas < cantPersonas; linea++){
        if (linea == arrayRandom[cantElegidas]) {
            fscanf(filePersonas,"%[^,],%[^,],%d,%d,%d,%d\n", nombre, apellido, &codigo, &edad, &sexo, &sexualidad);
            fprintf(fileSalida,"%s, %s, %s, %d, %c, %c\n", nombre, apellido, localidades[codigo-1], edad, genero(sexo), interes(sexualidad));
            cantElegidas++;
        }
        else fscanf(filePersonas, "%[^\n]\n", buffer);
    }
    fclose(filePersonas);
    fclose(fileSalida);
    liberar_memoria(localidades,cantLocalidades);
    free(localidades);
    free(arrayRandom);
}

int main(int argc, char **argv){
    if (argc != 4) {
        printf("No se ingreso la cantidad correcta de argumentos\n");
        return -1;
    }
    char *entrada = argv[1], *localidades = argv[2], *salida = argv[3];
    int cantPersonas=0, total=0;
    total = cant_lineas(entrada);
    printf("Ingrese la cantidad de personas: ");
    scanf("%d",&cantPersonas);
    while ((cantPersonas > total) || (cantPersonas < 0)){
        printf("Por favor ingrese una cantidad menor a %d y mayor a 0: ", total);
        scanf("%d",&cantPersonas);
    }
    int cantLocalidades = cant_lineas (localidades);
    generar_personas(cantPersonas, total, cantLocalidades, entrada, localidades, salida);
    return 0;
}
