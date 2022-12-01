#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
// #include <linux/can.h>
#include <linux/can/raw.h>
#include "socketcan_wrapper.h"

static int s_sock;
static int s_blocked_read_used = 0;

int open_port(const char *port)
{
    struct ifreq ifr;
    struct sockaddr_can addr;

    memset(&ifr, 0, sizeof(struct ifreq));
    memset(&addr, 0, sizeof(struct sockaddr_can));

    /* open socket */
    s_sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(s_sock < 0)
    {
        printf("open %s device error.\n", port);
        return -1;
    }

    addr.can_family = AF_CAN;
    strcpy(ifr.ifr_name, port);

    if (ioctl(s_sock, SIOCGIFINDEX, &ifr) < 0)
    {
        printf("ioctl SIOCGIFINDEX failed.\n");
        return -1;
    }

    addr.can_ifindex = ifr.ifr_ifindex;

    fcntl(s_sock, F_SETFL, O_NONBLOCK);

    if (bind(s_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("bind socket error.\n");
        return -1;
    }

    return 0;
}

int read_frame_blocked(struct can_frame *frame_rd)
{
    // struct can_frame frame_rd;
    int n_recv_bytes = 0;

    if (frame_rd == NULL)
    {
        printf("%s, %d: read buffer is not allocated.\n", __func__, __LINE__);
        return -1;
    }

    memset(frame_rd, 0, sizeof(struct can_frame));

    s_blocked_read_used = 1;
    
    while (s_blocked_read_used)
    {
        struct timeval timeout = {1, 0};
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(s_sock, &readSet);

        if (select((s_sock + 1), &readSet, NULL, NULL, &timeout) >= 0)
        {
            if (!s_blocked_read_used)
            {
                break;
            }
            if (FD_ISSET(s_sock, &readSet))
            {
                n_recv_bytes = read(s_sock, frame_rd, sizeof(struct can_frame));
                return n_recv_bytes;
            }
        }
    }

    return n_recv_bytes;
}

void stop_read_frame_blocked()
{
    s_blocked_read_used = 0;
}

/*
 https://en.wikipedia.org/wiki/SocketCAN
*/
void write_port_test()
{
    struct can_frame w_frame;
    int n_bytes = 0;

    w_frame.can_id = 0x123;
    w_frame.can_dlc = 2;
    w_frame.data[0] = 0x11;
    w_frame.data[1] = 0x22;

    n_bytes = write(s_sock, &w_frame, sizeof(struct can_frame));

    printf("Wrote %d bytes\n", n_bytes);
}

int write_can_message(int msg_id, int len, unsigned char *p_buffer)
{
    struct can_frame w_frame;
    int n_bytes = 0;
    int i = 0;

    w_frame.can_id = msg_id;
    w_frame.can_dlc = len;

    memcpy(w_frame.data, p_buffer, len);

    n_bytes = write(s_sock, &w_frame, sizeof(struct can_frame));
    if (n_bytes < 0) {
        printf("%s, %d: write error: %d\n", __func__, __LINE__, n_bytes);
    }

    return n_bytes;
}

int write_can_frame(struct can_frame *w_frame)
{
    int n_bytes = 0;

    n_bytes = write(s_sock, w_frame, sizeof(struct can_frame));
    if (n_bytes < 0) {
        printf("%s, %d: write error, ret: %d\n", __func__, __LINE__, n_bytes);
    }

    return n_bytes;
}

int close_port()
{
    close(s_sock);
    return 0;
}
