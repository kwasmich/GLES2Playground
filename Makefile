CC=clang
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
        OpenGLES2Core.c\
        Arabesque/Arabesque.c\
        FlyingCubes/FlyingCubes.c\
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

#copy:
#	echo "Copying from RPi"
#	rsync -aLv pi@developi:/opt/vc ~/Developer/RPi/opt/
#	rsync -aLv pi@developi:/usr/include ~/Developer/RPi/usr/
#	rsync -aLv pi@developi:/usr/local/include ~/Developer/RPi/usr/local
#	echo "Copying to RPi"
#	echo ${SRCROOT}/${PROJECT_NAME}
#	rsync -aLv -f"- .*" ${SRCROOT}/${PROJECT_NAME} pi@developi:~/Developer
#	#ssh pi@developi "cd ~/Developer/${PROJECT_NAME};./configure;make"
#	ssh pi@developi "cd ~/Developer/${PROJECT_NAME};make"
