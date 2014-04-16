SUBDIRS = null_character_device lifo_character_device
all:
	 $(foreach var,$(SUBDIRS),cd $(var) && make && cd ..;)
clean:
	$(foreach var,$(SUBDIRS),cd $(var) && make clean && cd ..;)
