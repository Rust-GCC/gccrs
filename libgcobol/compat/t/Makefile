#
# A simple Makefile to demonstrate how the compat/lib programs are used.
#

COBC = gcobol -g -O0
CFLAGS = -fPIC
COBCFLAGS = -fPIC -ffixed-form

INCLUDE = ../../posix/cpy  ../../posix/udf ../gnu/cpy

FLAGS = -dialect mf $(addprefix -I,$(INCLUDE))

COMPAT = $(subst .cbl,.o,$(wildcard ../gnu/lib/*.cbl)) \
	$(subst .cbl,.o,$(wildcard ../gnu/udf/*.cbl)) \
	$(subst .c,.o,$(wildcard ../gnu/udf/*.c))

POSIXOBJS = $(subst .cbl,.o,$(wildcard ../../posix/udf/posix-*.cbl))

test: smoke
	./$^

smoke: smoke.cbl libcompat.so libposix.so
	$(ENV) $(COBC) -o $@ $(FLAGS) $(COBCFLAGS) $< $(LDFLAGS) \
		-L. -lcompat -lposix -Wl,-rpath=$(PWD)

smoke-old: smoke.cbl $(COMPAT)
	$(ENV) $(COBC) -o $@ $(FLAGS) $(COBCFLAGS) $(LDFLAGS) $^

byte: byte-stream-test.cbl libcompat.so libposix.so
	$(ENV) $(COBC) -o $@ $(FLAGS) $(COBCFLAGS) $< $(LDFLAGS) \
                -L. -lcompat -lposix -Wl,-rpath=$(PWD)

libcompat.so: $(COMPAT)
	$(ENV) $(COBC) -o $@ -shared $(FLAGS) $(COBCFLAGS) $(LDFLAGS) $(COMPAT)

libposix.so: $(POSIXOBJS)
	$(ENV) $(COBC) -o $@ -shared -ffixed-form \
	       $(FLAGS) $(COBCFLAGS) $(LDFLAGS) $^

%.o : %.cbl
	$(ENV) $(COBC) -c -o $@ $(FLAGS) $(COBCFLAGS) $^

% : %.cbl
	$(ENV) $(COBC) -o $@ $(FLAGS) $(COBCFLAGS) $(LDFLAGS) $^

echo:
	@echo wildcard is $(wildcard ../lib/gnu/*.cbl)
	@echo COMPAT is $(COMPAT)

clean:
	rm -f smoke libcompat.so libposix.so $(COMPAT) $(POSIXOBJS)
