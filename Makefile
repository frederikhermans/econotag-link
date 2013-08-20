TARGET=redbee-econotag
CONTIKI = ../contiki
CONTIKI_TARGET_SOURCEFILES += mc1322x-receiver.c tokenizer.c

node1.upload: link_logger
	mc1322x-load.pl -e -f link_logger_redbee-econotag.bin -t /dev/ttyUSB2 -c 'bbmc -l redbee-econotag -i 0 reset'

node2.upload: link_logger
	mc1322x-load.pl -e -f link_logger_redbee-econotag.bin -t /dev/ttyUSB1 -c 'bbmc -l redbee-econotag -i 1 reset'

include $(CONTIKI)/Makefile.include
