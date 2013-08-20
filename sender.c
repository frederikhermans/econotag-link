/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * $Id: example-broadcast.c,v 1.3 2010/11/06 15:03:48 adamdunkels Exp $
 */

/**
 * \file
 *         Testing the broadcast layer in Rime
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "net/rime.h"
#include "random.h"

#include "dev/button-sensor.h"

#include "dev/leds.h"

#include <mc1322x.h>

#include <stdio.h>
#include <string.h>
/*---------------------------------------------------------------------------*/
PROCESS(example_abc_process, "Broadcast example");
PROCESS(receiver_process, "Receiver");
AUTOSTART_PROCESSES(&example_abc_process);
/*---------------------------------------------------------------------------*/
static void
abc_recv(struct abc_conn *c)
{
}
static const struct abc_callbacks abc_call = {abc_recv};
static struct abc_conn broadcast;

#define PAYLOAD_LEN 40
static uint8_t payload[PAYLOAD_LEN];

PROCESS_THREAD(receiver_process, ev, data) {
  PROCESS_BEGIN();

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_abc_process, ev, data)
{
  static struct etimer et;
  static int i;
  static uint16_t seqno;

  PROCESS_EXITHANDLER(abc_close(&broadcast);)

  PROCESS_BEGIN();

  for (i=2;i<PAYLOAD_LEN;i++) {
    payload[i] = i-2;
  }

  set_channel(5);
  set_power(0);
  abc_open(&broadcast, 129, &abc_call);

  seqno = 0;
  while(1) {
    etimer_set(&et, CLOCK_SECOND / 16);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    memcpy(payload, &seqno, sizeof(uint16_t));
    packetbuf_copyfrom(payload, PAYLOAD_LEN);
    abc_send(&broadcast);
    printf("broadcast message sent\n");
    seqno += 1;
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
