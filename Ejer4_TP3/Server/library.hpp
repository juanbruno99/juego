#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <list>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <queue>
#include <iomanip>
#include <sstream>
#include <list>
#include <dirent.h>
#include <sys/stat.h>

#define TAM 100
#define TAMBUF  1024 //10000
#define MAXQ 10
#define CANT_VALORES 50

//Variables Externas
bool acepto_conexiones = true;
int listen_socket = 0;
extern 	pthread_t hiloEscucha;
pthread_mutex_t mutex_acepto_conexiones = PTHREAD_MUTEX_INITIALIZER;


//cosas nuevas a reubicar
using namespace std;
void inicializarJugador(int id , int numSocket);
void* partidaCliente(void* socket);
void inicializarEstructuras();
//Estructura del Protocolo
/*typedef struct {
	short int cord1;
	short int cord2;
	short int color;
} t_protocolo;*/

typedef struct {
	//t_protocolo protocolo;
	int socket;
}t_tListenParams;

typedef struct{
	int clientes[TAM]; //Nº de Socket.
	bool comenzoJugada[TAM]; //Si el jugador está jugando (false) y si no (true).
	int cantidadConectados; //Cantidad de clientes conectados en el momento.
}t_info;

t_info  info;
char buffer[TAMBUF];
vector<pthread_t> threadVec;

//Prototipos de funciones
void* aceptarConexiones(void * puerto);
void inicializarMatrizPartida(int matriz[10][10], unsigned short int filas, unsigned short int columnas);
void printMatriz(int matriz[10][10], int filas, int columnas);
void desordenar_matriz(int matriz[10][10], size_t filas, size_t columnas);


//Funciones Implementacion

//Hilo que se encarga de Aceptar Conexiones
void* aceptarConexiones(void * puerto){

	char *puertoEscucha = (char*) puerto;
	printf("\n Puerto: %s \n", puertoEscucha);

	//Creamos el Socket de Escucha
	int comm_socket = 0;
	unsigned short int listen_port = htons(atoi(puertoEscucha));
	unsigned long int listen_ip_address = 0;
	struct sockaddr_in listen_address, con_address;
	socklen_t con_addr_len;
	int optval = 1 , i = 0;

	listen_socket = socket(AF_INET,SOCK_STREAM,0);

	if(listen_socket < 0){
		fprintf(stderr , "Error al crear el Socket de Escucha.\n");
		exit(0);
	}

	setsockopt(listen_socket , SOL_SOCKET , SO_REUSEADDR , &optval, sizeof(optval));

	//Asigno dirección y configuracion.puerto_escucha
	bzero(&listen_address , sizeof(listen_address));
	listen_address.sin_family = AF_INET;
	listen_address.sin_port = listen_port;
	listen_ip_address = htonl(INADDR_ANY);
	printf("\nIP de escucha server:%ld\n", listen_ip_address);
	listen_address.sin_addr.s_addr = listen_ip_address;

	bzero(&(listen_address.sin_zero),8);

      	if((bind(listen_socket,(struct sockaddr *)&listen_address,sizeof(struct sockaddr)))<0){
		fprintf(stderr , "Error al asignar la Dirección IP.\n");
		exit(0);
    	}

	//Comenzamos a escuchar conexiones
	if( (listen(listen_socket,MAXQ)) < 0 ){
		fprintf(stderr , "Error al escuchar conexiones.\n");
		exit(0);
	}

	bzero(&con_address,sizeof(con_address));
	con_addr_len = sizeof(struct sockaddr_in);

	//Aceptamos conexiones
	while( 1 ){
		usleep(50000);
		comm_socket = accept(listen_socket,(struct sockaddr *)&con_address, &con_addr_len);

		pthread_mutex_lock( &mutex_acepto_conexiones );

		if( acepto_conexiones ){
			//Le enviamos esto al Cliente
			strncpy( buffer , "Te has conectado al servidor" , TAMBUF);
			inicializarJugador(i , comm_socket); //guardamos los datos del jugador
			printf("Partida asignada al jugador: %d (%d).\n", i , info.clientes[i]);
			pthread_t threadId;
			pthread_create(&threadId, NULL, &partidaCliente, (void *)info.clientes[i]);  	
			threadVec.push_back(threadId); 
		}
		else {
			strncpy( buffer , "No se aceptan más conexiones" , TAMBUF);
		}
		send( comm_socket , buffer, TAMBUF ,0);
		pthread_mutex_unlock( &mutex_acepto_conexiones );

		i++;

	} //fin while
	pthread_exit(NULL);
}

