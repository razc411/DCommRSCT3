#include "top.h"
#include <gps.h>
void read_data(gps_data_t* gps_data){

    int i = 0;

    while(1){
        sleep(2);
        if(!gps_waiting(gps_data, 500000)){
            //deal with error (check_errors)
            break;
        }
        else{
            if((i = gps_read(gps_data)) == -1){
                //deal with error (check_errors)
                break;
            }
            else{
                if(gps_data->set & PACKET_SET)
                    print_data(gps_data);   
            }
        }
    }
}

//Will check if recieved data is valid
bool check_errors(gps_data_t* gps_data){
    return true;
}
