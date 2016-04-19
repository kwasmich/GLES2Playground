#CCC_CC=clang
#CC=clang
LIBS=
CFLAGS=-g -std=gnu1x -Wall -Wextra -pedantic -Wno-gnu -Wno-variadic-macros -O0
#--analyze
#-fsanitize=address -fno-omit-frame-pointer -funwind-tables -rdynamic
CFLAGS+=-I.
CFLAGS+=-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
CFLAGS+=-I/Users/kwasmich/Developer/RPi/opt/vc/include -I/Users/kwasmich/Developer/RPi/opt/vc/include/interface/vcos/pthreads -I/Users/kwasmich/Developer/RPi/opt/vc/include/interface/vmcs_host/linux -I/Users/kwasmich/Developer/RPi/usr/include -I/Users/kwasmich/Developer/RPi/usr/include/arm-linux-gnueabihf
CFLAGS+=`pkg-config --cflags $(LIBS)`
LDLIBS=-L/opt/vc/lib -lGLESv2 -lEGL -lbcm_host -lpthread -lm
#-fsanitize=address
LDLIBS+=`pkg-config --libs $(LIBS)`
SOURCES=$(wildcard *.c)\
        $(wildcard */*.c)\
        $(wildcard */*/*.c)
OBJECTS=$(SOURCES:%.c=%.o)
DEPS=$(sort $(patsubst %, %.deps, $(OBJECTS)))
EXECUTABLE=GLES2Playground

all: $(EXECUTABLE)

-include $(DEPS)

$(DEPS):
	@true

$(EXECUTABLE): $(OBJECTS)
#	$^ == $(OBJECTS) (the list of prerequisites)
	$(CC) $^ $(LDLIBS) -o $@

%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< -MMD -MF $@.deps

.PHONY: clean
clean:
	rm -f $(DEPS) $(OBJECTS) $(EXECUTABLE)

.PHONY: analyse
analyse:
	make clean
	scan-build make
	cppcheck --template="{file}:{line}: warning: {severity} ({id}): {message}" --enable=all --inconclusive --inconclusive .
