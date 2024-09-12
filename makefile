cymeteo: meteo.o main.o
	gcc meteo.o main.o -o cymeteo -Wall -lm
meteo.o: meteo.c meteo.h
	gcc -c meteo.c -o meteo.o -Wall
main.o: main.c meteo.h
	gcc -c main.c -o main.o -Wall
clean:
	rm -f *.o
