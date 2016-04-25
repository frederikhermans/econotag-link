#include "contiki.h"
#include "dev/serial-line.h"

#include <mc1322x.h>

#include "receiver.h"
#include "link_logger.h"

#include "lib/crc16.h"

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

    /* Disable header filtering. */
    set_prm_mode(PROMISC);
    /* Disable CRC check. */
    set_fcs_mode(NO_FCS);

    printf("receiver process started.\n");

    while (1) {
        PROCESS_WAIT_EVENT();
        if (ev == EVENT_DATA_INDICATION) {
            volatile packet_t *pkt = (volatile packet_t *) data;
            int len = pkt->length-2;

            uint8_t *crc_ptr = (uint8_t *) pkt->data+1+len;
            uint16_t crc_expected = *crc_ptr << 8 | *(crc_ptr+1);
            uint16_t crc_computed = crc16_data((uint8_t *) pkt->data+1, len, 0);
            printf("crc in packet is %04x, computed %04x\n", crc_expected,
                   crc_computed);

            dump_packet((uint8_t *) pkt->data+1, len,
                        crc_expected == crc_computed,
                        RSSI_UNKNOWN, NOISE_UNKNOWN, pkt->lqi);
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
