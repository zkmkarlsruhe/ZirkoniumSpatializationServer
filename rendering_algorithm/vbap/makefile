


pd_darwin: vbap.pd_darwin define_loudspeakers.pd_darwin
.SUFFIXES: .pd_darwin

DARWINCFLAGS = -DPD -O2 -Wall -W -Wshadow -Wstrict-prototypes \
    -Wno-unused -Wno-parentheses -Wno-switch -arch i386 -arch x86_64

.c.pd_darwin:
	clang $(DARWINCFLAGS) $(LINUXINCLUDE) -o $*.o -c $*.c
	clang -bundle -undefined suppress -arch i386 -arch x86_64 \
            -flat_namespace -o $*.pd_darwin $*.o 
	rm -f $*.o

clean: ; rm -f *.pd_darwin *.o
