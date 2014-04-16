obj-m += 	lifo_character_driver/lifo_character_driver.o \
			null_character_driver/null_character_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
