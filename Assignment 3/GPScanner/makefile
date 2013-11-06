
CC=g++ -Wall -W -pedantic -cstd=c++11 
CLIB=-lgps

dcgps: dcgps.o gps-utils.o gpsprint.o
	$(CC) -o dcgps dcgps.o gps-utils.o gpsprint.o $(CLIB)

clean:
	rm -f *.o core.* dcgps
dcgps.o: dcgps.cpp
	$(CC) -c dcgps.cpp
gpsutils.o: gpsutils.cpp
	$(CC) -c gps-utils.cpp
gpsprint.o: gpsprint.cpp
	$(CC) -c gpsprint.cpp