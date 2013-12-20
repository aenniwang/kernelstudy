KDIR=/lib/modules/$(shell uname -r)/build

all:x86 app

x86:
	make -C $(KDIR) M=$(shell pwd) modules

app: 	
	gcc app.c -o testapp

clean:
	@rm *.o *.ko modules.* Module* testapp *.mod.c .tmp_versions -rf
	@rm .vma* -rf


obj-m := vma.o
