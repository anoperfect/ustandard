ustandard_path=/home/ben/report/xxxdev/httplivestreaming/ustandard



all:
	gcc -Wall -Werror -g -I$(ustandard_path)/include -L$(ustandard_path)/lib -Wl,-rpath=$(ustandard_path)/lib -lustandard -lpthread test-upid.c
