ifneq ($(KERNELRELEASE),)
# read by kbuild
CFLAGS_ldd.o := -DDEBUG
obj-m += ldd.o

else
#read by make command
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
endif
