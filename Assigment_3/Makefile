CC=gcc
CFLAGS=-Wall -Werror
TARGETS=countnames shell

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean: clean-logs
	rm -f $(TARGETS)

clean-logs:
	rm -f *.out *.err