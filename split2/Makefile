obj-m := my_char_device.o  # This will be the name of your single .ko file (my_char_device.ko)
my_char_device-objs := mod351.o mod352.o # These are the object files that form my_char_device.o

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
