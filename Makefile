CC = cc

SRCDIR = src
INCDIR = src

CFLAGS = -I$(INCDIR)

SRCS := $(wildcard $(SRCDIR)/*.c)
HDRS := $(wildcard $(INCDIR)/*.h)

aoc: $(SRCS) $(HDRS)
	$(CC) -o $@ $(SRCS) $(CFLAGS)
