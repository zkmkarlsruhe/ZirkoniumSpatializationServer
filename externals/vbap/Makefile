# Makefile for zirkvbap

lib.name = zirkvbap

class.sources = zirkvbap.c define_loudspeakers.c

datafiles = define_loudspeakers.h max2pd.h zirkvbap.h \
            README.md $(wildcard *.pd)

define forDarwin
  make-multi-arch = yes
  cflags += -mmacosx-version-min=10.9
endef

PDLIBBUILDER_DIR=.
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder