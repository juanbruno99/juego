#!/usr/bin/awk -f

#FUNCIONES
function esFechaValida(DATO,REGISTRO){ #VALIDA FECHAS. FORMATOS ACEPTADOS: MM/DD/AAAA 
	if(DATO !~ /^[0-3][0-9][\/-][0-1][0-9][\/-][0-9][0-9]$/ && DATO !~ /^[0-3][0-9][\/-][0-1][0-9][\/-][0-2][0-9][0-9][0-9]$/ &&
	DATO != "" && DATO !~ /^[ ]+$/){
		print " Fecha inválida en el registro número: " REGISTRO " Fecha informada: " DATO  "\n";
		return 0;
	}
	else
		return 1;
}

function cambiarFormatoFecha(DATO){ #RECIBE UNA FECHA CON FORMATO  

	split(DATO,arrayFecha,"/");
	split(arrayFecha[1],mes," ");
	fecha_modif= arrayFecha[2] "/" mes[1] "/" arrayFecha[3];
	return fecha_modif;
}

function calculoNuevoVencimiento(fecha_venc){ #SUMA A UNA FECHA 15 DIAS

	return "Falta funcion";

}

#COMIENZO DEL PROCESO DE ARCHIVO
BEGIN{

	#SETEO EL SEPARADOR DE CAMPOS	
	FS=":";
	
	
	#GENERO ARCHIVO PARA GUARDAR ERRORES	
	#log_errores="/tmp/log_errores"

	#system("touch " log_errores);

};


{
	#TRABAJO LOS REGISTROS DE NUMERO DE CLIENTE
	if( $1 ~ /liente/ ) {
		print  $0 "\n"; 
	}
	
	#TRABAJO LOS REGISTROS DE FECHA DE COMPRA
	if( $1 ~ /echa/ ){   #&& esFechaValida($2,NR)) {
		#fecha_compra =cambiarFormatoFecha($2); 
		print $1 ": " cambiarFormatoFecha($2) "\n";
	}
	
	#TRABAJO LOS REGISTROS DE VENCIMIENTO
	if( $1 ~ /encimiento/ ){   #&& esFechaValida($2,NR)) {
		#GUARDO LA FECHA POR SI TENGO QUE CALCULAR UN NUEVO VENCIMIENTO		
		fecha_venc = $2; 
		print $1 ": " cambiarFormatoFecha($2) "\n";
	}
	
	#TRABAJO LOS REGISTROS DE ESTADO
	if($1 ~ /stado/){
		
		#SI EL ESTADO ES PENDIENTE AGREGO EL NUEVO VENCIMIENTO Y LUEGO EL ESTADO
		if($2 ~ /[Pp][eE][nN][dD][iI][eE][nN][Tt][eE]/) {
			print "Segundo Vencimiento: " calculoNuevoVencimiento(fecha_venc)  "\n";
			print $0 "\n"; 
		}
		else { #SINO AGREGO SOLO EL ESTADO
			print $0 "\n"; 
		}
	}

};

END{ 
		

};
