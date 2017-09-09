ustandard = ../../
tustandard = /home/ben/ustandard
SRC += $(wildcard *.c)

all:
	$(CROSS_COMPILER)gcc -Wall -Werror -g -I$(ustandard)/include -L$(ustandard)/lib -lustandard -Wl,-rpath=$(ustandard)/lib -lpthread -Wl,-rpath=$(ustandard)/lib $(SRC)

clean:
	@rm -i `find . -name a.out`
