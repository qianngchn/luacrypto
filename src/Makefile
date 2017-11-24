PLAT:=none
PLATS:=linux mingw

CC:=gcc -std=gnu99
CFLAGS:=-Wall -O2 $(PLATCFLAGS)
SRCS:=rc4.c crc32.c
SRCS+=luacrypto.c
LIB:=crypto.so

ALL:=$(LIB)

STRIP:=strip -s
RM:=rm -rf

.PHONY:all none $(PLATS) clean

all:$(PLAT)

none:
	@echo "Please select a PLATFORM from these:"
	@echo "    $(PLATS)"
	@echo "then do 'make PLATFORM' to complete constructions."

$(PLATS):
	@$(MAKE) $(ALL)

$(LIB):$(SRCS)
	$(CC) $^ $(CFLAGS) -shared -fPIC -o $@
	$(STRIP) $@

clean:
	$(RM) $(ALL)
