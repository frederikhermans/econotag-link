#include "contiki.h"
#include "dev/cc2420.h"
#include "rime.h"

#include "link_logger.h"

PROCESS(receiver_process, "Receiver");

static void incoming_packet(void) {
    int length = packetbuf_totlen();
    int noise = cc2420_rssi();
    int crc_ok = -1;
    int rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);
    int lqi = packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY);

    noise = cc2420_rssi();
    dump_packet(packetbuf_dataptr(), length, crc_ok, rssi, noise, lqi);
}

RIME_SNIFFER(sniffer, incoming_packet, NULL);

PROCESS_THREAD(receiver_process, ev, data) {
    PROCESS_BEGIN();

    rime_sniffer_add(&sniffer);

    PROCESS_END();
}
