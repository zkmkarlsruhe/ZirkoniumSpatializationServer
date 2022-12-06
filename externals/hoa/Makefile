# Makefile for zirkhoa

lib.name = zirkhoa

class.sources = zirkhoa.cpp

datafiles = README.md zirkhoa-help.pd

cflags = -I HoaLibrary-Light/ThirdParty/Eigen -I HoaLibrary-Light/Sources

suppress-wunused = yes

define forDarwin
 cflags += -stdlib=libc++ -mmacosx-version-min=10.9
endef

PDLIBBUILDER_DIR=.
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder