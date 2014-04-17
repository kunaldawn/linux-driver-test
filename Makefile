obj-m += 	lifo_character_driver/lifocdev.o \
			null_character_driver/null_character_driver.o

lifo_character_driver/lifocdev-objs := lifo_character_driver/memory_manager.o lifo_character_driver/lifo_character_driver.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
