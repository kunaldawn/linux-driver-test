obj-m += 	lifo_character_device/lifo_driver.o \
			null_character_device/dummy.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
