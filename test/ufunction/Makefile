ustandard_path=$(ROOT_USTANDARD)
SRC += $(wildcard *.c)


all:
	gcc -Wall -Werror -g -I$(ustandard_path)/include -L$(ustandard_path)/lib -Wl,-rpath=$(ustandard_path)/lib -lustandard -lpthread -lrt $(SRC)