//Inicializamos los datos del Cliente que acaba de conectarse
void inicializarJugador(int id , int numSocket){
	info.clientes[id] = numSocket;
	info.comenzoJugada[id] = false;
	info.cantidadConectados++;
}

void inicializarMatrizPartida(int matriz[10][10], unsigned short int filas, unsigned short int columnas) {
short int i,j, val1,val2,aux1,aux2;
int count=0;

	srand(time(NULL));
		
	for(i=0;i<10;i++) {
		for(j=0;j<10;j++) {
			matriz[i][j]=count;
			count++;	
		}
		
		if(i==4)
			count=0;
	}

	desordenar_matriz(matriz,10,10);
}

void printMatriz(int matriz[10][10], int filas, int columnas) {
int i, j;

	for(i=0;i<10;i++) {
		for(j=0;j<10;j++) {
			printf("  %d", matriz[i][j]);		
		}
		printf("\n");
	}	
	
}

void desordenar_matriz(int matriz[10][10], size_t filas, size_t columnas) {
    if (filas > 1 && columnas > 1){
        size_t i,j;
	for (i = 0; i < filas - 1; i++){

		for(j = 0; j < columnas -1; j++){
			 size_t a = i + rand() / (RAND_MAX / (filas - i) + 1);
			 size_t b = j + rand() / (RAND_MAX / (columnas - j) + 1);
			 int t = matriz[a][b];
			 matriz[a][b]= matriz[i][j];
			 matriz[i][j] = t;
		}
        }
    }
}

void* partidaCliente(void* socket){
//escucho en los sockets de jugadores
//t_tListenParams* params = (t_tListenParams*) tListenParams;
	int comm_socket = (int) socket;


	int i,j;

	while(1) {
		usleep(500000);

		strcpy(buffer,"\0");
		recv( comm_socket , buffer, TAMBUF, 0); 
		if(strcmp(buffer,"\0")!= 0){
			printf("\nMensaje de cliente %d: %s\n",comm_socket, buffer);
		}
		//VER COMO HAGO CUANDO EL CLIENTE TERMINO DE JUGAR YA QUE TENGO QUE SALIR DE ESTE WHILE

		/*strcpy(buffer,"\0");
		printf("MENSAJE DSP DE BORRADO: %s M \n",buffer);
		//sends y receives del juego
		strcpy( buffer , "pruebaenviocliente\0");// , TAMBUF);
		send( comm_socket , buffer, TAMBUF ,0);
		//printf("tam_recibido %u\nbuf%p\n", tam_recibido, buffer); 
		strcpy(buffer,"\0");
		printf("MENSAJE DSP DE BORRADO: %s M \n",buffer);
		strcpy( buffer , "fin"); // , strlen("fin"));
		send( comm_socket , "fin", strlen("fin"),0);
		
		//printf("tam_recibido %u\nbuf%p\n", tam_recibido, buffer); 
		
		//sleep(1);
		
		/*if(recv( socket ,buffer, TAMBUF ,0) == -1){
			printf("Error recibiendo mensajes del cliente\n");
		}*/

			
	}//fin while(1)
	
}


//Inicializa la estructura t_info
void inicializarEstructuras(){
	int i;

	for( i = 0 ; i < TAM ; i++){
		info.clientes[i] = -1;
		info.comenzoJugada[i] = false;
	}

	info.cantidadConectados = 0;
}
