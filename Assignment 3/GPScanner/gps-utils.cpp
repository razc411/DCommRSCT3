#include "top.h"
#include <gps.h>
//Will read data from stream. Passed a stream pointer.
void read_data(gps_data_t* gps_data){

    while(1){
        if(!gps_waiting(gps_data, 500000)){
            Cleanup(GPS_TIMEOUT);
        }
        else{
            errno = 0;
            if(gps_read(gps_data) == -1){
                //print failure
                Cleanup(errno == 0 ? GPS_GONE : GPS_ERROR);
            }
            else{
                if(check_errors(gps_data)){
                    print_data(gps_data);
                }
            }
        }
    }
}

//Will check if recieved data is valid
bool check_errors(gps_data_t* gps_data){
    return true;
}
