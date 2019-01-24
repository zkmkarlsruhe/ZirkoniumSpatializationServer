#! /bin/bash
#

# stop on error
.SHELLFLAGS = -e

# local externals dir
EXTERN_DIR = externals

EXTERN_EXT = pd_darwin

PHONY: clean \
	externals externals-clean \
	hoa hoa-clean \
	hrtf hrtf-clean \
	vbap vbap-clean

all: externals

clean: externals-clean

##### externals

externals: hoa hrtf vbap

externals-clean: hoa-clean hrtf-clean vbap-clean

### hoa

hoa: zirkhoa.$(EXTERN_EXT)

zirkhoa.$(EXTERN_EXT):
	cp $(EXTERN_DIR)/hoa/$@ .
	cp $(EXTERN_DIR)/hoa/*-help.pd .

hoa-clean:
	rm -f zirkhoa*

### hrtf

hrtf: zirkhrtf~.$(EXTERN_EXT)

zirkhrtf~.$(EXTERN_EXT):
	cp $(EXTERN_DIR)/hrtf/$@ .
	cp $(EXTERN_DIR)/hrtf/*_data.txt .
	cp $(EXTERN_DIR)/hrtf/*-help.pd .

hrtf-clean:
	rm -f zirkhrtf*

### vbap

vbap: zirkvbap.$(EXTERN_EXT) define_loudspeakers.$(EXTERN_EXT)

zirkvbap.$(EXTERN_EXT):
	cp $(EXTERN_DIR)/vbap/$@ .
	cp $(EXTERN_DIR)/vbap/*-help.pd .

define_loudspeakers.$(EXTERN_EXT):
	cp $(EXTERN_DIR)/vbap/$@ .

vbap-clean:
	rm -f zirkvbap* define_loudspeakers*
