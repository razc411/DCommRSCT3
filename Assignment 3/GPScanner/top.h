#ifndef TOP_H
#define TOP_H
#include <iostream>
#include "gps-utils.cpp"
#include "gpsrint.cpp"
#include "main.cpp"
#include <QTextStream>
#include <gps.h>
#include <pthread.h>
#include <stdlib.h>
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

#endif // TOP_H
