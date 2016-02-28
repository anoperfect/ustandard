ustandard = /home/ben/report/xxxdev/httplivestreaming/ustandard
tustandard = /home/ben/ustandard
SRC += $(wildcard *.c)

all:
	$(CROSS_COMPILER)gcc -Wall -Werror -g -I$(ustandard)/include -L$(ustandard)/lib -lustandard$(CROSS_LIBRARY_SUFFIX) -Wl,-rpath=$(ttustandard)/lib -lpthread -Wl,-rpath=$(ustandard)/lib $(SRC)

clean:
	@rm -i `find . -name a.out`
