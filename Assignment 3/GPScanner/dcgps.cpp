/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: gps-utils.cpp - An application that takes data from a gps dongle and posts 
-- location information as well as information regarding the visible satillites.
--
--
-- PROGRAM: DCGPS
--
-- FUNCTIONS:
-- int main
-- int openStream(gps_data_t *gps_data_ptr)
-- void winSetup(WINDOW *satWin, WINDOW *mWin)
--
--
-- DATE: November 6, 2013
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Tim Kim, Ramzi Chennafi
--
-- PROGRAMMER: Tim Kim, Ramzi Chennafi
--
-- NOTES:
-- The file contains the main function and openStream function. It basically opens
-- the stream to gpsd and starts the stream, while setting the ncurses window up. If there is no error, 
-- it will start the read thread.
----------------------------------------------------------------------------------------------------------------------*/
 
#include "top.h"
 
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: main
--
-- DATE: November 6, 2013
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Tim Kim
--
-- PROGRAMMER: Tim Kim
--
-- INTERFACE: int main()
--
-- RETURNS: int.
--
-- NOTES:
-- This function mallocs the gps_data_t and Win_Display struct and calls openStream function. It will print
-- error message and exit the program if opening stream is unsuccessful. If successful,
-- it will begin a new read function thread. Also sets up the ncurses windows.
----------------------------------------------------------------------------------------------------------------------*/
int main()
{
    
    WINDOW *satWin, *mWin, *hWin;
    int ch;
    pthread_t pt;
    
    gps_data_t *gps_data_ptr = (gps_data_t*)malloc(sizeof(struct gps_data_t));
    
    if(!openStream(gps_data_ptr)){
        cout << "Failed to open the gps stream. Check if the device is connected." << endl;
        exit(0);               
    }

    initscr();
    cbreak();
    keypad(stdscr, true);
    noecho();

    start_color();
    refresh();
    satWin = create_newwin(15, 80, 0, 0);
    mWin = create_newwin(9, 80, 15, 0);
    winSetup(satWin, mWin);

    Win_Display* w_display = (Win_Display*)malloc(sizeof(struct Win_Display));
    w_display->gps_data = gps_data_ptr;
    w_display->satWin = satWin;
    w_display->mWin = mWin;
    
    pthread_create(&pt, NULL, read_data, (void*) w_display);

    while((ch = getch()) != 'q'){} //was going to implement some extra modes, but time constraints stopped that

    endwin();
    cleanup(GPS_QUIT, w_display);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: openStream
--
-- DATE: November 6, 2013
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ramzi Chennafi
--
-- PROGRAMMER: Ramzi Chennafi
--
-- INTERFACE: int openStream(gps_data_t* gps_data_ptr)
--                  gps_data_t* gps_data: a pointer to gps_data_t struct
--
-- RETURNS: int.
--
-- NOTES:
-- It will start the data report stream from the sensor if a socket connection to the
-- gpsd daemon is successful and source.device is not NULL.
----------------------------------------------------------------------------------------------------------------------*/
int openStream(gps_data_t *gps_data_ptr){
    
    static struct fixsource_t source;

    source.server = "localhost";
    source.port = DEFAULT_GPSD_PORT;
    source.device = NULL;   
    
    unsigned int flags = WATCH_ENABLE;

    if(gps_open(source.server, source.port, gps_data_ptr) == -1){
        return 0; 
    }

    if(source.device != NULL){
        flags |= WATCH_DEVICE;
    }

    gps_stream(gps_data_ptr, flags, source.device);

    return 1;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: winSetup
--
-- DATE: November 10, 2013
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ramzi Chennafi
--
-- PROGRAMMER: Ramzi Chennafi
--
-- INTERFACE: void winSetup(WINDOW *satWin, WINDOW *mWin)
--
-- RETURNS: nothing
--
-- NOTES:
-- Sets up the ncurses window.
----------------------------------------------------------------------------------------------------------------------*/
void winSetup(WINDOW *satWin, WINDOW *mWin){
 
    wattrset(satWin, A_UNDERLINE);
    wattrset(mWin, A_UNDERLINE);
    
    mvwprintw(satWin, 1, 2, "DCGPS");
    mvwprintw(satWin, 1, 30, "PRN    Elevation    Azimuth     SNR      Used");
    mvwprintw(satWin, 7, 2, "GPSD Status:");
    mvwprintw(mWin, 3, 5, "Longitude");
    mvwprintw(mWin, 3, 19, "Latitude");
    mvwprintw(mWin, 3, 33, "Altitude");
    mvwprintw(mWin, 3, 47, "Current Speed");
    mvwprintw(mWin, 3, 66, "Fix Mode");
    mvwprintw(satWin, 8, 2, "Avail. Sats:");
    mvwprintw(satWin, 9, 4, "Used Sats:");
    wattrset(satWin, A_NORMAL);
    wattrset(mWin, A_NORMAL);
    
    mvwprintw(satWin, 7, 15, "Active");
    mvwprintw(satWin, 8, 15, "N/A");
    mvwprintw(satWin, 9, 15, "N/A");
    mvwprintw(mWin, 4, 5, "   N/A");
    mvwprintw(mWin, 4, 19, "  N/A");
    mvwprintw(mWin, 4, 33, "N/A");
    mvwprintw(mWin, 4, 52, "N/A");
    mvwprintw(mWin, 4, 69, "N/A");

    mvwprintw(satWin, 3, 2, "A gps location");
    mvwprintw(satWin, 4, 2, "and satillite tracking");  
    mvwprintw(satWin, 5, 2, "program.");
    mvwprintw(satWin, 13, 1, "  By Ramzi and Tim");
    
    mvwprintw(mWin, 8, 32, "Q: Exit program");
    wattrset(satWin, A_UNDERLINE);
    mvwvline(satWin, 1, 25, 0, 13);
    wattrset(satWin, A_NORMAL);
    wrefresh(satWin);
    wrefresh(mWin);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: create_newwin
--
-- DATE: November 10, 2013
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Ramzi Chennafi
--
-- PROGRAMMER: Ramzi Chennafi
--
-- INTERFACE: WINDOW *create_newwin(int height, int width, int starty, int startx)
--
-- RETURNS: WINDOW*
--
-- NOTES:
-- Creates ncurses windows.
----------------------------------------------------------------------------------------------------------------------*/
WINDOW *create_newwin(int height, int width, int starty, int startx)
{   WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0 , 0);       
    wrefresh(local_win);        

    return local_win;
}



