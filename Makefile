# server externals Makfile

# stop on error
.SHELLFLAGS = -e

# local externals dir
EXTERN_DIR = externals

# externals extension
UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)
  # Mac
  EXTERN_EXT = d_fat
else ifeq ($(OS), Windows_NT)
  # Windows, use Mingw
  EXTERN_EXT = dll
else
  # Linux or *BSD
  EXTERN_EXT = pd_linux
endif

.PHONY: clean clobber \
	extern extern-clean extern-clobber \
	hoa hoa-depends hoa-clean hoa-clobber \
	hrtf hrtf-clean hrtf-clobber \
	vbap vbap-clean vbap-clobber

all: extern

clean: extern-clean

clobber: extern-clobber

##### all externals

extern: hoa hrtf vbap

extern-clean: hoa-clean hrtf-clean vbap-clean

extern-clobber: hoa-clobber hrtf-clobber vbap-clobber

### hoa

hoa: hoa-depends zirkhoa.$(EXTERN_EXT)

hoa-depends: $(EXTERN_DIR)/hoa/HoaLibrary-Light/ThirdParty

$(EXTERN_DIR)/hoa/HoaLibrary-Light/ThirdParty:
	cd $(EXTERN_DIR)/hoa && git submodule update --init --recursive

zirkhoa.$(EXTERN_EXT):
	make -C $(EXTERN_DIR)/hoa
	cp $(EXTERN_DIR)/hoa/$@ .
	cp $(EXTERN_DIR)/hoa/*-help.pd .

hoa-clean:
	make -C $(EXTERN_DIR)/hoa clean

hoa-clobber:
	rm -f zirkhoa*

### hrtf

hrtf: zirkhrtf~.$(EXTERN_EXT)

zirkhrtf~.$(EXTERN_EXT):
	make -C $(EXTERN_DIR)/hrtf
	cp $(EXTERN_DIR)/hrtf/$@ .
	cp $(EXTERN_DIR)/hrtf/*_data.txt .
	cp $(EXTERN_DIR)/hrtf/*-help.pd .

hrtf-clean:
	make -C $(EXTERN_DIR)/hrtf clean

hrtf-clobber:
	rm -f zirkhrtf*

### vbap

vbap: zirkvbap.$(EXTERN_EXT) define_loudspeakers.$(EXTERN_EXT)

zirkvbap.$(EXTERN_EXT):
	make -C $(EXTERN_DIR)/vbap
	cp $(EXTERN_DIR)/vbap/$@ .
	cp $(EXTERN_DIR)/vbap/*-help.pd .

define_loudspeakers.$(EXTERN_EXT):
	cp $(EXTERN_DIR)/vbap/$@ .

vbap-clean:
	make -C $(EXTERN_DIR)/vbap clean

vbap-clobber:
	rm -f zirkvbap* define_loudspeakers*
