#include "data.h"


BOOL	 read_control_char(const p_sensor_data, PCHAR);
BOOL wait_for_buffer_input(p_ring_buffer);
VOID	   broadcast_TxEnq(p_sensor_data);
VOID			  transmit(p_sensor_data);

VOID	 create_data_frame(TCHAR[1020], p_ring_buffer);
VOID  create_control_frame(TCHAR[2], const char c);


// thread proc: determine action to take based on control char
LPTHREAD_START_ROUTINE thread_transmit(LPVOID pVoid) {

	p_sensor_data ptd = (p_sensor_data)pVoid;
	COMSTAT cs;

	// let's get going
	broadcast_TxEnq(ptd);


	CHAR ctrl;
	while( true )
	{
		// begin critical section
		// did user cancel?
		if( ptd->idle ) 
			return 0;
		// end critical section

		// timeout -> false
		if(! read_control_char( ptd, &ctrl ))
			return 0;
		
		// read control character
		switch(ctrl)
		{
			// RxEOT
			case 0x04:
			return 0;

			// RxACK
			case 0x06:
				// begin transmission
				// write_to_buffer();
				wait_for_buffer_input( &ptd->ring_buff );
				transmit( ptd );
			break;

			// RxNAK
			case 0x15:
				// begin retransmission
				transmit( ptd );
			break;

			default:
			continue;
		}
	}

	return 0;
}


VOID create_control_frame(TCHAR ctrl_buff[2], const char c) {
	ctrl_buff[0] = 0x16;
	ctrl_buff[1] = c;
}


VOID create_data_frame(TCHAR data_buff[1020], p_ring_buffer ptd) {
	
	static char dc = 0x11;
	data_buff[0] = 0x16;
	data_buff[1] = dc;
	
	int head = ptd->head;
	int tail = ptd->tail;

	for(int i = 0; i < ptd->count; ++i)
		data_buff[i+2] = ptd->buff[head + i];

	data_buff[1018] = 0x00;
	data_buff[1019] = 0x00;

}

VOID transmit( p_sensor_data ptd ) {
					
	TCHAR ctrl_buff[2];
	TCHAR data_buff[1024];

	create_control_frame(ctrl_buff, 0x06);
	create_data_frame(data_buff, &ptd->ring_buff );

	WriteFile(ptd->hcomm, ctrl_buff, sizeof(ctrl_buff), NULL, NULL); // control frame
	WriteFile(ptd->hcomm, data_buff, sizeof(data_buff), NULL, NULL); // data frame
}

// TxEnq
VOID broadcast_TxEnq( p_sensor_data ptd ) {
	// broadcast RxEnqf
	TCHAR ctrl_buff[2];
	create_control_frame(ctrl_buff, 0x05);

	WriteFile(ptd->hcomm, ctrl_buff, sizeof(ctrl_buff), NULL, NULL);
}


// Straight-up read from buffer
BOOL read_control_char( const p_sensor_data ptd, PCHAR ctrl ) {
	
	// returns true if ACK, FALSE if NACK
	// wait for RxAck
	DWORD dwEvent;
	SetCommMask(ptd->hcomm, EV_RXCHAR);;
	WaitCommEvent(ptd->hcomm, &dwEvent, NULL);	

	if( dwEvent & EV_RXCHAR ) {
		// assign to ctrl
		// return true
	}

	return false;
}


BOOL wait_for_buffer_input( p_ring_buffer p_ring_buff ) {

	while( p_ring_buff->count < 5 ) { }
	return true;
}
