-include mk.test.target
OPTS += -Wall -Werror -g 
USE_USTANDARD += -I$(ROOT_USTANDARD)include -L$(ROOT_USTANDARD)lib -lustandard$(CROSS_LIBRARY_SUFFIX) -Wl,-rpath=$(ROOT_USTANDARD)lib -lpthread -lrt
INC	+= -I$(ROOT_USTANDARD)/test

ifdef test_target	
$(info user defined test target)
targets :
	for tg in $(test_target); \
	do \
		$(CROSS_COMPILER)gcc -o $(addprefix a.out_, $$tg) $(addsuffix .c, $$tg) $(OPTS) $(INC) $(USE_USTANDARD) ; \
	done
else
target : 
	$(CROSS_COMPILER)g++ $(wildcard *.cpp) $(OPTS) $(INC) $(USE_USTANDARD)
endif

all : targets

clean:
	@rm -i `find . -name "a.out*"`
