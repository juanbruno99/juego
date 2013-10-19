#include "library.hpp"
#include <pthread.h>


//Variables Globales
extern bool acepto_conexiones;
int matriz[10][10];
extern t_info info; //inicializar los vectores

//Threads
using namespace std;
pthread_t hiloEscucha;
extern vector<pthread_t> threadVec;

//Main Bloque
int main(int argc, char *argv[]) {
	
	int i;
	//FALTAN VALIDACIONES DE PARAMETROS
	if(argc < 1){
		exit(EXIT_FAILURE);
	}
	
	//Inicializo estructura t_info
	inicializarEstructuras();

	//Creamos un hilo que se encargue de aceptar las conexiones
	if(pthread_create(&hiloEscucha,NULL , aceptarConexiones ,(void*) argv[1]) < 0){
	printf("error");	
	fprintf(stderr , "Error al crear el hilo que acepta conexiones.\n");
	exit(0);
   	}

	printf("Esperando conexiones . . . \n");

	//genero un hilo que trabaje con cada jugador
	while(true){usleep(500000);}
	
	printf("Esperando finalización de los hijos\n");
	pthread_join(hiloEscucha, NULL); //descomentar para esperar las conexiones de clientes
	for(int m = 0; m < info.cantidadConectados; m++){
		pthread_join(threadVec[m], NULL);
	}

//matriz para el memotest
//inicializarMatrizPartida(matriz, 10, 10);
//printMatriz(matriz, 10, 10);

exit(0);


}
