#include "top.h"
#include <gps.h>
//Will read data from stream. Passed a stream pointer.
void read_data(gps_data_t* gps_data){

    while(1){
        if(!gps_waiting(gps_data, 500000)){
            break;
        }
        else{
            if(int i = gps_read(gps_data) == -1){
                break;
            }
            else{
                cout << i << endl;
                print_data(gps_data);
                break;
            }
        }
    }
}

//Will check if recieved data is valid
bool check_errors(gps_data_t* gps_data){
    return true;
}
