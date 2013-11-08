#include "top.h"

//Will perform main functions, including open stream, start new thread, and cleanup
int main()
{
    gps_data_t *gps_data_ptr = (gps_data_t*)malloc(sizeof(struct gps_data_t));

    if(!openStream(gps_data_ptr)){
        exit(0);               
    }
    //open data stream
    while(1){
            
        read_data(gps_data_ptr);    
        sleep(1);
    }

}

//Will open a gps data stream
int openStream(gps_data_t *gps_data_ptr){
    
    int i;
    static struct fixsource_t source;
    unsigned int flags = WATCH_ENABLE;

    if((i = gps_open(source.server, source.port, gps_data_ptr)) == -1){
        //print failure 
        return 0;
    }

    if(source.device == NULL){
        flags |= WATCH_DEVICE; 
    }

    while(!(gps_stream(gps_data_ptr, flags, source.device))){}

    return 1;
}

//Will clean up data structures when completed
void freeData(gps_data_t *gps_data_ptr){
    //gps_close(gps_data_ptr, WATCH_DISABLE, NULL);
    gps_close(gps_data_ptr);

}

