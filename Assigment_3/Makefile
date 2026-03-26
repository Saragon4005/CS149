CC=gcc
CFLAGS=-Wall -Werror
TARGETS=countnames shell

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)