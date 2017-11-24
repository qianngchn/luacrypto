PLAT:=none
PLATS:=linux mingw

LIB:=crypto.so

INSTALL_TOP?=$(CURDIR)/local
INSTALL_LIB:=$(INSTALL_TOP)/lib/lua

INSTALL:=install -p
INSTALL_DATA:=$(INSTALL) -m0644

MKDIR:=mkdir -p
RM:=rm -rf

.PHONY:all none $(PLATS) install uninstall clean

all:$(PLAT)

none:
	@echo "Please select a PLATFORM from these:"
	@echo "    $(PLATS)"
	@echo "then do 'make PLATFORM' to complete constructions."

$(PLATS) clean:
	@cd src && $(MAKE) $@

install:
	$(MKDIR) $(INSTALL_LIB)
	cd src && $(INSTALL_DATA) $(LIB) $(INSTALL_LIB)

uninstall:
	cd $(INSTALL_LIB) && $(RM) $(LIB)
