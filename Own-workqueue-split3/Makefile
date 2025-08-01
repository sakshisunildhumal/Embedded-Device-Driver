obj-m += run.o
run-objs := 12-5-1.o 12-5-2.o 12-5-3.o

ifdef ARCH
  KSRC = ../../linux
else
  KSRC = /lib/modules/$(shell uname -r)/build
endif

all:
	make -C $(KSRC) M=$(shell pwd) modules

clean:
	make -C $(KSRC) M=$(shell pwd) clean
