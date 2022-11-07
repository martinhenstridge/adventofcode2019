CC = cc

SRCDIR = src
INCDIR = src

CFLAGS = -I$(INCDIR) -O0 -ggdb

SRCS := $(wildcard $(SRCDIR)/*.c)
HDRS := $(wildcard $(INCDIR)/*.h)

aoc: $(SRCS) $(HDRS)
	$(CC) -o $@ $(SRCS) $(CFLAGS)
