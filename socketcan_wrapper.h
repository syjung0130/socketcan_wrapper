#ifndef __SOCKETCAN_WRAPPER_H__
#define __SOCKETCAN_WRAPPER_H__

#include <linux/can.h>

int open_port(const char *port);

/* 
    read message using can_frame structure.
    because this function uses blocked io,
    you have to use this function in thread.
*/
int read_frame_blocked(struct can_frame *frame_rd);


/* check writing hard-coded message */
void write_port_test();

/* write CAN message using msg id, length, buffer */
int write_can_message(int msg_id, int len, unsigned char *p_buffer);

/* write CAN message using can_frame structure */
int write_can_frame(struct can_frame *w_frame);

int close_port();

#endif