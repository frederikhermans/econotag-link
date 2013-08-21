#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "contiki.h"
#include "dev/serial-line.h"
#include "rime.h"
#include "uart1.h"

#include "link_logger.h"
#include "tokenizer.h"

PROCESS(link_logger_process, "Link logger");
AUTOSTART_PROCESSES(&link_logger_process, &receiver_process);

static struct {
    uint8_t txpower;
    uint8_t channel;
    uint32_t seqval;
    uint8_t payload_len;
    uint16_t period;
    uint16_t jitter;
    int to_send;
} config = { -1, -1, 0, 35, CLOCK_SECOND/2, 0, 0 };

static struct abc_conn abc_con;
static void abc_recv(struct abc_conn *con) { }
static const struct abc_callbacks abc_cb = { abc_recv };

static void handle_serial_input(const char *line) {
    int pos = 0;
    const char *token;

    /* Consume the first token. */
    token = token_next(line, &pos);

    switch (token[0]) {
//    case 't':
//        config.txpower = atoi(token_next(line, &pos));
//        printf("Setting TX power to %d.\n", config.txpower);
//        /* XXX Don't use the packet buf attr for TX power if we
//         * use cc2420_set_txpower(...) */
//        cc2420_set_txpower(config.txpower);
//        break;
//    case 'c':
//        config.channel = atoi(token_next(line, &pos));
//        printf("Setting channel to %d\n", config.channel);
//        cc2420_set_channel(config.channel);
//        break;
    case 's':
        config.to_send = atoi(token_next(line, &pos));
        printf("Sending %d packets.\n", config.to_send);
        break;
    case 'p':
        config.period = atoi(token_next(line, &pos));
        printf("Setting period to %d ticks.\n", config.period);
        break;
//    case 'j':
//        config.jitter = atoi(token_next(line, &pos));
//        printf("Setting jitter to %u ticks.\n", config.jitter);
//        break;
//    case 'l':
//        config.payload_len = atoi(token_next(line, &pos)) - 2;
//        printf("Setting payload length to %d bytes.\n", config.payload_len + 2);
//        break;
    case 'r':
        WDTCTL = 0;
        break;
//    case 'z':
//        printf("txpower=%u, channel=%u, seqno=%u, to_send=%d, period=%d, "
//               "jitter=%u, payload_len=%d, rime_addr=%02x:%02x\n",
//               config.txpower, config.channel,
//               config.seqno, config.to_send, config.period, config.jitter,
//               config.payload_len + 2, rimeaddr_node_addr.u8[0],
//               rimeaddr_node_addr.u8[1]);
//        break;
    default:
        printf("?\n");
    }
}


uint32_t seqnext() {
    config.seqval =  22695477 * config.seqval + 1;
    return config.seqval;
}


void send_packet() {
    uint8_t buf[128];
    int i;
    uint32_t seqval = config.seqval;

    memcpy(buf, &seqval, sizeof(uint32_t));
    
    for (i=4;i<128;i++) {
        uint8_t val = (seqnext() >> 16) % 16;
        buf[i] = (val << 4) | val;
    }

    packetbuf_copyfrom(buf, config.payload_len);
    printf("send: seqval=%lu\n", seqval);
    abc_send(&abc_con);
}


void dump_packet(uint8_t *payload, int length, int crc_ok, int rssi,
                 int noise, int lqi) {
    int i;

    printf("recv: len=%d, rssi=%d, lqi=%d, noise=%d, crc_ok=%d, payload=(",
            length, rssi, lqi, noise, crc_ok);
    for (i=0;i<length;i++) {
        printf("%x ", payload[i]);
    }
    printf("), rssi_ovr=[]\n"); // TODO: RSSI sampling during packet reception.
}

PROCESS_THREAD(link_logger_process, ev, data) {
    static struct etimer et;
    PROCESS_BEGIN();

    uart1_set_input(serial_line_input_byte);
    //serial_line_init();
    abc_open(&abc_con, 128, &abc_cb);

    printf("Hello, world!\n");

    cc2420_set_channel(16);
    cc2420_on();

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == serial_line_event_message ||
                                 ((config.to_send > 0 || config.to_send == -1) &&
                                  etimer_expired(&et)));


        if (ev == serial_line_event_message && data != NULL) {
            handle_serial_input((const char *) data);
        }


        if ((config.to_send > 0 || config.to_send == -1) && etimer_expired(&et)) {
            uint16_t delay = config.period;
            if (config.jitter) {
                delay += (rand() % (2*config.jitter+1)) - config.jitter;
                printf("waiting %u ticks.\n", delay);
            }
            etimer_set(&et, delay);
            /* Send a packet to the next peer. */
            send_packet();
            config.to_send = config.to_send == -1 ? -1 : config.to_send-1;
        }
    }

    abc_close(&abc_con);

    PROCESS_END();
}
