/* A simple SocketCAN example */

#include <stdio.h>
#include "socketcan_wrapper.h"
#include <pthread.h>
#include <string.h>

// vcan0, can0 모두 사용 가능.
#define CAN_DEV_NAME    "vcan0"

int main(void)
{
    printf("open %s device.\n", CAN_DEV_NAME);
    open_port(CAN_DEV_NAME);
    
    /* check writing hard-coded message. */
    {
        write_port_test();
    }

    /* write CAN message using msg id, length, buffer */
    {
        int msg_id = 0x123;
        int dlc = 8;
        unsigned char buffer[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
        int n_send_bytes = 0;

        n_send_bytes = write_can_message(msg_id, dlc, buffer);
        printf("write CAN message using msg id, length, buffer. send bytes: %d\n", n_send_bytes);
    }

    /* write CAN message using can_frame structure */
    {
        int i = 0;
        struct can_frame w_frame;
        int msg_id = 0xa0a, len = 8;
        unsigned char to_send[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
        int n_send_bytes = 0;

        memset(&w_frame, 0, sizeof(struct can_frame));
        w_frame.can_id = msg_id;
        w_frame.can_dlc = len;
        
        memcpy(w_frame.data, to_send, len);
        
        n_send_bytes = write_can_frame(&w_frame);
        printf("write CAN message using can_frame structure. send bytes: %d\n", n_send_bytes);
    }

    /* 
        read message using can_frame structure.
        because this function uses blocked io,
        you have to use this function in thread.
     */
    {
        struct can_frame read_frame;
        int n_recv_bytes = 0;

        memset(&read_frame, 0, sizeof(struct can_frame));
        n_recv_bytes = read_frame_blocked(&read_frame);
        if(n_recv_bytes)
        {
            printf("read CAN message(blocked io). msg id = 0x%03x, ", read_frame.can_id);
            printf("dlc = %d, ", read_frame.can_dlc);
            printf("data = ");
            {
                int i = 0;
                for (i = 0; i < read_frame.can_dlc; i++)
                    printf("%02X ", read_frame.data[i]);
                printf("\n");
            }
        }
    }

    return 0;
}