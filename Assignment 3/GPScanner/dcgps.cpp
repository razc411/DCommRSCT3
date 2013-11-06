#include "top.h"

//Will perform main functions, including open stream, start new thread, and cleanup
//Built in QT for if we choose to do a GUI
int main()
{
    struct gps_data_t *gps_data_ptr = (gps_data_t*)malloc(sizeof(struct gps_data_t));

    //open data stream
    if(!(openStream(gps_data_ptr))){
        //print failure
        cout << "failure!";
        return 0;
    }
    cout << "success!";
    //read_data(gps_data_ptr);
}

//Will open a gps data stream
int openStream(gps_data_t *gps_data_ptr){
    
    static struct fixsource_t source;
    unsigned int flags = WATCH_ENABLE;


    if(gps_open(source.server, source.port, gps_data_ptr) == -1){
        //print failure
        return 0;
    }
    if(source.device == NULL){
        //print failure
        return 0;
    }

    flags |= WATCH_DEVICE;

    gps_stream(gps_data_ptr, flags, source.device);

    return 1;
}

//Will clean up data structures when completed
void freeData(gps_data_t *gps_data_ptr){
    //gps_close(gps_data_ptr, WATCH_DISABLE_NULL);
    gps_close(gps_data_ptr);

}

