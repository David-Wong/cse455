# Makefile for project 1

PROJ1=CSE455
PROJ1_OBJS=CSE455Doc.o CSE455UI.o FeatureSet.o ImageDatabase.o \
		ImageView.o cse455.o features.o

IMAGELIB=ImageLib/libImage.a

CC=g++
CPPFLAGS=-Wall -O3 -I/uns/include
LIB_PATH=-L/uns/lib -L/usr/X11R6/lib
LIBS=-lfltk -lfltk_images -lpng -ljpeg -lX11 -lXext -lXi -lXmu

all: $(PROJ1)

$(IMAGELIB): 
	make -C ImageLib

$(PROJ1): $(PROJ1_OBJS) $(IMAGELIB)
	$(CC) -o $@ $(PROJ1_OBJS) $(LIB_PATH) $(LIBS) $(IMAGELIB)

clean:
	rm -f *.o *~ $(PROJ1)
