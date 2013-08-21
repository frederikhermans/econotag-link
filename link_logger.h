#ifndef _LINK_LOGGER_H
#define _LINK_LOGGER_H 1

#include "process.h"

#define CRC_UNKNOWN -1
#define RSSI_UNKNOWN  0xfff
#define NOISE_UNKNOWN 0xfff
#define LQI_UNKNOWN -1

PROCESS_NAME(receiver_process);

void radio_set_channel(int);
void radio_set_txpower(int);
void reset();
void dump_packet(uint8_t *payload, int length, int crc_ok, int rssi,
                 int noise, int lqi);

#endif
