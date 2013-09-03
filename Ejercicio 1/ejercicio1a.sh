#!/bin/bash

#TRABAJO PRACTICO N1
#EJERCICIO 1.A

#INTEGRANTES DEL GRUPO
#Paola Servis 
#Luis Escobar 
#Sergio Defusto
#Juan Marcos Bruno
#Sharon Denise Pesca Alba

#OBJETIVO
#El script determina si un fichero es un archivo o directorio

#PARAMETROS
#Recibe un parametro solo. El script valida si es un archivo o un directorio.

#SINTAXIS: ./ejercicio1a.sh fichero


####################-------------------------------##########################



### FUNCIONES

Error(){
	echo "error, sintaxis de uso: $0 archivo | directorio "
}
#La función Error  muestra un mensaje demostrando la correcta sintáxis de uso del
#script



### PROGRAMA PRINCIPAL


if test $# -lt 1  #Verifica que haya por lo menos un parámetro
then
	Error
elif test -d $1 #Verifica si el parámetro existe y es un directorio
then
	echo "$1 es UN DIRECTORIO"
elif test -f $1 #Verifica si el parámetro existe y es un archio
then
	echo "$1 es UN ARCHIVO"
else
	echo "$1 es un archivo especial o no existe"
fi
