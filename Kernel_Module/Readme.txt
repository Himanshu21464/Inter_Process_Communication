--------------------------KERNEL MODULE ---------------------------------

This kernel module is created by implementing a kernel syscall as a module in which it prints the PID,UID,PGID and command path of a given process (via command line argument passing)

TO compile this kernel modules we have created a makefile in which following commands run to compile the given kernel module:-

obj-m+=kmodule.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
  
  
  * After that we need to insert this module into our kernel to make it work by the following command:-
 
    "sudo insmod kmodule.ko "
    
   * TO remove/unload  the module :-
   
   "sudo rmmod kmodule.ko"
   
    
    
