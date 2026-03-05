CC=gcc
CFLAGS=-Wall -Werror
TARGETS=countnames shell1

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS) *.err *.out
