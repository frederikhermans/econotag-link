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
 * $Id: example-abc.c,v 1.3 2010/11/06 15:03:48 adamdunkels Exp $
 */

/**
 * \file
 *         Testing the abc layer in Rime
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
PROCESS(receiver_process, "Broadcast example");
AUTOSTART_PROCESSES(&receiver_process);
/*---------------------------------------------------------------------------*/

static void
abc_recv(struct abc_conn *c)
{
}

static const struct abc_callbacks abc_call = {abc_recv};
static struct abc_conn abc;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(receiver_process, ev, data)
{
  static struct etimer et;
  static int i;
  static uint16_t seqno, last_seqno = 0;

  PROCESS_EXITHANDLER(abc_close(&abc);)

  PROCESS_BEGIN();

  set_channel(5);
  abc_open(&abc, 129, &abc_call);

  while(1) {
//    etimer_set(&et, CLOCK_SECOND/4);

    PROCESS_WAIT_EVENT();
    printf("ev=%d data=%p\n", ev, data);
    if (ev == 210) {
      packet_t *p = (packet_t *) data;
      memcpy(&seqno, (uint8_t *) p->data+3, sizeof(uint16_t));
      printf("seqno=%u\n", seqno);
      if (seqno != last_seqno+1) {
	if (seqno > last_seqno) {
	  printf("missed %u packets\n", seqno-last_seqno-1);
	} else {
	  printf("last seqno was: %u\n", last_seqno);
	}
      }
      last_seqno = seqno;
      for (i=0;i<p->length-1;i++) {
	printf("%x ", p->data[i]);
      }
      printf("\n");
      free_packet((volatile packet_t *) data);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
