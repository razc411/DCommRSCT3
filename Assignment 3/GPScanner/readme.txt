DCGPS - A gpsd interfacing program - Written 2013/November
	
	included files - dcgps.cpp, gps-utils.cpp, gpsprint.cpp, top.h this readme and a documentation pdf

	This is a simple program with an ncurses interface for the purpose of interfacing with a gps dongle.
It takes gps data from the dongle and uses ncurses to display the satillite/user location data with regards 
to the current location of the dongle.

To compile this program, call the make file. Must have the gpsd dev lib, g++ and ncurses installed.

To use this program, install gpsd and start the daemon. After plugging in the dongle
you may have to use the following commands if the dongle is unresponsive.
	
	killall gpsd
	gpsd /dev/ttyUSB0 -F /var/run/gpsd.sock

Once this has been done, call dcgps with ./dcgps. The program is now good to go. Hit q to exit at any time.
**Program will not start unless a dongle is attached and working

A project by Ramzi Chennafi and Tim Kim from the CST - DComm option.