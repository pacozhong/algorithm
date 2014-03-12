CXX= g++
CC= gcc
CFLAGS= -fPIC -g -Wall -DDEBUG -DTIMING -O0 -D_GNU_SOURCE -pthread -Wno-deprecated -Werror

LINK= -L./ 
INCLUDE= -I./
RUN = -Wl,-rpath,./

SRC = stack.c \
	  tree_bs.c

all: main
#all: stack 

main: $(SRC:.c=.o) test_tree_bs.c
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LINK) $(RUN)

%.o: %.cpp Makefile
	$(CXX) $(CFLAGS) $(INCLUDE) -c -o $@ $<
%.o: %.c Makefile
	$(CC) $(CFLAGS) $(INCLUDE)  -c -o $@ $<
%.o: %.cc Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<



clean: 
	rm -f $(SRC:.c=.o) main 
install:
