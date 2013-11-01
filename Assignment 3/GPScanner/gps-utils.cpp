#include "top.h"

//Will read data from stream. Passed a stream pointer.
void read_data(void *ptr){
    gps_data_t *gpsdata = (gps_data_t)ptr;


    while(){
        if(!gps_waiting(gps_ptr, 500000)){
            Cleanup(GPS_TIMEOUT);
        }
        else{
            errno = 0;
            if(gps_read(gpsdata) == -1){
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
bool check_errors(){
    return true;
}
