
CC = gcc
CFLAGS = -O3 -g -Werror -Wall -std=c11 #-DDEBUG
CCCOM = $(CC) $(CFLAGS)
SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)
LDLIBS = -lmingw32 -lSDLmain -lSDL 
LDFLAGS = -L/mingw/lib 
EXE = life
RM = del

all: $(EXE)

$(EXE): $(OBJS)
	$(CCCOM) $(LDFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean 

clean: 
	$(RM) *.o *.exe *~ *.stckdump *.bak
 