/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: gps-utils.cpp - An application that takes data from a gps dongle and posts 
-- location information as well as information regarding the visible satillites.
--
--
-- PROGRAM: dcgps
--
-- FUNCTIONS:
-- void read_data(void *ptr)
-- static void cleanup(int control, Win_Display * w_display)
--
--
-- DATE: November 8, 2013
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Tim Kim
--
-- PROGRAMMER: Tim Kim
--
-- NOTES:
-- This file contains the main GPS loop which continuously reads sensor data and send
-- it to the print_data function. It also has the cleanup function which closes stream,
-- disables WATCH and prints the correct error message.
----------------------------------------------------------------------------------------------------------------------*/
 
#include "top.h"
 
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: read_data
--
-- DATE: November 8, 2013
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Tim Kim
--
-- PROGRAMMER: Tim Kim
--
-- INTERFACE: void read_data(void* ptr)
--                  void* ptr: a pointer to a Win_Display struct
--
-- RETURNS: void.
--
-- NOTES:
-- This function continuously reads the GPS data and calls print_data function. It will
-- print GPS_TIMEOUT error message if it cannot read gps_data in 500000 microseconds.
-- Also, if gps_read() returns -1, it will print GPS_GONE or GPS_ERROR error message.
----------------------------------------------------------------------------------------------------------------------*/
void read_data(void *ptr){

    Win_Display* wDisplay = (Win_Display*) ptr;

    while(1){
        sleep(1);
        if(!gps_waiting(wDisplay->gps_data, 500000)){
            cleanup(GPS_TIMEOUT, wDisplay);
            break;
        }
        else{
            int errno = 0;
            if((errno = gps_read(wDisplay->gps_data)) == -1){
                cleanup(errno == 0 ? GPS_GONE : GPS_ERROR, wDisplay);
                break;
            }
            else{
                if(wDisplay->gps_data->set & PACKET_SET){
                    print_data(wDisplay);   
                }
            }
        }
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: cleanup
--
-- DATE: November 8, 2013
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Tim Kim
--
-- PROGRAMMER: Tim Kim
--
-- INTERFACE: static void cleanup(int control, Win_Display* w_display)
--                  int control: controls which error message to print
--                  Win_Display* w_display: a pointer to window display struct
--
-- RETURNS: void.
--
-- NOTES:
-- This function closes stream, disables WATCH and frees malloced structures. Then, it prints the correct error
-- message using the switch statement.
----------------------------------------------------------------------------------------------------------------------*/
void cleanup(int control, Win_Display* w_display){

    gps_stream(w_display->gps_data, WATCH_DISABLE, NULL);
    gps_close (w_display->gps_data);
    free(w_display->gps_data);
    free(w_display);
    switch (control){
 
    case GPS_QUIT:
        exit(0);

    case GPS_GONE:
        fprintf(stderr, "ERROR: GPS signal lost.\n");
        break;
 
    case GPS_ERROR:
        fprintf(stderr, "ERROR: GPS READING error\n");
        break;
 
    case GPS_TIMEOUT:
        fprintf(stderr, "ERROR: GPS WAITING timeout\n");
        break;
 
    default:
        fprintf(stderr, "GPS signal caught successfully %d\n", control);
        break;
    }
 
    exit(1);
}