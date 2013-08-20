#include "contiki.h"

#include <mc1322x.h>

#include "receiver.h"
#include "link_logger.h"

PROCESS(receiver_process, "Receiver");

PROCESS_THREAD(receiver_process, ev, data) {
    PROCESS_BEGIN();

    set_channel(5);

    printf("receiver process started.\n");

    while (1) {
        PROCESS_WAIT_EVENT();
        if (ev == EVENT_FILTER_FAILED) {
            volatile packet_t *pkt = (volatile packet_t *) data;
            int len = pkt->length-2;

            dump_packet(pkt->data+1, len, CRC_UNKNOWN, RSSI_UNKNOWN, NOISE_UNKNOWN, LQI_UNKNOWN);
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
