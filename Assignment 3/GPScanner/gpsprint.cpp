#include "top.h"
//Will print valid gps data
void print_data(gps_data_t* gps_data){
    bool usedflags[MAXCHANNELS];

    for(int i = 0; i < MAXCHANNELS; i++){
        usedflags[i] = false;
        
        for(int j = 0; j < gps_data->satellites_used; j++){
            if(gps_data->used[j] == gps_data -> PRN[i])
                usedflags[i] = true;
        }
    }

    if(gps_data->satellites_visible != 0){
        for(int i = 0; i < MAX_POSSIBLE_SATS; i++){
            if(i < gps_data->satellites_visible){
                fprintf(stdout,  " PRN : %3d | Elevation : %02d | Azimuth : %03d |"
                                 " Data : %02d | Used : %c \n",
                                 gps_data->PRN[i],
                                 gps_data->elevation[i], gps_data->azimuth[i],
                                 (int)gps_data->ss[i],
                                 usedflags[i] ? 'Y' : 'N');
            }
        }
        if (gps_data->fix.mode >= MODE_2D && isnan(gps_data->fix.latitude) == 0){
            cout << "Latitude: " << gps_data->fix.latitude << " ";
            cout << ((gps_data->fix.latitude < 0) ? 'S' : 'N') << " | ";
        }
        if (gps_data->fix.mode >= MODE_2D && isnan(gps_data->fix.longitude) == 0) {
            cout << "Longitude: " << gps_data->fix.longitude << " ";
            cout << ((gps_data->fix.longitude < 0) ? 'W' : 'E') << endl;
        }
        cout << "***************************************" << endl;
    }

}

