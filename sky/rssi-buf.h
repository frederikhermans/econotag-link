#ifndef _RSSI_BUF_H
#define _RSSI_BUF_H

#define RSSI_BUF_SIZE   128

struct rssi_buffer {
    volatile int i;
    int8_t rssi[RSSI_BUF_SIZE];
    uint8_t freq[RSSI_BUF_SIZE];
};

struct rssi_buffer *cc2420_get_rssi();

#endif
