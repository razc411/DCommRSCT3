#include "top.h"

//Will print valid gps data
void print_data(gps_data_t *gps_data){
    bool usedflags[MAXCHANNELS];

    for(i = 0; i < MAXCHANNELS; i++){
        usedflags[i] = false;
        for(j = 0; j < gpsdata->satellites_used; j++){
            if(gpsdata->used[j] == gpsdata -> PRN[i])
                usedflags[i] = true;
        }
    }

    if(gpsdata->satillites_visible != 0){
        for(i = 0; i < MAX_POSSIBLE_SATS; i++){
            if(i < gpsdata->satellites_visible){
                //print satallite data
            }
        }
    }

}
