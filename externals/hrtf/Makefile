# Makefile for zirkhrtf~

lib.name = zirkhrtf~

class.sources = zirkhrtf~.c

datafiles = zirkhrtf~-help.pd zirkhrtf_data.txt \
            parse-to-h.pl README.md LICENSE.txt voice.wav

define forDarwin
  make-multi-arch = yes
endef

PDLIBBUILDER_DIR=.
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder