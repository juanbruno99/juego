#!/bin/bash

#Ejercicio 4 TP 1 - Sistemas Operativos
#Fecha de Creación: 21/08/2013

#INTEGRANTES DEL GRUPO
#Paola Servis 
#Luis Escobar 
#Sergio Defusto
#Juan Marcos Bruno
#Sharon Denise Pesca Alba

#OBJETIVO
#Transformar un archivo de entrada con datos de clientes en un archivo de salida donde las fechas modifiquen su formato y se agreguen en caso de corresponder nuevos vencimientos con un agregado de 15 días sobre los vencimientos iniciales.

#SINTAXIS: ./clientes.sh [archivo_a_procesar] [ruta_de_archivo_salida]

####################-------------------------------##########################

 #Funciones de Script

#Funciones de Validaciones
function validarParametros() {
#Chequea si el primer parametro es llamada a ayuda
if [ "$1" = "-?" ]; then
	msg="Se invoco ayuda de script";
	return 1;
#chequea cantidad de parametros pasados
elif [ "$#" -ne 2 ]; then
	msg="ERROR: Cantidad de Parametros incorrectos"
	return 1;	
#chequea que parametro 1 sea archivo	
elif  [ ! -f "$1" ]; then
	msg="ERROR: Parametro pasado $1 no es un archivo"
	return 1;
#chequea que no este vacio el archivo
elif [ ! -s "$1" ]; then
	msg="ERROR: $1 esta vacio o no existe ."
	return 1;
#chequea que parametro 2 sea directorio
elif [ ! -d "$2" ]; then
	msg="ERROR: $2 no es un directorio"
	return 1;
fi
}

function usage() {
	echo "#########################################################"
	echo "# USO DEL SCRIPT:                                       #"
	echo "# Ejercicio4 param1 param2                              #"
	echo "# param1 > archivo a procesar                           #"
	echo "# param2 > directorio donde se guarda archivo resultado #"
	echo "# 						      #"
	echo "# Observaciones: 					      #"
	echo "# 						      #"	
	echo "#	° Si algún cliente tiene una fecha inválida o alguna  #"
	echo "#	fila contiene mas de dos campos el mismo no será      #"
	echo "#	procesado y el cliente sera considerado inválido      #"				
	echo "# 						      #"
	echo "# ° Si alguna fila del archivo no tiene el formato de   #"
	echo "# datos del cliente no sera procesada                   #"
	echo "#########################################################"
}

#Parte principal de Script
validarParametros $*

if [ "$?" -eq 1 ]; then
	echo "$msg"
	usage	
