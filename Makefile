TARGET=redbee-econotag
CONTIKI = ../contiki

all: sender receiver

sender.upload: sender
	mc1322x-load.pl -f sender_redbee-econotag.bin -t /dev/ttyUSB1 -c 'bbmc -l redbee-econotag -i 1 reset'

receiver.upload: receiver
	mc1322x-load.pl -f receiver_redbee-econotag.bin -t /dev/ttyUSB3 -c 'bbmc -l redbee-econotag -i 0 reset'

include $(CONTIKI)/Makefile.include
