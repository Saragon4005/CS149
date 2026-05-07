CC=gcc
CFLAGS=-Wall -Werror -D_REENTRANT
LDFLAGS=-pthread
TARGETS=countnames shell
COMMON=namecount.c

all: $(TARGETS)

countnames: countnames.c $(COMMON) namecount.h
	$(CC) $(CFLAGS) -o $@ countnames.c $(COMMON) $(LDFLAGS)

shell: shell.c $(COMMON) namecount.h
	$(CC) $(CFLAGS) -o $@ shell.c $(COMMON) $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.out *.err