elif test -s $1; then

	#SE PROCESA EL ARCHIVO
	cat $1 | awk '
	
	#FUNCIONES
	function esFechaValida(DATO){ #VALIDA FECHAS. FORMATOS ACEPTADOS: MM/DD/AAAA 
		if (DATO !~ /(^[ ]?0[1-9]|1[012])\/(0[1-9]|[12][0-9]|3[01])\/(19|20)[0-9][0-9]/) {
			return 0;
		}
		else
			return 1;
	}

	function cambiarFormatoFecha(DATO){ #RECIBE UNA FECHA CON FORMATO  

		split(DATO,arrayFecha,"/");
		split(arrayFecha[1],arrayMes," ");
		fecha_modif= arrayFecha[2] "/" arrayMes[1] "/" arrayFecha[3];
		return fecha_modif;
	}

	function calculoNuevoVencimiento(fecha_venc){ #SUMA A UNA FECHA 15 DIAS

		#Inicializo Vector con dias por mes	
		mesesDias[1]=31; mesesDias[2]=28; mesesDias[3]=31; mesesDias[4]=30; mesesDias[5]=31; mesesDias[6]=30; mesesDias[7]=31; 
		mesesDias[8]=31; mesesDias[9]=30; mesesDias[10]=31; mesesDias[11]=30; mesesDias[12]=31;
	
		#Sumo dias a fecha original
		split(fecha_venc,fecha,"/");
		dias=fecha[2]+15;mes=fecha[1]+0;anio=fecha[3]

		if(dias>mesesDias[mes]) {
		dias=dias-mesesDias[mes]
		mes++
			if(mes>12) {
			mes=1
			anio=anio+1
			}
		}

		if(length(mes)==1) {
			mes = "0" mes;
		}

		if(length(dias)==1) {
			dias = "0" dias;
		}
	
		return dias "/" mes "/" anio; 

	}
	
	function datosExisten(arrayCliente){ #valida que los campos nro_cliente, fecha_compra y fecha_vencimiento esten completos
		if(length(arrayCliente[1]) < 1 || length(arrayCliente[2]) < 1 || length(arrayCliente[3]) < 1)
			return 0;
		return 1;
	
	}

	function esClienteValido(DATO){#valida que el cliente sea numérico
		
		split(DATO,array,ORS);
		if(array[1] !~ /^[0-9]+/ && array[1] != "" && array[1] !~ /^[ ]+/)
			return 0;

		else 
			return 1; 

	}

	#COMIENZO DEL PROCESO DE ARCHIVO
	BEGIN{

		#SETEO EL SEPARADOR DE CAMPOS	
		FS=":";
		cliente_valido = 1;

	};


	{
		#TRABAJO LOS REGISTROS DE NUMERO DE CLIENTE
		if( $1 ~ /liente/ ) {
			cliente_valido = 1;
			if(NF==2 && esClienteValido($2))
				arrayCliente[1]= $0; 
			else
				cliente_valido = 0;
		}
	
		#TRABAJO LOS REGISTROS DE FECHA DE COMPRA
		if( $1 ~ /echa/ ){
			if(esFechaValida($2) && NF==2) {#si la fecha es válida y el cliente es válido guardo la linea
				arrayCliente[2]= $1 ": " cambiarFormatoFecha($2);
			}else #si no es correcta el cliente es inválido
				cliente_valido = 0;
		

		}
	
		#TRABAJO LOS REGISTROS DE VENCIMIENTO
		if( $1 ~ /encimiento/ ){   
			if(esFechaValida($2) && NF==2 && cliente_valido) { #si la fecha de vencimiento es válida y el cliente es válido guardo la linea
			
				arrayCliente[3]= $1 ": " cambiarFormatoFecha($2);
				#GUARDO LA FECHA POR SI TENGO QUE CALCULAR UN NUEVO VENCIMIENTO	
				fecha_venc = $2; 
			}else{
				cliente_valido = 0;
			}
		}
	
		#TRABAJO LOS REGISTROS DE ESTADO
		if($1 ~ /stado/ ){
			if(cliente_valido && datosExisten(arrayCliente) && NF==2 && ($2 ~ /[Pp][eE][nN][dD][iI][eE][nN][Tt][eE]/ || $2 ~ /[Pp][aA][gG][aA][dD][oO]/)){
				#SI EL CLIENTE ES VALIDO IMPRIMO TODOS LOS DATOS
				print arrayCliente[1];
				print arrayCliente[2];
				print arrayCliente[3];
				#SI EL ESTADO ES PENDIENTE AGREGO EL NUEVO VENCIMIENTO Y LUEGO EL ESTADO
				if($2 ~ /[Pp][eE][nN][dD][iI][eE][nN][Tt][eE]/) {
					print "Segundo Vencimiento: " calculoNuevoVencimiento(fecha_venc);
					print $0 "\n"; 
		 		}
				else { #SINO AGREGO SOLO EL ESTADO
					print $0 "\n"; 
				}
			}else{
				cliente_valido = 1; #limpio la variable para analizar proximo cliente
				arrayCliente[1]="";
				arrayCliente[2]="";
				arrayCliente[3]="";
			}
		
		}

	};

	END{ 
		

	}; ' > clientes.out
	mv clientes.out $HOME; 
	mv clientes.out $2; 
	clear		
	echo "El archivo de salida clientes.out se ha generado en la siguiente ruta: " $2;
	echo
	

fi







