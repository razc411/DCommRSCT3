#include "data.h"
#include "resource.h"

#define NUL 0x00
#define EOT 0x04
#define ENQ 0x05
#define ACK 0x06
#define DC1 0x11
#define DC2 0x12
#define NAK 0x15
#define SYN 0x16

BOOL read_control_frame(const p_sensor_data, CHAR);
BOOL read_data_frame(const p_sensor_data, PCHAR[]);
VOID recieve( const p_sensor_data);

// Straight-up read from buffer
BOOL read_control_char( const p_sensor_data ptd, CHAR ctrl ) {
	
	// returns true if controll char is ctrl, FALSE if not ctrl
	// wait for RxAck
	DWORD dwEvent;
	SetCommMask(ptd->hcomm, EV_RXCHAR);;
	WaitCommEvent(ptd->hcomm, &dwEvent, NULL);	

	if( dwEvent & EV_RXCHAR ) {
		if( ptd->ring_buff.buff[(ptd->ring_buff.head)] == SYN ){ //if first byte of control frame is SYN
			if( ptd->ring_buff.buff[(ptd->ring_buff.head + 1)] == ctrl ){ //if second byte of control frame is control char
				return true;
			}
		}
	}

	return false;
}

BOOL read_data_frame(const p_sensor_data, PCHAR s[1022]){
	for(int i = 2; i < ptd->count; ++i)
		data_buff[i+2] = ptd->buff[ptd->ring_buff.head + i];
	return true;
}

VOID recieve( const p_sensor_data ptd ){
	if(read_control_char(ptd, ENQ)){
		//begin recieving stuffs
		//send ACK
	} else if (read_control_char(ptd, DC1)){
		//read frame
	} else if (read_control_char(ptd, DC2)){
		//read frame
	} else if( read_control_char(ptd, EOT)){
		//stop reading
	} else {
		//error
	}
}