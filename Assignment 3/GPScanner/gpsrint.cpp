#include "top.h"
#include <gps.h>
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

    // if(gps_data->satillites_visible != 0){
    //     for(int i = 0; i < MAX_POSSIBLE_SATS; i++){
    //         if(i < gps_data->satellites_visible){
    //             //print satallite data
    //         }
    //     }
    // }

}
