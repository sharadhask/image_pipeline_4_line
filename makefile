CC=gcc
CFLAGS=-c -Wall -ggdb
LDFLAGS=
SOURCES=main.c autoexposure.c autofocus.c awb.c blacklevelsubtract.c colorcorrect.c colorinterpolate.c gammacorrect.c\
lens_correction.c matrix_utilities.c noise_reduction.c pgmreadwrite.c rgb2yuv.c tpg.c update_buffer.c utilities.c writeppm.c\
rgb2ycc422.c rgb2rgb565.c writebtu.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ipipe

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean: 
	rm -rf *.o $(EXECUTABLE)
