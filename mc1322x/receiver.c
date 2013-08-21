#include "contiki.h"
#include "dev/serial-line.h"

#include <mc1322x.h>

#include "receiver.h"
#include "link_logger.h"

PROCESS(receiver_process, "Receiver");

void radio_set_channel(int channel) {
    set_channel(channel-11);
}

void radio_set_txpower(int power) {
    set_power(power);
}

void reset() {
    printf("not implemented.\n");
}

PROCESS_THREAD(receiver_process, ev, data) {
    PROCESS_BEGIN();

    uart1_set_input(serial_line_input_byte);
    serial_line_init();

    printf("receiver process started.\n");

    while (1) {
        PROCESS_WAIT_EVENT();
        if (ev == EVENT_FILTER_FAILED) {
            volatile packet_t *pkt = (volatile packet_t *) data;
            int len = pkt->length-2;

            dump_packet((uint8_t *) pkt->data+1, len, CRC_UNKNOWN, RSSI_UNKNOWN, NOISE_UNKNOWN, LQI_UNKNOWN);
#if 0
            for (i=0;i<p->length-1;i++) {
                printf("%x ", p->data[i]);
            }
            printf("\n");
#endif

            free_packet((volatile packet_t *) data);
        }
    }

    PROCESS_END();
}
