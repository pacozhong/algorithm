CXX= g++
CC= gcc
CFLAGS= -fPIC -g -Wall -DDEBUG -DTIMING -O0 -D_GNU_SOURCE -pthread -Wno-deprecated -Werror

LINK= -L./ 
INCLUDE= -I./
RUN = -Wl,-rpath,./

SRC = stack.c \
	  tree_bs.c \
	  tree_rb.c

all: test_tree_bs test_tree_rb 
#all: stack 

test_tree_bs: $(SRC:.c=.o) test_tree_bs.c
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LINK) $(RUN)

test_tree_rb: $(SRC:.c=.o) test_tree_rb.c
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LINK) $(RUN)

%.o: %.cpp Makefile
	$(CXX) $(CFLAGS) $(INCLUDE) -c -o $@ $<
%.o: %.c Makefile
	$(CC) $(CFLAGS) $(INCLUDE)  -c -o $@ $<
%.o: %.cc Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<



clean: 
	rm -f $(SRC:.c=.o) test_tree_bs test_tree_rb 
install:
