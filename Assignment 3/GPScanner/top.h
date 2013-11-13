#ifndef TOP_H
#define TOP_H
#include <iostream>
#include <ncurses.h>
#include <gps.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <ctime>
#define MAXCHANNELS 72
#define MAX_POSSIBLE_SATS (MAXCHANNELS - 2)
#define GPS_QUIT    0   //GPS device is turned off
#define GPS_GONE    -1  //GPS device went away
#define GPS_ERROR   -2  //GPS cannot be read
#define GPS_TIMEOUT -3  //GPS canot be found
using namespace std;

struct Win_Display{
	gps_data_t *gps_data;
	WINDOW *satWin;
	WINDOW *mWin;
	pthread_mutex_t lock;
};

struct fixsource_t
{
    char *spec;
    char *server;
    char *port;
    char *device;
};

WINDOW *create_newwin(int height, int width, int starty, int startx);
void winSetup(WINDOW *satWin, WINDOW *mWin);
int openStream(gps_data_t*);
void read_data(void *ptr);
void print_data(Win_Display*);
void cleanup(int, Win_Display*);

#endif // TOP_H
