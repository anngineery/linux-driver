# good resource to understand this: https://docs.kernel.org/kbuild/modules.html

ifneq ($(KERNELRELEASE),)
# read by kbuild (invoked by the make command in the target)
obj-m += ldd.o	# kbuild builds ldd.o from ldd.c and generates ldd.ko after linking

else
#read by make command that I type in
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

debug:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules CFLAGS_ldd.o='-DDEBUG -save-temps'

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
endif
