servidor:	servidor.o 
			g++ -o servidor servidor.cpp library.hpp  -lpthread -lrt 

servidor.o:	servidor.cpp library.hpp
			g++ -c servidor.cpp

		
all:	servidor

clean:
		rm -f *.o
		rm -f servidor
