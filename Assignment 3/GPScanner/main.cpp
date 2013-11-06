#include "top.h"
struct gps_data_t* gps_data_ptr = malloc(sizeof(struct gps_data_t));
//Will perform main functions, including open stream, start new thread, and cleanup
//Built in QT for if we choose to do a GUI
int main(int argc, char *argv[])
{
    //open data stream
    if(!(openStream())){
        //print failure
        return a.exec();
    }

    //create read thread
    pthread_t thread1;
    int iret = pthread_create(&thread1, NULL, read_data, (void*) gps_data_ptr);
    
    return a.exec();
}

//Will open a gps data stream
int openStream(){
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
void freeData(){
    gps_close(gps_data_ptr, WATCH_DISABLE_NULL);
    gps_close(&gps_data_ptr);
}

