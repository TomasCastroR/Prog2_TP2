#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
/*El programa comienza solicitando que se ingrese por teclado una cantidad de personas (N), continua pidiendo hasta
que la cantidad de personas ingresadas sea mayor a 0 y menor o igual al total de personas en el archivo de entrada.

Luego, genera N numeros aleatorios sin repeir que representaran el numero de la linea de la persona a leer,
 y luego escribir en el archivo de salida, y los almacena en un arreglo.

Despues, arma un arreglo de strings con las localidades del archivo codigoLocalidades donde la posicion 
en la que se almacenan representa el codigo-1 de la respectiva localidad.

Finalmente, comienza a leer las personas que correspenden segun el numero aleatorio, almacena sus
datos en variables temporales y luego los escribe en el archivo de salida como corresponde.*/

int cant_lineas(FILE *archivo){
    int lineas = 0;
    for (char c = fgetc(archivo); c != EOF; c = fgetc(archivo)) {
        if (c == '\n')
            lineas++;
    }
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

int* crear_array_random(int cantPersonas, int totalPersonas){
    int *arrayIndex = malloc(sizeof(int)*totalPersonas);
    int *numsRandom = malloc(sizeof(int)*cantPersonas);
    int nRandom, ultimo = totalPersonas;
    for(int i = 0; i < totalPersonas; ++i)
        arrayIndex[i] = i;

    srand((unsigned int)time(NULL));
    for(int i = 0; i < cantPersonas; ++i){
        nRandom = rand() % (totalPersonas);
        numsRandom[i] = arrayIndex[nRandom];
        swap(&arrayIndex[nRandom], &arrayIndex[ultimo-1]);
        ultimo--;
    }
    qsort(numsRandom, cantPersonas, sizeof(int), comparar);
    free(arrayIndex);

    return numsRandom;
}

void eliminar_espacios(char *array){
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

char** crear_array_localidades(char *fLocalidades) {
    FILE *fileLocalidades;
    fileLocalidades = fopen(fLocalidades, "r");
    int cantLocalidades = cant_lineas(fileLocalidades);
    rewind(fileLocalidades);
    char **localidades = malloc (sizeof(char*)*cantLocalidades);
    char basura[50], buffer[200];
    for (int i = 0; i < cantLocalidades; ++i){
        fscanf(fileLocalidades,"%[^,],%[^\n]",basura, buffer);
        eliminar_espacios(buffer);
        localidades[i] = malloc(sizeof(char)*(strlen(buffer)+1));
        strcpy(localidades[i], buffer);
    }

    fclose(fileLocalidades);
    return localidades;
}

void liberar_memoria(char **array,int largo){
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

void generar_personas(int cantPersonas, int totalPersonas, char *fEntrada, char *fLocalidades, char *fSalida){
    FILE *filePersonas,*fileSalida;
    char buffer[200], nombre[100], apellido[100];
    int linea = 0, codigo, sexo, sexualidad, edad, cantLineas;

    int *arrayRandom = crear_array_random(cantPersonas, totalPersonas);
    char **localidades = crear_array_localidades(fLocalidades);

    filePersonas =fopen(fEntrada,"r");
    fileSalida =fopen(fSalida,"w");
    for(int i = 0; i< cantPersonas; i++){
        while(linea < arrayRandom[i]){
            fgets(buffer, 200,filePersonas);
            linea++;
        }
        fscanf(filePersonas,"%[^,],%[^,],%d,%d,%d,%d", nombre, apellido, &codigo, &edad, &sexo, &sexualidad);
        fprintf(fileSalida,"%s, %s, %s, %d, %c, %c\n", nombre, apellido, localidades[codigo-1], edad, genero(sexo), interes(sexualidad));
    }
    fclose(filePersonas);
    fclose(fileSalida);
    liberar_memoria(localidades,cantLineas);
    free(localidades);
    free(arrayRandom);
}

int main(int argc, char **argv){
    if (argc != 4) {
        printf("No se ingreso la cantidad correcta de argumentos\n");
        return -1;
    }
    char *entrada = argv[1], *localidades = argv[2], *salida = argv[3];
    int cantPersonas=0, lineas=0;
    FILE *fEntrada;
    fEntrada = fopen(entrada,"r");
    lineas = cant_lineas(fEntrada);
    fclose(fEntrada);
    printf("Ingrese la cantidad de personas: ");
    scanf("%d",&cantPersonas);
    while ((cantPersonas > lineas) || (cantPersonas < 0)){
        printf("Por favor ingrese una cantidad menor a %d y mayor a 0: ", lineas);
        scanf("%d",&cantPersonas);
    }
    generar_personas(cantPersonas, lineas, entrada, localidades, salida);
    return 0;
}
