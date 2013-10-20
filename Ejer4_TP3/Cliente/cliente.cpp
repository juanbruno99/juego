#include <string>
#include <sstream>
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
#include <netdb.h>
#include <ctype.h>

#define TAMBUF 1024
#define FIL 10
#define COL 10

//cosas nuevas a reubicar
bool fin_partida = false;
char matriz[FIL][COL]; //Matriz de pantalla de cliente

//Prototipos de funciones
void establecerConexionConElServidor(char *params[]);
void cierreAnormal();
int validarMovimiento(char movimientos[]);
void bufferAMatriz(char mat[FIL][COL], char buffer[]);
void printMatriz(char matriz[FIL][COL]); 

unsigned short int comm_port = 0;
unsigned long int comm_ip_address = 0;
struct sockaddr_in comm_address;
// Estructura del host, para utilizar hostname
struct hostent *data_host;
int caller_socket = 0;
char buffer[TAMBUF];

//Main
int main(int argc, char *argv[]) {

//VALIDAR IP Y PUERTO RECIBIDO

//conexion con el server
establecerConexionConElServidor(argv);

//Espero el mensaje de me conexion exitosa con el servidor - Recibe Matriz inicial de juego de server
recv( caller_socket , buffer, TAMBUF ,0); //Y la matriz inicial
//printf("\nMensaje de Server:%s\n", buffer);

//vector que va a guardar las coordenadas ingresadas por el usuario
 char movimientos[25];
  int i;
 
//Recibe y convierte buffer (protoclo), Imprime Matriz de juego recibida al iniciar la partida
bufferAMatriz(matriz, buffer);
printf("MATRIZ DE JUEGO:\n");
printMatriz(matriz);

while(!fin_partida){
	printf("Ingrese las coordenadas de las fichas a develar con el siguiente formato:\n");
	printf("X1 Y1 X2 Y2\n");
	gets(movimientos);
	//VALIDACION DE LAS COORDENADAS INGRESADAS POR EL USUARIO
	while(!validarMovimiento(movimientos)){
		fflush(stdin);
		printf("Formato ingresado incorrecto, reingrese\n"); 
		printf("X1 Y1 X2 Y2\n");
		gets(movimientos);	
	}
	//strcpy(buffer,"\0");
	//strncpy( buffer ,movimientos , TAMBUF);
	//if(send( caller_socket , buffer , TAMBUF , 0) < 0){
	//	printf("\nNo se pudo enviar el mensaje.\n");
	//}

	/*strcpy(buffer,"\0");
	//recv( caller_socket , buffer, TAMBUF ,0);
	printf("resultado comparacion: %d \n",strcmp( buffer, "fin" ));
	printf("mensaje buffeu: %s \n", buffer);
	if(strcmp( buffer, "fin" ) == 0)
		fin_partida = true;
	printf("\nMensaje de Server:%s\n", buffer);*/
	sleep(10); //PARA PROBAR
	fin_partida = true;
	
}

exit(0);
}

//Implementacion de Funciones

void cierreAnormal()
{
	printf("Cierre Anormal\n");
	close( caller_socket);
	exit(1);
}


void establecerConexionConElServidor(char *params[])
{
	//Trato de obtener la IP segun el hostname que esta en el config_cliente.ini
	data_host=gethostbyname(params[1]);
	comm_ip_address = inet_addr(params[1]);

	//Creamos el socket de comunicación
	caller_socket = socket(AF_INET , SOCK_STREAM , 0);

	//Asignamos una dirección IP
	comm_address.sin_family = AF_INET;
	comm_port = htons(atoi(params[2]));
	comm_address.sin_port = comm_port;

	comm_address.sin_addr.s_addr = comm_ip_address;

	bzero(&(comm_address.sin_zero), 8);

	//Nos conectamos al servidor
	if( connect( caller_socket , (struct sockaddr*) &comm_address , sizeof(struct sockaddr)) < 0 ){
		fprintf( stderr , "No se pudo conectar al servidor.\n");
		cierreAnormal();
	}
}

int validarMovimiento(char movimientos[]) {
int ok=1;

	//Cantidad de entradas incorrecta, deben ser 7 incluyendo espacios
	if(strlen(movimientos)>7) {
	ok=0;
	}
	//Caracteres de coordenadas deben ser numericos y del 0 al 9, funcion isdigit C++
	if(!(isdigit(movimientos[0]) && isdigit(movimientos[2]) && isdigit(movimientos[4]) && isdigit(movimientos[6])) ) {
	ok=0;
	}
	
return ok;
}

void bufferAMatriz(char mat[FIL][COL], char buffer[]) {
int i, j, k=0;

	for(i=0;i<FIL;i++) {
		for(j=0;j<COL;j++) {
			mat[i][j]=buffer[k];
			k++;		
		}
	}
}


void printMatriz(char matriz[FIL][COL]) {
int i, j;

	for(i=0;i<FIL;i++) {
		for(j=0;j<COL;j++) {
			printf("  %d", matriz[i][j]);		
		}
		printf("\n");
	}	
	
}
