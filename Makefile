CXX= g++
CC= gcc
CFLAGS= -fPIC -g -Wall -DDEBUG -DTIMING -O0 -D_GNU_SOURCE -pthread -Wno-deprecated -Werror

LINK= -L./ 
INCLUDE= -I./
RUN = -Wl,-rpath,./

SRC = xl_stack.c \
	  xl_bst.c \
	  xl_rbt.c

all: test_bst test_rbt 
#all: stack 

test_bst: $(SRC:.c=.o) test_bst.c
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LINK) $(RUN)

test_rbt: $(SRC:.c=.o) test_rbt.c
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LINK) $(RUN)

%.o: %.cpp Makefile
	$(CXX) $(CFLAGS) $(INCLUDE) -c -o $@ $<
%.o: %.c Makefile
	$(CC) $(CFLAGS) $(INCLUDE)  -c -o $@ $<
%.o: %.cc Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<



clean: 
	rm -f $(SRC:.c=.o) test_bst test_rbt
install:
