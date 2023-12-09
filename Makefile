CC = cc

SRCDIR = src
INCDIR = src

CFLAGS = -I$(INCDIR) -O0 -ggdb -lm

SRCS := $(wildcard $(SRCDIR)/*.c)
HDRS := $(wildcard $(INCDIR)/*.h)

aoc: $(SRCS) $(HDRS)
	$(CC) -o $@ $(SRCS) $(CFLAGS)
