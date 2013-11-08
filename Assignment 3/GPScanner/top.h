#ifndef TOP_H
#define TOP_H
#include <iostream>
#include <gps.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#define MAXCHANNELS 72
#define MAX_POSSIBLE_SATS (MAXCHANNELS - 2)
using namespace std;

struct fixsource_t
{
    char *spec;
    char *server;
    char *port;
    char *device;
};

void freeData(gps_data_t*);
int openStream(gps_data_t*);
void read_data(gps_data_t*);
void print_data(gps_data_t*);


#endif // TOP_H
