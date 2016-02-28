target			:= ulogshm
target_cross	:= ulogshm_arm

libustandard := -I$(ROOT_USTANDARD)/include -L$(ROOT_USTANDARD)/lib -lustandard -lpthread -lm -lrt -Wl,-rpath=$(ROOT_USTANDARD)/lib
libustandard_cross :=-I$(ROOT_USTANDARD)/include -L$(ROOT_USTANDARD)/lib -lustandard_arm -lpthread -lm -lrt 


all:
	gcc -g -Wall -o $(target) $(libustandard) main.c
	$(CROSS_COMPILER)gcc -g -Wall -o $(target_cross) $(libustandard_cross) main.c





clean:
	rm $(target) $(target_cross)
