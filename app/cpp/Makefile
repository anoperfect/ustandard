OPTS += -Wall -Werror -g -I.
USE_USTANDARD += -I$(ROOT_USTANDARD)include -L$(ROOT_USTANDARD)lib -lustandard$(CROSS_LIBRARY_SUFFIX) -Wl,-rpath=$(ROOT_USTANDARD)lib -lpthread -lrt
all:
	g++ cpptest.cpp $(OPTS) $(USE_USTANDARD)
