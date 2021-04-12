INCDIR      = /usr/local/include
LIBDIR      =  /usr/local/lib

PLATFORM := $(shell uname -s)

CC	=  gcc -std=c11  -fopenmp

LIB = -lgraphblas -llagraph
WFLAGS	= -Wall

OPTS = -O3
CCFLAGS	= $(OPTS) $(WFLAGS) $(DEFS) $(INCLUDE)

SOURCES		= $(wildcard *.c)

OBJECTS		= $(SOURCES:.c=.o)
TARGETS		= $(SOURCES:.c=.exe)
DSYM   		= $(SOURCES:.c=.exe.dSYM)
HEADERS		= $(wildcard $(INCDIR)/*.h)
PCHS		= $(HEADERS:=.gch)

.PHONY		: all clean

all		: exe

%.s 		: %.c $(HEADERS)
		$(CC) -S $(CCFLAGS) $<

%.o  		: %.c $(HEADERS)
		$(CC) -c $(CCFLAGS) $<

exe  		: $(OBJECTS)
		$(CC) $(OBJECTS) -o main.exe $(LIB)

clean		:
		/bin/rm -rf a.out $(TARGETS) $(OBJECTS) $(PCHS) $(DSYM) *~


squeaky		: clean
		/bin/rm -f $(SOURCES:%.c=%.d)


.SECONDARY	: $(OBJECTS)

-include $(SOURCES:%.c=%.d)
