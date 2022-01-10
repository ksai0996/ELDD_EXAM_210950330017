obj-m :=char_dev.o

		KERNELDIR = /lib/modules/$(shell uname -r)/build

		PWD :=$(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc -o char_dev char_dev.c

clean:
	
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm char_dev

