#ifndef DATA_H
#define DATA_H

#include <windows.h>

typedef struct ring_buffer {
	CHAR buff[1024];
	int head;
	int tail;
	int count;
} ring_buffer;


struct sensor_data {
	// handle to the device
	HANDLE hcomm;

	// ring buffer
	struct ring_buffer ring_buff; 

	// is it idle or not?
	bool idle;

	int y;
};

typedef ring_buffer* p_ring_buffer;
typedef sensor_data* p_sensor_data;

#endif