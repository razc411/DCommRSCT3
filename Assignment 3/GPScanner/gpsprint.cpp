/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: gps-utils.cpp - An application that takes data from a gps dongle and posts 
-- location information as well as information regarding the visible satillites.
--
--
-- PROGRAM: dcgps
--
-- FUNCTIONS:
-- void print_data(Win_Display* wDisplay)
--
--
-- DATE: November 8, 2013
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ramzi Chennafi
--
-- PROGRAMMER: Ramzi Chennafi
--
-- NOTES:
-- The file contains the print_data function which handles all of the printing out of
-- GPS report data.
----------------------------------------------------------------------------------------------------------------------*/
 
#include "top.h"
 
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: print_data
--
-- DATE: November 8, 2013
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ramzi Chennafi
--
-- PROGRAMMER: Ramzi Chennafi
--
-- INTERFACE: void print_data(Win_Display* wDisplay)
--
-- RETURNS: void.
--
-- NOTES:
-- This function prints GPS data including Time stamp(UTC), PRN,
-- Elevation, Azimuth, SNR, and Used flag for each satellite that are visible. if fix.mode
-- is greater than or equal to MODE_2D then it will also print out Latitude and Logitude.
-- If its equal to MODE_3D, the altitude will also be printed. Current mode and speed are printed as well.
----------------------------------------------------------------------------------------------------------------------*/

void print_data(Win_Display* wDisplay){
    bool usedflags[MAXCHANNELS];
    int y = 2;

    time_t now;
    time(&now);
    char buf[sizeof "2014-10-08T24:00:00Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
    
    mvwprintw(wDisplay->mWin, 1, 1, "Timestamp : %s", buf);

    if(!isnan(wDisplay->gps_data->satellites_used))
        mvwprintw(wDisplay->satWin, 9, 15, "%d  ", wDisplay->gps_data->satellites_used);
    else
        mvwprintw(wDisplay->satWin, 9, 15, "N/A");

    if(!isnan(wDisplay->gps_data->satellites_visible))
        mvwprintw(wDisplay->satWin, 8, 15, "%d  ", wDisplay->gps_data->satellites_visible);
    else
        mvwprintw(wDisplay->satWin, 8, 15, "N/A");

    for(int i = 0; i < MAXCHANNELS; i++){
        usedflags[i] = false;
        
        for(int j = 0; j < wDisplay->gps_data->satellites_used; j++){
            if(wDisplay->gps_data->used[j] == wDisplay->gps_data -> PRN[i])
                usedflags[i] = true;
        }
    }
    
    for(int i = 2; i < 14; i++){
        mvwprintw(wDisplay->satWin, i, 29, "                                                  ");
    }
    wrefresh(wDisplay->satWin);

    if(wDisplay->gps_data->satellites_visible != 0){
        for(int i = 0; i < MAX_POSSIBLE_SATS; i++){
            if(i < wDisplay->gps_data->satellites_visible){
                mvwprintw(wDisplay->satWin,y++,29, "%3d        %2d          %3d°     %02d dB     %s",
                                 wDisplay->gps_data->PRN[i],
                                 wDisplay->gps_data->elevation[i], wDisplay->gps_data->azimuth[i],
                                 (int)wDisplay->gps_data->ss[i],
                                 usedflags[i] ? "Yes" : "No");
            }
        }
    }

    if (wDisplay->gps_data->fix.mode >= MODE_2D && isnan(wDisplay->gps_data->fix.latitude) == 0)
        mvwprintw(wDisplay->mWin, 4, 20, "%.2f° %c    ", wDisplay->gps_data->fix.latitude,((wDisplay->gps_data->fix.latitude < 0) ? 'S' : 'N'));
    else
        mvwprintw(wDisplay->mWin, 4, 20, " N/A     ");

    if (wDisplay->gps_data->fix.mode >= MODE_2D && isnan(wDisplay->gps_data->fix.longitude) == 0) 
        mvwprintw(wDisplay->mWin, 4, 5, "%.2f° %c", wDisplay->gps_data->fix.longitude,((wDisplay->gps_data->fix.longitude < 0) ? 'E' : 'W'));
    else    
        mvwprintw(wDisplay->mWin, 4, 5, "   N/A       ");
    
    if (wDisplay->gps_data->fix.mode >= MODE_3D && isnan(wDisplay->gps_data->fix.altitude) == 0){
        mvwprintw(wDisplay->mWin, 4, 33, "           "); //this if statement is an inexplicable fix to a very annoying problem
        wrefresh(wDisplay->mWin);
        mvwprintw(wDisplay->mWin, 4, 35, "%0.2f      ", wDisplay->gps_data->fix.altitude);
    }
    else
        mvwprintw(wDisplay->mWin, 4, 33, "  N/A       ");

    if(!isnan(wDisplay->gps_data->fix.speed))
        mvwprintw(wDisplay->mWin, 4, 52, "%.2f     ", wDisplay->gps_data->fix.speed);
    else
        mvwprintw(wDisplay->mWin, 4, 52, "N/A      ");

    if(wDisplay->gps_data->fix.mode == MODE_2D)            
        mvwprintw(wDisplay->mWin, 4, 69, "2D ");
    
    else if(wDisplay->gps_data->fix.mode == MODE_3D)
        mvwprintw(wDisplay->mWin, 4, 69, "3D ");
    
    else
        mvwprintw(wDisplay->mWin, 4, 69, "N/A ");

    wrefresh(wDisplay->satWin);
    wrefresh(wDisplay->mWin);
}


