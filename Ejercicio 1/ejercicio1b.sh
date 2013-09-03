#!/bin/bash

#TRABAJO PRACTICO N1
#EJERCICIO 1.B

#INTEGRANTES DEL GRUPO
#Paola Servis 
#Luis Escobar 
#Sergio Defusto
#Juan Marcos Bruno
#Sharon Denise Pesca Alba

#OBJETIVO
#El script determina la cantidad de lineas o caractéres de un archivo

#PARAMETROS
#El script debe recibir 2 parámetros. El primero debe ser un archivo
#y el segundo debe ser el caracter L o C para indicar que se quiere contar

#SINTAXIS:
#./ejercicio1b nombre_archivo L|C
#L: Con éste parámetro el script cuenta líneas.
#C: Con este parámetro el script cuenta caractéres.


####################-------------------------------##########################


### FUNCIONES

Error()
{	echo "Error. La sintaxis del script es la siguiente:"
	echo "Para informar: $0 nombre_archivo L"
	echo "Para informar: $0 nombre_archivo C"
}
#La función anterior muestra la correcta forma de invocar al script.


### PROGRAMA PRINCIPAL


if test $# -lt 2 #Verifica que haya 2 o más parámetros para seguir.
then
	Error
elif test -f $1 && (test $2 = "L" || test $2 = "C") #Verifica si el parámetro 1 existe y es un archivo, además verifica si el parametro 2 es el caracter L o C.
then
	if test $2 = "L"
	then
		wc  -l $1 #Cuenta la cantidad de lineas del archivo
	elif test $2 = "C" 
	then
		wc -m $1 #Cuenta la cantidad de caracteres
	fi
else
	Error
fi
