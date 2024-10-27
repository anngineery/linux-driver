CFLAGS_ldd.o := -DDEBUG
obj-m += ldd.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo rmmod ldd
	lsmod
	sudo dmesg --clear
	sudo insmod ldd.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
