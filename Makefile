#CCC_CC=clang
#CC=clang
LIBS=
CFLAGS=-g -std=gnu1x -Wall -pedantic -Wno-gnu -Wno-variadic-macros -O0
#--analyze
#-fsanitize=address -fno-omit-frame-pointer -funwind-tables -rdynamic
CFLAGS+=-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
CFLAGS+=`pkg-config --cflags $(LIBS)`
LDLIBS=-L/opt/vc/lib -lGLESv2 -lEGL -lbcm_host -lpthread -lm
#-fsanitize=address
LDLIBS+=`pkg-config --libs $(LIBS)`
SOURCES=main.c\
        eglcore.c\
        glcore.c\
        globaltime.c\
        OpenGLES2Core.c\
        Arabesque/Arabesque.c\
        AudioVisualizer/AudioVisualizer.c\
        FlyingCubes/FlyingCubes.c\
        Font/Font.c\
        Font/FreeType3D.c\
        noise.c\
        colorspace.c
OBJECTS=$(SOURCES:%.c=%.o)
EXECUTABLE=GLES2Playground #${PRODUCT_NAME}
#SRCROOT=/Users/kwasmich/Developer/RPi
#PROJECT_NAME=GLES2Playground

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
#	$^ == $(OBJECTS) (the list of prerequisites)
	$(CC) $^ $(LDLIBS) -o $@

#%.o: %.c
#.c.o:
#	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

analyse:
	make clean
	scan-build make
	cppcheck --template="{file}:{line}: warning: {severity} ({id}): {message}" --enable=all --inconclusive --inconclusive .
