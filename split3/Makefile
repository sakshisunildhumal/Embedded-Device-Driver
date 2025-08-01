obj-m := my3difcode.o  
my3difcode-objs := mod351.o mod352.o mod353.o 

ifdef ARCH
  #You can update your Beaglebone path here.
  KSRC = ../../linux
else
  KSRC = /lib/modules/$(shell uname -r)/build
endif

mods:
	$(MAKE) -C $(KSRC) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KSRC) M=$(shell pwd) clean
